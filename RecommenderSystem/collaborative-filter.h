#ifndef RS_COLLABORATIVE_FILTER_H
#define RS_COLLABORATIVE_FILTER_H

#include "patch.h"
#include "dataset.h"
#include "collaborative-filter/transformer.h"
#include "collaborative-filter/transformer/invertible.h"
#include "collaborative-filter/transformer/two-way.h"
#include "relation.h"
#include "predictor.h"
#include "prediction.h"
#include <queue>
#include <stack>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <string>
#include <map>
#include <windows.h>

class CollaborativeFilter {
public:
   CollaborativeFilter(std::string const name, Dataset const &dataset, Transformer &transformer, Predictor &predictor)
   : name(name)
   , m_dataset(dataset)
   , m_transformer(transformer)
   , m_predictor(predictor)
   , m_predictors(std::vector<std::map<int, Relation>>(dataset.numUsers(), std::map<int, Relation>()))
   {}
   ~CollaborativeFilter();
   void init();
   Rating const *hideRating(int const user, int const item);
   std::pair<int, int> const *unhideRating();
   Prediction const *predict(int const user, int const item) const;
   bool related() const;
   virtual std::string toString() const = 0;
   std::string const name;
protected:
   void relateRanges() const;
   void predictRanges(std::string const predictionsFile);
   virtual std::string rangeToString(int const range) const = 0;
   virtual bool rangeCommonality(int const predictor, int const predicted, int const ratings, int const range) const = 0;
   virtual bool rangeCompatibility(int const predictor, int const predicted, double const coeff, int const range) const = 0;
   void initRangeRelationFiles();
   void initRangePredictionFiles();
   void clearRanges();
   Prediction const *predict(int const user, int const item, int const range) const;
   virtual bool commonality(int const predictor, int const predicted, int const ratings) const = 0;
   virtual bool compatibility(int const predictor, int const predicted, double const coeff) const = 0;
   virtual void determinePrediction(User const &predictor, User const &predicted, std::vector<RatingPair> const &ratings);
   std::string relationFilename(std::string const name) const;
   std::string relationFilename() const;
   std::string predictionFilename(std::string const name) const;
   std::string predictionFilename() const;
   Transformer &m_transformer;
   Predictor &m_predictor;
   Dataset const m_dataset;
   std::vector<std::map<int, Relation>> m_predictors;
   std::unordered_map<int, std::ofstream*> m_rangeRelationFiles;
   std::unordered_map<int, std::ofstream*> m_rangePredictionFiles;
   int m_rangeCount;
private:
   void relate(User const &user1, User const &user2);
   void reRelate(std::unordered_map<int, std::vector<int>> const changed);
   void printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::ofstream *output) const;
   std::pair<int, int> *m_hiddenRating;
}; // CollaborativeFilter

#endif // RS_COLLABORATIVE_FILTER_H
