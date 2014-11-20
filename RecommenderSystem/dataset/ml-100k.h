#ifndef DS_ML_100K_H
#define DS_ML_100K_H

#include "../parser.h"

class MovieLens100k: public Parser {
public:
	MovieLens100k() : Parser(c_name, c_scaleSize) {}
protected:
	void parseUsers(Dataset &dataset) const;
	void parseItems(Dataset &dataset) const;
	void parseRatings(Dataset &dataset) const;
private:
	static std::string const c_name;
	static std::string const c_users;
	static std::string const c_items;
	static std::string const c_ratings;
	static int const c_scaleSize;
}; // MovieLens100k

#endif // DS_ML_100K_H

