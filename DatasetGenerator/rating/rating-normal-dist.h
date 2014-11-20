#ifndef DSG_RATING_NORMAL_DIST_H
#define DSG_RATING_NORMAL_DIST_H

#include "../rating-factory.h"

class RatingNormalDist: public RatingFactory {
public:
	RatingNormalDist(int const scaleSize, double const stdDev);
   double rating();
	std::string toString() const;
private:
   double const m_stdDev;
   std::normal_distribution<double> m_distribution;
};

#endif // DSG_RATING_NORMAL_DIST_H
