#include "eng.h"
#include <iostream>

SymbioEng::SymbioEng()
	: driver(nullptr)
	, observer(nullptr)
{
	driver = new SymbioDb();
}

SymbioEng::~SymbioEng()
{
	delete driver;
}

bool SymbioEng::deleteAccount(const Account& acc)
{
	bool res = false;

	if (driver)
		res = driver->deleteAccount(acc);

	return res;
}

bool SymbioEng::deleteAccounts(const std::vector<std::string>& userids)
{
	bool res = false;

	// TODO: skip loop and provide id array directly to db driver ?

	if (driver)
	{
		/*std::vector<Account> vec;

		for (const std::string& uid : userids)
		{
			for (const Account& acc : accArray)
			{
				const std::string uid2 = acc.getUserId()->toString();

				if (uid2.compare(uid) == 0)
				{
					vec.push_back(acc);
					break;
				}
			}
		}

		if (vec.size() > 0)
			res = driver->deleteAccounts(vec);*/

		if (userids.size() > 0)
			res = driver->deleteAccounts(userids);
	}

	return res;
}

bool SymbioEng::createAccount(const Account& acc)
{
	bool res = false;

	if (driver)
		res = driver->createAccount(acc);

	return res;
}

bool SymbioEng::getAccountDetails(const Account& acc, AccountDetails& det)
{
	bool res = false;

	if (driver)
		res = driver->getAccountDetails(acc, det);

	return res;
}

void SymbioEng::loadAccounts()
{
	accArray.clear(); // empty old stuff

	if (driver)
		driver->loadAccounts(this);
}

void SymbioEng::onAccountLoaded(const Account& a)
{
	//std::cout << "SymbioEng::onAccountLoaded()" << std::endl; // DEBUG

	accArray.push_back(a);
}

void SymbioEng::onAccountsLoadCompleted()
{
	std::cout << "SymbioEng::onAccountsLoadCompleted()" << std::endl; // DEBUG

	if (observer)
		observer->onAccountsLoadCompleted(accArray);
}
