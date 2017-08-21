
#if !defined DATA_CPP

#define DATA_CPP

#include "data.h"

#include <iostream>
#include <fstream>
#include <ctype.h>         /* for toupper function */

#define max(A,B) ((A) >= (B) ? (A) : (B))

DataFile::DataFile () {

  Cnext = 0;
  
  Cline = "";

  Cword = "";

  Ccolumns = 0;

  Cnumstatus = NULL;

  Cnumber = NULL;
  
  }


DataFile::~DataFile () {

  clear ();
  
  }


std::string DataFile::name () {

  return Filename;

  }


std::string DataFile::strip_spaces (const std::string& line) {

    // Strip any spaces at the start or end

  int s, e;

  if (line == "")

    return "";
    
  std::string::size_type frontwhite = line.find_first_not_of(" \t\n");
  std::string::size_type endwhite = line.find_last_not_of(" \t\n");
  return line.substr(frontwhite, endwhite-frontwhite+1);
  }


std::string DataFile::first_word (const std::string& line) {

    // Return the first word in the line (converted to upper case)

  int s, e;
  std::string templine;
  
  if (line == "")

    return "";
    
  const char * c = line.c_str();

  for (s = 0; c [s]; s++)

    if (c [s] != ' ')

      break;

  if (!c [s])

    return "";

  for (e = s; c [e]; e++)

    if (c [e] == ' ')

      break;


  templine = line;
  templine = upcase(templine.substr (s, e - s));
  return templine;

  }


std::string DataFile::following_words (const std::string& line) {

    // Return the line after the first word

  int s, e;
  std::string templine;
  
  if (line == "")

    return "";
    
  const char * c = line.c_str();

  for (s = 0; c [s]; s++)

    if (c [s] != ' ')

      break;

  for (; c [s]; s++)

    if (c [s] == ' ')

      break;

  for (; c [s]; s++)

    if (c [s] != ' ')

      break;

  if (!c [s])

    return "";

  for (e = strlen (c) - 1; e >= 0; e--)

    if (c [e] != ' ')

      break;

  return line.substr (s, e - s + 1);

  }


int DataFile::find_column (const std::string head) {

  int c;

  std::string heading;


    // Check the heading against the list of aliases

  heading = alias (head);


    // Check the list of headings

  for (c = 0; c < Heading.size () && c < Column.size (); c++)
  
    if (upcase(Heading [c]) == heading)

      return c + 1;


    // Failure
    
  return 0;

  }


int DataFile::delete_column (const std::string head) {

  int c;


    // Find any alias

  std::string heading = alias (head);
    

    // Find the column

  c = find_column (head);

  if (c) {

      // Delete the column and heading

    Column.erase (Column.begin() + c - 1 );

    Heading.erase (Heading.begin() +  c - 1 );
    
    }


    // Delete any aliases
    
  for (c = 0; c < Alias.size (); )

    if (first_word (Alias [c]) == heading)

      Alias.erase (Alias.begin() + c);

    else

      c++;


  return TRUE;

  }

    
int DataFile::add_column (const std::string head) {
  
    // Check the heading

  std::string hh = strip_spaces (head);

  if ((hh.length()) == 0)

    return FALSE;
      

    // Delete any column with the given heading

  delete_column (hh);


    // Add the new column and heading

  Heading.insert (Heading.begin() + Column.size (), hh);

  Column.insert (Column.end(), new Columnvector);


  return TRUE;

  }
  

int DataFile::add_alias (std::string heading, std::string alias) {

  std::string additional;
  char aspace = ' ';
  additional.append(&aspace);
  additional.append(strip_spaces(heading));
  additional.append(&aspace);
  additional.append(strip_spaces(alias));
  Alias.insert (Alias.end(), additional);

  return TRUE;
  
  }


int DataFile::find_alias (const std::string oname) {

    // Strip any spaces

  std::string name = upcase(strip_spaces (oname));

  if (name == "")

    return 0;


    // Compare the name to all the aliases

  for (int c = 0; c < Alias.size (); c++)

    if (following_words (upcase(Alias [c])) == name)

      return c + 1;


    // Failure

  return 0;

  }



