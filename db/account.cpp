#include "account.h"
#include "userid.h"

////////////////////////////////////////////////////////////////////////////////

// Account
Account::Account()
	: userid(nullptr)
	, created(0)
	, type(None)
{
	//
}

Account::Account(AccountType t, int uid)
	: userid(nullptr)
	, created(0)
	, type(t)
{
	userid = new UserIdInt(uid);
}

Account::Account(AccountType t, const std::string& uid)
	: userid(nullptr)
	, created(0)
	, type(t)
{
	userid = new UserIdStr(uid);
}

Account::~Account()
{
	delete userid;
}

Account::Account(const Account& acc)
	: userid(nullptr)
	, created(acc.created)
	, type(acc.type)
{
	if (acc.userid->getType() == UserId::UserIdType::Integer)
		userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
	else
		userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
}

Account& Account::operator=(const Account& acc)
{
	if (this != &acc)
	{
		type = acc.type;
		created = acc.created;

		if (userid)
			delete userid;

		if (acc.userid->getType() == UserId::UserIdType::Integer)
			userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
		else
			userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

AccountCustomer::AccountCustomer(int uid)
	: Account(Customer, uid)
{
	//
}

AccountCustomer::AccountCustomer(const std::string& uid)
	: Account(Customer, uid)
{
	//
}

AccountCustomer::~AccountCustomer()
{
	//
}

AccountCustomer::AccountCustomer(const AccountCustomer& acc)
{
	type = acc.type;
	created = acc.created;
	lastName = acc.lastName;
	firstName = acc.firstName;

	if (userid)
		delete userid;

	if (acc.userid->getType() == UserId::UserIdType::Integer)
		userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
	else
		userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
}

AccountCustomer& AccountCustomer::operator=(const AccountCustomer& acc)
{
	if (this != &acc)
	{
		type = acc.type;
		created = acc.created;
		lastName = acc.lastName;
		firstName = acc.firstName;

		if (userid)
			delete userid;

		if (acc.userid->getType() == UserId::UserIdType::Integer)
			userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
		else
			userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
	}
	return *this;
}

const std::string& AccountCustomer::getLastName() const
{
	return lastName;
}

void AccountCustomer::setLastName(const std::string& n)
{
	lastName = n;
}

const std::string& AccountCustomer::getFirstName() const
{
	return firstName;
}

void AccountCustomer::setFirstName(const std::string& n)
{
	firstName = n;
}

////////////////////////////////////////////////////////////////////////////////

AccountEnterprise::AccountEnterprise(int uid)
	: Account(Enterprise, uid)
{
	//
}

AccountEnterprise::AccountEnterprise(const std::string& uid)
	: Account(Enterprise, uid)
{
	//
}

AccountEnterprise::~AccountEnterprise()
{
	//
}

AccountEnterprise::AccountEnterprise(const AccountEnterprise& acc)
{
	type = acc.type;
	created = acc.created;
	name = acc.name;
	businessId= acc.businessId;

	if (userid)
		delete userid;

	if (acc.userid->getType() == UserId::UserIdType::Integer)
		userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
	else
		userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
}

AccountEnterprise& AccountEnterprise::operator=(const AccountEnterprise& acc)
{
	if (this != &acc)
	{
		type = acc.type;
		created = acc.created;
		name = acc.name;
		businessId = acc.businessId;

		if (userid)
			delete userid;

		if (acc.userid->getType() == UserId::UserIdType::Integer)
			userid = new UserIdInt(static_cast<UserIdInt*>(acc.userid)->getUserId());
		else
			userid = new UserIdStr(static_cast<UserIdStr*>(acc.userid)->getUserId());
	}
	return *this;
}

const std::string& AccountEnterprise::getName() const
{
	return name;
}

void AccountEnterprise::setName(const std::string& n)
{
	name = n;
}

const std::string& AccountEnterprise::getBusinessid() const
{
	return businessId;
}

void AccountEnterprise::setBusinessid(const std::string& n)
{
	businessId = n;
}
