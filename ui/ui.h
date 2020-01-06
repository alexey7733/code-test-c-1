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

// NewAccountDialog
class NewAccountDialog : public QDialog
{
public:
	NewAccountDialog(QWidget* parent = nullptr);
	virtual ~NewAccountDialog();
	void setSpecs(const AccountSpecs& specs);
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

// AccountDetailsDialog
class AccountDetailsDialog : public QDialog
{
	Q_OBJECT
public:
	AccountDetailsDialog(QWidget* parent = nullptr);
	virtual ~AccountDetailsDialog();
	void setCreated(const QString& str);
	void setAccountType(const QString& str);
	void setFirstName(const QString& str);
	void setLastName(const QString& str);
	void setCompanyName(const QString& str);
	void setBusinessId(const QString& str);
private:
	QLineEdit* leType;
	QLineEdit* leCreated;
	QLineEdit* leFirstName;
	QLineEdit* leLastName;
	QLineEdit* leCompanyName;
	QLineEdit* leBusinessid;
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
private:
	QTableView* table;
	QStandardItemModel* model;
	QComboBox* combo;
};

#endif // __UIH__
