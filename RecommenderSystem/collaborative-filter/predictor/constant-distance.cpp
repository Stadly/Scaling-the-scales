#include "constant-distance.h"

ConstantDistance::ConstantDistance(int const maxDist) : m_maxDist(maxDist) {
   reset();
} // ConstantDistance

void ConstantDistance::reset() {
   m_foundDist = -1;
   m_foundPredictors = 0;
} // reset

void ConstantDistance::found(std::pair<int, int> const &p) {
   m_foundDist = p.second;
   ++m_foundPredictors;
} // found

bool ConstantDistance::relevant(std::pair<int, int> const &p) const {
   return p.second <= m_maxDist && (m_foundDist == -1 || p.second <= m_foundDist);
} //relevant

int ConstantDistance::dist() const {
   return m_foundDist;
} // dist

int ConstantDistance::predictors() const {
   return m_foundPredictors;
} // predictors
