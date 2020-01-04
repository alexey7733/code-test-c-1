#include "ui.h"
#include "account.h"
#include "userid.h"
#include <string>
#include <iostream>
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

static const char* TXT_IBAN = "IBAN";
static const char* TXT_INTEGER = "Integer";
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

////////////////////////////////////////////////////////////////////////////////

SymbioUi::SymbioUi(QWidget* parent)
	: QMainWindow(parent)
	, engine(nullptr)
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
	engine = new SymbioEng();
	engine->setObserver(this);
	engine->loadAccounts();
}

SymbioUi::~SymbioUi()
{
	delete engine;
}

void SymbioUi::setupTableHeaders()
{
	QStringList headers;
	int count = (int)sizeof(TABLE_HEADER) / sizeof(char *);
	for (int i = 0; i < count; ++i)
		headers << TABLE_HEADER[i];
	model->setHorizontalHeaderLabels(headers);
}

void SymbioUi::showNewCustomerDialog()
{
	NewCustomerAccountDialog dialog;

	if (dialog.exec() == QDialog::Accepted)
	{
		QString uid = dialog.getUserId();
		QString fname = dialog.getFirstName();
		QString lname = dialog.getLastName();

		QByteArray bauid = uid.toLatin1();
		char* d0 = (char*)bauid.data();

		QByteArray bafname = fname.toLatin1();
		char* d1 = (char*)bafname.data();

		QByteArray balname = lname.toLatin1();
		char* d2 = (char*)balname.data();

		//std::cout << "SymbioUi::showNewCustomerDialog()" << " Customer " << d0 << " " << d1 << " " << d2 << std::endl; // DEBUG

		if (dialog.isIbanChecked())
		{
			AccountCustomer acc(d0);
			acc.setFirstName(d1);
			acc.setLastName(d2);

			if (engine->createAccount(acc))
				engine->loadAccounts();
			else
				std::cout << "SymbioUi::showNewCustomerDialog(1) ERROR! failed to add customer account" << std::endl;
		}
		else // integer
		{
			int userid = std::atoi(d0);

			AccountCustomer acc(userid);
			acc.setFirstName(d1);
			acc.setLastName(d2);

			if (engine->createAccount(acc))
				engine->loadAccounts();
			else
				std::cout << "SymbioUi::showNewCustomerDialog(2) ERROR! failed to add customer account" << std::endl;
		}
	}
}

void SymbioUi::showNewEnterpriseDialog()
{
	NewEnterpriseAccountDialog dialog;

	if (dialog.exec() == QDialog::Accepted)
	{
		QString uid = dialog.getUserId();
		QByteArray bauid = uid.toLatin1();
		char* d0 = (char*)bauid.data();

		QString companyname = dialog.getCompanyName();
		QByteArray bacname = companyname.toLatin1();
		char* d1 = (char*)bacname.data();

		QString businessid = dialog.getBusinessId();
		QByteArray babid = businessid.toLatin1();
		char* d2 = (char*)babid.data();

		//std::cout << "SymbioUi::showNewEnterpriseDialog()" << " Enterprise " << d0 << " " << d1 << " " << d2 << std::endl; // DEBUG

		if (dialog.isIbanChecked())
		{
			AccountEnterprise acc(d0);
			acc.setName(d1);
			acc.setBusinessid(d2);

			if (engine->createAccount(acc))
				engine->loadAccounts();
			else
				std::cout << "SymbioUi::showNewEnterpriseDialog(1) ERROR! failed to add enterprise account" << std::endl;
		}
		else // integer
		{
			int userid = std::atoi(d0);

			AccountEnterprise acc(userid);
			acc.setName(d1);
			acc.setBusinessid(d2);

			if (engine->createAccount(acc))
				engine->loadAccounts();
			else
				std::cout << "SymbioUi::showNewEnterpriseDialog(2) ERROR! failed to add enterprise account" << std::endl;
		}
	}
}

void SymbioUi::onAddButtonClicked()
{
	QString txt = combo->currentText();
	
	if (txt.compare(TXT_CUSTOMER) == 0)
		showNewCustomerDialog();
	else if (txt.compare(TXT_ENTERPRISE) == 0)
		showNewEnterpriseDialog();
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
		if (engine->deleteAccounts(vec))
			engine->loadAccounts();
		else
			std::cout << "ERROR! failed to delete accounts" << std::endl; // DEBUG
	}
}

