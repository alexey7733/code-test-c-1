#ifndef __ACCOUNTH__
#define __ACCOUNTH__

#include <time.h>
#include <string>
#include <vector>

static const char* FIELD_TYPE = "type";
static const char* FIELD_USERID = "userid";
static const char* FIELD_CREATED = "created";
static const char* FIELD_LASTNAME = "lastname";
static const char* FIELD_FIRSTNAME = "firstname";
static const char* FIELD_BUSINESSID = "businessid";
static const char* FIELD_COMPANYNAME = "companyname";

class UserId;

// AccountType
enum AccountType
{
	None = 0,
	Customer,
	Enterprise
};

// Account
class Account
{
public:
	Account() : type(AccountType::None), created(0) {}
	time_t getCreated() const { return created; }
	AccountType getAccountType() const { return type; }
	const std::string& getUserId() const { return userid; }
	const std::string& getFirstName() const { return firstName; }
	const std::string& getLastName() const { return lastName; }
	const std::string& getCompanyName() const { return companyName; }
	const std::string& getBusinessId() const { return businessId; }

	void setCreated(time_t t) { created = t; }
	void setAccountType(AccountType t) { type = t; }
	void setUserId(const std::string& u) { userid = u; }
	void setLastName(const std::string& n) { lastName = n; }
	void setFirstName(const std::string& n) { firstName = n; }
	void setCompanyName(const std::string& n) { companyName = n; }
	void setBusinessId(const std::string& n) { businessId = n; }

private:
	AccountType type;
	time_t created;
	std::string userid;
	std::string lastName;
	std::string firstName;
	std::string companyName;
	std::string businessId;
};

// AccountField
class AccountField
{
public:
	enum FieldType
	{
		Integer = 0,
		String,
		Iban
	};
public:
	AccountField() : type(Integer), required(false) {}
	AccountField(FieldType t, bool r, const std::string& n) : type(t), required(r), name(n) {}
	AccountField(const AccountField& acc);
	AccountField& operator=(const AccountField& acc);
public:
	FieldType type;
	bool required;
	std::string name;
	std::string value;
};

// AccountSpecs
class AccountSpecs
{
public:
	AccountSpecs() : type(AccountType::None) {}
	AccountSpecs(const AccountSpecs& acc);
	AccountSpecs& operator=(const AccountSpecs& acc);
	AccountSpecs(AccountType t);
	virtual ~AccountSpecs() {}
	AccountType getAccountType() const { return type; }
	std::vector<AccountField>& getFields() { return fields; }
	const std::vector<AccountField>& getFields() const { return fields; }
protected:
	virtual void generateFields();
protected:
	AccountType type;
	std::vector<AccountField> fields;
};

#endif // __ACCOUNTH__
