#ifndef RS_TRANSFORMER_FUNCTION_H
#define RS_TRANSFORMER_FUNCTION_H

#include <iostream>
#include <memory>

class TransformerFunction {
public:
   virtual double operator()(double const r) const = 0;
   virtual std::string serialize() const = 0;
};

#endif // RS_TRANSFORMER_FUNCTION_H
