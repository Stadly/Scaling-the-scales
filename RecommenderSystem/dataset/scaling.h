#ifndef DS_SCALING_H
#define DS_SCALING_H

#include "../parser.h"

class DsScaling: public Parser {
public:
	DsScaling() : Parser(c_name, c_scaleSize) {}
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
}; // DsScaling

#endif // DS_SCALING_H

