#ifndef RS_TRANSFORMER_MINIMIZER_AVERAGE_H
#define RS_TRANSFORMER_MINIMIZER_AVERAGE_H

#include "../minimizer.h"
#include <cmath>

class Average: public TransformerMinimizer {
public:
   Average();
   void reset();
   void report(double const v1, double const v2);
   double value() const;
   double preValue(int const count) const;
private:
   double m_diff;
   int m_count;
};

#endif // RS_TRANSFORMER_MINIMIZER_AVERAGE_H
