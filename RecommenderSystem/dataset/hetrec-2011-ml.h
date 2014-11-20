#ifndef DS_HETREC_2011_ML_H
#define DS_HETREC_2011_ML_H

#include "../parser.h"
#include <unordered_map>

class HetRec2011MovieLens: public Parser {
public:
	HetRec2011MovieLens() : Parser(c_name, c_scaleSize) {}
protected:
	void parseUsers(Dataset &dataset) const;
	void parseItems(Dataset &dataset) const;
	void parseRatings(Dataset &dataset) const;
private:
	static std::string const c_name;
	static std::string const c_ratings;
	static int const c_scaleSize;
}; // HetRec2011MovieLens

#endif // DS_HETREC_2011_ML_H

