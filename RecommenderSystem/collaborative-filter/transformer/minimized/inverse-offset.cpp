#include "inverse-offset.h"

 /**
 * Class for calculating the best transformation function between two users
 * Based on inverse-offset relationship
 * As used by IRA
 * The function returned is not necessarily invertible
 * 
 * @param scaleSize Size of the rating scale
 * @param minimizer TransformerMinimizer defining an objective
 */
InverseOffset::InverseOffset(int const scaleSize, TransformerMinimizer &minimizer) : TransformerMinimized(scaleSize, minimizer) {
   reset();
} // InverseOffset

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> InverseOffset::deserialize(std::string const &serialized) const {
   return InverseOffsetFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void InverseOffset::reset() {
   m_sign = 1;
   m_offset = 0;
   m_bestSign = 1;
   m_bestOffset = 0;
   TransformerMinimized::reset();
} // reset

/**
 * Try next transformation function
 */
void InverseOffset::next() {
   if(patch::doubleLt(m_diff, m_bestDiff) || -1 == m_bestDiff) {
      m_bestSign = m_sign;
      m_bestOffset = m_offset;
      m_bestDiff = m_diff;
   } // end if
   m_diff = 0;
   
   // TODO Only check m_offset in {-m_scaleSize+1,...,m_scaleSize-1} for m_sign = 1, and {2,...,2*m_scaleSize for m_sign = -1
   m_offset = -m_offset;
   if(2*m_scaleSize == m_offset)
      m_offset = 0;
   else {
      if(0 <= m_offset)
         ++m_offset;
      return;
   } // end if
   m_sign = -m_sign;
} // next

/**
 * @return Whether all transformation functions have been tried
 */
bool InverseOffset::done() const {
   return -1 != m_bestDiff && m_offset == 0 && m_sign == 1;
} // done

/**
 * @param r A rating
 * @return The transformed rating
 */
double InverseOffset::value(double const r) const {
   return m_sign*r + m_offset;
} // value

/**
 * Get the best transformation function, as calculated by the minimizer
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> InverseOffset::getFunc() const {
   return InverseOffsetFunction::getFunc(m_bestSign, m_bestOffset);
} // getFunc
