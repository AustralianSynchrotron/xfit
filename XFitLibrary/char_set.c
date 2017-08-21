/***********************************************************************}
{                                                                       }
{  the vector character set vector_c                                    }
{                                                                       }
{***********************************************************************/

#if !defined (CHAR_SET_C)

#define CHAR_SET_C

#include "char_set.h"

#define E ,{ -1, 0 },
#define S ,{ -1, 1 }


  /* BOX = 583 x 640 */

vector_character c_A [] =

    { {  92,  83 },
      {   0,   0 }, {  30,   0 }  E
      {  71,   0 }, { 100,   0 }  E
      {  22,  36 }, {  78,  36 }  E
      {   9,   0 }, {  46, 100 }  E
      {  16, 100 }, {  56, 100 }, {  93,   0 } S };

vector_character c_B [] =

    { {  78,  83 },
      {  22, 100 }, {  22,   0 }  E
      {  22,  52 }, {  66,  52 }  E
      {   0,   0 }, {  73,   0 }, {  89,   6 },
      { 100,  18 }, { 100,  33 }, {  88,  46 },
      {  66,  52 }, {  82,  59 }, {  91,  70 },
      {  91,  83 }, {  82,  94 }, {  65, 100 }, {   0, 100 } S };

vector_character c_C [] =

    { {  74,  83 },
      {  96,  99 }, {  96,  76 }  E
      {  96,  82 }, {  82,  93 }, {  64, 100 }, {  37, 100 },
      {  13,  88 }, {   0,  66 }, {   0,  34 }, {  14,  12 },
      {  41,   0 }, {  70,   0 }, {  89,   8 }, { 100,  17 }  S };

vector_character c_D [] =

    { {  75,  83 },
      {  14,   0 }, {  14, 100 }  E
      {   0,   0 }, {  61,   0 }, {  85,  11 }, { 100,  36 },
      { 100,  64 }, {  85,  89 }, {  61, 100 }, {   0, 100 }  S };

vector_character c_E [] =

    { {  75,  83 },
      {  19,   0 }, {  19, 100 }  E
      {  19,  52 }, {  61,  52 }  E
      {  61,  38 }, {  61,  66 }  E
      {   0,   0 }, { 100,   0 }, { 100,  29 }  E
      {   0, 100 }, {  96, 100 }, {  96,  77 }  S };

vector_character c_F [] =

    { {  75,  83 },
      {   0,   0 }, {  56,   0 }  E
      {  61,  38 }, {  61,  66 }  E
      {  19,   0 }, {  19, 100 }  E
      {  19,  52 }, {  61,  52 }  E
      {   0, 100 }, { 100, 100 }, { 100,  77 }  S };

vector_character c_G [] =

    { {  71,  86 },
      {  54,  41 }, { 100,  41 }  E
      {  91,  80 }, {  91,  98 }  E
      {  91,  41 }, {  91,   7 }, {  67,   0 }, {  38,   0 },
      {  12,  12 }, {   0,  34 }, {   0,  67 }, {  12,  88 },
      {  35, 100 }, {  61, 100 }, {  78,  95 }, {  91,  87 }  S };

vector_character c_H [] =

    { {  78,  83 },
      {   0,   0 }, {  35,   0 }  E
      {  65,   0 }, { 100,   0 }  E
      {   5, 100 }, {  35, 100 }  E
      {  65, 100 }, {  95, 100 }  E
      {  16,  52 }, {  84,  52 }  E
      {  16,   0 }, {  16, 100 }  E
      {  84,   0 }, {  84, 100 }  S };

vector_character c_I [] =

    { {  57,  83 },
      {   0,   0 }, { 100,   0 }  E
      {   0, 100 }, { 100, 100 }  E
      {  50,   0 }, {  50, 100 }  S };

vector_character c_J [] =

    { {  78,  85 },
      {  30, 100 }, { 100, 100 }  E
      {   0,  42 }, {   0,  15 }, {  10,   7 }, {  25,   0 },
      {  46,   0 }, {  62,   9 }, {  72,  24 }, {  72, 100 }  S };

vector_character c_K [] =

    { {  85,  83 },
      {   0,   0 }, {  38,   0 }  E
      {   0, 100 }, {  38, 100 }  E
      {  17,   0 }, {  17, 100 }  E
      {  68, 100 }, {  96, 100 }  E
      {  17,  41 }, {  90, 100 }  E
      {  36,  57 }, {  54,  49 }, {  66,  38 },
      {  77,  17 }, {  85,   0 }, { 100,   0 }  S };

