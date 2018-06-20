
class Wanderer : public Unit
{
 public:
  int invocation;
  int state;
  int idTarget;

  Wanderer(int a, int b, int c, int d, int e, int f)
  {
    id = a;
    position.x = b;
    position.y = c;
    invocation = d;
    state = e;
    idTarget = f;
  }
};

