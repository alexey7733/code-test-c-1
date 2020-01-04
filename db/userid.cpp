#include "userid.h"

////////////////////////////////////////////////////////////////////////////////

UserIdInt::UserIdInt()
	: uid(0)
{
	//
}

UserIdInt::UserIdInt(int data)
	: uid(data)
{
	//
}

int UserIdInt::getUserId()
{
	return uid;
}

void UserIdInt::setUserId(int data)
{
	uid = data;
}

UserId::UserIdType UserIdInt::getType()
{
	return UserIdType::Integer;
}

std::string UserIdInt::toString() const
{
	return std::to_string(uid);
}

////////////////////////////////////////////////////////////////////////////////

UserIdStr::UserIdStr()
	: uid("")
{
	//
}

UserIdStr::UserIdStr(const std::string& data)
	: uid(data)
{
	//
}

const std::string& UserIdStr::getUserId()
{
	return uid;
}

void UserIdStr::setUserId(const std::string& data)
{
	uid = data;
}

UserId::UserIdType UserIdStr::getType()
{
	return UserIdType::Iban;
}

std::string UserIdStr::toString() const
{
	return uid;
}
