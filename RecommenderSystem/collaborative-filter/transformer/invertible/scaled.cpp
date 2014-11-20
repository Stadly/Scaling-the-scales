#include "scaled.h"

 /**
 * Class for calculating the best transformation function between two users
 * Based on scaled relationship
 * Almost as used by Scaling the scales, but inverse relationships are not considered
 * The function returned is invertible
 * 
 * @param scaleSize Size of the rating scale
 * @param minimizer TransformerMinimizer defining an objective
 */
ScaledInvertible::ScaledInvertible(int const scaleSize, TransformerMinimizer &minimizer) : TransformerInvertible(scaleSize, minimizer) {
   m_bits = 0;
   while(0 < scaleSize >> m_bits)
      ++m_bits;
   reset();
} // ScaledInvertibles

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> ScaledInvertible::deserialize(std::string const &serialized) const {
   return ScaledFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void ScaledInvertible::reset() {
   m_visited.clear();
   m_min1 = 1;
   m_max1 = m_scaleSize;
   m_min2 = 1;
   m_max2 = m_scaleSize;
   m_bestMin1 = 0;
   m_bestMax1 = 0;
   m_bestMin2 = 0;
   m_bestMax2 = 0;
   TransformerInvertible::reset();
} // reset

/**
 * Try next transformation function
 */
void ScaledInvertible::next() {
   m_visited.insert(hash());
   
   if(patch::doubleLt(m_diff, m_bestDiff) || -1 == m_bestDiff) {
      m_bestMin1 = m_min1;
      m_bestMax1 = m_max1;
      m_bestMin2 = m_min2;
      m_bestMax2 = m_max2;
      m_bestDiff = m_diff;
   } // end if
   m_diff = 0;
   
   while(!done() && ((m_min1 == m_max1 && m_min2 != m_max2) || (m_min1 != m_max1 && m_min2 == m_max2) || 0 < m_visited.count(hash()))) {
      if(m_max2 < m_scaleSize) {
         ++m_min2;
         ++m_max2;
      } else if(m_max1 < m_scaleSize) {
         ++m_min1;
         ++m_max1;
         m_max2 = m_max2-m_min2+1;
         m_min2 = 1;
      } else if(m_max2-m_min2 == m_scaleSize-1) {
         m_min2 = 1;
         m_max2 = m_max1-m_min1;
         m_min1 = 1;
         m_max1 = m_scaleSize;
      } else if(m_max2-m_min2 < m_max1-m_min1) {
         m_max1 = m_max1-m_min1;
         m_min1 = 1;
         m_max2 = m_max2-m_min2+1;
         m_min2 = 1;
      } else {
         m_max1 = m_max1-m_min1+1;
         m_min1 = 1;
         m_max2 = m_max2-m_min2+2;
         m_min2 = 1;
      } // end if
   } // end while
} // next

/**
 * @return Whether all transformation functions have been tried
 */
bool ScaledInvertible::done() const {
   return -1 != m_bestDiff && m_scaleSize-1 == m_min1 && m_scaleSize == m_max1 && 1 == m_min2 && m_scaleSize == m_max2;
} // done

/**
 * @param r A rating
 * @return The transformed rating
 */
double ScaledInvertible::value(double const r) const {
   return ScaledFunction::value(r, m_min1, m_max1, m_min2, m_max2);
} // value

/**
 * @param r A rating
 * @return The inverse transformed rating
 */
double ScaledInvertible::valueInverse(double const r) const {
   return ScaledFunction::value(r, m_min2, m_max2, m_min1, m_max1);
} // valueInverse

/**
 * Get the best transformation function, as calculated by the minimizer
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> ScaledInvertible::getFunc() const {
   return ScaledFunction::getFunc(m_bestMin1, m_bestMax1, m_bestMin2, m_bestMax2);
} // getFunc

/**
 * Get the inverse of the best transformation function, as calculated by the minimizer
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> ScaledInvertible::getFuncInverse() const {
   return ScaledFunction::getFunc(m_bestMin2, m_bestMax2, m_bestMin1, m_bestMax1);
} // getFuncInverse

/**
 * @return A hash value for the current transformation function
 */
long long ScaledInvertible::hash() const {
   return ScaledFunction::hash(m_min1, m_max1, m_min2, m_max2, m_bits);
} // hash
