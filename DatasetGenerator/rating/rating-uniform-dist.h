#ifndef DSG_RATING_UNIFORM_DIST_H
#define DSG_RATING_UNIFORM_DIST_H

#include "../rating-factory.h"

class RatingUniformDist: public RatingFactory {
public:
	RatingUniformDist(int const scaleSize);
   double rating();
	std::string toString() const;
private:
   std::uniform_real_distribution<double> m_distribution;
};

#endif // DSG_RATING_UNIFORM_DIST_H
