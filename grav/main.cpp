#include <glut.h>
#include <windows.h>
#include <stdio.h>
#include <omp.h>

#include "grav.h"
#include "molecule.h"
#include "timer.h"
#include "color.h"

#define DELAY 1000
#define NUM_OF_MOLECULES 500

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
    glClear(GL_COLOR_BUFFER_BIT);

#pragma omp parallel
    {
#pragma omp for
      for (int i = 0; i < (int)mol.size(); i++)
        mol[i].Move(mol, delta_t);

#pragma omp for
      for (int i = 0; i < (int)mol.size(); i++)
        mol[i].update();

#if COLLISION
#pragma omp for
      for (int i = 0; i < (int)mol.size(); i++)
        mol[i].collision(mol, i);
#endif
    }
     
    for (int i = 0; i < (int)mol.size(); i++)
    {
      int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

      if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH)
      {
        if (Image[y][x][0] <= LOOP_COLOR_B && Image[y][x][1] <= LOOP_COLOR_G && Image[y][x][2] <= LOOP_COLOR_R)
        {
          Image[y][x][0] = MAIN_COLOR_B;
          Image[y][x][1] = MAIN_COLOR_G;
          Image[y][x][2] = MAIN_COLOR_R;
        }
        else 
          if (Image[y][x][0] <= COLOR_TRESHOLD_B && Image[y][x][1] <= COLOR_TRESHOLD_G && Image[y][x][2] <= COLOR_TRESHOLD_R)
          {
            Image[y][x][0] += COLOR_SHIFT_B;
            Image[y][x][1] += COLOR_SHIFT_G;
            Image[y][x][2] += COLOR_SHIFT_R;
          }
      }
    }
    glPixelZoom(Scale, Scale);
    glDrawPixels(WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image);

    for (int i = 0; i < (int)mol.size(); i++)
    {
      int y = (int)mol[i].Pos.Y, x = (int)mol[i].Pos.X;

      if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH)
      {
        Image[y][x][0] = LOOP_COLOR_B;
        Image[y][x][1] = LOOP_COLOR_G;
        Image[y][x][2] = LOOP_COLOR_R;
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

  FreeConsole();

  sprintf_s (r, "%ldx%ld:32", WIDTH, HEIGHT);

  for (int i = 0; i < NUM_OF_MOLECULES; i++)
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