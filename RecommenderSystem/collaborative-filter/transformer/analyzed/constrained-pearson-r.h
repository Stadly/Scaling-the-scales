#ifndef RS_TRANSFORMER_ANALYZED_CONSTRAINED_PEARSON_R_H
#define RS_TRANSFORMER_ANALYZED_CONSTRAINED_PEARSON_R_H

#include "../analyzed.h"
#include "../function/constrained-pearson-r.h"
#include <cmath>

class ConstrainedPearsonR: public TransformerAnalyzed {
public:
   ConstrainedPearsonR(int const scaleSize);
   std::shared_ptr<TransformerFunction const> getFunc() const;
   std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized) const;
protected:
   void reset();
   void report(double const r1, double const r2);
   void done();
private:
   double m_mid;
   double m_sum1;
   double m_sum2;
   double m_sum3;
};

#endif // RS_TRANSFORMER_ANALYZED_CONSTRAINED_PEARSON_R_H
