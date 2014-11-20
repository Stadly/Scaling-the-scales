   #include "user-inverse-offset-scaled.h"

/**
 * @param itemAvg Average rating for the item
 * @return A generated rating
 */
int UserInverseOffsetScaled::rating(double const itemAvg) {
   double rating = m_factory->rating(itemAvg);
   
   rating = m_min + (rating-1)/(m_factory->scaleSize-1)*(m_max-m_min);
   
   rating = std::max(1, std::min((int)m_factory->scaleSize, (int)round(rating)));
   
   return (int)rating;
} // rating