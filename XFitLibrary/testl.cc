

#include <stdlib.h>
#include <iostream.h>


int hash (int i1, int i2, double d1, double d2, double d3,
                          double d4, double d5, double d6, double d7) {

  union { unsigned int i [2]; double d; } ha [9];

  ha [0].i [0] = i1;
  ha [0].i [1] = 0;
  ha [1].i [0] = i2;
  ha [1].i [1] = 0;
  
  ha [2].d = d1;
  ha [3].d = d2;
  ha [4].d = d3;
  ha [5].d = d4;
  ha [6].d = d5;
  ha [7].d = d6;
  ha [8].d = d7;

  int index = 0;
  
  for (int i = 0; i < 9; i++) {

    index = ((unsigned int) index) * 618031 + ha [i].i [0];
    index = ((unsigned int) index) * 391961 + ha [i].i [1];

    }

  index = ((unsigned int) index) % 1000000;

  return index;

  }

#define NUMBER 101

#define Rx(c) (d [(c % (NUMBER * NUMBER * NUMBER)) / (NUMBER * NUMBER)])
#define Ry(c) (d [(c % (NUMBER * NUMBER)) / NUMBER])
#define Rz(c) (d [c % NUMBER])

main () {

  /* for (unsigned int i = 391960; i < 391980; i++) {

    for (unsigned int j = 2; j < 65536; j++)

      if (i % j == 0)

        break;

    if (j == 65536)

      cout << i << endl;

    } */

  double d [101];

  int c;

  for (c = 0; c < 100; c++)

    d [c] = c / 100.0;

  d [100] = 0.707107;

  int * used;

  used = new int [NUMBER * NUMBER * NUMBER];

  for (c = 0; c < NUMBER * NUMBER * NUMBER; c++) {

    int h = hash (0, 1, Rx(c), Ry(c), Rz(c), 0, 0, 0, 0);

    used [c] = h;

    if (c % 10000 == 0)
    
      cout << c << ": " << h << endl;

    }

  int count = 0, ccount;

  c = 0;
    
  for (int n = 0; n < 100; n++) {

    c = c + rand ();

    c = c % (NUMBER * NUMBER * NUMBER);

    ccount = 0;

    for (int cc = 0; cc < NUMBER * NUMBER * NUMBER; cc++)

      if (used [cc] == used [c])

        if (cc != c) {

          cout << c << ": " << Rx(c) << " " << Ry(c) << " " << Rz(c) << " = "
               << cc << ": " << Rx(cc) << " " << Ry(cc) << " " << Rz(cc) << endl;

          ccount++;

          }

    if (ccount > 0)
    
      cout << c << ": " << used [c] << "  count = " << ccount << endl;

    count += ccount;

    }

  cout << " count = " << count << endl;

  delete[] used;

  }


