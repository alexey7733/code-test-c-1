#ifndef __ENGH__
#define __ENGH__

#include <map>
#include <vector>
#include <string>
#include "db.h"
#include "account.h"

/*!
*  \brief     Engine callback interface class
*  \details   This class is used SymbioEng to signal completion of database operations
*/
class EngineObserver
{
public:
	/**
	* Called upon account load database opertation is completed
	* @param vec List of loaded accounts
	*/
	virtual void onAccountsLoadCompleted(const std::vector<Account>& vec) = 0;
};

/*!
*  \brief     Engine class, singleton
*  \details   This class plays an intermediate role between the main program and the database storage
*/
class SymbioEng : public DatabaseObserver
{
public:
	/**
	* Static function, creates and returns class object
	* @return Instance of the class, creates
	*/
	static SymbioEng* instance();

	/**
	* Deallocates resources used by the class
	*/
	void release();

	virtual ~SymbioEng();

	/**
	* Sets engine observer
	* @param obs An engine observer
	*/
	void setObserver(EngineObserver* obs) { observer = obs; }

	/**
	* @return Map of available account specs, where account type is used as the map key and specsas the map value
	*/
	const std::map<AccountType, AccountSpecs>& getAccountSpecs() const { return specs; }

	/**
	* @param userids List of account id's to delete
	* @return bool TRUE on success, FALSE otherwise
	*/
	bool deleteAccounts(const std::vector<std::string>& userids);
	/**
	* @param spec An account specification filled with data to be stored in database
	* @return bool TRUE on success, FALSE otherwise
	*/
	bool createAccount(const AccountSpecs& spec);
	/**
	* Loads all accounts from the database. onAccountLoaded() and onAccountsLoadCompleted() callbacks will be called during function call.
	*/
	void loadAccounts();
	/**
	* Feteches account details specified by account userid
	* @param uid Account user id to fetch 
	*/
	bool getAccountDetails(const std::string& uid, Account& det);

	// From LoadAccountsObserver

	/**
	* Callback, used by database to signal that accounts were loaded
	*/

	/**
	* Callback, used by database to signal that new account has been loaded
	* @param Account loaded user account
	*/
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
