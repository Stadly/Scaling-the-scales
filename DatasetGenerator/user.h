#ifndef DSG_USER_H
#define DSG_USER_H

#include "user-factory.h"

class UserFactory;
class User {
public:
	User(UserFactory *const factory) : m_factory(factory) {}
   virtual int rating(double const itemAvg) = 0;
protected:
   UserFactory *const m_factory;
};

#endif // DSG_USER_H
