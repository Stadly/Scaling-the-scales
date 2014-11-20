#include "likeminds.h"

/**
 * An implementation of the recommendation algorithm LikeMinds
 * 
 * @param dataset A dataset
 * @param minCorr The minimum agreement scalar value (defaults to 0)
 */
LikeMinds::LikeMinds(Dataset const &dataset, double const minCorr)
: m_minCorr(minCorr)
, CollaborativeFilter("likeminds", dataset, *new AgreementScalar(dataset.scaleSize), *new ConstantDistance(1)) {

} // LikeMinds

/**
 * Calculate prediction relations
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param minCorr The minimum agreement scalar value
 */
void LikeMinds::relate(double const minCorr) {
   relateRanges(minCorr, minCorr, 1);
} // relate

/**
 * Calculate prediction relations for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param minCorrFrom The loosest value for the minimum agreement scalar value
 * @param minCorrTo The tightest value for the minimum agreement scalar value
 * @param minCorrStep The incremental step value for the minimum agreement scalar value
 */
void LikeMinds::relateRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
   initRanges(minCorrFrom, minCorrTo, minCorrStep);
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
 * @param minCorrFrom The loosest value for the minimum agreement scalar value
 * @param minCorrTo The tightest value for the minimum agreement scalar value
 * @param minCorrStep The incremental step value for the minimum agreement scalar value
 */
void LikeMinds::predictRanges(std::string const predictionsFile, double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
   initRanges(minCorrFrom, minCorrTo, minCorrStep);
   initRangePredictionFiles();
   CollaborativeFilter::predictRanges(predictionsFile);
   clearRanges();
} // predictRanges

/**
 * Initialize the ranges of parameters
 * 
 * @param minCorrFrom The loosest value for the minimum agreement scalar value
 * @param minCorrTo The tightest value for the minimum agreement scalar value
 * @param minCorrStep The incremental step value for the minimum agreement scalar value
 */
void LikeMinds::initRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
   clearRanges();
   for(double minCorr = minCorrFrom; patch::doubleLe(minCorr, minCorrTo); minCorr += minCorrStep) {
      m_rangeMinCorr.push_back(minCorr);
      ++m_rangeCount;
   } // end for
} // initRanges

/**
 * Clear the ranges of parameters
 */
void LikeMinds::clearRanges() {
   CollaborativeFilter::clearRanges();
   m_rangeMinCorr.clear();
} // clearRanges

/**
 * Whether the users have enough in common to predict each other
 * Always true
 * 
 * @param predictor A user
 * @param predicted A user
 * @param ratings The number of items rated in common
 * @return Whether the users can predict each other
 */
bool LikeMinds::commonality(int const predictor, int const predicted, int const ratings) const {
   return commonality();
} // commonality

/**
 * Whether the users have enough in common to predict each other
 * Always true
 * 
 * @return Whether the users can predict each other
 */
bool LikeMinds::commonality() const {
   return true;
} // commonality

/**
 * Whether the users have enough in common to predict each other
 * Always true
 * 
 * @param predictor A user
 * @param predicted A user
 * @param ratings The number of items rated in common
 * @param range The range to use parameters from
 * @return Whether the users can predict each other
 */
bool LikeMinds::rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const {
   return commonality();
} // rangeCommonality

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param predictor A user
 * @param predicted A user
 * @param coeff The agreement scalar
 * @return Whether the users can predict each other
 */
bool LikeMinds::compatibility(int const predictor, int const predicted, double const coeff) const {
   return compatibility(coeff, m_minCorr);
} // compatibility

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param coeff The agreement scalar
 * @param minCorr The minimum agreement scalar
 * @return Whether the users can predict each other
 */
bool LikeMinds::compatibility(double const coeff, double const minCorr) const {
   return patch::doubleLt(minCorr, coeff);
} // compatibility

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param predictor A user
 * @param predicted A user
 * @param coeff The agreement scalar
 * @param range The range to use parameters from
 * @return Whether the users can predict each other
 */
bool LikeMinds::rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const {
   if(-1 == range)
      return compatibility(predictor, predicted, coeff);
   
   return compatibility(coeff, m_rangeMinCorr.at(range));
} // rangeCompatibility

/**
 * @return A name for the recommendation algorithm and its parameters
 */
std::string LikeMinds::toString() const {
   return toString(m_minCorr);
} // toString

/**
 * @param minCorr The minimum agreement scalar value
 * @return A name for the recommendation algorithm and the given parameters
 */
std::string LikeMinds::toString(double const minCorr) const {
   return name+"-"+patch::to_string(minCorr);
} // toString

/**
 * @param range The range to use parameters from
 * @return A name for the recommendation algorithm and the parameters of the range
 */
std::string LikeMinds::rangeToString(int const range) const {
   return toString(m_rangeMinCorr.at(range));
} // rangeToString
