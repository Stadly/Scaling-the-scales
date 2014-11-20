#ifndef RS_TRANSFORMER_ANALYZED_AGREEMENT_SCALAR_H
#define RS_TRANSFORMER_ANALYZED_AGREEMENT_SCALAR_H

#include "../analyzed.h"
#include "../function/agreement-scalar.h"
#include <cmath>

class AgreementScalar: public TransformerAnalyzed {
public:
   AgreementScalar(int const scaleSize);
   std::shared_ptr<TransformerFunction const> getFunc() const;
   std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized) const;
protected:
   void reset();
   void report(double const r1, double const r2);
   void done();
private:
   double closeness(double const diff) const;
   int m_count;
   double m_close;
   double m_min1;
   double m_min2;
   double m_max1;
   double m_max2;
};

#endif // RS_TRANSFORMER_ANALYZED_AGREEMENT_SCALAR_H
