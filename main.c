#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MUR  0
#define PORTAIL 1
#define VIDE 2

using namespace std;

int MyExplorer = -1;

class Plateau
{
 public:
  int width;
  int height;
  int** plateau;
  int** monsterDistance;

  Plateau(const int a,const int b)
    {
      width = a;
      height = b;
      plateau = new int*[width];
      monsterDistance = new int*[width];
      
      for(int i = 0; i < width; i++)
	{
	  plateau[i] = new int[height];
	  monsterDistance = new int[height];
	}
    }

  void setPlateauLine(int index, string elements)
    {
      for(int i = 0; i < width; i++)
	{
	  if(elements[i] == '#')
	    plateau[i][index] = MUR;
	  else if (elements[i] == 'w')
	    plateau[i][index] = PORTAIL;
	  else if (elements[i] == '.')
	    plateau[i][index] = VIDE;
	  else
	    cerr << "ERREUR description plateau : " << elements << " " << i << "\n";
	}
    }
};

class Unit
{
 public:
  
  int id;
  int x;
  int y;

  Unit()
    {
      id = 0;
      x = 0;
      y = 0;
    }

  Unit(int a, int b, int c)
    {
      id = a;
      x = b;
      y = c;
    }
};

class Explorer : public Unit
{
 public:

  int health;

  Explorer(int a, int b, int c, int d, int e, int f)
    {
      Unit(a, b, c);
      health = d;
    }
};

class Wanderer : public Unit
{
 public:
  int invocation;
  int state;
  int idTarget;

  Wanderer(int a, int b, int c, int d, int e, int f)
    {
      Unit(a, b, c);
      invocation = d;
      state = e;
      idTarget = f;
    }
};

vector<Explorer> explorers;
vector<Wanderer> wanderers;

Explorer getExplorerById(int id)
{
  for(int i = 0; i < explorers.size(); i++)
    {
      if (id == explorers[i].id)
	return explorers[i];
    }

  return explorers[0];
}


int main()
{
    int width;
    cin >> width; cin.ignore();
    int height;
    cin >> height; cin.ignore();
    Plateau plateau(width, height);
    for (int i = 0; i < height; i++) {
        string line;
        getline(cin, line);
	plateau.setPlateauLine(i, line);
    }
    int sanityLossLonely; // how much sanity you lose every turn when alone, always 3 until wood 1
    int sanityLossGroup; // how much sanity you lose every turn when near another player, always 1 until wood 1
    int wandererSpawnTime; // how many turns the wanderer take to spawn, always 3 until wood 1
    int wandererLifeTime; // how many turns the wanderer is on map after spawning, always 40 until wood 1
    cin >> sanityLossLonely >> sanityLossGroup >> wandererSpawnTime >> wandererLifeTime; cin.ignore();

    // game loop
    while (1) {
        int entityCount; // the first given entity corresponds to your explorer
        cin >> entityCount; cin.ignore();
        for (int i = 0; i < entityCount; i++) {
            string entityType;
            int id;
            int x;
            int y;
            int param0;
            int param1;
            int param2;
            cin >> entityType >> id >> x >> y >> param0 >> param1 >> param2; cin.ignore();

	    if (entityType == "EXPLORER")
	      {
		if (MyExplorer == -1)
		  MyExplorer = id;
		explorers.push_back(Explorer(id, x, y, param0, param1, param2));
	      }
	    else if(entityType == "WANDERER")
	      {
		wanderers.push_back(Wanderer(id, x, y, param0, param1, param2));
	      }
	    else
	      cerr << "ERREUR lecture unit : " << entityType << "\n";
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << "WAIT" << endl; // MOVE <x> <y> | WAIT
    }
}