std::string DataFile::alias (const std::string oname) {

  int c;


    // Find any alias

  c = find_alias (oname);


    // If there is an alias substitute the actual name

  if (c)  

    return first_word (Alias [c - 1]);


    // Otherwise return the name without redundant spaces in upper case
    
  return upcase(strip_spaces (oname));
  
  } 


int DataFile::find_section (const std::string section) {

  int c;

  std::string com = strip_spaces (section);

  for (c = 0; c < Data.size (); c++)
        
    if (upcase(Data [c]) == upcase(com))

      return c + 1;

  return 0;

  } 


int DataFile::find_data (const std::string section, const std::string keyword) {

  int c, spaces;


    // Find the section

  c = find_section (section);

  if (!c)

    return 0;


    // If there is a keyword, check for a match

  std::string key = upcase(strip_spaces (keyword));

  if (key == "")

    return c;


  while (c < Data.size ()) {
      
    for (spaces = 0; spaces < Data [c].length (); spaces++)

      if (Data [c][spaces] != ' ')

        break;

    if (spaces == 0 && Data [c] != "")

      break;
      
    if (upcase(Data [c]).find (key) == spaces)

      return c;

    c++;

    }


    // Failure

  return 0;

  } 


int DataFile::delete_data (const std::string section) {

    // delete the specified section

  int c, OK;

  OK = FALSE;

  while ((c = find_section (section)) != 0) {

    c--;
    
      // At the start of the section

    int deleted = TRUE;
        

      // Delete all the lines in this section
        
    while (c < Data.size ()) {

      deleted = deleted || Data [c] == "";

      if (!deleted)
          
        deleted = Data [c][0] == ' ';

      if (deleted) {

        Data.erase (Data.begin() + c);

        OK++;
          
        }
      else
          
        break;

      deleted = FALSE;

      }
    }

  return OK;
      
  }


int DataFile::add_data (const std::string section, const std::string n) {

  int c, last, count, OK;

  std::string data, tempn;
  

    // Delete any redundant spaces

  for (c = n.length () - 1; c >= 0; c--)

    if (n [c] != ' ')

      break;

  if (c < 0)

    return 0;
    
  tempn = n;
  data = tempn.substr (0, c + 1);


  OK = FALSE;

  c = find_section (section);

  if (!c) {

      // Create the section

    if (Data.size ())

      if (Data [Data.size () - 1] != "")
      
        Data.insert (Data.end(),"");

    Data.insert (Data.end(),strip_spaces (section));

    c = Data.size ();

    }

  c--;


    // Update the date stamp

  

    // Find the position to insert the new data

  last = c + 1;

  for (count = c + 1; count < Data.size (); count++) {

    if (Data [count] == "")

      break;

    else
      
      if (Data [count][0] != ' ') {

          // Insert a blank line before the next section

        Data.insert (Data.begin() + count, "");

        count++;
          
        break;

        }
    }


    // Insert the new data
    /* the clumsy way of prepending space is due to a problem with the +
       operator in GCC3.2 */

  std::string additional;
  char aspace = ' ';
  additional.append(&aspace);
  additional.append(&aspace);
  additional.append(data);
  //data.insert (data.begin(), additional);   //addition of strings gives link error in GCC3.2
  
  Data.insert (Data.begin() + count, data);

  return OK;

  }

  

std::string DataFile::data (const std::string section, const std::string keyword) {

  int c;


  if (section != "")

    Cnext = find_data (section, keyword);


  if (!Cnext)

    return "";

    
  while (Cnext < Data.size ()) {

      // Check for the start of the next section

    if (Data [Cnext] != "")

      if (Data [Cnext][0] != ' ')

        break;


      // Return the data

    std::string& o = Data [Cnext];

    std::string key = upcase(strip_spaces (keyword));

    Cnext++;


    if (key != "")
    
      c = upcase(o).find (key) + key.length ();

    else

      c = 0;

    while (c < o.length ()) {

      if (o [c] == ' ')

        break;

      c++;
        
      }

    if (c == o.length ())

      return "";
        
    return strip_spaces (o.substr (c, o.length () - c));
        
    }


    // No more lines

  Cnext = 0;

  return "";

  }


  // Return the column with the given heading

