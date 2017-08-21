/***********************************************************************}
{                                                                       }
{  Functions used when searching for files                              }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__SEARCH_C)

#define __SEARCH_C

#include "search.h"

  /**************************************************************}
  {                                                              }
  {  Each file is characterised by :                             }
  {                                                              }
  {     key (double-precision)                                   }
  {                                                              }
  {     index (int)                                              }
  {                                                              }
  {**************************************************************/

  /**************************}
  {                          }
  {  File search functions.  }
  {                          }
  {**************************/

#define FSR_searched(M,i) ((i) > (M).size ? 1 \
                            : (M).status [(i) / 4] & (1 << ((i) % 4)))

#define FSR_found(M,i)    ((i) > (M).size ? 0 \
                            : (M).status [(i) / 4] & (16 << ((i) % 4)))

#define set_FSR_searched(M,i) ((i) > (M).size ? 0 : \
                              ((M).status [(i) / 4] |= (1 << ((i) % 4))))

#define set_FSR_found(M,i)    ((i) > (M).size ? 0 : \
                              ((M).status [(i) / 4] |= (16 << ((i) % 4))))

#define reset_FSR_searched(M,i) ((i) > (M).size ? 0 : \
                              ((M).status [(i) / 4] &= ~(1 << ((i) % 4))))

#define reset_FSR_found(M,i)  ((i) > (M).size ? 0 : \
                              ((M).status [(i) / 4] &= ~(16 << ((i) % 4))))

void clear_FSR_node (FSR_node * node) {

  int i;

  node->key = 0;

  for (i = 0; i < (node->size + 3) / 4; i++)

    node->status [i] = 0;

  }

FSR_node * add_FSR_node (FSR_node ** next, int size) {

  FSR_node * node;

  node = calloc_check (1, sizeof (FSR_node) - 4 + (size + 3) / 4);

  if (node)

    node->size = size;

  *next = node;

  return node;

  }

void destruct_FSR_map (FSR_node ** node) {

  FSR_node * node0;

  for (node0 = *node; node0; node0 = *node) {

    *node = node0->next;

    free_check (node0);

    }
  }


  /**************************************************************}
  {                                                              }
  {  Open a file.                                                }
  {                                                              }
  {  If a previous attempt to open the file failed, return NULL  }
  {                                                              }
  {**************************************************************/

FILE * FSR_open_file (char filename [], double key, int index,

                      FSR_node ** map, int size, int ignore_found) {

  FILE * file;

  FSR_node * node;

    /* Find the first node with a matching key */

  node = NULL;

  while (*map != NULL && node == NULL)

    if ((*map)->key == key)

      node = *map;

    else

      map = &((*map)->next);


    /* If none of the nodes match, add a new one */

  if (node == NULL) {

    node = add_FSR_node (map, size);

    if (node)

      node->key = key;

    }


    /* If a previous attempt has failed, return 0 */

  if (node && !ignore_found)

    if (FSR_searched (*node, index) && !FSR_found (*node, index)) {

      return NULL;

      }


    /* Attempt to open the file */

  file = fopen (filename, "r");


    /* Update the status */

  if (node) {

    set_FSR_searched (*node, index);

    if (file)

      set_FSR_found (*node, index);

    else

      reset_FSR_found (*node, index);

    }

  return file;

  }

#endif /* __SEARCH_C */
