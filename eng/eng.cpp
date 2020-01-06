#include "eng.h"
#include "account.h"
#include "userid.h"
//#include <iostream>

////////////////////////////////////////////////////////////////////////////////

SymbioEng* SymbioEng::singleton = nullptr;

////////////////////////////////////////////////////////////////////////////////

SymbioEng* SymbioEng::instance()
{
	if (!singleton)
	{
		singleton = new SymbioEng();
	}
	return singleton;
}

void SymbioEng::release()
{
	if (singleton)
		delete singleton;
	singleton = nullptr;
}

SymbioEng::SymbioEng()
	: driver(nullptr)
	, observer(nullptr)
{
	driver = new SymbioDb();

	generateAccountSpecs();
}

SymbioEng::~SymbioEng()
{
	delete driver;
}

bool SymbioEng::deleteAccounts(const std::vector<std::string>& userids)
{
	bool res = false;

	if (userids.size() > 0)
		res = driver->deleteAccounts(userids);

	return res;
}

bool SymbioEng::createAccount(const AccountSpecs& spec)
{
	AccountType type = spec.getAccountType();

	if (type == AccountType::None)
		return false;

	const std::vector<AccountField>& fields = spec.getFields();

	std::string userid;
	std::string firstname;
	std::string lastname;
	std::string companyname;
	std::string businessid;

	for (const AccountField& f : fields)
	{
		if (f.name.compare(FIELD_USERID) == 0)
			userid = f.value;
		if (f.name.compare(FIELD_FIRSTNAME) == 0)
			firstname = f.value;
		else if (f.name.compare(FIELD_LASTNAME) == 0)
			lastname = f.value;
		else if (f.name.compare(FIELD_COMPANYNAME) == 0)
			companyname = f.value;
		else if (f.name.compare(FIELD_BUSINESSID) == 0)
			businessid = f.value;
	}

	bool res = false;

	Account acc;
	acc.setUserId(userid);
	acc.setAccountType(type);

	if (AccountType::Customer == type)
	{
		acc.setFirstName(firstname);
		acc.setLastName(lastname);
	}
	else if (AccountType::Enterprise == type)
	{
		acc.setCompanyName(companyname);
		acc.setBusinessId(businessid);
	}

	res = driver->createAccount(acc);

	return res;
}

bool SymbioEng::getAccountDetails(const std::string& uid, Account& det)
{
	bool res = false;

	for (const Account& acc : accArray)
	{
		const std::string& uid2 = acc.getUserId();

		if (uid2.compare(uid) == 0)
		{
			res = driver->getAccountDetails(acc, det);

			return res;
		}
	}

	return res;
}

void SymbioEng::loadAccounts()
{
	accArray.clear(); // empty old stuff

	driver->loadAccounts(this);
}

void SymbioEng::onAccountLoaded(const Account& a)
{
	//std::cout << "SymbioEng::onAccountLoaded()" << std::endl; // DEBUG

	accArray.push_back(a);
}

void SymbioEng::onAccountsLoadCompleted()
{
	//std::cout << "SymbioEng::onAccountsLoadCompleted()" << std::endl; // DEBUG

	if (observer)
		observer->onAccountsLoadCompleted(accArray);
}

void SymbioEng::generateAccountSpecs()
{
	specs[AccountType::Customer] = AccountSpecs(AccountType::Customer);
	specs[AccountType::Enterprise] = AccountSpecs(AccountType::Enterprise);
}
