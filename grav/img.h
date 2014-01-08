#ifndef __IMG_H_
#define __IMG_H_

#include "grav.h"

class img
{
  int Height, Width;
  bool Init;

public:
  byte *Src;

  img (int W, int H) : Height (H), Width (W)
  {  
    Init = false;

    Src = (byte *)malloc (H * W * NumOfColors * sizeof(byte));

    if (Src == NULL)
      return;

    Init = true;
  }

  byte *set (int x, int y, int c)
  {
    return &Src[y * Width * NumOfColors + x * NumOfColors + c];
  }

  void SetColor (byte r, byte g, byte b)
  {
    for (int i = 0; i < Height * Width; i++)
    {
      Src[NumOfColors * i] = b;
      Src[NumOfColors * i + 1] = g;
      Src[NumOfColors * i + 2] = r;
    }
  }

  bool IsInit ()
  {
    return Init;
  }

  ~img ()
  {
    if (IsInit())
      free (Src);
  }
};

#endif