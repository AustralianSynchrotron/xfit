/***********************************************************************}
{                                                                       }
{  draw the model                                                       }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_MODEL_H)

#define DRAW_MODEL_H


typedef struct {

  char * symbol;

  double covalent, x, y, z, sigma2;

  int number, connected;

  } AtomData;

typedef struct {
    
  int atoms;

  AtomData * atom;

  } ModelData;


  /*********************************************************************}
  {                                                                     }
  {  Redraw the model window                                            }
  {                                                                     }
  {*********************************************************************/

ModelData * redraw_model ();


#endif /* DRAW_MODEL_H */

