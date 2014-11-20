#include "user-inverse-offset.h"

/**
 * @param itemAvg Average rating for the item
 * @return A generated rating
 */
int UserInverseOffset::rating(double const itemAvg) {
   int rating = m_factory->rating(itemAvg) + m_offset;
   
   if(m_inverse)
      rating = m_factory->scaleSize+1 - rating;
   
   rating = std::max(1, std::min((int)m_factory->scaleSize, (int)round(rating)));
   
   return rating;
} // rating