Columnvector * DataFile::column (const std::string head) {

  int c;


    // Find the column

  c = find_column (head);

  if (c) 

    return Column [c - 1];


    // Failure
    
  return NULL;

  }


int DataFile::write (const std::string filename) {

  int c;

  
    // Send the program data

  std::ofstream out;

  out.open (filename.c_str());

  if (out.fail ())

    return FALSE;
  
      
  for (c = 0; c < Data.size (); c++)
  
	  out << Data [c] << std::endl;

  out << std::endl;


    // Send the aliases

  out << "DATA" << std::endl;
  
  for (c = 0; c < Alias.size (); c++)

	  out << Alias [c] << std::endl;

  out << std::endl;


    // Send the headings

  for (c = 0; c < Heading.size (); c++)

    out << " " << Heading [c];

  out << std::endl;


    // Send the actual data
    
  int points = 0;

  out.setf (std::ios::scientific);
  out.setf (std::ios::fixed);

  out.precision (9);

  for (c = 0; c < Column.size (); c++)

    points = max (points, Column [c]->Nrows ());

  for (int point = 1; point <= points; point++) {

    out << ' ';
    
    for (c = 0; c < Column.size (); c++) {

      if (c > 0)

        out << ',';

      if (point <= Column [c]->Nrows ())

        out << Column [c]->operator () (point);

      }

    out << std::endl;

    }

  out.close ();
  
  return TRUE;

  }


void DataFile::clear () {

    // Delete the temporary variables

  Cnext = 0;
  
  if (Cnumstatus)

    delete[] Cnumstatus;

  Cnumstatus = NULL;

  if (Cnumber)

    delete[] Cnumber;

  Cnumber = NULL;

  Ccolumns = 0;


    // Clear the data



    // Clear the headings
    

    // Delete the data columns

  for (int c = 0; c < Column.size (); c++)

    delete Column [c];


    // Clear the file name

  Filename = "";


    // Clear the current line buffer

  Cline = "";
  
  Cword = "";
  
  }


int DataFile::numbers () {

    // Parse the set of numbers in the current line

  int count, c, co;

  count = c = 0;

  int length;

  if (Cline == "")

    return 0;
    
  const char * line = Cline.c_str();

  length = Cline.length ();

  while (c < length) {

    unsigned int n;

    int OK;
    
    double d;

    char ch [2];

    ch [1] = 0;


      // Skip whitespace

    while (line [c] == ' ')

      c++;
      
 
      // Attempt to read a number

    d = 0;

    n = 0;

    sscanf (line + c, "%lf%n", &d, &n);

    OK = n;

    c += n;
    

      // Skip whitespace

    while (line [c] == ' ')

      c++;
      
 
      // Possibly remove a single ','

    if (line [c] == ',') {

      c++;

      if (!OK)
      
        OK = -1;

      }


      // Save the number

    if (OK) {

        // Add space

      if (count > Ccolumns - 2) {

        char * oCnumstatus = Cnumstatus;

        double * oCnumber = Cnumber;

        int oCcolumns = Ccolumns;

        Ccolumns = Ccolumns + 8;
        
        Cnumstatus = new char [Ccolumns];
        
        Cnumber = new double [Ccolumns];        

        for (co = 0; co < oCcolumns - 1; co++) {

          Cnumstatus [co] = oCnumstatus [co];
            
          Cnumber [co] = oCnumber [co];

          }

        if (oCnumstatus)

          delete[] oCnumstatus;

        if (oCnumber)

          delete[] oCnumber;

        }

      if (OK > 0)

        Cnumstatus [count] = 'Y';

      else

        Cnumstatus [count] = 'N';

      Cnumstatus [count + 1] = 0;

      Cnumber [count] = d;
      
      count++;

      }
    else

      break;
          
    }

  return count;
  
  }


#define BUFFER 256 
    
