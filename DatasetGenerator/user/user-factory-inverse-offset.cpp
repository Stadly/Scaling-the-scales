#include "user-factory-inverse-offset.h"

/**
 * Class for generating users conforming to the model used by IRA
 * 
 * @param scaleSize Size of the rating scale
 * @param certaintyStdDev Standard deviation representing the randomness of ratings
 * @param inverseProb Probability of a user being a contrarian
 * @param offsetStdDev Standard deviation for the offset the users use when rating
 */
UserFactoryInverseOffset::UserFactoryInverseOffset(int const scaleSize, double const certaintyStdDev, double const inverseProb, double const offsetStdDev) : m_inverseProb(inverseProb), m_offsetStdDev(offsetStdDev), UserFactory("UserInverseOffset", scaleSize, certaintyStdDev) {
   m_generator = std::default_random_engine(time(0));
   m_distribution = std::normal_distribution<double>(0, offsetStdDev);
   srand(time(0));
} // UserFactoryInverseOffset

/**
 * @return A generated user
 */
User *UserFactoryInverseOffset::user() {
   bool inverse = (double)rand()/RAND_MAX < m_inverseProb;
   int offset = m_distribution(m_generator);
   
   return new UserInverseOffset(this, inverse, offset);
} // user

/**
 * @return A string representing the parameters used to create users
 */
std::string UserFactoryInverseOffset::toString() const {
	return UserFactory::toString()+"-"+patch::to_string(m_inverseProb)+"-"+patch::to_string(m_offsetStdDev);
} // toString
