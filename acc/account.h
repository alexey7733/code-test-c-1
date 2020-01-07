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

/** AccountType: type of the user account
* * None used for default constructor to work
* * Customer
* * Enterprise
*/
enum AccountType
{
	None = 0,
	Customer,
	Enterprise
};

/*!
*  \brief     Data class, holds user account data
*  \details   This class is used by the majority of the software modules
*/
class Account
{
public:
	Account() : type(AccountType::None), created(0) {}
	/**
	* @return time_t Time stamp when account has been added to database
	*/
	time_t getCreated() const { return created; }
	/**
	* @return AccountType, e.g. Customer, Enterprise etc.
	*/
	AccountType getAccountType() const { return type; }
	/**
	* @return string User id
	*/
	const std::string& getUserId() const { return userid; }
	/**
	* @return string User First Name, if account type is Customer
	*/
	const std::string& getFirstName() const { return firstName; }
	/**
	* @return string User Last Name, if account type is Customer
	*/
	const std::string& getLastName() const { return lastName; }
	/**
	* @return string Company Name, if account type is Enterprise
	*/
	const std::string& getCompanyName() const { return companyName; }
	/**
	* @return string Business id, if account type is Enterprise
	*/
	const std::string& getBusinessId() const { return businessId; }

	/**
	* @param ts Time stamp of account creation
	*/
	void setCreated(time_t ts) { created = ts; }
	/**
	* @param t Account type, e.g. Customer, Enterprise etc.
	*/
	void setAccountType(AccountType t) { type = t; }
	/**
	* @param u User id
	*/
	void setUserId(const std::string& u) { userid = u; }
	/**
	* @param n User last name, if account type is Customer
	*/
	void setLastName(const std::string& n) { lastName = n; }
	/**
	* @param n User first name, if account type is Customer
	*/
	void setFirstName(const std::string& n) { firstName = n; }
	/**
	* @param n Company name, if account type is Enterprise
	*/
	void setCompanyName(const std::string& n) { companyName = n; }
	/**
	* @param n Business id, if account type is Enterprise
	*/
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

/*!
*  \brief     AccountField class
*  \details   This class is used to specify account data fields according to the database table schema.
*             This is mostly used to simplify new account creation in the main software module.
*             The class is also used by the main software module to hold the user input added in new account creation dialog.
*/
class AccountField
{
public:
	/** FieldType:
	*  * Integer - 32 bit integer
	*  * String - Aa-Zz
	*  * Iban - 34 chars long string with 2-letter prefix FI followed by 2 digits and the followed by 30 digits
	*/
	enum FieldType
	{
		Integer = 0,
		String,
		Iban
	};
public:
	AccountField() : type(Integer), required(false) {}
	/**
	* Constructor
	* @param type One of the account type enum
	* @param required Flag, defines if account field is required or not
	* @param name Account field name, matches 1-1 with the database table column name
	*/
	AccountField(FieldType type, bool required, const std::string& name) : type(type), required(required), name(name) {}
	AccountField(const AccountField& acc);
	AccountField& operator=(const AccountField& acc);
public:
	FieldType type;
	bool required;
	/** Account field name, matches 1-1 with the database table column name:
	* * static const char* FIELD_TYPE = "type";
	* * static const char* FIELD_USERID = "userid";
	* * static const char* FIELD_CREATED = "created";
	* * static const char* FIELD_LASTNAME = "lastname";
	* * static const char* FIELD_FIRSTNAME = "firstname";
	* * static const char* FIELD_BUSINESSID = "businessid";
	* * static const char* FIELD_COMPANYNAME = "companyname";
	*/
	std::string name;
	std::string value;
};

/*!
*  \brief     AccountSpecs class
*  \details   This class is used as collection of fields of different account types. This is so called specification of the user account.
*/
class AccountSpecs
{
public:
	AccountSpecs() : type(AccountType::None) {}
	AccountSpecs(const AccountSpecs& acc);
	AccountSpecs& operator=(const AccountSpecs& acc);
	AccountSpecs(AccountType t);
	virtual ~AccountSpecs() {}
	/**
	* @return AccountType The type of account (Customer, Enterprise etc.)
	*/
	AccountType getAccountType() const { return type; }
	/**
	* @return List of account fields, e.g. account specification according to database schema
	*/
	std::vector<AccountField>& getFields() { return fields; }
	const std::vector<AccountField>& getFields() const { return fields; }
protected:
	virtual void generateFields();
protected:
	AccountType type;
	std::vector<AccountField> fields;
};

#endif // __ACCOUNTH__
