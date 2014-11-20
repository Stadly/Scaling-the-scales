#include "user.h"

/**
 * @param item An item
 * @return The rating the user has given to item. null if no rating exists
 */
Rating const *User::getRating(int const item) const {
   auto it = m_ratings.find(Rating(id, item, 0));
   if(it == m_ratings.end())
      return NULL;
   return &*it;
} // getRating

/**
 * @param items A list of items
 * @return Whether the user has rated any of the items in the list
 */
bool User::hasRated(std::vector<int> const items) const {
   for(auto const &item : items)
      if(0 < m_ratings.count(Rating(id, item, 0)))
         return true;
   return false;
} // hasRated

/**
 * @param user A user
 * @return Ratings for items that both this user and user have rated
 */
std::vector<RatingPair> User::commonRatings(User const &user) const {
//   TODO Create some ostrem_iterator and use this instead
//   std::set_intersection(ratings.begin(), ratings.end(), u.ratings.begin(), u.ratings.end(), std::back_inserter(c));
   std::vector<RatingPair> c;
   for(auto r1 = m_ratings.begin(), r2 = user.m_ratings.begin(); r1 != m_ratings.end() && r2 != user.m_ratings.end();) {
      if(r1->item < r2->item)
         ++r1;
      else if(r2->item < r1->item)
         ++r2;
      else {
         c.push_back(RatingPair(r1->item, r1->user, r2->user, r1->rating, r2->rating));
         ++r1;
         ++r2;
      } // end if
   } // end for
   return c;
} // commonRatings
