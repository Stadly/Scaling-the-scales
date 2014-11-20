#ifndef DSG_PATCH_H
#define DSG_PATCH_H

#include <sstream>
#include <iostream>
#include <vector>

namespace patch {
//   Replace patch::to_string with std::to_string and remove the below code when MinGW fixes the problem
	template<typename T> std::string to_string(T const &n) {
		std::ostringstream stm;
		stm << n;
		return stm.str();
	} // to_string
   
   template<typename T> unsigned long long fact(T const n) {
      unsigned long long res = 1;
      for(int i = 2; i <= n; ++i)
         res *= i;
      return res;
   } // fact
   
   template<typename T> unsigned long long binom(T const n, T const k) {
      return fact(n)/(fact(k)*fact(n-k));
   } // binom
   
   template<typename T> std::vector<std::vector<unsigned long long>> binom(T const n) {
      std::vector<std::vector<unsigned long long>> pascal;
      pascal.push_back({1,0});
      for(int i = 1; i <= n; ++i) {
         pascal.push_back({1});
         for(int j = 1; j <= i; ++j)
            pascal.at(i).push_back(pascal.at(i-1).at(j-1) + pascal.at(i-1).at(j));
         pascal.at(i).push_back(0);
      } // end for
      return pascal;
   } // binom
} // patch

#endif // DSG_PATCH_H