vector_character c_L [] =

    { {  75,  83 },
      {   0, 100 }, {  58, 100 }  E
      {  29,   0 }, {  29, 100 }  E
      {   0,   0 }, { 100,   0 }, { 100,  36 }  S };

vector_character c_M [] =

    { {  93,  83 },
      {   0,   0 }, {  31,   0 }  E
      {  69,   0 }, { 100,   0 }  E
      {  11,   0 }, {  11, 100 }  E
      {  89,   0 }, {  89, 100 }  E
      {   2, 100 }, {  20, 100 }, {  49,  32 },
      {  53,  32 }, {  80, 100 }, {  99, 100 }  S };

vector_character c_N [] =

    { {  86,  83 },
      {   4,   0 }, {  38,   0 }  E
      {  67, 100 }, { 100, 100 }  E
      {  17,   0 }, {  17, 100 }  E
      {   0, 100 }, {  21, 100 },
      {  83,   0 }, {  88,   0 }, {  88, 100 }  S };

vector_character c_O [] =

    { {  77,  86 },
      {  37,   0 }, {  63,   0 }, {  87,  13 },
      { 100,  37 }, { 100,  63 }, {  86,  88 },
      {  63, 100 }, {  37, 100 }, {  14,  88 },
      {   0,  63 }, {   0,  37 }, {  13,  13 }, {  37,   0 }  S };

vector_character c_P [] =

    { {  70,  83 },
      {   0,   0 }, {  61,   0 }  E
      {  20,   0 }, {  20, 100 }  E
      {   0, 100 }, {  69, 100 }, {  89,  92 }, { 100,  79 },
      { 100,  66 }, {  90,  53 }, {  66,  44 }, {  20,  44 }  S };

vector_character c_Q [] =

    { {  77,  86 },
      {  54,   0 }, {  21, -16 }, {  21, -19 }, {  44, -14 },
      {  62, -14 }, {  78, -19 }, {  87, -19 }, { 100, -13 }  E
      {  37,   0 }, {  63,   0 }, {  87,  13 },
      { 100,  37 }, { 100,  63 }, {  86,  88 },
      {  63, 100 }, {  37, 100 }, {  14,  88 },
      {   0,  63 }, {   0,  37 }, {  13,  13 }, {  37,   0 }  S };

vector_character c_R [] =

    { {  86,  83 },
      {   0,   0 }, {  37,   0 }  E
      {  16,   0 }, {  16, 100 }  E
      {  16,  48 }, {  49,  48 }  E
      {   0, 100 }, {  57, 100 }, {  73,  92 },
      {  82,  81 }, {  82,  67 }, {  70,  54 }, {  49,  48 },
      {  64,  38 }, {  75,  26 }, {  90,   0 }, { 100,   0 }  S };

vector_character c_S [] =

    { {  64,  87 },
      {   0,   2 }, {   0,  26 }  E
      {  95,  77 }, {  95,  98 }  E
      {   0,  16 }, {  16,   6 }, {  35,   0 },
      {  67,   0 }, {  89,   8 }, { 100,  20 },
      { 100,  34 }, {  90,  44 }, {  72,  49 }, {  37,  54 },
      {  18,  59 }, {   6,  70 }, {   6,  81 }, {  16,  92 },
      {  36, 100 }, {  63, 100 }, {  79,  95 }, {  95,  85 }  S };

vector_character c_T [] =

    { {  72,  83 },
      {  17,   0 }, {  83,   0 }  E
      {  50,   0 }, {  50, 100 }  E
      {   0,  69 }, {   0, 100 }, { 100, 100 }, { 100,  69 }  S };

vector_character c_U [] =

    { {  83,  85 },
      {   0, 100 }, {  35, 100 }  E
      {  65, 100 }, { 100, 100 }  E
      {  13, 100 }, {  13,  25 }, {  23,   9 }, {  40,   0 },
      {  60,   0 }, {  77,   9 }, {  87,  25 }, {  87, 100 }  S };

vector_character c_V [] =

    { {  90,  83 },
      {   0, 100 }, {  31, 100 }  E
      {  69, 100 }, { 100, 100 }  E
      {   9, 100 }, {  49,   0 }, {  53,   0 }, {  91, 100 }  S };

