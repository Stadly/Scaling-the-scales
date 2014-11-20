#ifndef RS_TRANSFORMER_ANALYZED_INVERSE_OFFSET_H
#define RS_TRANSFORMER_ANALYZED_INVERSE_OFFSET_H

#include "../analyzed.h"
#include "../function/inverse-offset.h"
#include <cmath>

class InverseOffsetAnalyzed: public TransformerAnalyzed {
public:
   InverseOffsetAnalyzed(std::string const &algorithm, int const scaleSize);
   std::shared_ptr<TransformerFunction const> getFunc() const;
   std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized) const;
protected:
   void reset();
   void report(double const r1, double const r2);
   void done();
private:
   int optimalOffsetSelect(std::vector<int> const &diff) const;
   int optimalOffsetSort(std::vector<int> &diff) const;
   int selectionRand(std::vector<int> const &diff, int k) const;
   int selectionDet(std::list<int> &diff, int k) const;
   std::string const m_algorithm;
   int m_sign;
   int m_offset;
   std::vector<int> m_rating1;
   std::vector<int> m_rating2;
   std::vector<int> m_diff1;
   std::vector<int> m_diff2;
   int diff(int sign, int offset) const;
};

#endif // RS_TRANSFORMER_ANALYZED_INVERSE_OFFSET_H
