
#if !defined DATA_H

#define DATA_H

//#include "common.h"

#include "cvector.h"

#include <string>

#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "stringextra.h"
  // The data file class

typedef Columnvector * PColumnvector;

class DataFile {

  private:

    std::string                              Filename;

    std::vector<std::string>          Data;
    std::vector<std::string>          Alias;
    std::vector<std::string>          Heading;
    std::vector<PColumnvector>   Column;

    enum { version = 1 };


    std::string              Cline, Cword;

    int                 Ccolumns;

    char *              Cnumstatus;

    double *            Cnumber;

    int                 Cnext;


    int                 numbers ();
        
    int                 next_line (FILE * in);

    int                 find_section (const std::string);

    int                 find_data (const std::string, const std::string);

    int                 find_alias (const std::string oname);

    int                 find_column (const std::string head);

    std::string              strip_spaces (const std::string&);

    std::string              first_word (const std::string&);

    std::string              following_words (const std::string&);


  public:

                        DataFile ();
                       ~DataFile ();

    void                clear ();
    
    std::string              name ();

    int                 read (const std::string);

    int                 write (const std::string);

    std::string              data (const std::string = "", const std::string = "");

    std::string              alias (const std::string);

    Columnvector      * column (const std::string);

    int                 delete_data (const std::string section);
    
    int                 add_data (const std::string, const std::string);

    int                 delete_column (const std::string heading);
    
    int                 add_column (const std::string heading);

    int                 add_alias (const std::string heading, const std::string alias);

  };


#endif // DATA_H

