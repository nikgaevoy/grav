#ifndef __TIMER_H_
#define __TIMER_H_

#include <time.h>

class timer
{
  clock_t t;
public:

  timer () : t (clock ())
  {}

  double update ()
  {
    double rez = ((double)(clock () - timer::t)) / CLOCKS_PER_SEC;
    timer::t = clock ();
    return rez;
  }
};

#endif