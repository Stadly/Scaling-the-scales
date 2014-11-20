#include "core.h"
#include "dataset.h"

/**
 * Calculate and print a prediction
 * 
 * @param user A user
 * @param item An item
 * @param filter A recommendation algorithm
 * @param output An outputfilestream. null for standard output
 */
void RecommenderSystem::predict(int const user, int const item, CollaborativeFilter &filter, std::ofstream *output) const {
   Rating const *r = filter.hideRating(user-1, item-1);
   Prediction const *p = filter.predict(user-1, item-1);

   if(NULL == output)
      printPrediction(user, item, r, p, filter.name);
   else
      printPrediction(user, item, r, p, *output);
   delete p;
} // predict

/**
 * Calculate and print a prediction to standard output for all recommendation algorithms
 * @param user A user
 * @param item An item
 */
void RecommenderSystem::predict(int const user, int const item) const {
   for(CollaborativeFilter *filter : m_filters)
      predict(user, item, *filter, NULL);
   std::cout << '\n';
} // predict

/**
 * Go through all ratings in the dataset, and calculate predictions for number of them, chosen randomly
 * Use all recommendation algorithms, and print the results to file
 * 
 * @param number The number of predictions to calculate
 */
void RecommenderSystem::predictProbability(int const number) const {
   std::vector<std::ofstream*> files = predictionFiles();
   int count = 0;
   for(auto rating = m_dataset.ratingBegin(); rating != m_dataset.ratingEnd(); ++rating)
      if(patch::doubleLt((double)rand()/RAND_MAX, (double)number/m_dataset.ratingCount())) {
         ++count;
         std::cout << "(" << count << ") predict user " << rating->user+1 << ", item " << rating->item+1 << std::endl;
         for(int i = 0; i < m_filters.size(); ++i)
            predict(rating->user+1, rating->item+1, *m_filters.at(i), files.at(i));
      } // end if
   
   for(std::ofstream *file : files) {
      file->close();
      delete file;
   } // end for
} // predictRandomly

/**
 * Calculate predictions for all ratings in the dataset
 * Use all recommendation algorithms, and print the results to file
 */
void RecommenderSystem::predictAll() const {
   std::vector<std::ofstream*> files = predictionFiles();
   int count = 0;
   for(auto user = m_dataset.userBegin(); user != m_dataset.userEnd(); ++user) {
      for(auto rating = user->ratingBegin(); rating != user->ratingEnd(); ++rating) {
         ++count;
         std::cout << "(" << count << ") predict user " << rating->user+1 << ", item " << rating->item+1 << std::endl;
         for(int i = 0; i < m_filters.size(); ++i)
            predict(rating->user+1, rating->item+1, *m_filters.at(i), files.at(i));
      } // end for
   } // end for
   
   for(std::ofstream *file : files) {
      file->close();
      delete file;
   } // end for
} // predictAll

/**
 * Calculate predictions specified by a file
 * Use all recommendation algorithms, and print the results to file
 * 
 * @param filename A file with user-item-pairs
 */
void RecommenderSystem::predictFile(std::string const filename) const {
   std::ifstream predictionFile(m_dataset.predictionDir()+filename);
   std::vector<std::ofstream*> files = predictionFiles();
   int count = 0;
   while(predictionFile) {
      std::string s;
      std::getline(predictionFile, s, '\t'); // user
      if(!s.empty()) {
         int user = atoi(s.c_str());
         std::getline(predictionFile, s, '\t'); // item
         int item = atoi(s.c_str());
         std::getline(predictionFile, s);
         
         ++count;
         std::cout << "(" << count << ") predict user " << user << ", item " << item << std::endl;
         for(int i = 0; i < m_filters.size(); ++i)
            predict(user, item, *m_filters.at(i), files.at(i));
      } // end if
   } // end while
   
   for(std::ofstream *file : files) {
      file->close();
      delete file;
   } // end for
} // predictFile

/**
 * Ask user to specify the users and items to calculate predictions for
 */
void RecommenderSystem::predictManual() const {
   while(true) {
      int user, item;
      std::cout << "Please enter the user to predict (terminate by entering 0):\n";
      std::cin >> user;
      if(user == 0)
         break;
      std::cout << "Please enter the item to predict:\n";
      std::cin >> item;
      
      predict(user, item);
   } // end while
} // predictManual

