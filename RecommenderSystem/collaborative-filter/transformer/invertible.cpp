#include "invertible.h"

TransformerInvertible::~TransformerInvertible() {
   delete &m_minimizer;
} // ~TransformerInvertible

/**
 * A Transformer that calculates the relationship between users by changing parameters to minimize an objective defined by a TransformerMinimizer
 * Every rating pair is considered several times, until the the best function is found
 * The function returned is invertible
 * 
 * @param scaleSize Size of the rating scale
 * @param minimizer TransformerMinimizer defining an objective
 */
TransformerInvertible::TransformerInvertible(int const scaleSize, TransformerMinimizer &minimizer) : m_minimizer(minimizer), Transformer(scaleSize) {
   reset();
} // TransformerInvertible

/**
 * Reset the transformer
 */
void TransformerInvertible::reset() {
   m_diff = 0;
   m_bestDiff = -1;
} // reset

/**
 * Report result from current function
 * 
 * @param d Difference produced by the function
 */
void TransformerInvertible::report(double const d) {
   m_diff += d;
} // report

/**
 * @return Best result from functions so far
 */
double TransformerInvertible::getCoeff() const {
   return m_bestDiff;
} // getCoeff

/**
 * Find the function minimizing the objective defined by the TransformerMinimizer
 * 
 * @param user1 A user
 * @param user2 A user
 * @param ratings The ratings given to items both users have rated
 */
void TransformerInvertible::run(int const user1, int const user2, std::vector<RatingPair> const &ratings) {
   for(reset(); !done(); next()) {
      for(RatingPair const &r : ratings) {
         if(m_bestDiff != -1 && patch::doubleLt(m_bestDiff, m_minimizer.preValue(ratings.size())))
            break;
         
         double const r1 = user1 == r.user1 ? r.rating1 : r.rating2;
         double const r2 = user1 == r.user1 ? r.rating2 : r.rating1;
         double const t1 = value(r1);
         double const t2 = valueInverse(r2);
         
         if(patch::doubleLt(std::abs(r1 - t2), std::abs(r2 - t1)))
            m_minimizer.report(t1, r.rating2);
         else
            m_minimizer.report(t2, r.rating1);
      } // end for
      report(m_minimizer.value());
      m_minimizer.reset();
   } // end for
} // run
