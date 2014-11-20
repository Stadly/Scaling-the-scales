#ifndef RS_CORE_H
#define RS_CORE_H

#include "collaborative-filter.h"
#include "dataset.h"
#include <vector>
#include <iostream>
#include <windows.h>

class RecommenderSystem {
public:
   RecommenderSystem(Dataset const &dataset) : m_dataset(dataset) {};
   void predict(int const user, int const item) const;
   void predictProbability(int const number) const;
   void predictAll() const;
   void predictFile(std::string file) const;
   void predictManual() const;
   void addFilter(CollaborativeFilter *filter);
   void init() const;
   void terminate();
private:
   void predict(int const user, int const item, CollaborativeFilter &filter, std::ofstream *output) const;
   void printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::string const &filter) const;
   void printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::ofstream &output) const;
   std::vector<std::ofstream*> predictionFiles() const;
   std::vector<std::vector<std::ofstream*>> predictionRangeFiles() const;
   Dataset const &m_dataset;
   std::vector<CollaborativeFilter*> m_filters;
};

#endif // RS_CORE_H
