#ifndef RS_IRA_H
#define RS_IRA_H

#include "../collaborative-filter.h"
#include "predictor/constant-distance.h"
#include "transformer/minimized/inverse-offset.h"
#include "transformer/minimizer/average.h"
#include "transformer/analyzed/inverse-offset.h"

class Ira: public CollaborativeFilter {
public:
   Ira(Dataset const &dataset, int const itemsInCommon, double const fracInCommon, double const maxDiff, int const maxDist, std::string const &algorithm = "median-sort");
   void relate(int const itemsInCommon, double const fracInCommon, double const maxDiff);
   void relateRanges
   ( int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
   , double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
   , double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
   );
   void predictRanges
   ( std::string const predictionsFile
   , int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
   , double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
   , double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
   );
protected:
   void initRanges
   ( int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
   , double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
   , double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
   );
   void clearRanges();
   std::string rangeToString(int const range) const;
   bool rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const;
   bool rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const;
   std::string toString() const;
   bool commonality(int const predictor, int const predicted, int const ratings) const;
   bool compatibility(int const predictor, int const predicted, double const coeff) const;
private:
   std::string toString(int const itemsInCommon, double const fracInCommon, double const maxDiff) const;
   bool commonality(int const predictor, int const ratings, int const itemsInCommon, double const fracInCommon) const;
   bool compatibility(double const coeff, double const maxDiff) const;
   Transformer &transformer(std::string const &algorithm, int scaleSize) const;
   int const m_itemsInCommon;
   double const m_fracInCommon;
   double const m_maxDiff;
   std::vector<int> m_rangeItemsInCommon;
   std::vector<double> m_rangeFracInCommon;
   std::vector<double> m_rangeMaxDiff;
};

#endif // RS_IRA_H
