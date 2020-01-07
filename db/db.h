#ifndef __DBH__
#define __DBH__

#include <string>
#include <vector>

struct sqlite3;

class Account;
class AccountCustomer;
class AccountEnterprise;

/*!
*  \brief     Databse observer interface class
*  \details   This class is used by SymbioDb class to signal completion of database operations
*/
class DatabaseObserver
{
public:
	/**
	* Called when new account is loaded
	* @param a Loaded account
	*/
	virtual void onAccountLoaded(const Account& a) = 0;
	/**
	* Called upon account load database opertation is completed
	*/
	virtual void onAccountsLoadCompleted() = 0;
};

/*!
*  \brief     Databse driver class
*  \details   This class uses SQLite database to store account data
*/
class SymbioDb
{
public:
	SymbioDb();
	virtual ~SymbioDb();

	/**
	* Creates new user account
	* @param account User account data for the new account
	* @return bool TRUE on success, FALSE otherwise
	*/
	bool createAccount(const Account& account);

	/**
	* Deletes accounts
	* @param List of user account id's to delete
	* @return bool TRUE on success, FALSE otherwise
	*/
	bool deleteAccounts(const std::vector<std::string>& vec);

	/**
	* Loads accounts from database
	* @param Callback interface to signal upon database operation completion
	*/
	void loadAccounts(DatabaseObserver* obs);

	/**
	* Fetches account details
	* @param account An account to fetch
	* @param result An account where to store fetched results
	* @return bool TRUE on success, FALSE otherwise
	*/
	bool getAccountDetails(const Account& account, Account& result);
private:
	void dropTables();
	void createTables();
	bool createCustomerAccount(const Account& a);
	bool createEnterpriseAccount(const Account& a);
private:
	sqlite3* db;
};

#endif // __DBH__
