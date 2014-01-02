#ifndef __GRAV_H_
#define __GRAV_H_

#include <stdlib.h>

#define WALLS 1
#define COLLISION 1
#define PARALLEL 1

#define HEIGHT 1080
#define WIDTH 1920
#define G 6.67384e-11

typedef unsigned char byte;

bool IsInField (int x, int y)
{
  return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

#endif