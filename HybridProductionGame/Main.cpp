#include "Engine.h"

int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	Engine e; // This line will be removed by the Singleton pattern.
	return e.run();
}