#include <windows.h>
#include <glut.h>
#include <stdio.h>

#include "grav.h"
#include "molecule.h"
#include "timer.h"
#include "color.h"
#include "img.h"

std::vector<molecule> mol;
float Scale = 1;
bool IsPause = 0;
int NumOfMolecules, Width, Height;
double Delay;
bool Walls, Parallel, Collision;
img *Image;

bool IsInField (int x, int y)
{
  return (x >= 0 && x < Width && y >= 0 && y < Height);
}

void Draw (void)
{
  static timer maintimer;
  double delta_t = maintimer.update() / Delay;

  if (!IsPause)
  {
    glClearColor(0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT);

    if (Parallel)
    {
#pragma omp parallel
      {
#pragma omp for
        for (int i = 0; i < (int)mol.size (); i++)
          mol[i].Move (mol, delta_t);

#pragma omp for
        for (int i = 0; i < (int)mol.size (); i++)
          mol[i].update ();
      }

      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].collision (mol, i);

#pragma omp parallel
      {
#pragma omp for
        for (int i = 0; i < (int)mol.size (); i++)
        {
          int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

          if (IsInField (x, y))
          {
            *(Image->set (x, y, 0)) = MAIN_COLOR_B + COLOR_SHIFT_B * (mol[i].mass / 1e15);
            *(Image->set (x, y, 1)) = MAIN_COLOR_G + COLOR_SHIFT_G * (mol[i].mass / 1e15);
            *(Image->set (x, y, 2)) = MAIN_COLOR_R + COLOR_SHIFT_R * (mol[i].mass / 1e15);
          }
        }
      }
      glPixelZoom (Scale, Scale);
      glDrawPixels (Width, Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image->Src);

#pragma omp parallel
      {
#pragma omp for
        for (int i = 0; i < (int)mol.size (); i++)
        {
          int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

          if (IsInField (x, y))
          {
            *(Image->set (x, y, 0)) = LOOP_COLOR_B;
            *(Image->set (x, y, 1)) = LOOP_COLOR_G;
            *(Image->set (x, y, 2)) = LOOP_COLOR_R;
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].Move (mol, delta_t);

      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].update ();

      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].collision (mol, i);

      for (int i = 0; i < (int)mol.size (); i++)
      {
        int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

        if (IsInField (x, y))
        {
          *(Image->set (x, y, 0)) = MAIN_COLOR_B + COLOR_SHIFT_B * (mol[i].mass / 1e15);
          *(Image->set (x, y, 1)) = MAIN_COLOR_G + COLOR_SHIFT_G * (mol[i].mass / 1e15);
          *(Image->set (x, y, 2)) = MAIN_COLOR_R + COLOR_SHIFT_R * (mol[i].mass / 1e15);
        }
      }
      glPixelZoom (Scale, Scale);
      glDrawPixels (Width, Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image->Src);

      for (int i = 0; i < (int)mol.size (); i++)
      {
        int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

        if (IsInField (x, y))
        {
          *(Image->set (x, y, 0)) = LOOP_COLOR_B;
          *(Image->set (x, y, 1)) = LOOP_COLOR_G;
          *(Image->set (x, y, 2)) = LOOP_COLOR_R;
        }
      }
    }
  }
  else
    Sleep(30);

  glFinish();
  glutPostRedisplay();
}

void Keyboard (byte Key, int x, int y)
{
  switch(Key)
  {
  case 32:
    IsPause ^= 1;
    return;

  default:
    break;

  case 27:
    delete Image;
    exit (EXIT_SUCCESS);
  }
}

void main ( int argc, char *argv[] )
{
  char r[30];
  FILE *set;
  int i, w, p, c;

  set = fopen ("settings.txt", "rt");

  if (set == NULL)
  {
    FILE *f;

    f = fopen ("error.txt", "wt");

    fprintf (f, "error reading file!\n");

    fclose (f);

    return;
  }

  fscanf (set, "%d", &NumOfMolecules);
  while (getc (set) != '\n');
  fscanf (set, "%dx%d", &Width, &Height);
  while (getc (set) != '\n');
  fscanf (set, "%d", &w);
  while (getc (set) != '\n');
  fscanf (set, "%d", &c);
  while (getc (set) != '\n');
  fscanf (set, "%d", &p);
  while (getc (set) != '\n');
  fscanf (set, "%lf", &Delay);
  while (getc (set) != '\n');

  Walls = !!w;
  Collision = !!c;
  Parallel = !!p;

  fclose (set);

  Image = new img (Width, Height);

  if (!Image->IsInit ())
  {
    FILE *f;

    f = fopen ("error.txt", "wt");
    fprintf (f, "No memory!\n");
    fclose (f);

    return;
  }

  Image->SetColor (LOOP_COLOR_R, LOOP_COLOR_G, LOOP_COLOR_B);

  FreeConsole ();

  sprintf_s (r, "%ldx%ld:32", Width, Height);

  for (i = 0; i < NumOfMolecules; i++)
    mol.push_back(molecule(vec<double> (rand() % (Width / 2) + (Width / 4), rand() % (Height / 2) + (Height / 4)),
                  (rand() % 10 + 2) * 1e15, Width, Height, Walls, Collision));

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB);

  glutGameModeString(r);
  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
    glutEnterGameMode();
  else
  {
    FILE *f;

    f = fopen ("error.txt", "wt");
    fprintf (f, "ERROR ENTERING FULLSCREEN MODE!\n");
    fclose(f);

    return;
  }

  glutDisplayFunc (Draw);
  glutKeyboardFunc (Keyboard);

  glutMainLoop();
}