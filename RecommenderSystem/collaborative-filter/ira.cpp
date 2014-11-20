#include "ira.h"

/**
 * An implementation of the recommendation algorithm IRA
 * 
 * @param dataset A dataset
 * @param itemsInCommon The first horting condition hort_common
 * @param fracInCommon The second horting condition hort_frac
 * @param maxDiff Maximum average difference for the transformation functions t_diff_max
 * @param maxDist Maximum distance from user to predictor max_path_length
 */
Ira::Ira(Dataset const &dataset, int const itemsInCommon, double const fracInCommon, double const maxDiff, int const maxDist, std::string const &algorithm)
: m_itemsInCommon(itemsInCommon)
, m_fracInCommon(fracInCommon)
, m_maxDiff(maxDiff)
, CollaborativeFilter("ira-"+algorithm, dataset, transformer(algorithm, dataset.scaleSize), *new ConstantDistance(maxDist))
{

} // Ira

/**
 * @param algorithm Name of the transformation algorithm to use
 * @param scaleSize Size of the rating scale
 * @return A Transformer
 */
Transformer &Ira::transformer(std::string const &algorithm, int scaleSize) const {
   if("median-" == algorithm.substr(0, 7))
      return *new InverseOffsetAnalyzed(algorithm, scaleSize);
   else if("exhaust" == algorithm)
      return *new InverseOffset(scaleSize, *new Average());
} // transformer

/**
 * Calculate prediction relations
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param itemsInCommon The first horting condition hort_common
 * @param fracInCommon The second horting condition hort_frac
 * @param maxDiff Maximum average difference for the transformation functions t_diff_max
 */
void Ira::relate(int const itemsInCommon, double const fracInCommon, double const maxDiff) {
   relateRanges
   ( itemsInCommon, itemsInCommon, 1
   , fracInCommon, fracInCommon, 1
   , maxDiff, maxDiff, 1
   );
} // relate

/**
 * Calculate prediction relations for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param itemsInCommonFrom The loosest value for the first horting condition hort_common
 * @param itemsInCommonTo The tightest value for the first horting condition hort_common
 * @param itemsInCommonStep The incremental step for the first horting condition hort_common
 * @param fracInCommonFrom The loosest value for the second horting condition hort_frac
 * @param fracInCommonTo The tightest value for the second horting condition hort_frac
 * @param fracInCommonStep The incremental step for the second horting condition hort_frac
 * @param maxDiffFrom The tightest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffTo The loosest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffStep The incremental step for the maximum average difference for the transformation functions t_diff_max
 */
void Ira::relateRanges
( int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
, double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
, double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
) {
   initRanges
   ( itemsInCommonFrom, itemsInCommonTo, itemsInCommonStep
   , fracInCommonFrom, fracInCommonTo, fracInCommonStep
   , maxDiffFrom, maxDiffTo, maxDiffStep
   );
   initRangeRelationFiles();
   CollaborativeFilter::relateRanges();
   clearRanges();
} // relateRanges

/**
 * Predict ratings specified by a file for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param predictionsFile A file specifying ratings to predict
 * @param itemsInCommonFrom The loosest value for the first horting condition hort_common
 * @param itemsInCommonTo The tightest value for the first horting condition hort_common
 * @param itemsInCommonStep The incremental step for the first horting condition hort_common
 * @param fracInCommonFrom The loosest value for the second horting condition hort_frac
 * @param fracInCommonTo The tightest value for the second horting condition hort_frac
 * @param fracInCommonStep The incremental step for the second horting condition hort_frac
 * @param maxDiffFrom The tightest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffTo The loosest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffStep The incremental step for the maximum average difference for the transformation functions t_diff_max
 */
void Ira::predictRanges
( std::string const predictionsFile
, int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
, double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
, double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
) {
   initRanges
   ( itemsInCommonFrom, itemsInCommonTo, itemsInCommonStep
   , fracInCommonFrom, fracInCommonTo, fracInCommonStep
   , maxDiffFrom, maxDiffTo, maxDiffStep
   );
   initRangePredictionFiles();
   CollaborativeFilter::predictRanges(predictionsFile);
   clearRanges();
} // predictRanges

/**
 * Initialize the ranges of parameters
 * 
 * @param itemsInCommonFrom The loosest value for the first horting condition hort_common
 * @param itemsInCommonTo The tightest value for the first horting condition hort_common
 * @param itemsInCommonStep The incremental step for the first horting condition hort_common
 * @param fracInCommonFrom The loosest value for the second horting condition hort_frac
 * @param fracInCommonTo The tightest value for the second horting condition hort_frac
 * @param fracInCommonStep The incremental step for the second horting condition hort_frac
 * @param maxDiffFrom The tightest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffTo The loosest value for the maximum average difference for the transformation functions t_diff_max
 * @param maxDiffStep The incremental step for the maximum average difference for the transformation functions t_diff_max
 */
