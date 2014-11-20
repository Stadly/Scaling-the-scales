#ifndef RS_TRANSFORMER_TWOWAY_H
#define RS_TRANSFORMER_TWOWAY_H

#include "../transformer.h"
#include "minimizer.h"

class TransformerTwoWay : public Transformer {
public:
   ~TransformerTwoWay();
   TransformerTwoWay(int const scaleSize, TransformerMinimizer &minimizer);
   void run(int const user1, int const user2, std::vector<RatingPair> const &ratings);
   double getCoeff() const;
protected:
   virtual void reset();
   virtual void next() = 0;
   virtual bool done() const = 0;
   virtual double value(double const r) const = 0;
   double m_diff;
   double m_bestDiff;
private:
   void report(double const d);
   TransformerMinimizer &m_minimizer;
};

#endif // RS_TRANSFORMER_TWOWAY_H

