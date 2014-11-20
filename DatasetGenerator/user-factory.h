#ifndef DSG_USER_FACTORY_H
#define DSG_USER_FACTORY_H

#include "patch.h"
#include "user.h"
#include <random>
#include <time.h>
#include <iostream>

class User;
class UserFactory {
public:
   UserFactory(std::string const name, int const scaleSize, double const certaintyStdDev);
   virtual User *user() = 0;
   virtual std::string toString() const;
   double rating(int itemAvg);
	signed char const scaleSize;
protected:
   double const m_certaintyStdDev;
   std::default_random_engine m_generator;
	std::string const m_name;
};

#endif // DSG_USER_FACTORY_H
