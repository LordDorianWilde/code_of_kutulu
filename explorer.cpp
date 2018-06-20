
class Explorer : public Unit
{
 public:

  int health;

  Explorer(int a, int b, int c, int d, int e, int f)
    {
      id = a;
      position.x = b;
      position.y = c;
      health = d;
    }
};
