#ifndef __UIH__
#define __UIH__

#include "eng.h"
#include <vector>
#include <qdialog.h>
#include <qmainwindow.h>
#include <qtableview.h>

class SymbioEng;
class QLabel;
class QWidget;
class QComboBox;
class QRadioButton;
class QStandardItemModel;

/*!
*  \brief     New account dialog
*  \details   Used to add new account
*/
class NewAccountDialog : public QDialog
{
public:
	NewAccountDialog(QWidget* parent = nullptr);
	virtual ~NewAccountDialog();
	/**
	* Initializes dialog UI
	* @param specs User account specs, e.g. description of available text input fields
	*/
	void setSpecs(const AccountSpecs& specs);
	/**
	* @return AccountSpecs Specification of accounts along with the user input data
	*/
	const AccountSpecs& getAccountSpecs() { return m_Specs; }
private:
	void onOkButtonPressed();
	const char* fieldToUiName(const char* n);
private:
	AccountSpecs m_Specs;
	QRegExpValidator* valAlpha;
	QRegExpValidator* valDigit;
	QRegExpValidator* valIban;
	std::vector<QLineEdit*> m_Fields;
};

/*!
*  \brief     User account details dialog
*  \details   Shows account data details
*/
class AccountDetailsDialog : public QDialog
{
	Q_OBJECT
public:
	AccountDetailsDialog(QWidget* parent = nullptr);
	virtual ~AccountDetailsDialog();
	/**
	* Initializes created read-only string
	* @param str Created time stamp
	*/
	void setCreated(const QString& str);
	/**
	* Initializes account type read-only string
	* @param str Account type, e.g. Customer or Enterprise
	*/
	void setAccountType(const QString& str);
	/**
	* Initializes first name read-only string
	* @param str First Name if account type is Customer
	*/
	void setFirstName(const QString& str);
	/**
	* Initializes last name read-only string
	* @param str Last Name if account type is Customer
	*/
	void setLastName(const QString& str);
	/**
	* Initializes company name read-only string
	* @param str Company Name if account type is Enterprise
	*/
	void setCompanyName(const QString& str);
	/**
	* Initializes company business id read-only string
	* @param str Company business ID if account type is Enterprise
	*/
	void setBusinessId(const QString& str);
private:
	QLineEdit* leType;
	QLineEdit* leCreated;
	QLineEdit* leFirstName;
	QLineEdit* leLastName;
	QLineEdit* leCompanyName;
	QLineEdit* leBusinessid;
};

/*!
*  \brief     Main program
*  \details   User interface of the main program
*/
class SymbioUi : public QMainWindow
	, public EngineObserver
{
	Q_OBJECT
public:
	SymbioUi(QWidget* parent = nullptr);
	virtual ~SymbioUi();
	/**
	* Callback inherited from EngineObserver interface class
	* @param vec List of loaded user accounts, called upon engine's load accounts call is completed
	*/
	virtual void onAccountsLoadCompleted(const std::vector<Account>& vec);
private slots:
	void onAddButtonClicked();
	void onDelButtonClicked();
	void onDetailsButtonClicked();
	void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
private:
	void setupTableHeaders();
private:
	QTableView* table;
	QStandardItemModel* model;
	QComboBox* combo;
};

#endif // __UIH__
