#include "inverse-offset.h"

/**
 * Class for calculating the best transformation function between two users
 * Based on inverse-offset relationship
 * As used by IRA
 * 
 * @param algorithm Which analysis-algorithm to use
 * @param scaleSize Size of the rating scale
 */
InverseOffsetAnalyzed::InverseOffsetAnalyzed(std::string const &algorithm, int const scaleSize) : m_algorithm(algorithm), TransformerAnalyzed(scaleSize) {
   reset();
} // InverseOffsetAnalyzed

/**
 * Convert a string representing a function into a function
 * 
 * @param serialized A string representing a function
 * @return The function represented by the string
 */
std::shared_ptr<TransformerFunction const> InverseOffsetAnalyzed::deserialize(std::string const &serialized) const {
   return InverseOffsetFunction::deserialize(serialized);
} // deserialize

/**
 * Reset the transformer
 */
void InverseOffsetAnalyzed::reset() {
   m_rating1.clear();
   m_rating2.clear();
   m_diff1.clear();
   m_diff2.clear();
   TransformerAnalyzed::reset();
} // reset

/**
 * Report ratings
 * 
 * @param r1 A rating
 * @param r2 A transformed rating
 */
void InverseOffsetAnalyzed::report(double const r1, double const r2) {
   m_rating1.push_back(r1);
   m_rating2.push_back(r2);
   m_diff1.push_back(r2-r1);
   m_diff2.push_back(r1+r2);
} // report

/**
 * Finalize the analysis
 */
void InverseOffsetAnalyzed::done() {
   int med1, med2;
   if("median-select-" == m_algorithm.substr(0, 14)) {
      med1 = optimalOffsetSelect(m_diff1);
      med2 = optimalOffsetSelect(m_diff2);
   } else if("median-sort" == m_algorithm) {
      med1 = optimalOffsetSort(m_diff1);
      med2 = optimalOffsetSort(m_diff2);
   } // end if
   int tot1 = diff(1, med1);
   int tot2 = diff(-1, med2);
   
   if(tot1 <= tot2) {
      m_sign = 1;
      m_offset = med1;
      m_coeff = (double)tot1/m_rating1.size();
   } else {
      m_sign = -1;
      m_offset = med2;
      m_coeff = (double)tot2/m_rating1.size();
   } // end if
} // done

/**
 * @param sign Sign used by the function
 * @param offset Offset used by the function
 * @return The total difference between real and transformed ratings
 */
int InverseOffsetAnalyzed::diff(int sign, int offset) const {
   int diff = 0;
   for(int i = 0; i < m_rating1.size(); ++i)
      diff += std::abs(m_rating1.at(i)*sign + offset - m_rating2.at(i));
   return diff;
} // diff

/**
 * Get the best transformation function, as calculated by the analysis
 * 
 * @return A transformation function
 */
std::shared_ptr<TransformerFunction const> InverseOffsetAnalyzed::getFunc() const {
   return InverseOffsetFunction::getFunc(m_sign, m_offset);
} // getFunc

/**
 * Calculate optimal offset using a selection algorithm for finding the median
 * 
 * @param diff List of differences between ratings
 * @return The optimal offset
 */
int InverseOffsetAnalyzed::optimalOffsetSelect(std::vector<int> const &diff) const {
   if(0 == diff.size())
      return 0;
   int med1, med2;
   if("median-select-det" == m_algorithm) {
      std::list<int> list(diff.begin(), diff.end());
      med1 = selectionDet(list, diff.size()/2);
      med2 = 0 == diff.size()%2 ? selectionDet(list, diff.size()/2-1) : med1;
   } else if("median-select-rand" == m_algorithm) {
      med1 = selectionRand(diff, diff.size()/2);
      med2 = 0 == diff.size()%2 ? selectionRand(diff, diff.size()/2-1) : med1;
   } // end if
   if(med1*med2 <= 0)
      return 0;
   if(std::abs(med1) <= std::abs(med2))
      return med1;
   return med2;
} // optimalOffsetSelect

/**
 * Calculate optimal offset by sorting the differences
 * 
 * @param diff List of differences between ratings
 * @return The optimal offset
 */
int InverseOffsetAnalyzed::optimalOffsetSort(std::vector<int> &diff) const {
   std::sort(diff.begin(), diff.end());
   
   if(0 == diff.size())
      return 0;
   if(0 == diff.size()%2) {
      int med1 = diff.at(diff.size()/2);
      int med2 = diff.at(diff.size()/2-1);
      if(med1*med2 <= 0)
         return 0;
      if(std::abs(med1) < std::abs(med2))
         return med1;
      return med2;
   } // end if
   return diff.at(diff.size()/2);
} // optimalOffsetSort

/**
 * Randomized selection algorithm
 * Expected linear running time
 * 
 * @param diff List of differences between ratings
 * @param k Which positioned difference to select
 * @return The k-th smallest difference
 */
int InverseOffsetAnalyzed::selectionRand(std::vector<int> const &diff, int k) const {
   int pivot = diff.at(rand()%diff.size());
   std::vector<int> a1;
   std::vector<int> a2;
   for(int i : diff) {
      if(i < pivot)
         a1.push_back(i);
      else if(pivot < i)
         a2.push_back(i);
   } // end for
   if(a1.size() <= k && k < diff.size()-a2.size())
      return pivot;
   if(k < a1.size())
      return selectionRand(a1, k);
   return selectionRand(a2, k-(diff.size()-a2.size()));
} // selectionRand

/**
 * Deterministic selection algorithm
 * Linear running time
 * 
 * @param diff List of differences between ratings
 * @param k Which positioned difference to select
 * @return The k-th smallest difference
 */
int InverseOffsetAnalyzed::selectionDet(std::list<int> &diff, int k) const {
   std::list<int> b;
   for(auto it = diff.begin(); it != diff.end();) {
      std::list<int> c;
      auto pos = it;
      int count = 0;
      while(count < 5 && it != diff.end()) {
         ++it;
         ++count;
      } // end while
      c.splice(c.begin(), diff, pos, it);

      c.sort();
      pos = c.begin();
      std::advance(pos, count/2);
      b.push_back(*pos);
      diff.splice(it, c);
   } // end for
   int medB;
   if(b.size() <= 2)
      medB = *b.begin();
   else
      medB = selectionDet(b, b.size()/2);
   std::list<int> a1;
   std::list<int> a2;
   for(int i : diff) {
      if(i < medB)
         a1.push_back(i);
      else if(medB < i)
         a2.push_back(i);
   } // end for
   if(a1.size() <= k && k < diff.size()-a2.size())
      return medB;
   if(k < a1.size())
      return selectionDet(a1, k);
   return selectionDet(a2, k-(diff.size()-a2.size()));
} // selectionDet
