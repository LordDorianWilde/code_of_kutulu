#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#define MUR  0
#define PORTAIL 1
#define VIDE 2

#define NBR_TURN_LIGHT 3
#define NBR_TURN_PLAN 5

class Unit
{
 public:

  int id;
  Position position;

  Unit()
    {
      id = 0;
      position = Position();
    }

  Unit(int a, int b, int c)
    {
      id = a;
      position.x = b;
      position.y = c;
    }
};
