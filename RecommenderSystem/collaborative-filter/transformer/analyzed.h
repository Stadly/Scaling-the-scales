#ifndef RS_TRANSFORMER_ANALYZED_H
#define RS_TRANSFORMER_ANALYZED_H

#include "../transformer.h"

class TransformerAnalyzed : public Transformer {
public:
   TransformerAnalyzed(int const scaleSize);
   void run(int const user1, int const user2, std::vector<RatingPair> const &ratings);
   double getCoeff() const;
protected:
   virtual void reset();
   virtual void report(double const r1, double const r2) = 0;
   virtual void done() = 0;
   double m_coeff;
};

#endif // RS_TRANSFORMER_ANALYZED_H
