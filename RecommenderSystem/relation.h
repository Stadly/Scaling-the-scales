#ifndef RS_RELATION_H
#define RS_RELATION_H

#include "user.h"
#include <memory>

class Relation {
public:
   Relation(int const predictor, int const predicted, int const commonRatings, double const funcError, std::shared_ptr<TransformerFunction const> func) : predictor(predictor), predicted(predicted), commonRatings(commonRatings), funcError(funcError), func(func) {}
   unsigned short const predictor;
   unsigned short const predicted;
   unsigned short const commonRatings;
   float const funcError;
   std::shared_ptr<TransformerFunction const> func;
}; // Relation

#endif // RS_RELATION_H
