#ifndef RS_TRANSFORMER_FUNCTION_AGREEMENT_SCALAR_H
#define RS_TRANSFORMER_FUNCTION_AGREEMENT_SCALAR_H

#include "../function.h"
#include "../../../patch.h"
#include <memory>


class AgreementScalarFunction: public TransformerFunction {
public:
   static std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized);
   AgreementScalarFunction(int const min1, int const min2, int const max1, int const max2)
   : m_min1(min1), m_min2(min2), m_max1(max1), m_max2(max2) {}
   double operator()(double const r) const;
   std::string serialize() const;
private:
   unsigned char const m_min1;
   unsigned char const m_min2;
   unsigned char const m_max1;
   unsigned char const m_max2;
};

#endif // RS_TRANSFORMER_FUNCTION_AGREEMENT_SCALAR_H
