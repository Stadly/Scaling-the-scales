#ifndef RS_PATCH_H
#define RS_PATCH_H

#include <sstream>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#include <iostream>

namespace patch {
   double const precision = 0.0000001;
   
   // TODO Replace patch::to_string with std::to_string and remove the below code when MinGW fixes the problem
   template<typename T> std::string to_string(T const &n) {
      std::ostringstream stm;
      stm << n;
      return stm.str();
   } // to_string
   
   template<typename T> T gcd(T a, T b) {
      while(b != 0) {
         T t = b;
         b = a%b;
         a = t;
      } // end while
      return std::abs(a);
   } // gcd
   
   template<typename T> bool doubleLt(T const a, T const b) {
      return a < b+precision;
   } // doubleLt
   
   template<typename T> bool doubleLe(T const a, T const b) {
      return a <= b+precision;
   } // doubleLe
} // patch

#endif // RS_PATCH_H
