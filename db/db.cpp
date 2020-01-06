#include "db.h"
#include "userid.h"
#include "account.h"
#include "sqlite3.h"
#include <iostream>

SymbioDb::SymbioDb()
	: db(0)
{
	int res = sqlite3_open("symbioDemo.db", &db);

	if (res)
	{
		std::cout << "ERROR: can't open database" << std::endl;

		return;
	}

	//dropTables();

	createTables();
}

SymbioDb::~SymbioDb()
{
	if (db)
	{
		int res = sqlite3_close(db);

		if (res)
			std::cout << "ERROR: can't close database" << std::endl;
	}

	db = 0;
}

void SymbioDb::dropTables()
{
	//int res = 
	sqlite3_exec(db, "DROP TABLE account", 0, 0, 0);

	//std::cout << "drop table res=" << res << std::endl; // DEBUG
}

void SymbioDb::createTables()
{
	char* errMsg = 0;

	const char* SQL = "CREATE TABLE IF NOT EXISTS account ( \
		userid TEXT PRIMARY KEY NOT NULL, \
		type INTEGER NOT NULL, \
		created INTEGER NOT NULL, \
		firstname TEXT DEFAULT NULL, \
		lastname TEXT DEFAULT NULL, \
		companyname TEXT DEFAULT NULL, \
		businessid TEXT DEFAULT NULL \
		)";

	int res = sqlite3_exec(db, SQL, 0, 0, &errMsg);

	if (errMsg)
	{
		std::cout << "ERROR: " << errMsg << std::endl; // DEBUG
		sqlite3_free(errMsg);
		errMsg = 0;
	}
}

bool SymbioDb::createAccount(const Account& a)
{
	AccountType t = a.getAccountType();

	if (t == AccountType::Customer)
		return createCustomerAccount(a);
	else if (t == AccountType::Enterprise)
		return createEnterpriseAccount(a);

	return false;
}

bool SymbioDb::deleteAccounts(const std::vector<std::string>& vec)
{
	std::string SQL = "DELETE FROM account WHERE userid IN (";

	int size = (int)vec.size();

	for (int i = 0; i < size; ++i)
	{
		SQL += "\"";
		SQL += vec[i];
		SQL += "\"";

		if (i < size - 1)
			SQL += ",";
	}

	SQL += ")";

	std::cout << " SQL=" << SQL << std::endl; // DEBUG

	char* errMsg = 0;

	int res = sqlite3_exec(db, SQL.c_str(), NULL, NULL, &errMsg);

	if (errMsg)
	{
		std::cout << "ERROR " << errMsg << std::endl;
		sqlite3_free(errMsg);

		return false;
	}

	return true;
}

void SymbioDb::loadAccounts(DatabaseObserver* obs)
{
	std::string SQL = "SELECT userid, type, created FROM account";

	char* errMsg = 0;

	int rc = sqlite3_exec(db, SQL.c_str(), [](void *param, int argc, char **argv, char **colname)->int
	{
		DatabaseObserver* observer = static_cast<DatabaseObserver*>(param);

		Account acc;

		for (int i = 0; i < argc; i++)
		{
			//std::cout << colname[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl; // DEBUG

			if (strcmp(colname[i], "userid") == 0)
				acc.setUserId(argv[i] ? argv[i] : "");
			if (strcmp(colname[i], "type") == 0)
				acc.setAccountType(argv[i] ? (AccountType)std::atoi(argv[i]) : AccountType::None);
			else if (strcmp(colname[i], "created") == 0)
				acc.setCreated(argv[i] ? std::atol(argv[i]) : 0);
		}

		if (observer)
			observer->onAccountLoaded(acc);

		return 0;
	}
	, obs, &errMsg);

	if (errMsg)
	{
		std::cout << "ERROR: " << errMsg << std::endl; // DEBUG
		sqlite3_free(errMsg);
	}

	if (obs)
		obs->onAccountsLoadCompleted();
}

