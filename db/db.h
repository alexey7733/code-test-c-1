#ifndef __DBH__
#define __DBH__

#include <string>
#include <vector>

struct sqlite3;

class Account;
class AccountDetails;
class AccountCustomer;
class AccountEnterprise;

// LoadAccountsObserver
class LoadAccountsObserver
{
public:
	virtual void onAccountLoaded(const Account& a) = 0;
	virtual void onAccountsLoadCompleted() = 0;
};

// SymbioDb
class SymbioDb
{
public:
	SymbioDb();
	virtual ~SymbioDb();
	bool createAccount(const Account& a);
	bool deleteAccount(const Account& a);
	bool deleteAccounts(const std::vector<Account>& vec);
	bool deleteAccounts(const std::vector<std::string>& vec);
	void loadAccounts(LoadAccountsObserver* obs);
	bool getAccountDetails(const Account& a, AccountDetails& dt);
private:
	void dropTables();
	void createTables();
	bool createCustomerAccount(const Account& a);
	bool createEnterpriseAccount(const Account& a);
private:
	sqlite3* db;
};

#endif // __DBH__
