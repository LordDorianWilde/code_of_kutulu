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

using namespace std;

int myExplorerId = -1;

class Position
{
 public:
  int x;
  int y;

  Position()
    {
      x = -1;
      y = -1;
    }

  Position(int a, int b)
    {
      x = a;
      y = b;
    }
};

class Plateau
{
 public:
  int width;
  int height;
  int** plateau;
  int** monsterDistance;
  vector<Position> cases;

  Plateau(const int a,const int b)
    {
      width = a;
      height = b;
      plateau = new int*[width];
      monsterDistance = new int*[width];

      for(int i = 0; i < width; i++)
        {
          plateau[i] = new int[height];
          monsterDistance[i] = new int[height];
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

  int getPositionStatus(int x, int y)
  {
    if (x < 0 || x >= width)
      return MUR;
    if (y < 0 || y >= height)
      return MUR;

    return plateau[x][y];
  }

  void initMonsterDistance()
  {
    for (int i = 0; i < width; i++)
      {
        for (int j = 0; j < height; j++)
          {
            monsterDistance[i][j] = -1;
          }

      }

    cases.clear();
  }

  void calculMonsterDistance()
  {
    vector<Position> aux;
    int dist = 0;

    while(cases.size() != 0)
      {
        for(int k = 0; k < cases.size(); k++)
          {
            int i = cases[k].x;
            int j = cases[k].y;

            if (monsterDistance[i][j] == -1)
                monsterDistance[i][j] = dist;

            if (getPositionStatus(i-1, j) == VIDE && monsterDistance[i-1][j] == -1)
              aux.push_back(Position(i-1, j));
            if (getPositionStatus(i+1, j) == VIDE && monsterDistance[i+1][j] == -1)
              aux.push_back(Position(i+1, j));
            if (getPositionStatus(i, j-1) == VIDE && monsterDistance[i][j-1] == -1)
              aux.push_back(Position(i, j-1));
            if (getPositionStatus(i, j+1) == VIDE && monsterDistance[i][j+1] == -1)
              aux.push_back(Position(i, j+1));
          }

        dist++;
        cases = aux;
        aux.clear();
      }    
  }

  Position getMaxDistanceMonster()
  {
    Position maxPos = Position(0,0);

    for(int i = 0; i < width; i++)
      {
        for (int j = 0; j < height; j++)
          {
            if (monsterDistance[i][j] > monsterDistance[maxPos.x][maxPos.y])
              {
                maxPos = Position(i, j);
              }
          }
      }

    return maxPos;
  }

  Position getCloseMaxDistance(Position pos)
  {
    Position maxPos = pos;

    if (getPositionStatus(pos.x-1, pos.y) == VIDE && monsterDistance[pos.x-1][pos.y] > monsterDistance[maxPos.x][maxPos.y])
      maxPos = Position(pos.x-1, pos.y);
    if (getPositionStatus(pos.x+1, pos.y) == VIDE && monsterDistance[pos.x+1][pos.y] > monsterDistance[maxPos.x][maxPos.y])
      maxPos = Position(pos.x+1, pos.y);
    if (getPositionStatus(pos.x, pos.y-1) == VIDE && monsterDistance[pos.x][pos.y-1] > monsterDistance[maxPos.x][maxPos.y])
      maxPos = Position(pos.x, pos.y-1);
    if (getPositionStatus(pos.x, pos.y+1) == VIDE && monsterDistance[pos.x][pos.y+1] > monsterDistance[maxPos.x][maxPos.y])
      maxPos = Position(pos.x, pos.y+1);

    return maxPos;
  }
};

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

vector<Explorer> explorers;
vector<Wanderer> wanderers;
int nbr_use_light = 3;
int nbr_use_plan = 2;
int initial_life = -1;
int nbr_turn_left_light = 0;
int nbr_turn_left_plan = 0;


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
        plateau.initMonsterDistance();
        myExplorerId = -1;
        explorers.clear();
        wanderers.clear();
	nbr_turn_left_light = max(0, nbr_turn_left_light-1);
	nbr_turn_left_plan = max(0, nbr_turn_left_plan-1);

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
    		if (myExplorerId == -1)
    		{
    		  cerr << "my explo : " << x << " " << y << endl;
    		  myExplorerId = id;
    		}
		if (initial_life == -1)
		  initial_life = param0;
    		Explorer e = Explorer(id, x, y, param0, param1, param2);
    		explorers.push_back(e);
	      }
	    else if(entityType == "WANDERER")
	      {
    		Wanderer w = Wanderer(id, x, y, param0, param1, param2);
    		wanderers.push_back(w);
    		plateau.cases.push_back(w.position);
	      }
	    else
	      cerr << "ERREUR lecture unit : " << entityType << "\n";
        }

        cerr << myExplorerId << endl;
        Explorer myExplorer = getExplorerById(myExplorerId);
        cerr << "myExplorer position : " << myExplorer.position.x << " " << myExplorer.position.y << endl;
        
        plateau.calculMonsterDistance();
        
        if ( myExplorer.position.x != -1 && myExplorer.position.y != -1 &&
	     plateau.monsterDistance[myExplorer.position.x][myExplorer.position.y] != -1)
        {
	    Position nextPos = plateau.getCloseMaxDistance(myExplorer.position);

	    if (plateau.monsterDistance[nextPos.x][nextPos.y] <= 1 &&
		nbr_turn_left_light == 0 &&
		nbr_use_light > 0)
	      {
		nbr_use_light--;
		nbr_turn_left_light = NBR_TURN_LIGHT;
		cout << "LIGHT" << endl;
	      }
	    else if (myExplorer.health < initial_life / 2 &&
		     nbr_turn_left_plan == 0 &&
		     nbr_use_plan > 0)
	      {
		nbr_use_plan--;
		nbr_turn_left_plan = NBR_TURN_PLAN;
		cout << "PLAN" << endl;
	      }
	    else
	      {
		cout << "MOVE " << nextPos.x << " " << nextPos.y << endl; // MOVE <x> <y> | WAIT
	      }
        }
        else
        {
            cout << "WAIT" << endl;
        }
    }
}