void Ira::initRanges
( int const itemsInCommonFrom, int const itemsInCommonTo, int const itemsInCommonStep
, double const fracInCommonFrom, double const fracInCommonTo, double const fracInCommonStep
, double const maxDiffFrom, double const maxDiffTo, double const maxDiffStep
) {
   clearRanges();
   for(int itemsInCommon = itemsInCommonFrom; itemsInCommon <= itemsInCommonTo; itemsInCommon += itemsInCommonStep)
      for(double fracInCommon = fracInCommonFrom; patch::doubleLe(fracInCommon, fracInCommonTo); fracInCommon += fracInCommonStep)
         for(double maxDiff = maxDiffFrom; patch::doubleLe(maxDiff, maxDiffTo); maxDiff += maxDiffStep) {
            m_rangeItemsInCommon.push_back(itemsInCommon);
            m_rangeFracInCommon.push_back(fracInCommon);
            m_rangeMaxDiff.push_back(maxDiff);
            ++m_rangeCount;
         } // end for
} // initRanges

/**
 * Clear the ranges of parameters
 */
void Ira::clearRanges() {
   CollaborativeFilter::clearRanges();
   m_rangeItemsInCommon.clear();
   m_rangeFracInCommon.clear();
   m_rangeMaxDiff.clear();
} // clearRanges

/**
 * Whether the users have enough in common to predict each other
 * The horting condition
 * 
 * @param predictor A user
 * @param predicted A user
 * @param ratings The number of items rated in common
 * @return Whether the users can predict each other
 */
bool Ira::commonality(int const predictor, int const predicted, int const ratings) const {
   return commonality(predicted, ratings, m_itemsInCommon, m_fracInCommon);
} // commonality

/**
 * Whether the users have enough in common to predict each other
 * The horting condition
 * 
 * @param predicted The predicted user
 * @param ratings The number of items rated in common
 * @param itemsInCommon The first horting condition hort_common
 * @param fracInCommon The second horting condition hort_frac
 * @return Whether the users can predict each other
 */
bool Ira::commonality(int const predicted, int const ratings, int const itemsInCommon, double const fracInCommon) const {
   return itemsInCommon <= ratings || patch::doubleLe(fracInCommon, (double)ratings/m_dataset.getUser(predicted).numRatings());
} // commonality

/**
 * Whether the users have enough in common to predict each other
 * The horting condition
 * 
 * @param predictor A user
 * @param predicted A user
 * @param ratings The number of items rated in common
 * @param range The range to use parameters from
 * @return Whether the users can predict each other
 */
bool Ira::rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const {
   if(-1 == range)
      return commonality(predictor, predicted, ratings);
   
   return commonality(predicted, ratings, m_rangeItemsInCommon.at(range), m_rangeFracInCommon.at(range));
} // rangeCommonality

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param predictor A user
 * @param predicted A user
 * @param coeff The average error of the transformation function
 * @return Whether the users can predict each other
 */
bool Ira::compatibility(int const predictor, int const predicted, double const coeff) const {
   return compatibility(coeff, m_maxDiff);
} // compatibility

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param coeff The average error of the transformation function
 * @param maxDiff The maximum average error of a transformation function
 * @return Whether the users can predict each other
 */
bool Ira::compatibility(double const coeff, double const maxDiff) const {
   return patch::doubleLe(coeff, maxDiff);
} // compatibility

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param predictor A user
 * @param predicted A user
 * @param coeff The average error of the transformation function
 * @param range The range to use parameters from
 * @return Whether the users can predict each other
 */
bool Ira::rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const {
   if(-1 == range)
      return compatibility(predictor, predicted, coeff);
   
   return compatibility(coeff, m_rangeMaxDiff.at(range));
} // rangeCompatibility

/**
 * @return A name for the recommendation algorithm and its parameters
 */
std::string Ira::toString() const {
   return toString(m_itemsInCommon, m_fracInCommon, m_maxDiff);
} // toString

/**
 * @param itemsInCommon The first horting condition hort_common
 * @param fracInCommon The second horting condition hort_frac
 * @param maxDiff Maximum average difference for the transformation functions t_diff_max
 * @return A name for the recommendation algorithm and the given parameters
 */
std::string Ira::toString(int const itemsInCommon, double const fracInCommon, double const maxDiff) const {
   return name+"-"+patch::to_string(itemsInCommon)+"-"+patch::to_string(fracInCommon)+"-"+patch::to_string(maxDiff);
} // toString

/**
 * @param range The range to use parameters from
 * @return A name for the recommendation algorithm and the parameters of the range
 */
std::string Ira::rangeToString(int const range) const {
   return toString(m_rangeItemsInCommon.at(range), m_rangeFracInCommon.at(range), m_rangeMaxDiff.at(range));
} // rangeToString
