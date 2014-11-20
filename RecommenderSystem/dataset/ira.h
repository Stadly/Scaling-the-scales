#ifndef DS_IRA_H
#define DS_IRA_H

#include "../parser.h"

class DsIra: public Parser {
public:
	DsIra() : Parser(c_name, c_scaleSize) {}
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
}; // DsIra

#endif // DS_IRA_H