vector_character c_W [] =

    { {  89,  84 },
      {   0, 100 }, {  32, 100 }  E
      {  67, 100 }, { 100, 100 }  E
      {   8, 100 }, {  19,   0 }, {  26,   0 }, {  48,  78 },
      {  52,  78 }, {  74,   0 }, {  80,   0 }, {  92, 100 }  S };

vector_character c_X [] =

    { {  82,  83 },
      {   0,   0 }, {  30,   0 }  E
      {  70,   0 }, { 100,   0 }  E
      {   2, 100 }, {  29, 100 }  E
      {  71, 100 }, {  98, 100 }  E
      {  10, 100 }, {  93,   0 }  E
      {   7,   0 }, {  90, 100 }  S };

vector_character c_Y [] =

    { {  81,  83 },
      {   0, 100 }, {  27, 100 }  E
      {  73, 100 }, { 100, 100 }  E
      {  21,   0 }, {  79,   0 }  E
      {  50,   0 }, {  50,  46 }  E
      {   8, 100 }, {  50,  46 }, {  92, 100 }  S };

vector_character c_Z [] =

    { {  61,  83 },
      {   6,  72 }, {   6, 100 }, {  94, 100 }, {  94,  95 },
      {   0,   6 }, {   0,   0 }, { 100,   0 }, { 100,  31 }  S };

vector_character c_a [] =

    { {  74,  65 },
      {  11,  92 }, {  39, 100 }, {  59, 100 },
      {  74,  91 }, {  80,  78 }, {  80,   2 }, { 100,   2 }  E
      {  80,  52 }, {  53,  56 }, {  32,  56 },
      {  10,  48 }, {   0,  34 }, {   0,  19 }, {   7,   7 },
      {  21,   0 }, {  42,   0 }, {  63,   9 }, {  80,  24 }  S };

vector_character c_b [] =

    { {  82,  91 },
      {   0, 100 }, {  18, 100 }, {  18,   1 }, {   0,   1 }  E
      {  18,  25 }, {  28,  11 }, {  48,   0 }, {  70,   0 },
      {  89,   9 }, { 100,  25 }, { 100,  44 }, {  89,  61 },
      {  70,  70 }, {  48,  70 }, {  28,  59 }, {  18,  45 }  S };

vector_character c_c [] =

    { {  71,  64 },
      {  94,  70 }, {  94,  97 }  E
      {  94,  81 }, {  79,  94 }, {  60, 100 }, {  36, 100 },
      {  12,  86 }, {   0,  64 }, {   0,  36 }, {  13,  13 },
      {  35,   0 }, {  65,   0 }, {  85,   7 }, { 100,  19 }  S };

vector_character c_d [] =

    { {  83,  91 },
      {  65, 100 }, {  82, 100 }, {  82,   1 }, { 100,   1 }  E
      {  82,  48 }, {  69,  62 }, {  52,  70 }, {  30,  70 },
      {  10,  61 }, {   0,  45 }, {   0,  25 }, {  10,  10 },
      {  30,   0 }, {  52,   0 }, {  70,  10 }, {  82,  25 }  S };

vector_character c_e [] =

    { {  69,  65 },
      {   0,  52 }, { 100,  52 }, {  99,  62 },
      {  92,  78 }, {  79,  93 }, {  59, 100 }, {  38, 100 },
      {  13,  87 }, {   0,  66 }, {   0,  37 }, {  14,  14 },
      {  41,   0 }, {  67,   0 }, {  86,   6 }, { 100,  15 }  S };

vector_character c_f [] =

    { {  68,  89 },
      {   0,   0 }, {  84,   0 }  E
      {   2,  67 }, {  86,  67 }  E
      { 100,  98 }, {  76, 100 }, {  55, 100 },
      {  39,  95 }, {  32,  86 }, {  32,   0 }  S };

vector_character c_g [] =

    { {  77,  61 },
      { 100,  96 }, {  81,  97 }, {  81, -22 },
      {  72, -41 }, {  57, -52 }, {  21, -52 }  E
      {  81,  67 }, {  68,  87 }, {  51, 100 }, {  30, 100 },
      {  10,  87 }, {   0,  65 }, {   0,  36 }, {  11,  12 },
      {  30,   0 }, {  50,   0 }, {  68,  13 }, {  81,  34 }  S };

