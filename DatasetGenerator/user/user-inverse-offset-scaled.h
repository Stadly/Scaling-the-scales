#ifndef DSG_USER_INVERSE_OFFSET_SCALED_H
#define DSG_USER_INVERSE_OFFSET_SCALED_H

#include "../user.h"

class UserInverseOffsetScaled: public User {
public:
	UserInverseOffsetScaled(UserFactory *const factory, int const min, int const max) : m_min(min), m_max(max), User(factory) {}
   int rating(double const itemAvg);
private:
   signed char const m_min;
   signed char const m_max;
};

#endif // DSG_USER_INVERSE_OFFSET_SCALED_H
