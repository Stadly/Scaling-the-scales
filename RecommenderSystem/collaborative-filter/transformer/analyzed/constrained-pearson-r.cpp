#include "constrained-pearson-r.h"

/**
 * Class for calculating the best transformation function between two users
 * Based on the constrained pearson r coefficient
 * As used by Firefly
 * 
 * @param scaleSize Size of the rating scale
 */
ConstrainedPearsonR::ConstrainedPearsonR(int const scaleSize) : TransformerAnalyzed(scaleSize) {
   m_mid = (double)(scaleSize+1)/2;
   reset();
} // ConstrainedPearsonR

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> ConstrainedPearsonR::deserialize(std::string const &serialized) const {
   return ConstrainedPearsonRFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void ConstrainedPearsonR::reset() {
   m_sum1 = 0;
   m_sum2 = 0;
   m_sum3 = 0;
   TransformerAnalyzed::reset();
} // reset

/**
 * Report ratings
 * 
 * @param r1 A rating
 * @param r2 A transformed rating
 */
void ConstrainedPearsonR::report(double const r1, double const r2) {
   m_sum1 += (r1-m_mid)*(r2-m_mid);
   m_sum2 += (r1-m_mid)*(r1-m_mid);
   m_sum3 += (r2-m_mid)*(r2-m_mid);
} // report

/**
 * Finalize the analysis
 */
void ConstrainedPearsonR::done() {
   m_coeff = m_sum1/sqrt(m_sum2*m_sum3);
} // done

/**
 * Get the best transformation function, as calculated by the analysis
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> ConstrainedPearsonR::getFunc() const {
   return std::shared_ptr<TransformerFunction const>(new ConstrainedPearsonRFunction());
} // getFunc
