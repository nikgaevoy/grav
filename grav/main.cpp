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

int BackgroundColorR = DEFAULT_BACKGROUND_COLOR_R, BackgroundColorG = DEFAULT_BACKGROUND_COLOR_G, BackgroundColorB = DEFAULT_BACKGROUND_COLOR_B;
int LoopColorR = DEFAULT_LOOP_COLOR_R, LoopColorG = DEFAULT_LOOP_COLOR_G, LoopColorB = DEFAULT_LOOP_COLOR_B;
int MainColorR = DEFAULT_MAIN_COLOR_R, MainColorG = DEFAULT_MAIN_COLOR_G, MainColorB = DEFAULT_MAIN_COLOR_B;
int ColorShiftR = DEFAULT_COLOR_SHIFT_R, ColorShiftG = DEFAULT_COLOR_SHIFT_G, ColorShiftB = DEFAULT_COLOR_SHIFT_B;
int ColorTresholdR = DEFAULT_COLOR_TRESHOLD_R, ColorTresholdG = DEFAULT_COLOR_TRESHOLD_G, ColorTresholdB = DEFAULT_COLOR_TRESHOLD_B;

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
            *(Image->set (x, y, 0)) = MainColorB + ColorShiftB * (mol[i].mass / 1e15);
            *(Image->set (x, y, 1)) = MainColorG + ColorShiftG * (mol[i].mass / 1e15);
            *(Image->set (x, y, 2)) = MainColorR + ColorShiftR * (mol[i].mass / 1e15);
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
            *(Image->set (x, y, 0)) = LoopColorB;
            *(Image->set (x, y, 1)) = LoopColorG;
            *(Image->set (x, y, 2)) = LoopColorR;
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
          *(Image->set (x, y, 0)) = MainColorB + ColorShiftB * (mol[i].mass / 1e15);
          *(Image->set (x, y, 1)) = MainColorG + ColorShiftG * (mol[i].mass / 1e15);
          *(Image->set (x, y, 2)) = MainColorR + ColorShiftR * (mol[i].mass / 1e15);
        }
      }
      glPixelZoom (Scale, Scale);
      glDrawPixels (Width, Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image->Src);

      for (int i = 0; i < (int)mol.size (); i++)
      {
        int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

        if (IsInField (x, y))
        {
          *(Image->set (x, y, 0)) = LoopColorB;
          *(Image->set (x, y, 1)) = LoopColorG;
          *(Image->set (x, y, 2)) = LoopColorR;
        }
      }
    }
  }
  else
  {
  Sleep(30);
  glFinish();
  return;
  }

  glFinish();
  glutSwapBuffers();
}

void Idle()
{
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

  case '+':
  Scale += 0.1;
  break;

  case '-':
  Scale -= 0.1;
  break;
  }
}

void main (int argc, char *argv[])
{
  char r[30];
  FILE *set, *col;
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

  col = fopen ("color.txt", "rt");

  if (col != NULL && getc (col) == '#')
  {
    fscanf (col, "%d", &BackgroundColorR);
    while (getc (col) != '\n');
    fscanf (col, "%d", &BackgroundColorG);
    while (getc (col) != '\n');
    fscanf (col, "%d", &BackgroundColorB);
    while (getc (col) != '\n');

    fscanf (col, "%d", &LoopColorR);
    while (getc (col) != '\n');
    fscanf (col, "%d", &LoopColorG);
    while (getc (col) != '\n');
    fscanf (col, "%d", &LoopColorB);
    while (getc (col) != '\n');

    fscanf (col, "%d", &MainColorR);
    while (getc (col) != '\n');
    fscanf (col, "%d", &MainColorG);
    while (getc (col) != '\n');
    fscanf (col, "%d", &MainColorB);
    while (getc (col) != '\n');

    fscanf (col, "%d", &ColorShiftR);
    while (getc (col) != '\n');
    fscanf (col, "%d", &ColorShiftG);
    while (getc (col) != '\n');
    fscanf (col, "%d", &ColorShiftB);
    while (getc (col) != '\n');

    fscanf (col, "%d", &ColorTresholdR);
    while (getc (col) != '\n');
    fscanf (col, "%d", &ColorTresholdG);
    while (getc (col) != '\n');
    fscanf (col, "%d", &ColorTresholdB);
    while (getc (col) != '\n');

    fclose (col);
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

  Image->SetColor (BackgroundColorR, BackgroundColorG, BackgroundColorB);

  FreeConsole ();

  sprintf_s (r, "%dx%d:32", Width, Height);

  for (i = 0; i < NumOfMolecules; i++)
    mol.push_back(molecule(vec<double> (rand() % (Width / 2) + (Width / 4), rand() % (Height / 2) + (Height / 4)),
                  (rand() % 10 + 2) * 1e15, Width, Height, Walls, Collision));

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);

  glutGameModeString(r);
  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
    glutEnterGameMode();
  else
  {
    FILE *f;

    f = fopen ("error.txt", "wt");
    fprintf (f, "ERROR ENTERING FULLSCREEN MODE!\n");
    fclose(f);

    delete Image;

    return;
  }

  glutDisplayFunc (Draw);
  glutKeyboardFunc (Keyboard);
  glutIdleFunc(Idle);

  glutMainLoop();
}