#include "user-factory-inverse-offset-scaled.h"

/**
 * Class for generating users conforming to the model used by Scaling the scales
 * 
 * @param scaleSize Size of the rating scale
 * @param certaintyStdDev Standard deviation representing the randomness of ratings
 * @param inverseProb Probability of a user being a contrarian
 */
UserFactoryInverseOffsetScaled::UserFactoryInverseOffsetScaled(int const scaleSize, double const certaintyStdDev, double const inverseProb) : m_inverseProb(inverseProb), UserFactory("UserInverseOffsetScaled", scaleSize, certaintyStdDev) {
   m_scaleDist = std::uniform_int_distribution<int>(0, scaleSize-1);
   m_inverseDist = std::uniform_real_distribution<double>(0, 1);
} // UserFactoryInverseOffsetScaled

/**
 * @return A generated user
 */
User *UserFactoryInverseOffsetScaled::user() {
   bool const inverse = m_inverseDist(m_generator) < m_inverseProb;
   
   int const size = m_scaleDist(m_generator);
   int const start = std::uniform_int_distribution<int>(1, scaleSize-size)(m_generator);
   
   int const min = inverse ? start+size : start;
   int const max = inverse ? start : start+size;
   
   return new UserInverseOffsetScaled(this, min, max);
} // user

/**
 * @return A string representing the parameters used to create users
 */
std::string UserFactoryInverseOffsetScaled::toString() const {
	return UserFactory::toString()+"-"+patch::to_string(m_inverseProb);
} // toString
