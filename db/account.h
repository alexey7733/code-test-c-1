#ifndef __ACCOUNTH__
#define __ACCOUNTH__

#include <time.h>
#include <string>

class UserId;

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

#endif // __ACCOUNTH__
