#ifndef A_PATCH_H
#define A_PATCH_H

// Replace patch::to_string with std::to_string and remove the below code when MinGW fixes the problem
#include <sstream>
namespace patch {
	template<typename T> std::string to_string(T const &n) {
		std::ostringstream stm;
		stm << n;
		return stm.str();
	} // to_string
} // patch

#endif // A_PATCH_H

