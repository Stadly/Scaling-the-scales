#ifndef RS_USER_H
#define RS_USER_H

#include "rating.h"
#include <vector>
#include <set>

class User {
public:
   int const id;
   int const age;
   int const gender;
   User(int const id, int const age, char const gender) : id(id), age(age), gender(gender) {}
   void addRating(Rating const &rating) {m_ratings.insert(rating);}
   Rating const *getRating(int const item) const;
   bool hasRated(std::vector<int> const items) const;
   int numRatings() const {return m_ratings.size();}
   std::vector<RatingPair> commonRatings(User const &user) const;
   // TODO Create a manual iterator instead?
   std::set<Rating>::const_iterator ratingBegin() const {return m_ratings.begin();}
   std::set<Rating>::const_iterator ratingEnd() const {return m_ratings.end();}
private:
   std::set<Rating, RatingItemComparator> m_ratings;
}; // User

#endif // RS_USER_H
