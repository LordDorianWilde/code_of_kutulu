
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
  
