#ifndef __MOLECULE_H_
#define __MOLECULE_H_

#include <vector>
#include "grav.h"
#include "vec.h"

using namespace std;
using namespace mth;  

class molecule 
{
public:
  double mass;
  vec<double> Pos, Speed, newPos, newSpeed;

  molecule (vec<double> P, double m) : Pos(P), mass(m), Speed(vec<double>(0, 0))
  {}

  void Move (vector<molecule> m, double delta_t)
  {
    vec<double> newPos;

    molecule::newPos = molecule::Pos + molecule::Speed * delta_t;
    molecule::newSpeed = molecule::Speed;

    for (unsigned int i = 0; i < m.size(); i++)
    {
      vec<double> tmp = (m[i].Pos - molecule::Pos).Normalizing();

      if (tmp.Len2() != 0)
      {
        tmp *= G * delta_t * m[i].mass / tmp.Len2();
        molecule::newPos += tmp * delta_t / 2;
        molecule::newSpeed += tmp;
      }
    }

#if WALLS
    if (molecule::newPos.X >= WIDTH)
    {
      molecule::newPos.X = WIDTH - 1;
      if (molecule::newSpeed.X > 0)
        molecule::newSpeed.X *= -1;
    }
    if (molecule::newPos.X < 0)
    {
      molecule::newPos.X = 0;
      if (molecule::newSpeed.X < 0)
        molecule::newSpeed.X *= -1;
    }
    if (molecule::newPos.Y >= HEIGHT)
    {
      molecule::newPos.Y = HEIGHT - 1;
      if (molecule::newSpeed.Y > 0)
        molecule::newSpeed.Y *= -1;
    }
    if (molecule::newPos.Y < 0)
    {
      molecule::newPos.Y = 0;
      if (molecule::newSpeed.Y < 0)
        molecule::newSpeed.Y *= -1;
    }
#endif

  }

  void update ()
  {
    molecule::Pos = molecule::newPos;
    molecule::Speed = molecule::newSpeed;
  }

  void collision (vector<molecule> &m, int num)
  {
    for (unsigned int i = 0; i < m.size(); i++)
      if (i != num && abs((int)m[i].Pos.X - (int)m[num].Pos.X) < 2 && abs((int)m[i].Pos.Y - (int)m[num].Pos.Y) < 2)
      {
        m[i].Pos = m[num].Pos;
        m[num].Speed = (m[i].Speed = (m[i].Speed * m[i].mass + m[num].Speed * m[num].mass) / (m[i].mass + m[num].mass));
        // TODO delete m[i]
      }
  }
};

#endif