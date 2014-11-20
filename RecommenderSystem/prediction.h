#ifndef RS_PREDICTION_H
#define RS_PREDICTION_H

class Prediction {
public:
   Prediction(int const user, int const item, double const rating, int const predictors, int const pathCount, int const pathLength, int const arcCount, double const avgCommonRatings, double const avgErrorFunc) : user(user), item(item), rating(rating), predictors(predictors), pathCount(pathCount), pathLength(pathLength), arcCount(arcCount), avgCommonRatings(avgCommonRatings), avgErrorFunc(avgErrorFunc) {}
   unsigned short const user;
   unsigned short const item;
   double const rating;
   unsigned short const predictors;
   unsigned int const pathCount;
   unsigned char const pathLength;
   unsigned short const arcCount;
   double const avgCommonRatings;
   double const avgErrorFunc;
}; // Prediction

#endif // RS_PREDICTION_H

