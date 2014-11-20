#include "inverse-offset.h"

// Map from hash values to function
std::unordered_map<long long, std::shared_ptr<TransformerFunction const>> InverseOffsetFunction::c_funcs;

/**
 * @param sign Sign used by the function
 * @param offset Offset used by the function
 * @return A function corresponding to the parameters
 */
std::shared_ptr<TransformerFunction const> InverseOffsetFunction::getFunc(int const sign, int const offset) {
   long long hashValue = hash(sign, offset);
   if(0 == c_funcs.count(hashValue))
      c_funcs.insert({hashValue, std::shared_ptr<TransformerFunction const>(new InverseOffsetFunction(sign, offset))});
   return c_funcs.at(hashValue);
} // getFunc

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> InverseOffsetFunction::deserialize(std::string const &serialized) {
   std::stringstream ss(serialized);
   std::string s;
   std::getline(ss, s, ','); // sign
   int sign = atoi(s.c_str());
   std::getline(ss, s); // offset
   int offset = atoi(s.c_str());
   return getFunc(sign, offset);
} // deserialize

/**
 * @param r A rating
 * @return The transformed rating
 */
double InverseOffsetFunction::operator()(double const r) const {
   return m_sign*r + m_offset;
} // operator()

/**
 * @return A string representation of the function
 */
std::string InverseOffsetFunction::serialize() const {
   return patch::to_string((int)m_sign)+","+patch::to_string((int)m_offset);
} // serialize

/**
 * @param sign Sign used by the funciton
 * @param offset Offset used by the function
 * @return A hash value corresponding to the parameters
 */
long long InverseOffsetFunction::hash(int const sign, int offset) {
   long long hash = 0;
   if(-1 == sign)
      hash |= (long long)1 << 32+2;
   if(offset < 0) {
      offset = -offset;
      hash |= (long long)1 << 32+1;
   }
   hash |= offset;
   return hash;
} // hash
