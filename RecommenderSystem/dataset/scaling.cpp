#include "scaling.h"

std::string const DsScaling::c_name = "scaling";
std::string const DsScaling::c_users = "dataset/"+DsScaling::c_name+"/u.user";
std::string const DsScaling::c_items = "dataset/"+DsScaling::c_name+"/u.item";
std::string const DsScaling::c_ratings = "dataset/"+DsScaling::c_name+"/u.data";
int const DsScaling::c_scaleSize = 13;

void DsScaling::parseUsers(Dataset &dataset) const {
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
	
void DsScaling::parseItems(Dataset &dataset) const {
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
	
void DsScaling::parseRatings(Dataset &dataset) const {
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
