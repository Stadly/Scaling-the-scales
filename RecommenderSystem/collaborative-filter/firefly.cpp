#include "firefly.h"

/**
 * An implementation of the recommendation algorithm Firefly
 * 
 * @param dataset A dataset
 * @param minCorr The minimum constrained pearson r coefficient value
 */
Firefly::Firefly(Dataset const &dataset, double const minCorr)
: m_minCorr(minCorr)
, CollaborativeFilter("firefly", dataset, *new ConstrainedPearsonR(dataset.scaleSize), *new ConstantDistance(1)) {

} // Firefly

/**
 * Calculate prediction relations
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param minCorr The minimum constrained pearson r coefficient value
 */
void Firefly::relate(double const minCorr) {
   relateRanges(minCorr, minCorr, 1);
} // relate

/**
 * Calculate prediction relations for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param minCorrFrom The loosest value for the minimum constrained pearson r coefficient value
 * @param minCorrTo The tightest value for the minimum constrained pearson r coefficient value
 * @param minCorrStep The incremental step value for the minimum constrained pearson r coefficient value
 */
void Firefly::relateRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
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
 * @param minCorrFrom The loosest value for the minimum constrained pearson r coefficient value
 * @param minCorrTo The tightest value for the minimum constrained pearson r coefficient value
 * @param minCorrStep The incremental step value for the minimum constrained pearson r coefficient value
 */
void Firefly::predictRanges(std::string const predictionsFile, double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
   initRanges(minCorrFrom, minCorrTo, minCorrStep);
   initRangePredictionFiles();
   CollaborativeFilter::predictRanges(predictionsFile);
   clearRanges();
} // predictRanges

/**
 * Initialize the ranges of parameters
 * 
 * @param minCorrFrom The loosest value for the minimum constrained pearson r coefficient value
 * @param minCorrTo The tightest value for the minimum constrained pearson r coefficient value
 * @param minCorrStep The incremental step value for the minimum constrained pearson r coefficient value
 */
void Firefly::initRanges(double const minCorrFrom, double const minCorrTo, double const minCorrStep) {
   clearRanges();
   for(double minCorr = minCorrFrom; patch::doubleLe(minCorr, minCorrTo); minCorr += minCorrStep) {
      m_rangeMinCorr.push_back(minCorr);
      ++m_rangeCount;
   } // end for
} // initRanges

/**
 * Clear the ranges of parameters
 */
void Firefly::clearRanges() {
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
bool Firefly::commonality(int const predictor, int const predicted, int const ratings) const {
   return commonality();
} // commonality

/**
 * Whether the users have enough in common to predict each other
 * Always true
 * 
 * @return Whether the users can predict each other
 */
bool Firefly::commonality() const {
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
bool Firefly::rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const {
   if(-1 == range)
      return commonality(predictor, predicted, ratings);
   
   return commonality(predicted, ratings, m_rangeMinCorr.at(range));
} // rangeCommonality

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param predictor A user
 * @param predicted A user
 * @param coeff The constrained pearson r coefficient value
 * @return Whether the users can predict each other
 */
bool Firefly::compatibility(int const predictor, int const predicted, double const coeff) const {
   return compatibility(coeff, m_minCorr);
} // compatibility

/**
 * Whether the users are compatible enough to predict each other
 * 
 * @param coeff The agreement scalar
 * @param minCorr The minimum constrained pearson r coefficient value
 * @return Whether the users can predict each other
 */
bool Firefly::compatibility(double const coeff, double const minCorr) const {
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
bool Firefly::rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const {
   if(-1 == range)
      return compatibility(predictor, predicted, coeff);
   
   return compatibility(coeff, m_rangeMinCorr.at(range));
} // rangeCompatibility

/**
 * @return A name for the recommendation algorithm and its parameters
 */
std::string Firefly::toString() const {
   return toString(m_minCorr);
} // toString

/**
 * @param minCorr The minimum constrained pearson r coefficient value
 * @return A name for the recommendation algorithm and the given parameters
 */
std::string Firefly::toString(double const minCorr) const {
   return name+"-"+patch::to_string(minCorr);
} // toString

/**
 * @param range The range to use parameters from
 * @return A name for the recommendation algorithm and the parameters of the range
 */
std::string Firefly::rangeToString(int const range) const {
   return toString(m_rangeMinCorr.at(range));
} // rangeToString
