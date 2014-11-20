#ifndef RS_DATASET_H
#define RS_DATASET_H

#include "user.h"
#include "item.h"
#include "rating.h"
#include "patch.h"
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <fstream>
#include <windows.h>

class Dataset {
public:
   Dataset(std::string const name, int const scaleSize) : name(name), scaleSize(scaleSize) {};
   void addUser(int const id, int const age, char const gender);
   void addItem(int const id, std::string const title);
   void addRating(int const user, int const item, int const rating);
   int ratingCount() const {return m_ratings.size();};
   int numUsers() const {return m_users.size();}
   User const &getUser(int const user) const;
   void createPredictionFile(std::string filename, int const number) const;
   std::string relationDir() const;
   std::string predictionDir() const;
   // TODO Create a manual iterator instead?
   std::vector<User>::const_iterator userBegin() const {return m_users.begin();}
   std::vector<User>::const_iterator userEnd() const {return m_users.end();}
   // TODO Create a manual iterator instead?
   std::vector<Rating>::const_iterator ratingBegin() const {return m_ratings.begin();}
   std::vector<Rating>::const_iterator ratingEnd() const {return m_ratings.end();}
   int const scaleSize;
   std::string const name;
private:
   std::vector<User> m_users;
   std::vector<Item> m_items;
   std::vector<Rating> m_ratings;
}; // Dataset

#endif // RS_DATASET_H

