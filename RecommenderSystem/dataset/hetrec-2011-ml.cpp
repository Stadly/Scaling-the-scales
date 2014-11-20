#include "hetrec-2011-ml.h"

std::string const HetRec2011MovieLens::c_name = "hetrec-2011-ml";
std::string const HetRec2011MovieLens::c_ratings = "dataset/hetrec-2011-ml/user_ratedmovies.dat";
int const HetRec2011MovieLens::c_scaleSize = 10;

void HetRec2011MovieLens::parseUsers(Dataset &dataset) const {
   for(int i = 0; i < 2113; ++i)
      dataset.addUser(i, 0, 'U');
} // parseUsers
	
void HetRec2011MovieLens::parseItems(Dataset &dataset) const {
   for(int i = 0; i < 10197; ++i)
      dataset.addItem(i, "");
} // parseItems
	
void HetRec2011MovieLens::parseRatings(Dataset &dataset) const {
   std::unordered_map<int, int> users;
   std::unordered_map<int, int> items;
	std::ifstream file;
	file.open(c_ratings);
	std::string s;
   std::getline(file, s);
	while(file) {
		std::string delim = "\t";
		std::getline(file, s);
		if(!s.empty()) {
			auto start = 0U;
			auto end = s.find(delim);
			int user = atoi(s.substr(start, end-start).c_str())-1;
         if(0 == users.count(user))
            users.insert({user, (int)users.size()});
         user = users.at(user);
			start = end + delim.length();
			end = s.find(delim, start);
			int item = atoi(s.substr(start, end-start).c_str())-1;
         if(0 == items.count(item))
            items.insert({item, (int)items.size()});
         item = items.at(item);
			start = end + delim.length();
			end = s.find(delim, start);
			int rating = (int)std::round(2*atof(s.substr(start, end-start).c_str()));
			dataset.addRating(user, item, rating);
		} // end if
	} // end while
	file.close();
} // parseRatings
