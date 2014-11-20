#include "user-factory.h"

/**
 * Class for generating users
 * 
 * @param name Name of the generator
 * @param scaleSize Size of the rating scale
 * @param certaintyStdDev Standard deviation representing the randomness of ratings
 */
UserFactory::UserFactory(std::string const name, int const scaleSize, double const certaintyStdDev) : m_name(name), scaleSize(scaleSize), m_certaintyStdDev(certaintyStdDev) {
   m_generator = std::default_random_engine(time(0));
} // UserFactory

/**
 * @param itemAvg Average rating value for the item
 * @return A generated rating for the item
 */
double UserFactory::rating(int itemAvg) {
   return std::normal_distribution<double>(itemAvg, m_certaintyStdDev)(m_generator);
} // rating

/**
 * @return A string representing the parameters used to create users
 */
std::string UserFactory::toString() const {
	return m_name+"-"+patch::to_string(m_certaintyStdDev);
} // toString
