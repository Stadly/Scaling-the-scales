#ifndef DSG_USER_FACTORY_INVERSE_OFFSET_SCALED_H
#define DSG_USER_FACTORY_INVERSE_OFFSET_SCALED_H

#include "../user-factory.h"
#include "user-inverse-offset-scaled.h"

class UserFactoryInverseOffsetScaled: public UserFactory {
public:
	UserFactoryInverseOffsetScaled(int const scaleSize, double const certaintyStdDev, double const inverseProb);
   User *user();
	std::string toString() const;
private:
   std::uniform_int_distribution<int> m_scaleDist;
   std::uniform_real_distribution<double> m_inverseDist;
   double const m_inverseProb;
};

#endif // DSG_USER_FACTORY_INVERSE_OFFSET_SCALED_H