vector_character c_h [] =

    { {  82,  90 },
      {   2,   0 }, {  33,   0 }  E
      {  69,   0 }, { 100,   0 }  E
      {   0, 100 }, {  17, 100 }, {  17,   0 }  E
      {  17,  52 }, {  31,  63 }, {  45,  70 },
      {  63,  70 }, {  77,  64 }, {  85,  53 }, {  85,   0 }  S };

vector_character c_i [] =

    { {  65,  93 },
      {   0,   0 }, { 100,   0 }  E
      {  11,  65 }, {  50,  65 }, {  50,   0 }  E
      {  44, 100 }, {  50, 100 },
      {  50,  90 }, {  44,  90 }, {  44, 100 }  S };

vector_character c_j [] =

    { {  46,  93 },
      {   0,  65 }, { 100,  65 }, { 100, -14 },
      {  86, -25 }, {  61, -32 }, {   0, -32 }  E
      {  76, 100 }, {  85, 100 },
      {  85,  90 }, {  76,  90 }, {  76, 100 }  S };

vector_character c_k [] =

    { {  75,  90 },
      {  61,  68 }, {  92,  68 }  E
      {  64,   0 }, { 100,   0 }  E
      {  19,  30 }, {  79,  68 }  E
      {  35,  40 }, {  85,   0 }  E
      {   0, 100 }, {  19, 100 }, {  19,   0 }, {   0,   0 }  S };

vector_character c_l [] =

    { {  65,  90 },
      {   0,   0 }, { 100,   0 }  E
      {  11, 100 }, {  50, 100 }, {  50,   0 }  S };

vector_character c_m [] =

    { {  93,  63 },
      {   0,   0 }, {  23,   0 }  E
      {   0,  96 }, {  12,  96 }, {  12,   0 }  E
      {  12,  78 }, {  22,  93 }, {  30, 100 }, {  39, 100 },
      {  45,  94 }, {  50,  83 }, {  50,   0 }, {  62,   0 }  E
      {  50,  78 }, {  60,  93 }, {  68, 100 }, {  77, 100 },
      {  83,  94 }, {  88,  83 }, {  88,   0 }, { 100,   0 }  S };

vector_character c_n [] =

    { {  78,  62 },
      {   0,   0 }, {  32,   0 }  E
      {  72,   0 }, { 100,   0 }  E
      {   2,  96 }, {  16,  96 }, {  16,   0 }  E
      {  16,  74 }, {  31,  91 }, {  45, 100 },
      {  64, 100 }, {  78,  91 }, {  86,  76 }, {  86,   0 }  S };

vector_character c_o [] =

    { {  70,  64 },
      {  37,   0 }, {  63,   0 }, {  87,  13 },
      { 100,  37 }, { 100,  63 }, {  87,  87 },
      {  63, 100 }, {  37, 100 }, {  13,  87 },
      {   0,  63 }, {   0,  37 }, {  13,  13 }, {  37,   0 }  S };

vector_character c_p [] =

    { {  82,  60 },
      {   0, -52 }, {  44, -52 }  E
      {   0,  96 }, {  18,  97 }, {  18, -52 }  E
      {  18,  67 }, {  31,  87 }, {  48, 100 }, {  70, 100 },
      {  90,  86 }, { 100,  64 }, { 100,  36 }, {  89,  13 },
      {  69,   0 }, {  50,   0 }, {  31,  13 }, {  18,  34 }  S };

vector_character c_q [] =

    { {  82,  60 },
      {  56, -52 }, { 100, -52 }  E
      { 100,  96 }, {  82,  97 }, {  82, -52 }  E
      {  82,  67 }, {  69,  87 }, {  52, 100 }, {  30, 100 },
      {  10,  87 }, {   0,  65 }, {   0,  36 }, {  11,  13 },
      {  31,   0 }, {  51,   0 }, {  69,  13 }, {  82,  34 }  S };

vector_character c_r [] =

    { {  72,  61 },
      {   0,   0 }, {  80,   0 }  E
      {   5,  99 }, {  30,  99 }, {  30,   0 }  E
      {  30,  67 }, {  60,  93 }, {  75, 100 },
      {  86, 100 }, {  95,  95 }, { 100,  90 }  S };

