#ifndef __GRAV_H_
#define __GRAV_H_

#include <stdlib.h>

#define G 6.67384e-11
#define NumOfColors 3

typedef unsigned char byte;

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
  _CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG))
static class __Dummy
{
public:
  /* Class constructor */
  __Dummy (VOID)
  {
    SetDbgMemHooks ();
  } /* End of '__Dummy' constructor */
} __oops;
#endif /* _DEBUG */

#ifdef _DEBUG
#  ifdef _CRTDBG_MAP_ALLOC
#    define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#  endif /* _CRTDBG_MAP_ALLOC */
#endif /* _DEBUG */

#endif