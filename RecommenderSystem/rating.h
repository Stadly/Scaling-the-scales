#ifndef RS_RATING_H
#define RS_RATING_H

#include "item.h"
#include <iostream>

class Rating {
public:
   int const user;
   int const item;
   double const rating;
   Rating(int const user, int const item, double const rating) : user(user), item(item), rating(rating) {}
private:
   void operator=(Rating const &);
}; // Rating


class RatingItemComparator {
public:
   bool operator()(const Rating &r1, const Rating &r2) const {return r1.item < r2.item;}
}; // RatingItemComparator


class RatingPair {
public:
   int const item;
   int const user1;
   int const user2;
   double const rating1;
   double const rating2;
   RatingPair(int const item, int const user1, int const user2, double const rating1, double const rating2) : item(item), user1(user1), user2(user2), rating1(rating1), rating2(rating2) {}
   RatingPair(RatingPair const &r) : item(r.item), user1(r.user1), user2(r.user2), rating1(r.rating1), rating2(r.rating2) {}
   RatingPair& operator=(RatingPair &&r) {return r;}
}; // RatingPair

#endif // RS_RATING_H
