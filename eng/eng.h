#ifndef __ENGH__
#define __ENGH__

#include <vector>
#include "db.h"

class Account;
class AccountDetails;

// EngineObserver
class EngineObserver
{
public:
	virtual void onAccountsLoadCompleted(const std::vector<Account>& vec) = 0;
};

// SymbioEng
class SymbioEng : public LoadAccountsObserver
{
public:
	SymbioEng();
	virtual ~SymbioEng();
	void setObserver(EngineObserver* obs) { observer = obs; }

	bool deleteAccount(const Account& acc);
	bool deleteAccounts(const std::vector<std::string>& userids);
	bool createAccount(const Account& acc);
	void loadAccounts();
	bool getAccountDetails(const Account& acc, AccountDetails& det);

	// From LoadAccountsObserver
	virtual void onAccountLoaded(const Account& a);
	virtual void onAccountsLoadCompleted();

private:
	SymbioDb* driver;
	EngineObserver* observer;
	std::vector<Account> accArray;
};

#endif // __ENGH__
