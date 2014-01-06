#ifndef __IMG_H_
#define __IMG_H_

#include "grav.h"

class img
{
  int Height, Width;
  bool Init;

public:
  byte ***Src;

  img (int Width, int Height) : Height (Height), Width (Width)
  {  
    Init = false;

    Src = (byte ***)malloc (Height * sizeof(byte **));
    for (int i = 0; i < Height; i++)
    {
      Src[i] = (byte **)malloc (Width * sizeof(byte *));
      for (int j = 0; j < Width; j++)
        Src[i][j] = (byte *)malloc (NumOfColors * sizeof(byte));
    }

    /*Src = (byte ***)malloc (Height * sizeof (byte **));
    if (Src == NULL)
      return;

    for (int i = 0; i < Height; i++)
    {
      Src[i] = (byte **)malloc (Width * sizeof (byte));

      if (Src[i] == NULL)
      {
        while (i--> 0)
          free (Src[i]);
        free (Src);

        return;
      }

      for (int j = 0; j < Width; j++)
      {
        Src[i][j] = (byte *)malloc (NumOfColors * sizeof(byte));

        if (Src[i][j] == NULL)
        {
          while (--j > 0)
            free (Src[i][j]);
          free (Src[i]);

          while (--i >= 0)
          {
            for (j = 0; j < Width; j++)
              free (Src[i][j]);
            free (Src[i]);
          }

          free (Src);

          return;
        }

        for (int c = 0; c < NumOfColors; c++)
          Src[i][j][c] = 0;
      }
    } */

    Init = true;
  }

  bool IsInit ()
  {
    return Init;
  }

  ~img ()
  {
    for (int i = 0; i < Height; i++)
    {
      for (int j = 0; j < Width; j++)
          free (Src[i][j]);
      free (Src[i]);
    }
  }
};

#endif