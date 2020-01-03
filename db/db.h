#ifndef __DBH__
#define __DBH__

#include <time.h>
#include <string>
#include <vector>

struct sqlite3;

////////////////////////////////////////////////////////////////////////////////

// UserId
class UserId
{
public:
	enum UserIdType
	{
		Integer = 0,
		Iban
	};
	virtual ~UserId() {}
	virtual UserIdType getType() = 0;
	virtual std::string toString() const = 0;
	//virtual bool operator==(const UserId& u) { return toString().compare(u.toString()) == 0; }
};

// UserIdInt
class UserIdInt : public UserId
{
public:
	UserIdInt() : uid(0) {}
	UserIdInt(int data) : uid(data) {}
	virtual ~UserIdInt() {}
	int getUserId() { return uid; }
	void setUserId(int data) { uid = data; }
	// From UserId
	virtual UserIdType getType() { return UserIdType::Integer; }
	virtual std::string toString() const { return std::to_string(uid); }
private:
	int uid;
};

// UserIdStr
class UserIdStr : public UserId
{
public:
	UserIdStr() : uid("") {}
	UserIdStr(const std::string& data) : uid(data) {}
	virtual ~UserIdStr() {}
	const std::string& getUserId() { return uid; }
	void setUserId(const std::string& data) { uid = data; }
	// From UserId
	virtual UserIdType getType() { return UserIdType::Iban; }
	virtual std::string toString() const { return uid; }
private:
	std::string uid;
};

////////////////////////////////////////////////////////////////////////////////

// Account
class Account
{
public:
	enum AccountType
	{
		None = 0,
		Customer,
		Enterprise
	};
public:
	Account();
	Account(AccountType type, int uid);
	Account(AccountType type, const std::string& uid);
	Account(const Account& acc);
	Account& operator=(const Account& acc);
	virtual ~Account();
	virtual AccountType getAccountType() const { return type; }
	time_t getCreated() const { return created; }
	void setCreated(time_t t) { created = t; }
	const UserId* getUserId() const { return userid; }
protected:
	UserId* userid;
	time_t created;
	AccountType type;
};

// AccountCustomer
class AccountCustomer : public Account
{
public:
	virtual ~AccountCustomer();
	AccountCustomer(int uid);
	AccountCustomer(const std::string& uid);
	AccountCustomer(const AccountCustomer& acc);
	AccountCustomer& operator=(const AccountCustomer& acc);
	const std::string& getLastName() const;
	void setLastName(const std::string& n);
	const std::string& getFirstName() const;
	void setFirstName(const std::string& n);
private:
	std::string lastName;
	std::string firstName;
};

// AccountEnterprise
class AccountEnterprise : public Account
{
public:
	virtual ~AccountEnterprise();
	AccountEnterprise(int uid);
	AccountEnterprise(const std::string& uid);
	AccountEnterprise(const AccountEnterprise& acc);
	AccountEnterprise& operator=(const AccountEnterprise& acc);
	const std::string& getName() const;
	void setName(const std::string& n);
	const std::string& getBusinessid() const;
	void setBusinessid(const std::string& n);
private:
	std::string name;
	std::string businessId;
};

////////////////////////////////////////////////////////////////////////////////

// AccountDetails
class AccountDetails
{
public:
	AccountDetails() : type(Account::AccountType::None), created(0) {}
	Account::AccountType type;
	time_t created;
	std::string lastName;
	std::string firstName;
	std::string companyName;
	std::string businessId;
};

////////////////////////////////////////////////////////////////////////////////

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
