#ifndef __UIH__
#define __UIH__

#include "eng.h"
#include <string>
#include <qdialog.h>
#include <qmainwindow.h>
#include <qtableview.h>

class SymbioEng;
class QLabel;
class QWidget;
class QComboBox;
class QRadioButton;
class QStandardItemModel;

// AccountDetailsDialog
class AccountDetailsDialog : public QDialog
{
	Q_OBJECT
public:
	AccountDetailsDialog(QWidget* parent = nullptr);
	virtual ~AccountDetailsDialog();
	void setCreated(time_t t);
	void setAccountType(Account::AccountType t);
	void setFirstName(const std::string& str);
	void setLastName(const std::string& str);
	void setCompanyName(const std::string& str);
	void setBusinessId(const std::string& str);
private:
	QLineEdit* leType;
	QLineEdit* leCreated;
	QLineEdit* leFirstName;
	QLineEdit* leLastName;
	QLineEdit* leCompanyName;
	QLineEdit* leBusinessid;
};

// NewAccountDialog
class NewAccountDialog : public QDialog
{
	Q_OBJECT
public:
	NewAccountDialog(QWidget* parent = nullptr);
	virtual ~NewAccountDialog();
	QString getUserId();
	bool isIbanChecked();
	bool isIntegerChecked();
protected slots:
	virtual void onOkButtonPressed();
	virtual void onRadioIbanClicked();
	virtual void onRadioIntegerClicked();
protected:
	QPushButton* btnOk;
	QRegExpValidator* valAlpha;
	QRegExpValidator* valDigit;
	QRegExpValidator* valIban;
	QLabel* lbltype;
	QLineEdit* userid;
	QRadioButton* rbIban;
	QRadioButton* rbInteger;
};

// NewCustomerAccount
class NewCustomerAccountDialog : public NewAccountDialog
{
	Q_OBJECT
public:
	NewCustomerAccountDialog(QWidget* parent = nullptr);
	virtual ~NewCustomerAccountDialog();
	QString getFirstName();
	QString getLastName();
	void setFirstName(const QString& str = "");
	void setLastName(const QString& str = "");
	void setUserId(const QString& str = "");
private slots:
	virtual void onOkButtonPressed();
private:
	QLineEdit* lastName;
	QLineEdit* firstName;
};

// NewEnterpriseAccountDialog
class NewEnterpriseAccountDialog : public NewAccountDialog
{
	Q_OBJECT
public:
	NewEnterpriseAccountDialog(QWidget* parent = nullptr);
	virtual ~NewEnterpriseAccountDialog();
	QString getCompanyName();
	QString getBusinessId();
private slots:
	virtual void onOkButtonPressed();
private:
	QLineEdit* companyName;
	QLineEdit* businessId;
};

// SymbioUi
class SymbioUi : public QMainWindow
	, public EngineObserver
{
	Q_OBJECT
public:
	SymbioUi(QWidget* parent = nullptr);
	virtual ~SymbioUi();
	// From EngineObserver
	virtual void onAccountsLoadCompleted(const std::vector<Account>& vec);
private slots:
	void onAddButtonClicked();
	void onDelButtonClicked();
	void onDetailsButtonClicked();
	void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
private:
	void setupTableHeaders();
	void showNewCustomerDialog();
	void showNewEnterpriseDialog();
private:
	SymbioEng* engine;
	QTableView* table;
	QStandardItemModel* model;
	QComboBox* combo;
};

#endif // __UIH__
