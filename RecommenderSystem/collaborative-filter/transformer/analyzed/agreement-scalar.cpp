#include "agreement-scalar.h"

/**
 * Class for calculating the best transformation function between two users
 * Based on an agreement scalar
 * As used by LikeMinds
 * 
 * @param scaleSize Size of the rating scale
 */
AgreementScalar::AgreementScalar(int const scaleSize) : TransformerAnalyzed(scaleSize) {
   reset();
} // AgreementScalar

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> AgreementScalar::deserialize(std::string const &serialized) const {
   return AgreementScalarFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void AgreementScalar::reset() {
   m_count = 0;
   m_close = 0;
   m_min1 = 0;
   m_min2 = 0;
   m_max1 = 0;
   m_max2 = 0;
   TransformerAnalyzed::reset();
} // reset

/**
 * @param diff Difference between the ratings
 * @return The closeness value
 */
double AgreementScalar::closeness(double const diff) const {
   int c13[] = {10, 9, 6, 4, 2, 1, 0, 0, -1, -6, -8, -10, -10};
   if(13 == m_scaleSize)
      return c13[(int)round(diff)];
   return 10-20*diff/(m_scaleSize-1);
} // closeness

/**
 * Report ratings
 * 
 * @param r1 A rating
 * @param r2 A transformed rating
 */
void AgreementScalar::report(double const r1, double const r2) {
   m_close += closeness(fabs(r1-r2));
   if(0 == m_count || patch::doubleLt(r1, m_min1))
      m_min1 = r1;
   if(0 == m_count || patch::doubleLt(r2, m_min2))
      m_min2 = r2;
   if(0 == m_count || patch::doubleLt(m_max1, r1))
      m_max1 = r1;
   if(0 == m_count || patch::doubleLt(m_max2, r2))
      m_max2 = r2;
   ++m_count;
} // report

/**
 * Finalize the analysis
 */
void AgreementScalar::done() {
   m_coeff = m_close*log2(m_count)/m_count;
} // done

/**
 * Get the best transformation function, as calculated by the analysis
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> AgreementScalar::getFunc() const {
   return std::shared_ptr<TransformerFunction const>(new AgreementScalarFunction(m_min1, m_min2, m_max1, m_max2));
} // getFunc
