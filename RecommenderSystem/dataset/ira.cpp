#include "ira.h"

std::string const DsIra::c_name = "ira";
std::string const DsIra::c_users = "dataset/"+DsIra::c_name+"/u.user";
std::string const DsIra::c_items = "dataset/"+DsIra::c_name+"/u.item";
std::string const DsIra::c_ratings = "dataset/"+DsIra::c_name+"/u.data";
int const DsIra::c_scaleSize = 13;

void DsIra::parseUsers(Dataset &dataset) const {
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
	
void DsIra::parseItems(Dataset &dataset) const {
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
	
void DsIra::parseRatings(Dataset &dataset) const {
	std::ifstream file;
	file.open(c_ratings);
	while(file) {
		std::string s;
		std::string delim = "\t";
		std::getline(file, s);
		if(!s.empty() && s.at(0) != '#') {
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
