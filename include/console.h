
#include <stdlib.h>
#include <stdio.h>

const char * get_symbol (const char key []);

const char * set_symbol (const char key [], const char value []);

int cerror (const char format [], ...);

int cwarn (const char format [], ...);

int fcout (FILE * file, const char format [], ...);

int cout (const char format [], ...);

void flush_cin ();

int openconsole ();