void SymbioUi::onDetailsButtonClicked()
{
	std::cout << "SymbioUi::onDetailsButtonClicked()" << std::endl;

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

		AccountDetails det;

		if (engine->getAccountDetails(userid, det))
		{
			AccountDetailsDialog dialog;

			std::string accTypeStr = det.type == Account::AccountType::Customer ? TXT_CUSTOMER : TXT_ENTERPRISE;
			dialog.setAccountType(QString(accTypeStr.c_str()));

			const char *timeStr = formatTimeStamp(det.created);
			dialog.setCreated(QString(timeStr));

			if (det.firstName.length() > 0)
				dialog.setFirstName(QString(det.firstName.c_str()));
			if (det.lastName.length() > 0)
				dialog.setLastName(QString(det.lastName.c_str()));
			if (det.companyName.length() > 0)
				dialog.setCompanyName(QString(det.companyName.c_str()));
			if (det.businessId.length() > 0)
				dialog.setBusinessId(QString(det.businessId.c_str()));

			int res = dialog.exec();

			std::cout << "Account details dialog exec=" << res << std::endl; // DEBUG
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
		Account::AccountType type = acc.getAccountType();
		const std::string userid = acc.getUserId()->toString();

		QStandardItem* item1 = new QStandardItem(QString("%0").arg(QString(userid.c_str())));
		item1->setEditable(false);
		//item1->setSelectable(false);
		model->setItem(row, COL_USERID, item1);
		table->setColumnWidth(COL_USERID, 100);

		const char* typeStr = (type == Account::AccountType::Customer ? TXT_CUSTOMER : TXT_ENTERPRISE);
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
	, btnOk(nullptr)
	, valAlpha(nullptr)
	, valDigit(nullptr)
	, valIban(nullptr)
	, lbltype(nullptr)
	, userid(nullptr)
	, rbIban(nullptr)
	, rbInteger(nullptr)
{

	setModal(true);

	valAlpha = new QRegExpValidator(QRegExp(REGEX_ALPHA), this);
	valDigit = new QRegExpValidator(QRegExp(REGEX_DIGIT), this);
	valIban = new QRegExpValidator(QRegExp(REGEX_IBAN), this);

	//QPushButton* 
	btnOk = new QPushButton(this);
	btnOk->setText(TXT_OK);
	btnOk->setGeometry(QRect(0, 150, 80, 24));
	//connect(ok, &QPushButton::clicked, this, &QDialog::accept);
	connect(btnOk, &QPushButton::clicked, this, &NewAccountDialog::onOkButtonPressed);

	QPushButton* cancel = new QPushButton(this);
	cancel->setText(TXT_CANCEL);
	cancel->setGeometry(QRect(80, 150, 80, 24));
	connect(cancel, &QPushButton::clicked, this, &QDialog::reject);

	lbltype = new QLabel(QString(TXT_ACCTYPE), this);

	rbInteger = new QRadioButton(TXT_INTEGER, this);
	rbInteger->setChecked(true);
	connect(rbInteger, &QRadioButton::pressed, this, &NewCustomerAccountDialog::onRadioIntegerClicked);

	rbIban = new QRadioButton(TXT_IBAN, this);
	rbIban->setChecked(false);
	connect(rbIban, &QRadioButton::pressed, this, &NewCustomerAccountDialog::onRadioIbanClicked);

	userid = new QLineEdit("", this);
	userid->setPlaceholderText("0123456789");
	userid->setValidator(valDigit);
}

NewAccountDialog::~NewAccountDialog()
{
	//
}

QString NewAccountDialog::getUserId()
{
	return userid->text();
}

bool NewAccountDialog::isIbanChecked()
{
	return rbIban->isChecked();
}

bool NewAccountDialog::isIntegerChecked()
{
	return rbInteger->isChecked();
}

void NewAccountDialog::onOkButtonPressed()
{
	//
}

void NewAccountDialog::onRadioIbanClicked()
{
	if (rbIban->isChecked())
		return;

	userid->setText("");
	userid->setPlaceholderText("FI01234567890");
	userid->setValidator(valIban);

	rbIban->setChecked(true);
	rbInteger->setChecked(false);
}

void NewAccountDialog::onRadioIntegerClicked()
{
	if (rbInteger->isChecked())
		return;

	userid->setText("");
	userid->setPlaceholderText("0123456789");
	userid->setValidator(valDigit);

	rbIban->setChecked(false);
	rbInteger->setChecked(true);
}

////////////////////////////////////////////////////////////////////////////////

NewCustomerAccountDialog::NewCustomerAccountDialog(QWidget* parent)
	: NewAccountDialog(parent)
	, lastName(nullptr)
	, firstName(nullptr)
{
	// first name
	QLabel* lblFirstName = new QLabel(QString(TXT_FIRSTNAME), this);
	lblFirstName->setGeometry(QRect(5, 0, 150, 20));

	firstName = new QLineEdit(this);
	firstName->setGeometry(QRect(5, 20, 150, 20));
	firstName->setClearButtonEnabled(true);
	firstName->setPlaceholderText(TXT_FIRSTNAME);
	firstName->setValidator(valAlpha);

	// last name
	QLabel* lblLastName = new QLabel(QString(TXT_LASTNAME), this);
	lblLastName->setGeometry(QRect(5, 40, 150, 20));

	lastName = new QLineEdit("", this);
	lastName->setGeometry(QRect(5, 60, 150, 20));
	lastName->setClearButtonEnabled(true);
	lastName->setPlaceholderText(TXT_LASTNAME);
	lastName->setValidator(valAlpha);

	// account type
	lbltype->setGeometry(QRect(5, 80, 150, 20));
	rbInteger->setGeometry(QRect(10, 100, 82, 17));
	rbIban->setGeometry(QRect(85, 100, 82, 17));
	userid->setGeometry(QRect(5, 120, 150, 20));
}

NewCustomerAccountDialog::~NewCustomerAccountDialog()
{
	//
}

QString NewCustomerAccountDialog::getFirstName()
{
	return firstName->text();
}

QString NewCustomerAccountDialog::getLastName()
{
	return lastName->text();
}

void NewCustomerAccountDialog::setFirstName(const QString& str)
{
	firstName->setText(str);
}

void NewCustomerAccountDialog::setLastName(const QString& str)
{
	lastName->setText(str);
}

void NewCustomerAccountDialog::setUserId(const QString& str)
{
	userid->setText(str);
}

void NewCustomerAccountDialog::onOkButtonPressed()
{
	//std::cout << "NewCustomerAccountDialog::onOkButtonPressed()" << std::endl;

	int pos = 0;

	QString fName = getFirstName();
	int length = fName.length();

	if (length < 1 || valAlpha->validate(fName, pos) != QValidator::Acceptable)
	{
		std::cout << "ERROR: invalid first name length=" << length << std::endl; // DEBUG

		return;
	}

	QString lName = getLastName();
	length = lName.length();

	if (length < 1 || valAlpha->validate(getLastName(), pos) != QValidator::Acceptable)
	{
		std::cout << "ERROR: invalid last name length=" << length << std::endl; // DEBUG

		return;
	}

	QString uid = getUserId();
	length = uid.length();

	if (isIntegerChecked())
	{
		if (length < 1 || valDigit->validate(getUserId(), pos) != QValidator::Acceptable)
		{
			std::cout << "ERROR: invalid userid INT length=" << length << std::endl; // DEBUG

			return;
		}
	}
	else
	{
		if (length < 4 || valIban->validate(getUserId(), pos) != QValidator::Acceptable)
		{
			std::cout << "ERROR: invalid userid IBAN length=" << length << std::endl; // DEBUG

			return;
		}
	}

	// input is valid
	accept();
}

////////////////////////////////////////////////////////////////////////////////

NewEnterpriseAccountDialog::NewEnterpriseAccountDialog(QWidget* parent)
	: NewAccountDialog(parent)
	, companyName(nullptr)
	, businessId(nullptr)
{
	// company name
	QLabel* lblCompanyName = new QLabel(QString(TXT_COMPANYNAME), this);
	lblCompanyName->setGeometry(QRect(5, 0, 150, 20));

	companyName = new QLineEdit(this);
	companyName->setGeometry(QRect(5, 20, 150, 20));
	companyName->setClearButtonEnabled(true);
	companyName->setPlaceholderText(TXT_COMPANYNAME);
	companyName->setValidator(valAlpha);

	// last name
	QLabel* lblBusinessId = new QLabel(QString(TXT_BUSINESSID), this);
	lblBusinessId->setGeometry(QRect(5, 40, 150, 20));

	businessId = new QLineEdit("", this);
	businessId->setGeometry(QRect(5, 60, 150, 20));
	businessId->setClearButtonEnabled(true);
	businessId->setPlaceholderText(TXT_BUSINESSID);
	businessId->setValidator(valDigit);

	lbltype->setGeometry(QRect(5, 80, 150, 20));
	rbInteger->setGeometry(QRect(10, 100, 82, 17));
	rbIban->setGeometry(QRect(85, 100, 82, 17));
	userid->setGeometry(QRect(5, 120, 150, 20));
}

NewEnterpriseAccountDialog::~NewEnterpriseAccountDialog()
{
	//
}

QString NewEnterpriseAccountDialog::getCompanyName()
{
	return companyName->text();
}

QString NewEnterpriseAccountDialog::getBusinessId()
{
	return businessId->text();
}

void NewEnterpriseAccountDialog::onOkButtonPressed()
{
	//std::cout << "NewEnterpriseAccountDialog::onOkButtonPressed()" << std::endl; // DEBUG

	int pos = 0;

	QString cname = getCompanyName();
	int length = cname.length();

	if (length < 1 || valAlpha->validate(cname, pos) != QValidator::Acceptable)
	{
		std::cout << "ERROR: invalid company name length=" << length << std::endl; // DEBUG

		return;
	}

	QString buid = getBusinessId();
	length = buid.length();

	if (length < 1 || valDigit->validate(buid, pos) != QValidator::Acceptable)
	{
		std::cout << "ERROR: invalid business id length=" << length << std::endl; // DEBUG

		return;
	}

	QString uid = getUserId();
	length = uid.length();

	if (isIntegerChecked())
	{
		if (length < 1 || valDigit->validate(getUserId(), pos) != QValidator::Acceptable)
		{
			std::cout << "ERROR: invalid userid INT length=" << length << std::endl; // DEBUG

			return;
		}
	}
	else
	{
		if (length < 4 || valIban->validate(getUserId(), pos) != QValidator::Acceptable)
		{
			std::cout << "ERROR: invalid userid IBAN length=" << length << std::endl; // DEBUG

			return;
		}
	}

	accept();
}
