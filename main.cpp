#include "gameWindow.h"
#include "time.h"
#include <Windows.h>
#include <irrKlang.h>

int main() {
	/*
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	char i = 0;

	while (i != 'q')
	{
		std::cout << "Press any key to play some sound, press 'q' to quit.\n";

		// play a single sound
		engine->play2D("wave2.wav", false);
		//fflush(0);
		std::cin >> i; // wait for user to press some key
	}
	engine->drop();
	*/
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	srand((unsigned int)time(NULL));
	int width = 1200;
	int height = 800;
	gameWindow newGameWindow("hello world!", width, height);
	newGameWindow.gameLoop();
	//::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	return 0;
}