vector_character c_s [] =

    { {  60,  65 },
      {   0,   4 }, {   0,  27 }  E
      {  95,  76 }, {  95,  97 }  E
      {  95,  85 }, {  79,  95 }, {  60, 100 },
      {  37, 100 }, {  17,  94 }, {   6,  83 },
      {   6,  72 }, {  14,  63 }, {  34,  57 }, {  73,  52 },
      {  90,  46 }, { 100,  35 }, { 100,  20 }, {  87,   7 },
      {  67,   0 }, {  39,   0 }, {  21,   4 }, {   0,  16 }  S };

vector_character c_t [] =

    { {  72,  85 },
      {   0,  74 }, {  85,  74 }  E
      {  25, 100 }, {  25,  14 }, {  32,   6 },
      {  47,   0 }, {  65,   0 }, {  83,   3 }, { 100,   9 }  S };

vector_character c_u [] =

    { {  79,  62 },
      {  64, 100 }, {  86, 100 }, {  86,   4 }, { 100,   4 }  E
      {   0, 100 }, {  18, 100 }, {  18,  24 }, {  24,   8 },
      {  36,   0 }, {  51,   0 }, {  67,   7 }, {  86,  25 }  S };

vector_character c_v [] =

    { {  85,  60 },
      {   0, 100 }, {  36, 100 }  E
      {  64, 100 }, { 100, 100 }  E
      {  13, 100 }, {  46,   0 }, {  54,   0 }, {  87, 100 }  S };

vector_character c_w [] =

    { {  87,  60 },
      {   0, 100 }, {  25, 100 }  E
      {  75, 100 }, { 100, 100 }  E
      {   7, 100 }, {  24,   0 }, {  27,   0 }, {  50,  71 },
      {  52,  71 }, {  72,   0 }, {  76,   0 }, {  93, 100 }  S };

vector_character c_x [] =

    { {  79,  60 },
      {   0,   0 }, {  32,   0 }  E
      {  67,   0 }, { 100,   0 }  E
      {   4, 100 }, {  32, 100 }  E
      {  67, 100 }, {  95, 100 }  E
      {   9, 100 }, {  95,   0 }  E
      {   4,   0 }, {  90, 100 }  S };

vector_character c_y [] =

    { {  80,  63 },
      {   0, 100 }, {  28, 100 }  E
      {  72, 100 }, { 100, 100 }  E
      {   6, 100 }, {  50,   0 }  E
      {   0, -42 }, {  54, -42 }  E
      {  32, -42 }, {  93, 100 }  S };

vector_character c_z [] =

    { {  58,  60 },
      {   3,  78 }, {   3, 100 }, {  98, 100 }, {  98,  98 },
      {   0,   2 }, {   0,   0 }, { 100,   0 }, { 100,  21 }  S };

vector_character c_LT [] =  /* < */

    { {  72,  78 },
      { 100, 100 }, {   0,  53 }, {  99,   6 }  S };

vector_character c_CM [] =  /* , */

    { {  27,  33 },
      {  43,  50 }, { 100,  50 },
      {  11, -68 }, {   0, -65 }, {  43,  50 }  S };

vector_character c_GT [] =  /* > */

    { {  72,  78 },
      {   1, 100 }, { 100,  52 }, {   0,   5 }  S };

vector_character c_PE [] =  /* . */

    { {  19,  12 },
      {  25, 100 }, {  75, 100 }, { 100,  63 },
      { 100,  18 }, {  75, -21 }, {  25, -21 },
      {   0,  18 }, {   0,  63 }, {  25, 100 }  S };

vector_character c_QU [] =  /* ? */

    { {  53,  85 },
      {  40,   8 }, {  60,   8 }, {  67,   3 },
      {  60,  -2 }, {  40,  -2 }, {  33,   3 }, {  40,   8 }  E
      {   0,  81 }, {   0,  93 }, {  30, 100 },
      {  66, 100 }, {  88,  93 }, { 100,  79 },
      { 100,  68 }, {  87,  56 }, {  48,  45 }, {  48,  34 }  S };

vector_character c_SL [] =  /* / */

    { {  58, 100 },
      {   0, -13 }, { 100, 100 }  S };

vector_character c_TI [] =  /* ~ */

    { {  66,  49 },
      {   0,  77 }, {  10,  92 }, {  20, 100 },
      {  33, 100 }, {  43,  92 }, {  57,  76 },
      {  67,  69 }, {  80,  69 }, {  90,  77 }, { 100,  92 }  S };

