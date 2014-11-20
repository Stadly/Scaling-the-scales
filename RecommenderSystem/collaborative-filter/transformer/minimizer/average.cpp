#include "average.h"

/**
 * A TransformerMinimizer for minimizing the average value
 */
Average::Average() {
   reset();
} // Average

/**
 * Reset the minimizer
 */
void Average::reset() {
   m_diff = 0;
   m_count = 0;
} // reset

/**
 * Report a difference
 * 
 * @param v1 A rating
 * @param v2 A transformed rating
 */
void Average::report(double const v1, double const v2) {
   m_diff += std::abs(v1-v2);
   ++m_count;
} // report

/**
 * @param count The total number of ratings
 * @return The smallest possible average difference
 */
double Average::preValue(int const count) const {
   return m_diff/count;
} // preValue

/**
 * @return The average difference
 */
double Average::value() const {
   return m_diff/m_count;
} // value
