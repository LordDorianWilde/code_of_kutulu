int myExplorerId = -1;

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
