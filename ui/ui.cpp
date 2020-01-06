#include "ui.h"
#include "account.h"
#include "userid.h"
#include <string>
#include <iostream>
#include <limits.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qradiobutton.h>

////////////////////////////////////////////////////////////////////////////////

static const int COL_USERID = 0;
static const int COL_ACCTYPE = 1;
static const int COL_CREATED = 2;
static const char *TABLE_HEADER[] = { "User id",  "Account type" , "Time created" };

//static const char* TXT_IBAN = "IBAN";
//static const char* TXT_INTEGER = "Integer";
static const char* TXT_ACCTYPE = "Account type";
static const char* TXT_CREATED = "Time created";
static const char* TXT_LASTNAME = "Last Name";
static const char* TXT_FIRSTNAME = "First Name";
static const char* TXT_OK = "Ok";
static const char* TXT_CANCEL = "Cancel";
static const char* TXT_NEW = "New";
static const char* TXT_DELETE = "Delete";
static const char* TXT_DETAILS = "Details";
static const char* TXT_CUSTOMER = "Customer";
static const char* TXT_ENTERPRISE = "Enterprise";

static const char* TXT_COMPANYNAME = "Company Name";
static const char* TXT_BUSINESSID = "Business ID";

static const QString REGEX_ALPHA = "[a-zA-Z]{1,12}";
static const QString REGEX_DIGIT = "[0-9]{1,10}";
static const QString REGEX_IBAN = "[A-Z]{2}[0-9]{2}[A-Z0-9]{0,30}"; // 2 letters, 2 digits, up to 30 alphanumeric characters

static const int BUF_SIZE = 80;
static char buf[BUF_SIZE] = { 0 };

////////////////////////////////////////////////////////////////////////////////

static const char* formatTimeStamp(time_t ts)
{
	struct tm t = *localtime(&ts);
	memset(buf, 0, BUF_SIZE);
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &t);
	return buf;
}

