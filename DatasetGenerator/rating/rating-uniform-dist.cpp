#include "rating-uniform-dist.h"

/**
 * Class for generating uniformly distributed ratings
 * 
 * @param scaleSize Size of the rating scale
 */
RatingUniformDist::RatingUniformDist(int const scaleSize) : RatingFactory("uniform", scaleSize) {
   m_generator = std::default_random_engine(time(0));
   m_distribution = std::uniform_real_distribution<double>(1, scaleSize);
} // RatingUniformDist

/**
 * @return A generated rating
 */
double RatingUniformDist::rating() {
   return m_distribution(m_generator);
} // rating

/**
 * @return A string representing the parameters used to create ratings
 */
std::string RatingUniformDist::toString() const {
	return m_name;
} // toString
