#include "collaborative-filter.h"

CollaborativeFilter::~CollaborativeFilter() {
   delete &m_transformer;
   delete &m_predictor;
} // ~CollaborativeFilter

/**
 * @return Filename of the relation file
 */
std::string CollaborativeFilter::relationFilename() const {
   return relationFilename(toString());
} // filename

/**
 * @param name String representing the parameters used
 * @return Filename of the relation file
 */
std::string CollaborativeFilter::relationFilename(std::string name) const {
   return m_dataset.relationDir()+m_dataset.name+"-"+name+".txt";
} // filename

/**
 * @return Filename of the prediction file
 */
std::string CollaborativeFilter::predictionFilename() const {
   return predictionFilename(toString());
} // filename

/**
 * @param name String representing the parameters used
 * @return Filename of the prediction file
 */
std::string CollaborativeFilter::predictionFilename(std::string name) const {
   return m_dataset.predictionDir()+m_dataset.name+"-"+name+".txt";
} // filename

/**
 * Initiate the recommendation algorithm
 * Calculate prediction relations, or read from file if available
 */
void CollaborativeFilter::init() {
   m_hiddenRating = NULL;
   CreateDirectory(m_dataset.relationDir().c_str(), NULL);
   CreateDirectory(m_dataset.predictionDir().c_str(), NULL);
   std::fstream file(relationFilename(), std::fstream::in);
   
   if(!file.is_open()) {
      for(auto user1 = m_dataset.userBegin(); user1 != m_dataset.userEnd(); ++user1) {
         std::cout << name << " relating user " << user1->id << std::endl;
         for(auto user2 = user1+1; user2 != m_dataset.userEnd(); ++user2)
            relate(*user1, *user2);
      } // end for
      file.open(relationFilename(), std::fstream::out);
      for(auto const &pred : m_predictors)
         for(auto const &rel : pred)
            file << rel.second.predictor << "|" << rel.second.predicted << "|" << rel.second.commonRatings << "|" << rel.second.funcError << "|" << (*rel.second.func).serialize() << '\n';
   } else {
      while(file.good()) {
         std::string s;
         std::getline(file, s, '|'); // predictor
         if(!s.empty()) {
            int predictor = atoi(s.c_str());
            std::getline(file, s, '|'); // predicted
            int predicted = atoi(s.c_str());
            std::getline(file, s, '|'); // commonRatings
            int commonRatings = atoi(s.c_str());
            std::getline(file, s, '|'); // funcError
            double funcError = atof(s.c_str());
            std::getline(file, s); // func
            m_predictors.at(predicted).insert({predictor, Relation(predictor, predicted, commonRatings, funcError, m_transformer.deserialize(s))});
         } // end if
      } // end while
   } // end if
   file.close();
} // init

/**
 * Hide a rating
 * 
 * @param user The user
 * @param item The item
 * @return The rating hidden. null if no rating was hidden
 */
Rating const *CollaborativeFilter::hideRating(int const user, int const item) {
   std::pair<int, int> const *unhidden = unhideRating();
   Rating const *rating = m_dataset.getUser(user).getRating(item);
   std::unordered_map<int, std::vector<int>> changed;
   if(NULL != unhidden) {
      changed.insert({unhidden->first, {unhidden->second}});
      delete unhidden;
   } // end if
   if(NULL != rating) {
      if(0 == changed.count(user))
         changed.insert({user, {}});
      changed.at(user).push_back(item);
      m_hiddenRating = new std::pair<int, int>(user, item);
   } // end if
   reRelate(changed);
   return rating;
} // hideRating

/**
 * Unhide any hidden rating
 * 
 * @return Pair containing the item and user of the unhidden rating. null if no rating was unhidden
 */
std::pair<int, int> const *CollaborativeFilter::unhideRating() {
   if(NULL != m_hiddenRating) {
      int const user = m_hiddenRating->first;
      int const item = m_hiddenRating->second;
      delete m_hiddenRating;
      m_hiddenRating = NULL;
      return new std::pair<int, int>(user, item);
   } // end if
   return NULL;
} // unhideRating

/**
 * Recalculate prediction relations
 * For each user in the map, recalculate prediction relations to all users having rated the items in the list
 * 
 * @param changed A map from users to lists of items
 */