//
// https://stackoverflow.com/questions/13661481/how-to-check-if-a-number-overflows-an-int
//
static bool isValidInteger(const std::string& str)
{
	const int max = INT_MAX; // TODO: define in database module ?

	int val = std::atoi(str.c_str());

	if (val == 0)
		return false;

	if (val < 0)
		return false;

	if (val < max)
		return true;

	int len = (int)str.length();

	int curval = 0;

	for (int i = 0; i < len; ++i)
	{
		char buf[2] = { str[i], 0 };

		int digit = std::atoi(buf);

		if (curval > max / 10 || (curval == max / 10 && digit > max % 10))
			return false;
		else
			curval = curval * 10 + digit;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

SymbioUi::SymbioUi(QWidget* parent)
	: QMainWindow(parent)
	, table(nullptr)
	, model(nullptr)
	, combo(nullptr)
{
	// TODO: use qtdesigner for ui ?

	setFixedSize(QSize(800, 600));

	table = new QTableView(this);
	table->setFixedSize(QSize(800, 600));
	model = new QStandardItemModel(table);
	table->setModel(model);

	setupTableHeaders();

	connect(table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SymbioUi::onSelectionChanged);

	combo = new QComboBox(this);
	combo->setEditable(false);
	combo->setMaxCount(2);
	combo->addItem(TXT_CUSTOMER);
	combo->addItem(TXT_ENTERPRISE);
	combo->setGeometry(QRect(720, 10, 80, 24));

	QPushButton* btnAdd = new QPushButton(this);
	btnAdd->setText(TXT_NEW);
	btnAdd->setGeometry(QRect(720, 50, 80, 24));
	connect(btnAdd, &QPushButton::clicked, this, &SymbioUi::onAddButtonClicked);

	QPushButton* btnDel = new QPushButton(this);
	btnDel->setText(TXT_DELETE);
	btnDel->setGeometry(QRect(720, 90, 80, 24));
	connect(btnDel, &QPushButton::clicked, this, &SymbioUi::onDelButtonClicked);

	QPushButton* btnDtl = new QPushButton(this);
	btnDtl->setText(TXT_DETAILS);
	btnDtl->setGeometry(QRect(720, 130, 80, 24));
	connect(btnDtl, &QPushButton::clicked, this, &SymbioUi::onDetailsButtonClicked);

	// engine
	SymbioEng::instance()->setObserver(this);
	SymbioEng::instance()->loadAccounts();
}

SymbioUi::~SymbioUi()
{
	SymbioEng::instance()->release();
}

void SymbioUi::setupTableHeaders()
{
	QStringList headers;
	int count = (int)sizeof(TABLE_HEADER) / sizeof(char *);
	for (int i = 0; i < count; ++i)
		headers << TABLE_HEADER[i];
	model->setHorizontalHeaderLabels(headers);
}

void SymbioUi::onAddButtonClicked()
{
	QString txt = combo->currentText();

	NewAccountDialog dialog;

	if (txt.compare(TXT_CUSTOMER) == 0)
	{
		const std::map<AccountType, AccountSpecs>& specMap = SymbioEng::instance()->getAccountSpecs();
		const AccountSpecs& spec = specMap.at(AccountType::Customer);
		dialog.setSpecs(spec);
	}
	else if (txt.compare(TXT_ENTERPRISE) == 0)
	{
		const std::map<AccountType, AccountSpecs>& specMap = SymbioEng::instance()->getAccountSpecs();
		const AccountSpecs& spec = specMap.at(AccountType::Enterprise);
		dialog.setSpecs(spec);
	}

	int res = dialog.exec();

	if (res)
	{
		const AccountSpecs& spec = dialog.getAccountSpecs();

		//std::cout << "SymbioUi::onAddButtonClicked() accepted" << std::endl; // DEBUG

		if (SymbioEng::instance()->createAccount(spec))
			SymbioEng::instance()->loadAccounts();
	}
}

void SymbioUi::onDelButtonClicked()
{
	std::vector<std::string> vec;

	const QModelIndexList list = table->selectionModel()->selectedIndexes();

	for (const QModelIndex& index : list)
	{
		int row = index.row();
		int col = index.column();

		//std::cout << "SymbioUi::onDelButtonClicked() [" << row << ", " << col << "]"; // DEBUG

		std::string userid;

		for (int col = 0; col < 3; ++col)
		{
			QStandardItem* item = model->item(row, col);
			QString text = item->text();
			QByteArray bytes = text.toLatin1();
			char* data = bytes.data();

			if (col == COL_USERID)
			{
				userid = std::string(data);

				vec.push_back(userid);

				continue;
			}
		}
	}

	if (vec.size() > 0)
	{
		if (SymbioEng::instance()->deleteAccounts(vec))
			SymbioEng::instance()->loadAccounts();
		else
			std::cout << "ERROR! failed to delete accounts" << std::endl; // DEBUG
	}
}

void SymbioUi::onDetailsButtonClicked()
{
	//std::cout << "SymbioUi::onDetailsButtonClicked()" << std::endl;

	const QModelIndexList list = table->selectionModel()->selectedIndexes();

	if (list.size() > 0)
	{
		const QModelIndex& index = list.at(0);
		int row = index.row();
		int col = index.column();

		std::string userid;

		for (int col = 0; col < 3; ++col)
		{
			QStandardItem* item = model->item(row, col);
			QString text = item->text();
			QByteArray bytes = text.toLatin1();
			char* data = bytes.data();

			if (col == COL_USERID)
			{
				userid = std::string(data);

				break;
			}
		}

		Account acc;

		if (SymbioEng::instance()->getAccountDetails(userid, acc))
		{
			AccountDetailsDialog dialog;

			std::string accTypeStr = acc.getAccountType() == AccountType::Customer ? TXT_CUSTOMER : TXT_ENTERPRISE;
			dialog.setAccountType(QString(accTypeStr.c_str()));

			const char *timeStr = formatTimeStamp(acc.getCreated());
			dialog.setCreated(QString(timeStr));

			const std::string& firtName = acc.getFirstName();
			const std::string& lastName = acc.getLastName();
			const std::string& companyName = acc.getCompanyName();
			const std::string& businessId = acc.getBusinessId();

			if (firtName.length() > 0)
				dialog.setFirstName(QString(firtName.c_str()));
			if (lastName.length() > 0)
				dialog.setLastName(QString(lastName.c_str()));
			if (companyName.length() > 0)
				dialog.setCompanyName(QString(companyName.c_str()));
			if (businessId.length() > 0)
				dialog.setBusinessId(QString(businessId.c_str()));

			int res = dialog.exec();

			//std::cout << "Account details dialog exec=" << res << std::endl; // DEBUG
		}
		else
			std::cout << "ERROR! failed to fetch account details" << std::endl; // DEBUG
	}
}

void SymbioUi::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	//std::cout << "SymbioUi::onSelectionChanged()" << std::endl;
}

void SymbioUi::onAccountsLoadCompleted(const std::vector<Account>& vec)
{
	model->clear();

	setupTableHeaders();

	int row = 0;
	for (const Account& acc : vec)
	{
		time_t created = acc.getCreated();
		AccountType type = acc.getAccountType();
		const std::string userid = acc.getUserId();

		QStandardItem* item1 = new QStandardItem(QString("%0").arg(QString(userid.c_str())));
		item1->setEditable(false);
		//item1->setSelectable(false);
		model->setItem(row, COL_USERID, item1);
		table->setColumnWidth(COL_USERID, 100);

		const char* typeStr = (type == AccountType::Customer ? TXT_CUSTOMER : TXT_ENTERPRISE);
		QStandardItem* item2 = new QStandardItem(QString("%0").arg(QString(typeStr)));
		item2->setEditable(false);
		item2->setSelectable(false);
		model->setItem(row, COL_ACCTYPE, item2);
		table->setColumnWidth(COL_ACCTYPE, 100);

		const char *timeStr = formatTimeStamp(created);
		QStandardItem* item3 = new QStandardItem(QString("%0").arg(QString(timeStr)));
		item3->setEditable(false);
		item3->setSelectable(false);
		model->setItem(row, COL_CREATED, item3);
		table->setColumnWidth(COL_CREATED, 180);

		if (row == 0)
		{
			QModelIndex index = model->index(0, 0);
			table->selectionModel()->select(index, QItemSelectionModel::SelectionFlag::Select | QItemSelectionModel::SelectionFlag::Rows);
		}

		++row;
	}
}

////////////////////////////////////////////////////////////////////////////////

AccountDetailsDialog::AccountDetailsDialog(QWidget* parent)
	: QDialog(parent)
	, leType(nullptr)
	, leCreated(nullptr)
	, leFirstName(nullptr)
	, leLastName(nullptr)
	, leCompanyName(nullptr)
	, leBusinessid(nullptr)
{
	leType = new QLineEdit(this);
	leType->setGeometry(QRect(0, 0, 150, 20));
	leType->setReadOnly(true);
	leType->setPlaceholderText(TXT_ACCTYPE);

	leCreated = new QLineEdit(this);
	leCreated->setGeometry(QRect(0, 20, 150, 20));
	leCreated->setReadOnly(true);
	leCreated->setPlaceholderText(TXT_CREATED);

	leFirstName = new QLineEdit(this);
	leFirstName->setGeometry(QRect(0, 40, 150, 20));
	leFirstName->setReadOnly(true);
	leFirstName->setPlaceholderText(TXT_FIRSTNAME);

	leLastName = new QLineEdit(this);
	leLastName->setGeometry(QRect(0, 60, 150, 20));
	leLastName->setReadOnly(true);
	leLastName->setPlaceholderText(TXT_LASTNAME);

	leCompanyName = new QLineEdit(this);
	leCompanyName->setGeometry(QRect(0, 80, 150, 20));
	leCompanyName->setReadOnly(true);
	leCompanyName->setPlaceholderText(TXT_COMPANYNAME);

	leBusinessid = new QLineEdit(this);
	leBusinessid->setGeometry(QRect(0, 100, 150, 20));
	leBusinessid->setReadOnly(true);
	leBusinessid->setPlaceholderText(TXT_BUSINESSID);

	QPushButton* ok = new QPushButton(this);
	ok->setText(TXT_OK);
	ok->setGeometry(QRect(0, 150, 80, 24));
	connect(ok, &QPushButton::clicked, this, &QDialog::accept);

	QPushButton* no = new QPushButton(this);
	no->setText(TXT_CANCEL);
	no->setGeometry(QRect(80, 150, 80, 24));
	connect(no, &QPushButton::clicked, this, &QDialog::reject);
}

AccountDetailsDialog::~AccountDetailsDialog()
{
	//
}

void AccountDetailsDialog::setCreated(const QString& str)
{
	leCreated->setText(str);
}

void AccountDetailsDialog::setAccountType(const QString& str)
{
	leType->setText(str);
}

void AccountDetailsDialog::setFirstName(const QString& str)
{
	leFirstName->setText(str);
}

void AccountDetailsDialog::setLastName(const QString& str)
{
	leLastName->setText(str);
}

void AccountDetailsDialog::setCompanyName(const QString& str)
{
	leCompanyName->setText(str);
}

void AccountDetailsDialog::setBusinessId(const QString& str)
{
	leBusinessid->setText(str);
}

////////////////////////////////////////////////////////////////////////////////

NewAccountDialog::NewAccountDialog(QWidget* parent)
	: QDialog(parent)
	, valAlpha(nullptr)
	, valDigit(nullptr)
	, valIban(nullptr)
{
	valAlpha = new QRegExpValidator(QRegExp(REGEX_ALPHA), this);
	valDigit = new QRegExpValidator(QRegExp(REGEX_DIGIT), this);
	valIban = new QRegExpValidator(QRegExp(REGEX_IBAN), this);
}

NewAccountDialog::~NewAccountDialog()
{
	//
}

const char* NewAccountDialog::fieldToUiName(const char* n)
{
	if (strcmp(n, FIELD_LASTNAME) == 0)
		return TXT_LASTNAME;
	else if (strcmp(n, FIELD_FIRSTNAME) == 0)
		return TXT_FIRSTNAME;
	else if (strcmp(n, FIELD_COMPANYNAME) == 0)
		return TXT_COMPANYNAME;
	else if (strcmp(n, FIELD_BUSINESSID) == 0)
		return TXT_BUSINESSID;
	else if (strcmp(n, FIELD_CREATED) == 0)
		return TXT_CREATED;
	else if (strcmp(n, FIELD_TYPE) == 0)
		return TXT_ACCTYPE;

	return nullptr;
}

void NewAccountDialog::setSpecs(const AccountSpecs& specs)
{
	m_Specs = specs;

	AccountType type = m_Specs.getAccountType();

	// TODO: exception ?
	if (type == AccountType::None)
		return;

	float y = 0.0f;
	const float x = 5.0f;
	const float width = 150.0f;
	const float height = 20.0f;

	const std::vector<AccountField>& fields = m_Specs.getFields();

	for (const AccountField& f : fields)
	{
		const char* locName = fieldToUiName(f.name.c_str());

		if (!locName)
			locName = f.name.c_str();

		QLabel* label = new QLabel(QString(locName), this);
		label->setGeometry(QRect(x, y, width, height));

		y += height;

		QLineEdit* edit = new QLineEdit(this);
		edit->setGeometry(QRect(x, y, width, height));

		y += height;

		if (f.type == AccountField::FieldType::String)
		{
			edit->setPlaceholderText("Aa-Zz");
			edit->setValidator(valAlpha);
		}
		else if (f.type == AccountField::FieldType::Integer)
		{
			edit->setPlaceholderText("01234567890");
			edit->setValidator(valDigit);
		}
		else if (f.type == AccountField::FieldType::Iban)
		{
			edit->setPlaceholderText("FI1234567890");
			edit->setValidator(valIban);
		}

		m_Fields.push_back(edit);
	}

	y += 5;

	QPushButton* ok = new QPushButton(this);
	ok->setText(TXT_OK);
	ok->setGeometry(QRect(0, y, 80, 24));
	//connect(ok, &QPushButton::clicked, this, &QDialog::accept);
	connect(ok, &QPushButton::clicked, this, &NewAccountDialog::onOkButtonPressed);

	QPushButton* no = new QPushButton(this);
	no->setText(TXT_CANCEL);
	no->setGeometry(QRect(80, y, 80, 24));
	connect(no, &QPushButton::clicked, this, &QDialog::reject);
}

void NewAccountDialog::onOkButtonPressed()
{
	std::vector<AccountField>& fields = m_Specs.getFields();

	int i = 0;
	for (QLineEdit* edit : m_Fields)
	{
		QString str = edit->text();
		int length = str.length();
		QByteArray ba = str.toLatin1();
		const char* value = ba.data();

		// validate input
		const AccountField& f = fields[i];

		if (f.required && length < 1)
		{
			std::cout << "ERROR! required value is missing name=" << f.name.c_str() << std::endl; // DEBUG

			return;
		}

		if (AccountField::FieldType::String == f.type)
		{
			int pos = 0;
			if (valAlpha->validate(str, pos) != QValidator::Acceptable)
			{
				std::cout << "ERROR! invalid input " << f.name.c_str() << "=" << value << std::endl; // DEBUG

				return;
			}
		}
		else if (AccountField::FieldType::Iban == f.type)
		{
			int pos = 0;
			if (valIban->validate(str, pos) != QValidator::Acceptable)
			{
				std::cout << "ERROR! invalid input " << f.name.c_str() << "=" << value << std::endl; // DEBUG

				return;
			}
		}
		else if (AccountField::FieldType::Integer == f.type)
		{
			int pos = 0;
			if (valDigit->validate(str, pos) != QValidator::Acceptable)
			{
				std::cout << "ERROR! invalid input " << f.name.c_str() << "=" << value << std::endl; // DEBUG

				return;
			}

			if (!isValidInteger(value))
			{
				std::cout << "ERROR! invalid input " << f.name.c_str() << "=" << value << std::endl; // DEBUG

				return;
			}
		}

		// don't copy empty string
		if (length > 0)
			fields[i].value = std::string(value);

		++i; // next
	}

	// input is valid
	accept();
}
