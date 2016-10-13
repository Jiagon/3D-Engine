#include "Engine.h"

int main()
{
	Engine engine = Engine();

	//------------------------------------------->>INITIALIZES ENGINE<<
	if (!engine.init())
	{
		return -1;
	}

	//------------------------------------------->>BUFFERS ENGINE<<
	if (!engine.bufferModels())
	{
		return false;
	}

	//------------------------------------------->>RUNS GAME LOOOP<<
	if (engine.useShader())
		engine.gameLoop();

	return 0;
}