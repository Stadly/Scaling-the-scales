#ifndef RS_PARSER_H
#define RS_PARSER_H

#include "dataset.h"
#include <fstream>
#include <algorithm>

class Parser {
public:
   Parser(std::string const name, int const scaleSize) : m_name(name), m_scaleSize(scaleSize) {}
   Dataset parse() const;
protected:
   virtual void parseUsers(Dataset &dataset) const = 0;
   virtual void parseItems(Dataset &dataset) const = 0;
   virtual void parseRatings(Dataset &dataset) const = 0;
   std::string const m_name;
   int const m_scaleSize;
}; // Parser

#endif // RS_PARSER_H