/**
 * Add new recommendation algorithm
 * 
 * @param filter A recommendation algorithm
 */
void RecommenderSystem::addFilter(CollaborativeFilter *filter) {
   m_filters.push_back(filter);
} // addFilter

/**
 * Open prediction files for all recommendation algorithms
 * 
 * @return The prediction filestreams
 */
std::vector<std::ofstream*> RecommenderSystem::predictionFiles() const {
   std::vector<std::ofstream*> files;
   for(CollaborativeFilter *filter : m_filters) {
      std::string filename = m_dataset.predictionDir()+m_dataset.name+"-"+filter->toString()+".txt";
      std::ofstream *file = new std::ofstream(filename, std::ofstream::app);
      files.push_back(file);
   } // end for
   return files;
} // predictionFiles

/**
 * Initialize recommendation algorithms
 */
void RecommenderSystem::init() const {
   for(CollaborativeFilter *filter : m_filters)
      filter->init();
} // init

/**
 * Terminate recommendation algorithms
 */
void RecommenderSystem::terminate() {
   for(CollaborativeFilter *filter : m_filters)
      delete filter;
   m_filters.clear();
} // destruct

/**
 * Print statistics for a prediction to standard out
 * @param user The user
 * @param item The item
 * @param r The real rating. null if none
 * @param p The calculated prediction. null if none
 * @param filter Name of the recommendation algorithm
 */
void RecommenderSystem::printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::string const &filter) const {
   std::string const predictors = p == NULL ? "0" : patch::to_string(p->predictors);
   std::string const pathCount = p == NULL ? "0" : patch::to_string(p->pathCount);
   std::string const pathLength = p == NULL ? "0" : patch::to_string((int)p->pathLength);
   std::string const arcCount = p == NULL ? "0" : patch::to_string(p->arcCount);
   std::string const avgCommonRatings = p == NULL ? "0" : patch::to_string(p->avgCommonRatings);
   std::string const avgErrorFunc = p == NULL ? "0" : patch::to_string(p->avgErrorFunc);
   
   std::string const rating = r == NULL ? "none" : patch::to_string(r->rating);
   std::string const prediction = p == NULL ? "none" : patch::to_string(p->rating);
   std::string const difference = p == NULL || r == NULL ? "none" : patch::to_string(fabs(p->rating - r->rating));
   std::cout << filter << '\t'
             << "User: " << user << '\t'
             << "Item: " << item << '\t'
             << "Rating: " << rating << '\t'
             << "Prediction: " << prediction << '\t'
             << "Difference: " << difference << '\t'
             << "Predictor count: " << predictors << '\t'
             << "Path count: " << pathCount << '\t'
             << "Path length: " << pathLength << '\t'
             << "Arc count: " << arcCount << '\t'
             << "Average common ratings: " << avgCommonRatings << '\t'
             << "Average error in relation function: " << avgErrorFunc << std::endl;
} // printPrediction

/**
 * Print statistics for a prediction to a file
 * @param user The user
 * @param item The item
 * @param r The real rating. null if none
 * @param p The calculated prediction. null if none
 * @param output An outputfilestream
 */
void RecommenderSystem::printPrediction(int const user, int const item, Rating const *r, Prediction const *p, std::ofstream &output) const {
   std::string const predictors = p == NULL ? "0" : patch::to_string(p->predictors);
   std::string const pathCount = p == NULL ? "0" : patch::to_string(p->pathCount);
   std::string const pathLength = p == NULL ? "0" : patch::to_string((int)p->pathLength);
   std::string const arcCount = p == NULL ? "0" : patch::to_string(p->arcCount);
   std::string const avgCommonRatings = p == NULL ? "0" : patch::to_string(p->avgCommonRatings);
   std::string const avgErrorFunc = p == NULL ? "0" : patch::to_string(p->avgErrorFunc);
   
   std::string const rating = r == NULL ? "-1" : patch::to_string(r->rating);
   std::string const prediction = p == NULL ? "-1" : patch::to_string(p->rating);
   std::string const difference = p == NULL || r == NULL ? "-1" : patch::to_string(fabs(p->rating - r->rating));
   output << user << '\t'
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
