#ifndef __UIH__
#define __UIH__

#include "eng.h"
#include <qdialog.h>
#include <qmainwindow.h>
#include <qtableview.h>

class SymbioEng;
class QLabel;
class QWidget;
class QComboBox;
class QRadioButton;
class QStandardItemModel;

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
	//QString getUserId();
	void setFirstName(const QString& str = "");
	void setLastName(const QString& str = "");
	void setUserId(const QString& str = "");
	//bool isIbanChecked();
	//bool isIntegerChecked();
private slots:
	virtual void onOkButtonPressed();
	//void onRadioIbanClicked();
	//void onRadioIntegerClicked();
private:
	//QLineEdit* userid;
	QLineEdit* lastName;
	QLineEdit* firstName;
	//QRadioButton* rbIban;
	//QRadioButton* rbInteger;
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
