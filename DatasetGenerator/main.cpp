#include "main.h"

int main(int argc, char** argv) {
   
//   Set parameters for the generated dataset
   int const scale = 13;
   int const userCount = 500;
   int const items = 500;
   int const itemsHot = 50;
   int const rated = 20;
   int const ratedHot = 10;
   int const ratedAtLeast = 0;
   
//   Set the distribution for the ratings
//   RatingFactory *ratingFactory = new RatingNormalDist(scale, (double)scale/8);
   RatingFactory *ratingFactory = new RatingUniformDist(scale);
   
//   Set the user model
//   UserFactory *userFactory = new UserFactoryInverseOffset(scale, 0.2, 0.01, 3);
   UserFactory *userFactory = new UserFactoryInverseOffsetScaled(scale, 0.2, 0.01);
   
//   Calculate the chance for the graph to be connected for different horting parameters
   int const itemsCold = items-itemsHot;
   int const ratedCold = rated-ratedHot;
   std::vector<std::vector<unsigned long long>> choose = patch::binom(items);
   std::vector<double> probHot;
   std::vector<double> probCold;
   std::vector<double> prob;
   for(int i = 0; i <= itemsHot; ++i)
      probHot.push_back(pow((double)(ratedHot*ratedHot)/(itemsHot*itemsHot), i)*pow(1-(double)(ratedHot*ratedHot)/(itemsHot*itemsHot), itemsHot-i)*choose.at(itemsHot).at(i));
   for(int i = 0; i <= itemsCold; ++i)
      probCold.push_back(pow((double)(ratedCold*ratedCold)/(itemsCold*itemsCold), i)*pow(1-(double)(ratedCold*ratedCold)/(itemsCold*itemsCold), itemsCold-i)*choose.at(itemsCold).at(i));
   for(int i = 0; i <= items; ++i) {
      double p = 0;
      for(int j = std::max(0, i-itemsCold); j <= itemsHot && j <= i; ++j)
         p += probHot.at(j)*probCold.at(i-j);
      prob.push_back(p);
   } // end for
   
//   Print the chance for the graph to be connected for different horting parameters
   double total = 0;
   std::cout << 0 << " " << 1-total << '\n';
   for(int i = 0; 1-total > log(userCount)/userCount && i <= items; ++i) {
      total += prob.at(i);
      std::cout << i+1 << " " << 1-total << '\n';
   } // end for
   std::cout << '\n' << log(userCount)/userCount << '\n';
   
//   Create items in the dataset
   srand(time(0));
   std::vector<double> itemAvg;
   for(int i = 0; i < items; ++i)
      itemAvg.push_back(ratingFactory->rating());
   
//   Create users in the dataset
   std::vector<User*> users;
   for(int i = 0; i < userCount; ++i)
      users.push_back(userFactory->user());
   
   std::fstream file;
   std::string filename;
   
//   Store info about the dataset
   filename = "dataset/u.info";
   file.open(filename, std::fstream::out);
   file << "scale size: " << scale << '\n'
        << "user count: " << userCount << '\n'
        << "item coumt: " << items << '\n'
        << "hot item count: " << itemsHot << '\n'
        << "avg items rated: " << rated << '\n'
        << "avg hot items rated: " << ratedHot << '\n'
        << "at least items rated: " << ratedAtLeast << '\n'
        << "rating factory: " << ratingFactory->toString() << '\n'
        << "user factory: " << userFactory->toString() << '\n';
   file.close();
   
//   Create and pring ratings in the dataset
   filename = "dataset/u.data";
   file.open(filename, std::fstream::out);
   for(int i = 0; i < userCount; ++i) {
      int ratingCount;
      std::string ratings;
      do {
         ratingCount = 0;
         ratings = "";
         for(int j = 0; j < items; ++j) {
            double prob = j < itemsHot ? (double)ratedHot/itemsHot : (double)(rated-ratedHot)/(items-itemsHot);
            if((double)rand()/RAND_MAX < prob) {
               ++ratingCount;
               int rating = users.at(i)->rating(itemAvg.at(j));
               ratings += patch::to_string(i+1) + '\t' + patch::to_string(j+1) + '\t' + patch::to_string(rating) + '\n';
            } // end if
         } // end for
      } while(ratingCount < ratedAtLeast);
      file << ratings;
   } // end for
   file.close();
   
//   Print the users
   filename = "dataset/u.user";
   file.open(filename, std::fstream::out);
   for(int i = 0; i < userCount; ++i)
      file << i+1 << "|" << 0 << "|" << 0 << '\n';
   file.close();
   
//   Print the items
   filename = "dataset/u.item";
   file.open(filename, std::fstream::out);
   for(int i = 0; i < items; ++i)
      file << i+1 << "|" << "title" << '\n';
   file.close();
   
   return 0;
} // main