void CollaborativeFilter::reRelate(std::unordered_map<int, std::vector<int>> const changed) {
   for(auto const &it : changed) {
      for(auto user2 = m_dataset.userBegin(); user2 != m_dataset.userEnd(); ++user2) {
         if(user2->id != it.first && user2->hasRated(it.second)) {
            auto pos = m_predictors.at(user2->id).find(it.first);
            if(pos != m_predictors.at(user2->id).end())
               m_predictors.at(user2->id).erase(pos);
            pos = m_predictors.at(it.first).find(user2->id);
            if(pos != m_predictors.at(it.first).end())
               m_predictors.at(it.first).erase(pos);
            relate(m_dataset.getUser(it.first), *user2);
         } // end if
      } // end for
   } // end for
} // reRelate

/**
 * Calculate prediction relations between user1 and user2
 * 
 * @param user1 A user
 * @param user2 A user
 */
void CollaborativeFilter::relate(User const &user1, User const &user2) {
   auto ratings = user1.commonRatings(user2);
   if(m_hiddenRating != NULL && (m_hiddenRating->first == user1.id || m_hiddenRating->first == user2.id)) {
      int hiddenItem = m_hiddenRating->second;
      ratings.erase(std::remove_if(ratings.begin(), ratings.end(), [hiddenItem](RatingPair const &r) {return r.item == hiddenItem;}), ratings.end());
   } // end if
   determinePrediction(user1, user2, ratings);
} // relate

/**
 * Determine whether user1 predicts user2 and wheter user2 predicts user1
 * 
 * @param user1 A user
 * @param user2 A user
 * @param ratings Ratings of the items both users have rated
 */
void CollaborativeFilter::determinePrediction(User const &user1, User const &user2, std::vector<RatingPair> const &ratings) {
   bool commonality1to2 = commonality(user1.id, user2.id, ratings.size());
   bool commonality2to1 = commonality(user2.id, user1.id, ratings.size());
   
   if(TransformerInvertible *transformer = dynamic_cast<TransformerInvertible *>(&m_transformer)) {
      
//      Force transformation functions to be inverse of each other
      if(!commonality1to2 && !commonality2to1)
         return;

      transformer->run(user1.id, user2.id, ratings);
      double coeff = transformer->getCoeff();

      if(!compatibility(user1.id, user2.id, coeff) || !compatibility(user2.id, user1.id, coeff))
         return;

      if(commonality1to2)
         m_predictors.at(user2.id).insert({user1.id, Relation(user1.id, user2.id, ratings.size(), coeff, transformer->getFunc())});
      if(commonality2to1)
         m_predictors.at(user1.id).insert({user2.id, Relation(user2.id, user1.id, ratings.size(), coeff, transformer->getFuncInverse())});
         
   } else if(TransformerTwoWay *transformer = dynamic_cast<TransformerTwoWay *>(&m_transformer)) {
      
//      Force transformation function to exist both ways, but independent of each other
      if(!commonality1to2 && !commonality2to1)
         return;

      m_transformer.run(user1.id, user2.id, ratings);
      double coeff1to2 = m_transformer.getCoeff();
      std::shared_ptr<TransformerFunction const> func1to2 = m_transformer.getFunc();
      if(!compatibility(user1.id, user2.id, coeff1to2))
         return;

      m_transformer.run(user2.id, user1.id, ratings);
      double coeff2to1 = m_transformer.getCoeff();
      std::shared_ptr<TransformerFunction const> func2to1 = m_transformer.getFunc();
      if(!compatibility(user2.id, user1.id, coeff2to1))
         return;

      if(commonality1to2)
         m_predictors.at(user2.id).insert({user1.id, Relation(user1.id, user2.id, ratings.size(), coeff1to2, func1to2)});
      if(commonality2to1)
         m_predictors.at(user1.id).insert({user2.id, Relation(user2.id, user1.id, ratings.size(), coeff2to1, func2to1)});
         
   } else {
      
//      Create transformation functions independent of each other
      if(commonality1to2) {
         m_transformer.run(user1.id, user2.id, ratings);
         if(compatibility(user1.id, user2.id, m_transformer.getCoeff()))
            m_predictors.at(user2.id).insert({user1.id, Relation(user1.id, user2.id, ratings.size(), m_transformer.getCoeff(), m_transformer.getFunc())});
      } // end if

      if(commonality2to1) {
         m_transformer.run(user2.id, user1.id, ratings);
         if(compatibility(user2.id, user1.id, m_transformer.getCoeff()))
            m_predictors.at(user1.id).insert({user2.id, Relation(user2.id, user1.id, ratings.size(), m_transformer.getCoeff(), m_transformer.getFunc())});
      } // end if
   } // end if
} // determinePrediction

