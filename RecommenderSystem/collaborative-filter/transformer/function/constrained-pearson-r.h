#ifndef RS_TRANSFORMER_FUNCTION_CONSTRAINED_PEARSON_R_H
#define RS_TRANSFORMER_FUNCTION_CONSTRAINED_PEARSON_R_H

#include "../function.h"
#include "../../../patch.h"

class ConstrainedPearsonRFunction: public TransformerFunction {
public:
   static std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized);
   double operator()(double const r) const;
   std::string serialize() const;
};

#endif // RS_TRANSFORMER_FUNCTION_CONSTRAINED_PEARSON_R_H
