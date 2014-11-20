#ifndef DSG_USER_INVERSE_OFFSET_H
#define DSG_USER_INVERSE_OFFSET_H

#include "../user.h"

class UserInverseOffset: public User {
public:
	UserInverseOffset(UserFactory *const factory, bool const inverse, int const offset) : m_inverse(inverse), m_offset(offset), User(factory) {}
   int rating(double const itemAvg);
private:
   bool const m_inverse;
   signed char const m_offset;
};

#endif // DSG_USER_INVERSE_OFFSET_H