/**
 * @param user A user
 * @param item An item
 * @return A predicted rating for the user and item
 */
Prediction const *CollaborativeFilter::predict(int const user, int const item) const {
   return predict(user, item, -1);
} // predict

/**
 * @param user A user
 * @param item An item
 * @param range The range to use
 * @return A predicted rating for the user and item, using the parameters set in range
 */
Prediction const *CollaborativeFilter::predict(int const user, int const item, int const range) const {
   std::vector<std::pair<int, int>> links;
   std::unordered_map<int, std::vector<std::tuple<double, int, double>>> predictions;
   std::unordered_map<int, int> dist;
   std::unordered_set<int> visited;
   links.push_back({user, 0});
   dist.insert({user, 0});
   visited.insert(user);
   m_predictor.reset();
   
//   Perform breadth-first search
   for(int pos = 0; pos < links.size(); ++pos) {
      auto const &p = links.at(pos);
      if(m_predictor.relevant(p)) {
         Rating const *r = m_hiddenRating == NULL || m_hiddenRating->first != p.first || m_hiddenRating->second != item ? m_dataset.getUser(p.first).getRating(item) : NULL;
         if(r != NULL) {
            m_predictor.found(p);
            predictions.insert({r->user, {std::make_tuple(r->rating, 0, 0)}});
         } else {
            // TODO Why does 1+p.second change inside the for loop, so I have to store it separately here?
            int depth = 1+p.second;
            for(auto const &rel : m_predictors.at(p.first)) {
               if(0 == visited.count(rel.second.predictor) && rangeCompatibility(rel.second.predictor, rel.second.predicted, rel.second.funcError, range) && rangeCommonality(rel.second.predictor, rel.second.predicted, rel.second.commonRatings, range)) {
                  links.push_back({rel.second.predictor, depth});
                  dist.insert({rel.second.predictor, depth});
                  visited.insert(rel.second.predictor);
               } // end if
            } // end for
         } // end if
      } // end if
   } // end for
   
//   Perform backtracking and transform ratings
   int arcs = 0;
   for(auto link = links.rbegin(); link != links.rend(); ++link) {
      ++link->second;
      if(m_predictor.relevant(*link) && 0 == predictions.count(link->first)) {
         for(auto const &rel : m_predictors.at(link->first)) {
            if(0 != predictions.count(rel.second.predictor) && link->second == dist.at(rel.second.predictor) && rangeCompatibility(rel.second.predictor, rel.second.predicted, rel.second.funcError, range) && rangeCommonality(rel.second.predictor, rel.second.predicted, rel.second.commonRatings, range)) {
               ++arcs;
               for(auto const &p2 : predictions.at(rel.second.predictor)) {
                  if(0 == predictions.count(link->first))
                     predictions.insert({link->first, {}});
                  predictions.at(link->first).push_back(std::make_tuple((*rel.second.func)(std::get<0>(p2)), std::get<1>(p2)+rel.second.commonRatings, std::get<2>(p2)+rel.second.funcError));
               } // end for
            } //end if
         } // end for
      } // end if
   } // end for
   
   if(0 == predictions.count(user))
      return NULL;
   int paths = predictions.at(user).size();
   double rating = 0;
   int commonRatings = 0;
   double errorFunc = 0;
   
//   Calculate prediction
   for(auto const &p2 : predictions.at(user)) {
      rating += std::get<0>(p2);
      commonRatings += std::get<1>(p2);
      errorFunc += std::get<2>(p2);
   } // end for
   
   return new Prediction(user, item, rating/paths, m_predictor.predictors(), paths, m_predictor.dist(), arcs, (double)commonRatings/(paths*m_predictor.dist()), (double)errorFunc/(paths*m_predictor.dist()));
} // predict

/**
 * @return Whether prediction relations already have been calculated
 */
bool CollaborativeFilter::related() const {
   std::ifstream file(relationFilename());
   return file.is_open();
} // related

/**
 * Calculate prediction relations for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 */
void CollaborativeFilter::relateRanges() const {
   if(0 == m_rangeRelationFiles.size())
      return;
   for(auto predicted = m_dataset.userBegin(); predicted != m_dataset.userEnd(); ++predicted) {
      std::cout << name << " relating user " << predicted->id << std::endl;
      for(auto rel : m_predictors.at(predicted->id)) {
         for(int range = 0; range < m_rangeCount; ++range) {
            if(0 < m_rangeRelationFiles.count(range) && rangeCommonality(rel.second.predictor, predicted->id, rel.second.commonRatings, range) && rangeCompatibility(rel.second.predictor, predicted->id, rel.second.funcError, range))
               *m_rangeRelationFiles.at(range) << rel.second.predictor << '|' << predicted->id << '|' << rel.second.commonRatings << '|' << rel.second.funcError << '|' << rel.second.func->serialize() << '\n';
         } // end for
      } // end for
   } // end for
} // relateRanges

