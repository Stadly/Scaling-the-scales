#include "scaled.h"

// Map from hash values to function
std::unordered_map<long long, std::shared_ptr<TransformerFunction const>> ScaledFunction::c_funcs;

/**
 * @param min1 Parameter used by the function
 * @param max1 Parameter used by the function
 * @param min2 Parameter used by the function
 * @param max2 Parameter used by the function
 * @return A function corresponding to the parameters
 */
std::shared_ptr<TransformerFunction const> ScaledFunction::getFunc(int const min1, int const max1, int const min2, int const max2) {
   long long hashValue = hash(min1, max1, min2, max2);
   if(0 == c_funcs.count(hashValue))
      c_funcs.insert({hashValue, std::shared_ptr<TransformerFunction const>(new ScaledFunction(min1, max1, min2, max2))});
   return c_funcs.at(hashValue);
} // getFunc

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> ScaledFunction::deserialize(std::string const &serialized) {
   std::stringstream ss(serialized);
   std::string s;
   std::getline(ss, s, ','); // min1
   int min1 = atoi(s.c_str());
   std::getline(ss, s, ','); // max1
   int max1 = atoi(s.c_str());
   std::getline(ss, s, ','); // min2
   int min2 = atoi(s.c_str());
   std::getline(ss, s); // max2
   int max2 = atoi(s.c_str());
   return getFunc(min1, max1, min2, max2);
} // deserialize

/**
 * @param r A rating
 * @return The transformed rating
 */
double ScaledFunction::operator()(double const r) const {
   return (r-m_min1)*((double)(m_max2-m_min2)/(m_max1-m_min1)) + m_min2;
} // operator()

/**
 * @return A string representation of the function
 */
std::string ScaledFunction::serialize() const {
   return patch::to_string((int)m_min1)+","+patch::to_string((int)m_max1)+","+patch::to_string((int)m_min2)+","+patch::to_string((int)m_max2);
} // serialize

double ScaledFunction::value(double const r, int const min1, int const max1, int const min2, int const max2) {
   double div = max2 == min2 && max1 == min1 ? 1 : (double)(max2-min2)/(max1-min1);
   return (r-min1)*div + min2;
} // value

/**
 * @param min1 Parameter used by the function
 * @param max1 Parameter used by the function
 * @param min2 Parameter used by the function
 * @param max2 Parameter used by the function
 * @return A hash value corresponding to the parameters
 */
long long ScaledFunction::hash(int const min1, int const max1, int const min2, int const max2) {
   long long hash = 0;
   hash |= (long long)min1;
   hash |= (long long)max1 << 16;
   hash |= (long long)min2 << 32;
   hash |= (long long)max2 << 48;
   return hash;
} // hash

/**
 * @param min1 Parameter used by the function
 * @param max1 Parameter used by the function
 * @param min2 Parameter used by the function
 * @param max2 Parameter used by the function
 * @param bits The number of bits in the size of the rating scale
 * @return A hash value corresponding to the parameters
 */
long long ScaledFunction::hash(int const min1, int const max1, int const min2, int const max2, int bits) {
   ++bits;
   long long hash = 0;
   int gcd = patch::gcd(max1-min1, max2-min2);
   long long numerator = 0 == gcd ? 1 : (max2-min2)/gcd;
   long long denominator = 0 == gcd ? 1 : (max1-min1)/gcd;
   
   long long a, b;
   if(std::abs(numerator) <= std::abs(denominator)) {
      a = min2 - (min1*numerator)/denominator;
      b = (min1*numerator)%denominator;
   } else {
      a = (min2*denominator)/numerator - min1;
      b = (min2*denominator)%numerator;
      hash |= (long long)1 << 4*bits;
   }
   if(numerator < 0) {
      numerator = -numerator;
      hash |= (long long)1 << 4*bits+1;
   }
   if(a < 0) {
      a = -a;
      hash |= (long long)1 << 4*bits+2;
   }
   
   hash |= denominator;
   hash |= numerator << bits;
   hash |= a << 2*bits;
   hash |= b << 3*bits;
   
   return hash;
} // hash
