#ifndef RS_ITEM_H
#define RS_ITEM_H

#include <string>

class Item {
public:
   int const id;
   std::string const title;
   Item(int const id, std::string const title) : id(id), title(title) {}
}; // Item

#endif // RS_ITEM_H

