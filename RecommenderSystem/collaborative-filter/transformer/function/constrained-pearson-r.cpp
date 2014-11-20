#include "constrained-pearson-r.h"

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> ConstrainedPearsonRFunction::deserialize(std::string const &serialized) {
   return std::shared_ptr<TransformerFunction const>(new ConstrainedPearsonRFunction());
} // deserialize

/**
 * @param r A rating
 * @return The transformed rating
 */
double ConstrainedPearsonRFunction::operator()(double const r) const {
   return r;
} // operator()

/**
 * @return A string representation of the function
 */
std::string ConstrainedPearsonRFunction::serialize() const {
   return "";
} // serialize
