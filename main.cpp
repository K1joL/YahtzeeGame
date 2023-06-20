#include "Game.h"
#include <iostream>

using namespace std;

int main()
{
	Game g;

	while (true)
	{
		g.startGame();

		cout << "Want to start new game?	(Y)es or (N)o" << endl;
		char answer;
		cin >> answer;
		if (answer == 'N')
			break;
	}


	return 0;
}