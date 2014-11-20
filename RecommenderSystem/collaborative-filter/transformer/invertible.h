#ifndef RS_TRANSFORMER_INVERTIBLE_H
#define RS_TRANSFORMER_INVERTIBLE_H

#include "../transformer.h"
#include "minimizer.h"

class TransformerInvertible : public Transformer {
public:
   ~TransformerInvertible();
   TransformerInvertible(int const scaleSize, TransformerMinimizer &minimizer);
   void run(int const user1, int const user2, std::vector<RatingPair> const &ratings);
   virtual std::shared_ptr<TransformerFunction const> getFuncInverse() const = 0;
   double getCoeff() const;
protected:
   virtual void reset();
   virtual void next() = 0;
   virtual bool done() const = 0;
   virtual double value(double const r) const = 0;
   virtual double valueInverse(double const r) const = 0;
   double m_diff;
   double m_bestDiff;
private:
   void report(double const d);
   TransformerMinimizer &m_minimizer;
};

#endif // RS_TRANSFORMER_INVERTIBLE_H

