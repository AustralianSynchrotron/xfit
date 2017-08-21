
#if !defined DDATA_H

#define DDATA_H


  /* DLL functions  (these are just normal for Unix) */

extern void *  ReadDData (char * filename);
extern void *  WriteDData (void * D, char * filename);
extern void *  NewDData ();
extern void    FreeDData (void * D);
extern int     AddDDataColumn (void * D,
                                        const char * ColName,
                                        int ColN, double * ColD);
extern int     AddDDataAlias (void * D,
                                       const char * ColName,
                                       const char * Alias);
extern int     AddDDataEntry (void * D,
                                       const char * Section,
                                       const char * Entry);
extern int     DDataColumn (void * D,
                                     const char * ColName,
                                     double ** ColD);
extern char *  DDataEntry (void * D,
                                    const char * Section,
                                    const char * Key);


#endif /* DDATA_H */