vector_character c_A2 [] =  /* ` */

    { {  22,  96 },
      {   0, 100 }, { 100,  82 }  S };

vector_character c_EX [] =  /* ! */

    { {  12,  92 },
      {  27,  98 }, {  50, 100 }, {  73,  98 },
      {  54,  36 }, {  49,  36 }, {  27,  98 }  E
      {   0,   3 }, {  25,   8 }, {  75,   8 },
      { 100,   3 }, {  75,  -2 }, {  25,  -2 }, {   0,   3 }  S };

vector_character c_AT [] =  /* @ */

    { {  59,  93 },
      {  93,  65 }, {  72,  65 }, {  52,  60 },
      {  42,  50 }, {  42,  40 }, {  51,  31 },
      {  70,  27 }, {  80,  27 }, {  93,  28 }  E
      { 100,  27 }, {  93,  27 }, {  93,  82 },
      {  84,  93 }, {  67, 100 }, {  41, 100 },
      {  16,  91 }, {   0,  70 }, {   0,  22 },
      {  12,   3 }, {  36,  -9 }, {  71,  -9 }, {  92,  -1 }  S };

vector_character c_HA [] =  /* # */

    { {  63,  96 },
      {  39, 100 }, {  27, -10 }  E
      {  73, 100 }, {  62, -10 }  E
      {   5,  59 }, { 100,  59 }  E
      {   0,  31 }, {  95,  31 }  S };

vector_character c_DO [] =  /* $ */

    { {  57,  98 },
      {  95,  84 }, {  95,  72 }  E
      {   0,  27 }, {   0,  12 }  E
      {  49, 100 }, {  49,  87 }  E
      {  49,  10 }, {  49, -15 }  E
      {  95,  77 }, {  79,  83 }, {  64,  87 },
      {  33,  87 }, {  16,  81 }, {   6,  72 },
      {   6,  64 }, {  11,  57 }, {  29,  52 }, {  79,  46 },
      {  94,  41 }, { 100,  33 }, { 100,  25 }, {  89,  16 },
      {  66,  10 }, {  34,  10 }, {  15,  15 }, {   0,  22 }  S };

vector_character c_PC [] =  /* % */

    { {  65,  92 },
      {   0,  37 }, { 100,  61 }  E
      {  25, 100 }, {  40, 100 }, {  54,  95 },
      {  61,  87 }, {  61,  77 }, {  54,  69 },
      {  40,  64 }, {  25,  64 }, {  11,  69 },
      {   4,  77 }, {   4,  87 }, {  11,  95 }, {  25, 100 }  E
      {  60,  33 }, {  75,  33 }, {  89,  28 },
      {  96,  20 }, {  96,  10 }, {  89,   2 },
      {  75,  -3 }, {  60,  -3 }, {  46,   2 },
      {  39,  10 }, {  39,  20 }, {  46,  28 }, {  60,  33 }  S };

vector_character c_CA [] =  /* ^ */

    { {  59,  89 },
      {   0,  61 }, {  50, 100 }, { 100,  61 }  S };

vector_character c_AM [] =  /* & */

    { {  57,  76 },
      {  77,  98 }, {  68,  95 }, {  54, 100 },
      {  42, 100 }, {  27,  94 }, {  19,  85 },
      {  19,  73 }, {  83,   0 }, { 100,   0 }  E
      {  35,  54 }, {   9,  45 }, {   0,  32 },
      {   0,  18 }, {  10,   5 }, {  29,  -3 }, {  45,  -3 },
      {  61,   4 }, {  81,  25 }, {  90,  44 }, {  98,  44 }  S };

vector_character c_AS [] =  /* * */

    { {  57,  90 },
      {  50, 100 }, {  50,  69 }  E
      {   0,  78 }, {  50,  69 }, { 100,  78 }  E
      {  20,  45 }, {  50,  69 }, {  80,  45 }  S };

vector_character c_OB [] =  /* ( */

    { {  19,  90 },
      { 100, 100 }, {  29,  72 }, {   0,  49 },
      {   0,  29 }, {  29,   6 }, { 100, -22 }  S };

