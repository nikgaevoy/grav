#include <windows.h>     
#include <glut.h>
#include <stdio.h>

#include "grav.h"
#include "molecule.h"
#include "timer.h"
#include "color.h"

#define DELAY 1000

byte Image[HEIGHT][WIDTH][3] = {0};
std::vector<molecule> mol;
float Scale = 1;
bool IsPause = 0;

void Draw (void)
{
  static timer maintimer;
  double delta_t = maintimer.update() / DELAY;

  if (!IsPause)
  {
    glClearColor(0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT);

#if PARALLEL
#pragma omp parallel
#endif
    {
#if PARALLEL
#pragma omp for
#endif
      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].Move (mol, delta_t);

#if PARALLEL
#pragma omp for
#endif
      for (int i = 0; i < (int)mol.size (); i++)
        mol[i].update ();
    }

#if COLLISION
    for (int i = 0; i < (int)mol.size (); i++)
      mol[i].collision (mol, i);
#endif

#if PARALLEL
#pragma omp parallel
#endif
    {
#if PARALLEL
#pragma omp for
#endif
      for (int i = 0; i < (int)mol.size (); i++)
      {
        int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

        if (IsInField (x, y))
        {
          Image[y][x][0] = MAIN_COLOR_B + COLOR_SHIFT_B * (mol[i].mass / 1e15);
          Image[y][x][1] = MAIN_COLOR_G + COLOR_SHIFT_G * (mol[i].mass / 1e15);
          Image[y][x][2] = MAIN_COLOR_R + COLOR_SHIFT_R * (mol[i].mass / 1e15);
        }
      }
    }
      glPixelZoom (Scale, Scale);
      glDrawPixels (WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image);

#if PARALLEL
#pragma omp parallel
#endif
    {
#if PARALLEL
#pragma omp for
#endif
      for (int i = 0; i < (int)mol.size (); i++)
      {
        int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

        if (IsInField (x, y))
        {
          Image[y][x][0] = LOOP_COLOR_B;
          Image[y][x][1] = LOOP_COLOR_G;
          Image[y][x][2] = LOOP_COLOR_R;
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
  case 27:
    exit (EXIT_SUCCESS);
  }
}

void main ( int argc, char *argv[] )
{
  char r[30];
  FILE *set;
  int NumOfMolecules;

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

  fclose (set);

  FreeConsole ();

  sprintf_s (r, "%ldx%ld:32", WIDTH, HEIGHT);

  for (int i = 0; i < NumOfMolecules; i++)
    mol.push_back(molecule(vec<double> (rand() % (WIDTH / 2) + (WIDTH / 4), rand() % (HEIGHT / 2) + (HEIGHT / 4)), (rand() % 10 + 2) * 1e15));

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