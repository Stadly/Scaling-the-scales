#ifndef RS_PREDICTOR_CONSTANT_DISTANCE_H
#define RS_PREDICTOR_CONSTANT_DISTANCE_H

#include "../../predictor.h"
#include <utility>
#include <algorithm>

class ConstantDistance: public Predictor {
public:
   ConstantDistance(int const maxDist);
   void reset();
   void found(std::pair<int, int> const &p);
   bool relevant(std::pair<int, int> const &p) const;
   int dist() const;
   int predictors() const;
private:
   int const m_maxDist;
   int m_foundDist;
   int m_foundPredictors;
};

#endif // RS_PREDICTOR_CONSTANT_DISTANCE_H