vector_character c_CB [] =  /* ) */

    { {  19,  90 },
      {   0, 100 }, {  71,  72 }, { 100,  49 },
      { 100,  29 }, {  71,   6 }, {   0, -22 }  S };

vector_character c_UL [] =  /* _ */

    { { 100,  86 },
      {   0, -50 }, { 100, -50 }  S };

vector_character c_MI [] =  /* - */

    { {  66,  42 },
      {   0, 100 }, { 100, 100 },
      { 100,  94 }, {   0,  94 }, {   0, 100 }  S };

vector_character c_PL [] =  /* + */

    { {  72,  77 },
      {   0,  53 }, { 100,  53 }  E
      {  50, 100 }, {  50,   7 }  S };

vector_character c_EQ [] =  /* = */

    { {  80,  53 },
      {   0, 100 }, { 100, 100 }  E
      {   0,  56 }, { 100,  56 }  S };

vector_character c_LI [] =  /* | */

    { {  10,  90 },
      {   0, 100 }, {   0, -22 }  S };

vector_character c_OC [] =  /* { */

    { {  29,  89 },
      { 100, 100 }, {  79,  99 }, {  58,  94 },
      {  50,  86 }, {  50,  52 }, {  41,  45 }, {  19,  40 },
      {   0,  39 }, {  19,  38 }, {  41,  33 }, {  50,  26 },
      {  50,  -8 }, {  58, -16 }, {  79, -21 }, { 100, -22 }  S };

vector_character c_CC [] =  /* } */

    { {  29,  89 },
      {   0, 100 }, {  21,  99 }, {  42,  94 },
      {  50,  86 }, {  50,  52 }, {  49,  45 }, {  81,  40 },
      { 100,  39 }, {  81,  38 }, {  59,  33 }, {  50,  26 },
      {  50,  -8 }, {  42, -16 }, {  21, -21 }, {   0, -22 }  S };

vector_character c_OS [] =  /* [ */

    { {  21,  89 },
      { 100, 100 }, {   0, 100 }, {   0, -22 }, { 100, -22 }  S };

vector_character c_CS [] =  /* ] */

    { {  21,  89 },
      {   0, 100 }, { 100, 100 }, { 100, -22 }, {   0, -22 }  S };

vector_character c_RS [] =  /* \ */

    { {  58, 100 },
      {   0, 100 }, { 100, -14 }  S };

vector_character c_CO [] =  /* : */

    { {  19,  60 },
      {  25, 100 }, {  75, 100 }, { 100,  93 },
      { 100,  84 }, {  75,  77 }, {  25,  77 },
      {   0,  84 }, {   0,  93 }, {  25, 100 }  E
      {  25,  19 }, {  75,  19 }, { 100,  12 },
      { 100,   3 }, {  75,  -4 }, {  25,  -4 },
      {   0,   3 }, {   0,  12 }, {  25,  19 }  S };

vector_character c_SC [] =  /* ; */

    { {  31,  60 },
      {  37,  29 }, {  79,  29 },
      {   9, -25 }, {   0, -23 }, {  37,  29 }  E
      {  55, 100 }, {  85, 100 }, { 100,  93 },
      { 100,  84 }, {  85,  77 }, {  55,  77 },
      {  40,  84 }, {  40,  93 }, {  55, 100 }  S };

vector_character c_IC [] =  /* " */

    { {  50,  88 },
      {   0, 100 }, {  29, 100 },
      {  15,  60 }, {   8,  60 }, {   0, 100 }  E
      {  71, 100 }, { 100, 100 },
      {  86,  60 }, {  79,  60 }, {  71, 100 }  S };

vector_character c_AP [] =  /* ' */

    { {  14,  90 },
      {   0, 100 }, { 100, 100 },
      {  62,  57 }, {  36,  57 }, {   0, 100 }  S };

vector_character c_0 [] =

    { {  77,  86 },
      {   0,  37 }, {   0,  63 }, {  14,  86 },
      {  37, 100 }, {  63, 100 }, {  86,  86 },
      { 100,  63 }, { 100,  37 }, {  86,  14 },
      {  63,   0 }, {  37,   0 }, {  14,  14 }, {   0,  37 }  S };

vector_character c_1 [] =

    { {  77,  86 },
      {  50,   0 }, {  50, 100 }  S };

