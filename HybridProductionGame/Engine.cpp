#include "Engine.h"
#include <iostream>
#define WIDTH 924
#define HEIGHT 703
#define FPS 60
using namespace std;

//Player animation
int angle = 0;

//Engine::Engine():m_bRunning(false) // Class initializer list.
//{ }
Engine::Engine()
{
	m_bRunning = false;
	m_iSpeed = 5;
}
Engine::~Engine() {}

bool Engine::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				if (IMG_Init(IMG_INIT_PNG))
				{
					m_pTexture_bg = IMG_LoadTexture(m_pRenderer, "bg.png");
					//Player shtuff
					m_pTexturePR = IMG_LoadTexture(m_pRenderer, "playerRight.png");


				}
				else return false; // Init init fail.
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	// Create the sprite.
	m_pSrc = { 0, 0, 61, 46 };
	m_pDst = { width / 2 - m_pSrc.w / 2 - 150, height / 2 - m_pSrc.h / 2, m_pSrc.w, m_pSrc.h };
	m_bRunning = true; // Everything is okay, start the engine.
	cout << "Success!" << endl;
	return true;
}

void Engine::wake()
{
	m_start = SDL_GetTicks();
}

// Moved sleep to .h to demonstrate an inline method.

void Engine::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			m_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		}
	}
}

// Keyboard utility function.
bool Engine::keyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Engine::update()
{
	if ((keyDown(SDL_SCANCODE_W) || keyDown(SDL_SCANCODE_UP)) && m_pDst.y > m_iSpeed)
	{
		angle = 270;
		m_pDst.y -= m_iSpeed;

	}
	if ((keyDown(SDL_SCANCODE_S) || keyDown(SDL_SCANCODE_DOWN)) && m_pDst.y < HEIGHT - m_pDst.h - m_iSpeed)
	{
		angle = 90;
		m_pDst.y += m_iSpeed;
		//cout << g_dst.y << " ";
	}

	if ((keyDown(SDL_SCANCODE_A) || keyDown(SDL_SCANCODE_LEFT)) && m_pDst.x > m_iSpeed)
	{
		angle = 180;
		m_pDst.x -= m_iSpeed;
	}


	//if (keyDown(SDL_SCANCODE_D) && g_dst.x< WIDTH-g_dst.w- g_iSpeed)
	if ((keyDown(SDL_SCANCODE_D) || keyDown(SDL_SCANCODE_RIGHT)) && m_pDst.x < WIDTH / 2 - m_pDst.w)
	{
		angle = 0;
		m_pDst.x += m_iSpeed;
	}
}

void Engine::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff.
	SDL_RenderCopy(m_pRenderer, m_pTexture_bg, NULL, NULL);
	SDL_RenderCopyEx(m_pRenderer, m_pTexturePR, &m_pSrc, &m_pDst, angle, nullptr, SDL_FLIP_NONE);
	
	// Draw anew.
	SDL_RenderPresent(m_pRenderer);
}

void Engine::clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyTexture(m_pTexturePR);
	SDL_DestroyTexture(m_pTexture_bg);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

int Engine::run()
{
	if (init("Hybrid Production Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_bRunning)
	{
		wake();
		handleEvents();
		update();
		render();
		if (m_bRunning)
			sleep();
	}
	clean();
	return 0;
}