int DataFile::next_line (FILE * in) {

    // Read the next line of a file into the current line buffer

  Cline = "";

  Cword = "";

  int c, word, next, l;

  char ch [2], line [BUFFER];

  ch [1] = 0;

  word = FALSE;

  line [0] = 0;
  
  next = 0;
  
  while (TRUE) {

    if (!line [next]) {

        // Refill the buffer

      char * B;
      
      B = fgets (line, BUFFER, in);

      if (!B)

        line [0] = 0;

      next = 0;

      }


    if (line [next]) {

      c = line [next];

      next++;

      }
    else

      c = EOF;

    ch [0] = (char) c;


    switch (c) {

      case EOF:
      
      case '\n':

          // End of line

          // Remove any redundant spaces

        for (l = Cline.length (); l > 0; l--)

          if (Cline [l - 1] != ' ')

            break;

        if (l < Cline.length ())

          Cline = Cline.substr (0, l);

        return c != EOF || Cline.length () > 0;
        
        
      case '\v':
      case '\r':
      case '\f':

          // Ignore selected whitespace characters

        break;


      case '\t':

          // Convert TABs to spaces (8 character steps)
          
        c = 8 - (Cline.length () % 8);

        ch [0] = ' ';
        
        while (--c > 0)

          Cline += ch;

        c = ' ';


      default:

          // Add the character to the line

        Cline += ch;


          // Build the first word

        *ch = (char) toupper (*ch);
        
        if (!word)

          if (c != ' ')

            Cword += ch;

          else

            word = Cword.length ();

      } 
    }
  }


  // Read a file

int DataFile::read (const std::string filename) {

    // Reset
    
  clear ();


    // Open the file

  FILE * in;

  in = fopen (filename.c_str(), "r");

  if (!in)

    return 0;
      
    
    // Get the program data (any sections before "DATA")
        
  while (next_line (in)) {

    if (Cword == "DATA")

      break;
      
    Data.insert (Data.end(), Cline);

    }


  int use_Cline = FALSE, start = FALSE, stop = FALSE;
  
  std::string headings = "";


    // Get the headings and heading aliases

  if (Cword == "DATA")

    start = TRUE;
    

    // Read up to a blank line or the data

  while (next_line (in)) {

    if (numbers ()) {

      use_Cline = TRUE;

      break;

      }

    if (start && !stop && headings != "") {
              
      Alias.insert (Alias.end(),headings);

      headings = "";

      }

    if (Cword == "")

      stop = start;

    else
        
      if (!start && Cword == "DATA" && Cline [0] != ' ')

        start = TRUE;

      else
    
        if ((start && !stop) ||
           ((!start || stop) && (Cword != "C" || Cline [0] == ' ')))

          headings = Cline;
           
    }


  int * length;

  int OK, count, c;
    

    // Copy the headings

  if (headings != "") {

    char s [2];

    int o = 0;

    std::string heading;

    const char * c = (const char *) headings.c_str();

    s [1] = 0;
    
    while (c [o]) {

      heading = "";
      
      while (c [o] == ' ')

        o++;

      while (c [o] != ' ' && c [o]) {

        *s = c [o];

        heading += s;

        o++;

        }

      if (heading != "")

        Heading.insert (Heading.end(),heading);

      }
    }    


    // Copy the data columns

  length = NULL;
    
  count = 0;
    
  if (use_Cline) {

    while (TRUE) {

      if (!use_Cline)
      
        if (!next_line (in))

          break;

      use_Cline = FALSE;
      
      if (Cword == "END")

        break;


      if (numbers ()) {

          // Create the columns if necessary

        if (count == 0)

          if (Column.size () == 0) {

            for (c = 0; Cnumstatus [c] && c < Heading.size (); c++)

              Column.insert (Column.end(), new Columnvector);


            length = new int [c];

            if (!length)

              return 0;

            for (c = 0; Cnumstatus [c]; c++)

              length [c] = 0;

            }


          // Add the new entries

        OK = 0;
        
        for (c = 0; c < Column.size () && Cnumstatus [c]; c++) {

          if (length [c] == count && Cnumstatus [c] == 'Y') {

            Columnvector * CV = Column [c];
            
            if (count + 1 > CV->Nrows ())

              CV->ReFrame (count + BUFFER);

            CV->operator () (count + 1) = Cnumber [c];

            length [c] = count + 1;

            OK = 1;
            
            }
          }
       
        count += OK;
        
        }
      }
    }


    // Truncate the vectors to their proper lengths
    
  if (length)

    for (c = 0; c < Column.size (); c++)

      Column [c]->ReFrame (length [c]);

  delete[] length;


  Filename = filename;
    
  return count;

  }


#endif // DATA_CPP

