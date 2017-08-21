/***********************************************************************}
{                                                                       }
{  the vector character set vector_c                                    }
{                                                                       }
{***********************************************************************/

#if !defined (CHAR_SET_H)

#define CHAR_SET_H


typedef struct {

  signed char x, y;

  } vector_character;


#if !defined (CHAR_SET_C)

extern vector_character * vector_c [];

#endif


#endif /* CHAR_SET_H */

