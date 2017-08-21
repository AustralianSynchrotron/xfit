// Extra string functions not provided by the standard class.
// So far, only upper case

#include <ctype.h>
#include <string>

std::string upcase (std::string lcasestr) {
   int i;
   std::string retstr;
   retstr = lcasestr;
   for(i=0;i<lcasestr.size();i++) {
      retstr[i] = toupper(lcasestr[i]);
      }
   return retstr;
   }

