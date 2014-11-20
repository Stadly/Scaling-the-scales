#ifndef RS_TRANSFORMER_H
#define RS_TRANSFORMER_H

#include "transformer/function.h"
#include "../patch.h"
#include "../rating.h"
#include <functional>
#include <vector>
#include <memory>

class Transformer {
public:
   Transformer(int const scaleSize) : m_scaleSize(scaleSize) {}
   virtual void run(int const user1, int const user2, std::vector<RatingPair> const &ratings) = 0;
   virtual double getCoeff() const = 0;
   virtual std::shared_ptr<TransformerFunction const> getFunc() const = 0;
   virtual std::shared_ptr<TransformerFunction const> deserialize(std::string const &serialized) const = 0;
protected:
   int const m_scaleSize;
};

#endif // RS_TRANSFORMER_H
