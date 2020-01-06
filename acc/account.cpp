#include "account.h"
#include "userid.h"

////////////////////////////////////////////////////////////////////////////////

AccountField::AccountField(const AccountField& acc)
	: type(Integer)
	, required(false)
{
	type = acc.type;
	name = acc.name;
	value = acc.value;
	required = acc.required;
}

AccountField& AccountField::operator=(const AccountField& acc)
{
	if (this != &acc)
	{
		type = acc.type;
		name = acc.name;
		value = acc.value;
		required = acc.required;
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

AccountSpecs::AccountSpecs(AccountType t)
	: type(t)
{
	generateFields();
}

AccountSpecs::AccountSpecs(const AccountSpecs& acc)
	: type(acc.type)
{
	fields.clear();
	generateFields();
}

AccountSpecs& AccountSpecs::operator=(const AccountSpecs& acc)
{
	if (this != &acc)
	{
		type = acc.type;

		fields.clear();
		generateFields();
	}
	return *this;
}

void AccountSpecs::generateFields()
{
	if (type == AccountType::Customer)
	{
		AccountField fUserid(AccountField::FieldType::Integer, true, FIELD_USERID);
		AccountField fLastName(AccountField::FieldType::String, true, FIELD_LASTNAME);
		AccountField fFirstName(AccountField::FieldType::String, true, FIELD_FIRSTNAME);

		fields.push_back(fUserid);
		fields.push_back(fFirstName);
		fields.push_back(fLastName);
	}
	else if (type == AccountType::Enterprise)
	{
		AccountField fUserid(AccountField::FieldType::Iban, true, FIELD_USERID);
		AccountField fBusinessId(AccountField::FieldType::Integer, true, FIELD_BUSINESSID);
		AccountField fCompanyName(AccountField::FieldType::String, true, FIELD_COMPANYNAME);

		fields.push_back(fUserid);
		fields.push_back(fCompanyName);
		fields.push_back(fBusinessId);
	}
}