vector_character c_2 [] =

    { {  77,  86 },
      {   0,  75 }, {  12,  87 }, {  28,  96 },
      {  47, 100 }, {  72,  99 }, {  90,  92 }, { 100,  78 },
      { 100,  66 }, {  90,  53 }, {   0,   0 }, { 100,   0 }  S };

vector_character c_3 [] =

    { {  77,  86 },
      {   0,  80 }, {  14,  92 }, {  37, 100 },
      {  63, 100 }, {  86,  92 }, { 100,  80 },
      { 100,  67 }, {  86,  55 }, {  63,  50 },
      {  86,  45 }, { 100,  33 }, { 100,  20 }, {  86,   8 },
      {  63,   0 }, {  37,   0 }, {  14,   8 }, {   0,  20 }  S };

vector_character c_4 [] =

    { {  77,  86 },
      {  70,   0 }, {  70, 100 }, {   0,  35 }, { 100,  35 }  S };

vector_character c_5 [] =

    { {  77,  86 },
      { 100, 100 }, {   5, 100 }, {   5,  53 },
      {  25,  60 }, {  50,  65 }, {  72,  62 },
      {  92,  52 }, { 100,  38 }, {  98,  20 }, {  88,   8 },
      {  65,   0 }, {  38,   0 }, {  15,   8 }, {   0,  20 }  S };

vector_character c_6 [] =

    { {  77,  86 },
      {  15,  51 }, {  37,  60 }, {  63,  60 },
      {  85,  51 }, { 100,  38 }, { 100,  22 },
      {  85,   9 }, {  63,   0 }, {  37,   0 },
      {  15,   9 }, {   0,  22 }, {   0,  38 }, {  80, 100 }  S };

vector_character c_7 [] =

    { {  77,  86 },
      {  20,   0 }, { 100, 100 }, {   0, 100 }  S };

vector_character c_8 [] =

    { {  77,  86 },
      {  63,  50 }, {  37,  50 }, {  14,  55 }, {   0,  67 },
      {   0,  80 }, {  14,  92 }, {  37, 100 }, {  63, 100 },
      {  86,  92 }, { 100,  80 }, { 100,  67 }, {  86,  55 },
      {  63,  50 }, {  86,  45 }, { 100,  33 }, { 100,  20 },
      {  86,   8 }, {  63,   0 }, {  37,   0 }, {  14,   8 },
      {   0,  20 }, {   0,  33 }, {  14,  45 }, {  37,  50 }  S };

vector_character c_9 [] =

    { {  77,  86 },
      {  85,  49 }, {  63,  40 }, {  37,  40 },
      {  15,  49 }, {   0,  62 }, {   0,  78 },
      {  15,  91 }, {  37, 100 }, {  63, 100 },
      {  85,  91 }, { 100,  78 }, { 100,  62 }, {  20,   0 }  S };


vector_character * vector_c [128] =

    {    0,    0,    0,    0,    0,    0,    0,    0,
         0,    0,    0,    0,    0,    0,    0,    0,
         0,    0,    0,    0,    0,    0,    0,    0,
         0,    0,    0,    0,    0,    0,    0,    0,
         0, c_EX, c_IC, c_HA, c_DO, c_PC, c_AM, c_AP,
      c_OB, c_CB, c_AS, c_PL, c_CM, c_MI, c_PE, c_SL,
       c_0,  c_1,  c_2,  c_3,  c_4,  c_5,  c_6,  c_7,
       c_8,  c_9, c_CO, c_SC, c_LT, c_EQ, c_GT, c_QU,
      c_AT,  c_A,  c_B,  c_C,  c_D,  c_E,  c_F,  c_G,
       c_H,  c_I,  c_J,  c_K,  c_L,  c_M,  c_N,  c_O,
       c_P,  c_Q,  c_R,  c_S,  c_T,  c_U,  c_V,  c_W,
       c_X,  c_Y,  c_Z, c_OS, c_RS, c_CS, c_CA, c_UL,
      c_A2,  c_a,  c_b,  c_c,  c_d,  c_e,  c_f,  c_g,
       c_h,  c_i,  c_j,  c_k,  c_l,  c_m,  c_n,  c_o,
       c_p,  c_q,  c_r,  c_s,  c_t,  c_u,  c_v,  c_w,
       c_x,  c_y,  c_z, c_OC, c_LI, c_CC, c_TI,    0 };

#undef E
#undef S

#endif /* CHAR_SET_C */
