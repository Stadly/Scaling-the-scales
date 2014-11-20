#ifndef RS_TRANSFORMER_MINIMIZER_H
#define RS_TRANSFORMER_MINIMIZER_H

#include <stdlib.h>

class TransformerMinimizer {
public:
   virtual void reset() = 0;
   virtual void report(double const v1, double const v2) = 0;
   virtual double value() const = 0;
   virtual double preValue(int const count) const = 0;
};

#endif // RS_TRANSFORMER_MINIMIZER_H
