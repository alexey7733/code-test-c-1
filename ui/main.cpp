#include "main.h"
#include "ui.h"
#include "eng.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	SymbioUi ui;
	ui.show();

	return app.exec();

	/*char c = 0;

	SymbioDb* db = new SymbioDb();

	std::cout << "Hello Symbio!" << std::endl;

	AccountCustomer c1(1024);
	c1.setFirstName("alexey");
	c1.setLastName("zerkin");

	AccountCustomer c2("1024");
	c1.setFirstName("alexey");
	c1.setLastName("zerkin");

	AccountEnterprise e1("2048");
	e1.setName("alexsoft");
	e1.setBusinessid("fi20686425");

	db->createAccount(c1);
	db->createAccount(c2);
	db->createAccount(e1);

	AccountDetails dt;
	db->getAccountDetails(c1, dt);

	printAccountDetails(dt);

	db->getAccountDetails(c2, dt);

	printAccountDetails(dt);

	db->getAccountDetails(e1, dt);

	printAccountDetails(dt);

	std::cin >> c; // to stop

	delete db;
	db = 0;*/
}
