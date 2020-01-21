#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
// Keep header clean of initializaions and function definitions.
// Method prototypes only.

class Engine
{
private: // Properties.
	bool m_bRunning; // Loop control flag.
	const Uint8* m_iKeystates; // Keyboard state container.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.
private: // Methods.
	bool init(const char*, int, int, int, int, int); // Parameter names are not needed in a function prototype.
	void wake(); // Function prototypes ARE statements, so don't forget the ';'
	void sleep() // Sleep is now an inline method. No definition in .cpp file.
	{ // This is just to demo. I don't want inline methods. MAYBE getters/setters will be ok.
		m_end = SDL_GetTicks();
		m_delta = m_end - m_start;
		if (m_delta < m_fps) // Engine has to sleep.
			SDL_Delay(m_fps - m_delta);
	}
	void handleEvents();
	bool keyDown(SDL_Scancode c);
	void update();
	void render();
	void clean();
public:
	Engine(); // Constructor.
	~Engine();
	int run();
};

