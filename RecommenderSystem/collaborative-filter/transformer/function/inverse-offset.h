#ifndef RS_TRANSFORMER_FUNCTION_INVERSE_OFFSET_H
#define RS_TRANSFORMER_FUNCTION_INVERSE_OFFSET_H

#include "../function.h"
#include "../../../patch.h"
#include <unordered_map>

class InverseOffsetFunction: public TransformerFunction {
public:
   static std::shared_ptr<TransformerFunction const> getFunc(int const sign, int const offset);
   static std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized);
   double operator()(double const r) const;
   std::string serialize() const;
private:
   static std::unordered_map<long long, std::shared_ptr<TransformerFunction const>> c_funcs;
   static long long hash(int const sign, int offset);
   InverseOffsetFunction(int const sign, int const offset) : m_sign(sign), m_offset(offset) {}
   signed char const m_sign;
   signed char const m_offset;
};

#endif // RS_TRANSFORMER_FUNCTION_INVERSE_OFFSET_H
