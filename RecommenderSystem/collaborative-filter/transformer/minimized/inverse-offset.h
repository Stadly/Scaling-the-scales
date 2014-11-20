#ifndef RS_TRANSFORMER_MINIMIZED_INVERSE_OFFSET_H
#define RS_TRANSFORMER_MINIMIZED_INVERSE_OFFSET_H

#include "../minimized.h"
#include "../function/inverse-offset.h"
#include <cmath>

class InverseOffset: public TransformerMinimized {
public:
   InverseOffset(int const scaleSize, TransformerMinimizer &minimizer);
   std::shared_ptr<TransformerFunction const> getFunc() const;
   std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized) const;
protected:
   void reset();
   void next();
   bool done() const;
   double value(double const r) const;
private:
   int m_sign;
   int m_offset;
   int m_bestSign;
   int m_bestOffset;
};

#endif // RS_TRANSFORMER_MINIMIZED_INVERSE_OFFSET_H
