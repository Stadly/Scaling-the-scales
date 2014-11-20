#include "dataset.h"

void Dataset::addUser(int const id, int const age, char const gender) {
   m_users.push_back(User(id, age, gender));
} // addUser

void Dataset::addItem(int const id, std::string const title) {
   m_items.push_back(Item(id, title));
} // addItem

void Dataset::addRating(int const user, int const item, int const rating) {
   Rating const r = Rating(user, item, rating);
   m_ratings.push_back(r);
   m_users.at(user).addRating(r);
} // addRating

User const &Dataset::getUser(int const user) const {
   return m_users.at(user);
} // getUser

std::string Dataset::relationDir() const {
   return "relations/"+name+"/";
} // relationDir

std::string Dataset::predictionDir() const {
   return "predictions/"+name+"/";
} // predictionDir

/**
 * Generate a file with random user-item-pairs for which ratings exist
 * @param filename Name of the file
 * @param number Number of user-item-pairs to output
 */
void Dataset::createPredictionFile(std::string filename, int const number) const {
   CreateDirectory(predictionDir().c_str(), NULL);
   std::ofstream file(predictionDir()+filename);
   for(auto rating = ratingBegin(); rating != ratingEnd(); ++rating) {
      if(patch::doubleLt((double)rand()/RAND_MAX, (double)number/ratingCount())) {
         file << rating->user+1 << '\t' << rating->item+1 << '\t' << '\n';
      } // end if
   } // end for
   file.close();
} // createPredictionFile
