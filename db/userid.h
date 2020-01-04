#ifndef __USERIDH__
#define __USERIDH__

#include <string>

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
};

// UserIdInt
class UserIdInt : public UserId
{
public:
	UserIdInt();
	UserIdInt(int data);
	virtual ~UserIdInt() {}
	int getUserId();
	void setUserId(int data);
	// From UserId
	virtual UserIdType getType();
	virtual std::string toString() const;
private:
	int uid;
};

// UserIdStr
class UserIdStr : public UserId
{
public:
	UserIdStr();
	UserIdStr(const std::string& data);
	virtual ~UserIdStr() {}
	const std::string& getUserId();
	void setUserId(const std::string& data);
	// From UserId
	virtual UserIdType getType();
	virtual std::string toString() const;
private:
	std::string uid;
};

#endif // __USERIDH__
