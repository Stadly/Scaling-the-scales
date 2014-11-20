#include "analyzed.h"

/**
 * A Transformer that calculates the relationship between users by analyzing the ratings
 * Every rating pair only has to be considered once
 * 
 * @param scaleSize Size of the rating scale
 */
TransformerAnalyzed::TransformerAnalyzed(int const scaleSize) : Transformer(scaleSize) {
   reset();
} // TransformerAnalyzed

/**
 * Reset the transformer
 */
void TransformerAnalyzed::reset() {
   m_coeff = 0;
} // reset

/**
 * @return The compatibility between the users
 */
double TransformerAnalyzed::getCoeff() const {
   return m_coeff;
} // getCoeff

/**
 * Find the best function by running the analysis
 * 
 * @param user1 A user
 * @param user2 A user
 * @param ratings The ratings given to items both users have rated
 */
void TransformerAnalyzed::run(int const user1, int const user2, std::vector<RatingPair> const &ratings) {
   reset();
   for(RatingPair const &r : ratings) {
      if(user1 == r.user1 && user2 == r.user2)
         report(r.rating1, r.rating2);
      else if(user1 == r.user2 && user2 == r.user1)
         report(r.rating2, r.rating1);
   } // end for
   done();
} // run
