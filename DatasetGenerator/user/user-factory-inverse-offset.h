#ifndef DSG_USER_FACTORY_INVERSE_OFFSET_H
#define DSG_USER_FACTORY_INVERSE_OFFSET_H

#include "../user-factory.h"
#include "user-inverse-offset.h"

class UserFactoryInverseOffset: public UserFactory {
public:
	UserFactoryInverseOffset(int const scaleSize, double const certaintyStdDev, double const inverseProb, double const offsetStdDev);
   User *user();
	std::string toString() const;
private:
   std::normal_distribution<double> m_distribution;
   double const m_inverseProb;
   double const m_offsetStdDev;
};

#endif // DSG_USER_FACTORY_INVERSE_OFFSET_H
