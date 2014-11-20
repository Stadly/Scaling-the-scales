#ifndef DSG_RATING_FACTORY_H
#define DSG_RATING_FACTORY_H

#include "patch.h"
#include <random>
#include <time.h>

class RatingFactory {
public:
   RatingFactory(std::string const name, int const scaleSize) : m_name(name), m_scaleSize(scaleSize) {}
   virtual double rating() = 0;
   virtual std::string toString() const = 0;
protected:
	std::string const m_name;
   std::default_random_engine m_generator;
	signed char const m_scaleSize;
};

#endif // DSG_RATING_FACTORY_H
