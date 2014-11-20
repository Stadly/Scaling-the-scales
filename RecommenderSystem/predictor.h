#ifndef RS_PREDICTOR_H
#define RS_PREDICTOR_H

#include <utility>

class Predictor {
public:
   virtual void reset() = 0;
   virtual void found(std::pair<int, int> const &p) = 0;
   virtual bool relevant(std::pair<int, int> const &p) const = 0;
   virtual int dist() const = 0;
   virtual int predictors() const = 0;
};

#endif // RS_PREDICTOR_H