bool SymbioDb::getAccountDetails(const Account& acc, Account& dt)
{
	const std::string& uid = acc.getUserId();

	std::string SQL = "SELECT type, created, firstname, lastname, companyname, businessid FROM account WHERE userid=\"";
	SQL += uid;
	SQL += "\"";

	char* errMsg = 0;

	int rc = sqlite3_exec(db, SQL.c_str(), [](void *param, int argc, char **argv, char **colname)->int
	{
		Account* data = static_cast<Account*>(param);

		for (int i = 0; i < argc; i++)
		{
			//std::cout << colname[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl; // DEBUG

			if (strcmp(colname[i], "type") == 0)
				data->setAccountType(argv[i] ? (AccountType)std::atoi(argv[i]) : AccountType::None);
			else if (strcmp(colname[i], "created") == 0)
				data->setCreated(argv[i] ? std::atol(argv[i]) : 0);
			else if (strcmp(colname[i], "firstname") == 0)
				data->setFirstName(argv[i] ? argv[i] : "");
			else if (strcmp(colname[i], "lastname") == 0)
				data->setLastName(argv[i] ? argv[i] : "");
			else if (strcmp(colname[i], "companyname") == 0)
				data->setCompanyName(argv[i] ? argv[i] : "");
			else if (strcmp(colname[i], "businessid") == 0)
				data->setBusinessId(argv[i] ? argv[i] : "");
		}

		return 0;
	}
	, &dt, &errMsg);

	if (errMsg)
	{
		std::cout << "ERROR: " << errMsg << std::endl;
		sqlite3_free(errMsg);

		return false;
	}

	return true;
}

bool SymbioDb::createCustomerAccount(const Account& acc)
{
	//const AccountCustomer& acc = static_cast<const AccountCustomer&>(a);

	time_t now;
	time(&now);

	const std::string& uid = acc.getUserId();
	AccountType type = acc.getAccountType();
	const std::string& lastName = acc.getLastName();
	const std::string& firstName = acc.getFirstName();

	char *errMsg = 0;

	std::string SQL = "INSERT INTO account (created, userid, type, firstname, lastname) VALUES (";
	SQL += std::to_string(now);
	SQL += ", ";
	SQL += "\"" + uid + "\"";
	SQL += ", ";
	SQL += std::to_string(type);
	SQL += ", ";
	SQL += "\"" + firstName + "\"";
	SQL += ", ";
	SQL += "\"" + lastName + "\"";
	SQL += ")";

	//std::cout << "sql=" << SQL << std::endl; // DEBUG

	int res = sqlite3_exec(db, SQL.c_str(), NULL, NULL, &errMsg);

	if (errMsg)
	{
		std::cout << "SymbioDb::createCustomerAccount() ERROR: " << errMsg << std::endl;  // DEBUG
		sqlite3_free(errMsg);
		return false;
	}

	return true;
}

bool SymbioDb::createEnterpriseAccount(const Account& acc)
{
	//const AccountEnterprise& acc = static_cast<const AccountEnterprise&>(a);

	time_t now;
	time(&now);

	std::string uid = acc.getUserId();
	AccountType type = acc.getAccountType();
	const std::string& name = acc.getCompanyName();
	const std::string& businessid = acc.getBusinessId();

	char *errMsg = 0;

	std::string SQL = "INSERT INTO account (created, userid, type, companyname, businessid) VALUES (";
	SQL += std::to_string(now);
	SQL += ", ";
	SQL += "\"" + uid + "\"";
	SQL += ", ";
	SQL += std::to_string(type);
	SQL += ", ";
	SQL += "\"" + name + "\"";
	SQL += ", ";
	SQL += "\"" + businessid + "\"";
	SQL += ")";

	//std::cout << "sql=" << SQL << std::endl; // DEBUG

	int res = sqlite3_exec(db, SQL.c_str(), NULL, NULL, &errMsg);

	if (errMsg)
	{
		std::cout << "SymbioDb::createEnterpriseAccount() ERROR: " << errMsg << std::endl;  // DEBUG
		sqlite3_free(errMsg);
		return false;
	}

	return true;
}
