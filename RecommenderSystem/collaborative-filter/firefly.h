#ifndef RS_FIREFLY_H
#define RS_FIREFLY_H

#include "../collaborative-filter.h"
#include "predictor/constant-distance.h"
#include "transformer/analyzed/constrained-pearson-r.h"

class Firefly: public CollaborativeFilter {
public:
   Firefly(Dataset const &dataset, double const minCorr);
   void relate(double const minCorr);
   void relateRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep);
   void predictRanges(std::string const predictionsFile, double const minCorrFrom, double const minCorrTo, double const minCorrStep);
protected:
   void initRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep);
   void clearRanges();
   std::string toString() const;
   bool commonality(int const predictor, int const predicted, int const ratings) const;
   bool compatibility(int const predictor, int const predicted, double const coeff) const;
   std::string rangeToString(int const range) const;
   bool rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const;
   bool rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const;
private:
   std::string toString(double const minCorr) const;
   bool commonality() const;
   bool compatibility(double const coeff, double const minCorr) const;
   double const m_minCorr;
   std::vector<double> m_rangeMinCorr;
};

#endif // RS_FIREFLY_H