/**
 * Predict ratings specified by a file for the ranges of parameters
 * The prediction relations are simply filtered from the prediction relations in the current recommendation algorithm
 * So the constraints must be tighter in the ranges
 * 
 * @param predictionsFile A file specifying ratings to predict
 */
void CollaborativeFilter::predictRanges(std::string const predictionsFile) {
   if(0 == m_rangePredictionFiles.size())
      return;
   std::ifstream file(m_dataset.predictionDir()+predictionsFile);
   int count = 0;
   while(file) {
      std::string s;
      std::getline(file, s, '\t'); // user
      if(!s.empty()) {
         int user = atoi(s.c_str());
         std::getline(file, s, '\t'); // item
         int item = atoi(s.c_str());
         std::getline(file, s);
         ++count;
         
         std::cout << "(" << count << ") predict user " << user << ", item " << item << std::endl;
         Rating const *rating = hideRating(user-1, item-1);
         for(int range = 0; range < m_rangeCount; ++range) {
            if(0 != m_rangePredictionFiles.count(range)) {
               Prediction const *prediction = predict(user-1, item-1, range);
               printPrediction(user, item, rating, prediction, m_rangePredictionFiles.at(range));
               delete prediction;
            } // end if
         } // end for
      } // end if
   } // end while
   file.close();
} // predictRanges

/**
 * Print statistics for a prediction to a file
 * @param user The user
 * @param item The item
 * @param r The real rating. null if none
 * @param p The calculated prediction. null if none
 * @param output An outputfilestream
 */
void CollaborativeFilter::printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::ofstream *output) const {
   std::string const predictors = p == NULL ? "0" : patch::to_string(p->predictors);
   std::string const pathCount = p == NULL ? "0" : patch::to_string(p->pathCount);
   std::string const pathLength = p == NULL ? "0" : patch::to_string((int)p->pathLength);
   std::string const arcCount = p == NULL ? "0" : patch::to_string(p->arcCount);
   std::string const avgCommonRatings = p == NULL ? "0" : patch::to_string(p->avgCommonRatings);
   std::string const avgErrorFunc = p == NULL ? "0" : patch::to_string(p->avgErrorFunc);
   
   std::string const rating = r == NULL ? "-1" : patch::to_string(r->rating);
   std::string const prediction = p == NULL ? "-1" : patch::to_string(p->rating);
   std::string const difference = p == NULL || r == NULL ? "-1" : patch::to_string(fabs(p->rating - r->rating));
   *output << user << '\t'
           << item << '\t'
           << rating << '\t'
           << prediction << '\t'
           << difference << '\t'
           << predictors << '\t'
           << pathCount << '\t'
           << pathLength << '\t'
           << arcCount << '\t'
           << avgCommonRatings << '\t'
           << avgErrorFunc << std::endl;
} // printPrediction

/**
 * Construct filestreams for the relation files for the ranges of parameters
 */
void CollaborativeFilter::initRangeRelationFiles() {
   for(int range = 0; range < m_rangeCount; ++range) {
      std::string filename = relationFilename(rangeToString(range));
      std::ifstream file(filename);
      if(file.is_open())
         file.close();
      else
         m_rangeRelationFiles.insert({range, new std::ofstream(filename)});
   } // end for
} // initRangeRelationFiles

/**
 * Construct filestreams for the prediction files for the ranges of parameters
 */
void CollaborativeFilter::initRangePredictionFiles() {
   for(int range = 0; range < m_rangeCount; ++range) {
      std::string filename = predictionFilename(rangeToString(range));
      m_rangePredictionFiles.insert({range, new std::ofstream(filename, std::ofstream::app)});
   } // end for
} // initRangePredictionFiles

/**
 * Clear range variables
 */
void CollaborativeFilter::clearRanges() {
   for(auto const &file : m_rangeRelationFiles) {
      file.second->close();
      delete file.second;
   } // end for
   m_rangeRelationFiles.clear();
   for(auto const &file : m_rangePredictionFiles) {
      file.second->close();
      delete file.second;
   } // end for
   m_rangePredictionFiles.clear();
   m_rangeCount = 0;
}
