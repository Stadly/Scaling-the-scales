#include "parser.h"

/**
 * @return A parsed dataset
 */
Dataset Parser::parse() const {
   Dataset dataset(m_name, m_scaleSize);

   parseUsers(dataset);
   parseItems(dataset);
   parseRatings(dataset);

   return dataset;
} // parse
