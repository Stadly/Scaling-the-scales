#ifndef RS_TRANSFORMER_FUNCTION_SCALED_H
#define RS_TRANSFORMER_FUNCTION_SCALED_H

#include "../function.h"
#include "../../../patch.h"
#include <unordered_map>

class ScaledFunction: public TransformerFunction {
public:
   static std::shared_ptr<TransformerFunction const> getFunc(int const min1, int const max1, int const min2, int const max2);
   static std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized);
   static long long hash(int const min1, int const max1, int const min2, int const max2, int const bits);
   static double value(double const r, int const min1, int const max1, int const min2, int const max2);
   double operator()(double const r) const;
   std::string serialize() const;
private:
   static std::unordered_map<long long, std::shared_ptr<TransformerFunction const>> c_funcs;
   static long long hash(int const min1, int const max1, int const min2, int const max2);
   ScaledFunction(int const min1, int const max1, int const min2, int const max2) : m_min1(min1), m_max1(max1), m_min2(min2), m_max2(max2) {}
   signed char const m_min1;
   signed char const m_max1;
   signed char const m_min2;
   signed char const m_max2;
};

#endif // RS_TRANSFORMER_FUNCTION_SCALED_H
