#include "ml-100k.h"

std::string const MovieLens100k::c_name = "ml-100k";
std::string const MovieLens100k::c_users = "dataset/ml-100k/u.user";
std::string const MovieLens100k::c_items = "dataset/ml-100k/u.item";
std::string const MovieLens100k::c_ratings = "dataset/ml-100k/u.data";
int const MovieLens100k::c_scaleSize = 5;

void MovieLens100k::parseUsers(Dataset &dataset) const {
	std::ifstream file;
	file.open(c_users);
	while(file) {
		std::string s;
		std::string delim = "|";
		std::getline(file, s);
		if(!s.empty()) {
			auto start = 0U;
			auto end = s.find(delim);
			int id = atoi(s.substr(start, end-start).c_str())-1;
			start = end + delim.length();
			end = s.find(delim, start);
			int age = s.substr(start, end-start).at(0);
			start = end + delim.length();
			end = s.find(delim, start);
			char gender = atoi(s.substr(start, end-start).c_str());
			dataset.addUser(id, age, gender);
		} // end if
	} // end while
	file.close();
} // parseUsers
	
void MovieLens100k::parseItems(Dataset &dataset) const {
	std::ifstream file;
	file.open(c_items);
	while(file) {
		std::string s;
		std::string delim = "|";
		std::getline(file, s);
		if(!s.empty()) {
			auto start = 0U;
			auto end = s.find(delim);
			int id = atoi(s.substr(start, end-start).c_str())-1;
			start = end + delim.length();
			end = s.find(delim, start);
			std::string title = s.substr(start, end-start);
			dataset.addItem(id, title);
		} // end if
	} // end while
	file.close();
} // parseItems
	
void MovieLens100k::parseRatings(Dataset &dataset) const {
	std::ifstream file;
	file.open(c_ratings);
	while(file) {
		std::string s;
		std::string delim = "\t";
		std::getline(file, s);
		if(!s.empty()) {
			auto start = 0U;
			auto end = s.find(delim);
			int user = atoi(s.substr(start, end-start).c_str())-1;
			start = end + delim.length();
			end = s.find(delim, start);
			int item = atoi(s.substr(start, end-start).c_str())-1;
			start = end + delim.length();
			end = s.find(delim, start);
			int rating = atoi(s.substr(start, end-start).c_str());
			dataset.addRating(user, item, rating);
		} // end if
	} // end while
	file.close();
} // parseRatings
