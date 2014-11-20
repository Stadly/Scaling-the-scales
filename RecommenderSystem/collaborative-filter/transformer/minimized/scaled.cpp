#include "scaled.h"

 /**
 * Class for calculating the best transformation function between two users
 * Based on scaled relationship
 * Almost as used by Scaling the scales, but inverse relationships are not considered, and
 * The function returned is not necessarily invertible
 * 
 * @param scaleSize Size of the rating scale
 * @param minimizer TransformerMinimizer defining an objective
 */
Scaled::Scaled(int const scaleSize, TransformerMinimizer &minimizer) : TransformerMinimized(scaleSize, minimizer) {
   m_bits = 0;
   while(0 < scaleSize >> m_bits)
      ++m_bits;
   reset();
} // Scaled

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> Scaled::deserialize(std::string const &serialized) const {
   return ScaledFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void Scaled::reset() {
   m_visited.clear();
   m_min1 = 1;
   m_max1 = m_scaleSize;
   m_min2 = 1;
   m_max2 = m_scaleSize;
   m_bestMin1 = 0;
   m_bestMax1 = 0;
   m_bestMin2 = 0;
   m_bestMax2 = 0;
   TransformerMinimized::reset();
} // reset

/**
 * Try next transformation function
 */
void Scaled::next() {
   m_visited.insert(hash());
   
   if(patch::doubleLt(m_diff, m_bestDiff) || -1 == m_bestDiff) {
      m_bestMin1 = m_min1;
      m_bestMax1 = m_max1;
      m_bestMin2 = m_min2;
      m_bestMax2 = m_max2;
      m_bestDiff = m_diff;
   } // end if
   m_diff = 0;
   
   while(!done() && ((m_min1 == m_max1 && m_min2 != m_max2) || 0 < m_visited.count(hash()))) {
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
bool Scaled::done() const {
   return -1 != m_bestDiff && m_scaleSize == m_min1 && m_scaleSize == m_max1 && 1 == m_min2 && m_scaleSize == m_max2;
} // done

/**
 * @param r A rating
 * @return The transformed rating
 */
double Scaled::value(double const r) const {
   return ScaledFunction::value(r, m_min1, m_max1, m_min2, m_max2);
} // value

/**
 * Get the best transformation function, as calculated by the minimizer
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> Scaled::getFunc() const {
   return ScaledFunction::getFunc(m_bestMin1, m_bestMax1, m_bestMin2, m_bestMax2);
} // getFunc

/**
 * @return A hash value for the current transformation function
 */
long long Scaled::hash() const {
   return ScaledFunction::hash(m_min1, m_max1, m_min2, m_max2, m_bits);
} // hash
