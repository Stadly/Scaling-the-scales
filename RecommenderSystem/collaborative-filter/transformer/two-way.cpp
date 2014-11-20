#include "two-way.h"

TransformerTwoWay::~TransformerTwoWay() {
   delete &m_minimizer;
} // ~TransformerTwoWay

/**
 * A Transformer that calculates the relationship between users by changing parameters to minimize an objective defined by a TransformerMinimizer
 * Every rating pair is considered several times, until the the best function is found
 * 
 * @param scaleSize Size of the rating scale
 * @param minimizer TransformerMinimizer defining an objective
 */
TransformerTwoWay::TransformerTwoWay(int const scaleSize, TransformerMinimizer &minimizer) : m_minimizer(minimizer), Transformer(scaleSize) {
   reset();
} // TransformerTwoWay

/**
 * Reset the transformer
 */
void TransformerTwoWay::reset() {
   m_diff = 0;
   m_bestDiff = -1;
} // reset

/**
 * Report result from current function
 * 
 * @param d Difference produced by the function
 */
void TransformerTwoWay::report(double const d) {
   m_diff += d;
} // report

/**
 * @return Best result from functions so far
 */
double TransformerTwoWay::getCoeff() const {
   return m_bestDiff;
} // getCoeff

/**
 * Find the function minimizing the objective defined by the TransformerMinimizer
 * 
 * @param user1 A user
 * @param user2 A user
 * @param ratings The ratings given to items both users have rated
 */
void TransformerTwoWay::run(int const user1, int const user2, std::vector<RatingPair> const &ratings) {
   for(reset(); !done(); next()) {
      for(RatingPair const &r : ratings) {
         if(m_bestDiff != -1 && patch::doubleLt(m_bestDiff, m_minimizer.preValue(ratings.size())))
            break;
         if(user1 == r.user1)
            m_minimizer.report(value(r.rating1), r.rating2);
         else
            m_minimizer.report(value(r.rating2), r.rating1);
      } // end for
      report(m_minimizer.value());
      m_minimizer.reset();
   } // end for
} // run
