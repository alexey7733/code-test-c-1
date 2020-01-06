#ifndef __ENGH__
#define __ENGH__

#include <map>
#include <vector>
#include <string>
#include "db.h"
#include "account.h"

// EngineObserver
class EngineObserver
{
public:
	virtual void onAccountsLoadCompleted(const std::vector<Account>& vec) = 0;
};

// SymbioEng
class SymbioEng : public DatabaseObserver
{
public:
	static SymbioEng* instance();
	void release();

	virtual ~SymbioEng();
	void setObserver(EngineObserver* obs) { observer = obs; }
	const std::map<AccountType, AccountSpecs>& getAccountSpecs() const { return specs; }

	bool deleteAccounts(const std::vector<std::string>& userids);
	bool createAccount(const AccountSpecs& spec);
	void loadAccounts();
	bool getAccountDetails(const std::string& uid, Account& det);

	// From LoadAccountsObserver
	virtual void onAccountLoaded(const Account& a);
	virtual void onAccountsLoadCompleted();

private:
	SymbioEng();
	void generateAccountSpecs();

private:
	static SymbioEng* singleton;

	SymbioDb* driver;
	EngineObserver* observer;
	std::vector<Account> accArray;
	std::map<AccountType, AccountSpecs> specs;
};

#endif // __ENGH__
