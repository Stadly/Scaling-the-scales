#include "agreement-scalar.h"

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> AgreementScalarFunction::deserialize(std::string const &serialized) {
   std::stringstream ss(serialized);
   std::string s;
   std::getline(ss, s, ','); // min1
   int min1 = atoi(s.c_str());
   std::getline(ss, s, ','); // min2
   int min2 = atoi(s.c_str());
   std::getline(ss, s, ','); // max1
   int max1 = atoi(s.c_str());
   std::getline(ss, s); // max2
   int max2 = atoi(s.c_str());
   return std::shared_ptr<TransformerFunction const>(new AgreementScalarFunction(min1, min2, max1, max2));
} // deserialize

/**
 * @param r A rating
 * @return The transformed rating
 */
double AgreementScalarFunction::operator()(double const r) const {
   return m_min2 + (r-m_min1)*(m_max2-m_min2)/(m_max1-m_min1);
} // operator()

/**
 * @return A string representation of the function
 */
std::string AgreementScalarFunction::serialize() const {
   return patch::to_string((int)m_min1)+","+patch::to_string((int)m_min2)+","+patch::to_string((int)m_max1)+","+patch::to_string((int)m_max2);
} // serialize
