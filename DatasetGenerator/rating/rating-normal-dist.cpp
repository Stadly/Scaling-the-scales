#include "rating-normal-dist.h"

/**
 * Class for generating normally distributed ratings
 * 
 * @param scaleSize Size of the rating scale
 * @param stdDev Standard deviation of the normal distribution
 */
RatingNormalDist::RatingNormalDist(int const scaleSize, double const stdDev) : m_stdDev(stdDev), RatingFactory("normal", scaleSize) {
   m_generator = std::default_random_engine(time(0));
   m_distribution = std::normal_distribution<double>((double)(scaleSize+1)/2, stdDev);
} // RatingNormalDist

/**
 * @return A generated rating
 */
double RatingNormalDist::rating() {
   double rating;
   do {
      rating = m_distribution(m_generator);
   } while(rating < 1 || rating > m_scaleSize);
   
   return rating;
} // rating

/**
 * @return A string representing the parameters used to create ratings
 */
std::string RatingNormalDist::toString() const {
	return m_name+"-"+patch::to_string(m_stdDev);
} // toString
