#include "GameLoop.h"


int main(int argc, char* argv[])
{

	//gameloop to keep game looping, while looping game will keep updating and rendering, if loop ends, the game cleans itself
	GameLoop gameLoop = GameLoop();
	if (gameLoop.init() < 0) return 1;
	while (gameLoop.keepAlive())
	{
		gameLoop.update();
		gameLoop.render();
		int main();

	}
	gameLoop.clean();
	return 0;
}