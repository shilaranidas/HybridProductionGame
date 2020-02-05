#include "Engine.h"
#include <iostream>

using namespace std;



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
					if (TTF_Init() == 0) // Font init success.
					{
						m_Font = TTF_OpenFont("aurora.TTF", 18); // 18 is our desired font size.
					}
					else return false; // Font init fail.

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
	m_pFSM = new FSM(); // Creates the state machine object/instance.
	m_pFSM->ChangeState(new TitleState()); // Invoking the ChangeState method to set the initial state, Title.
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
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button >= 1 && event.button.button <= 3)
				m_MouseState[event.button.button - 1] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button >= 1 && event.button.button <= 3)
				m_MouseState[event.button.button - 1] = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);
			break;
		}
	}
}

// Keyboard utility function.
bool Engine::KeyDown(SDL_Scancode c)
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
	
	
	GetFSM().Update(); // Invokes the update of the state machine.
}

void Engine::render()
{
	
	GetFSM().Render(); // Invokes the render of the state machine.
}

void Engine::clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyTexture(m_pTexturePR);
	SDL_DestroyTexture(m_pTexture_bg);
	SDL_DestroyTexture(m_pTitleText1);
	SDL_DestroyTexture(m_pTitleText2);
	GetFSM().Clean();
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
Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}
SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }

FSM& Engine::GetFSM() { return *m_pFSM; }

SDL_Point& Engine::GetMousePos() { return m_MousePos; }

bool Engine::GetMouseState(int idx) { return m_MouseState[idx]; }

void Engine::QuitGame() { m_bRunning = false; }

SDL_Rect* Engine::getDst()
{
	return &m_pDst;
}

//void Engine::setDst(SDL_Rect& newDst)
//{
//	m_pDst = newDst;
//}

SDL_Rect* Engine::getSrc()
{
	return &m_pSrc;
}

//void Engine::setSrc(SDL_Rect& newSrc)
//{
//	m_pSrc = newSrc;
//}

SDL_Texture* Engine::getTexturePR()
{
	return m_pTexturePR;
}

SDL_Texture* Engine::getTexture_bg()
{
	return m_pTexture_bg;
}

int Engine::getAngle()
{
	return angle;
}

void Engine::setAngle(int newAngle)
{
	angle = newAngle;
}

int Engine::getSpeed()
{
	return m_iSpeed;
}

TTF_Font* Engine::getFont()
{
	return m_Font;
}

SDL_Color Engine::getColor()
{
	return m_cTextColor;
}

SDL_Texture* Engine::getFontTexture1()
{
	return m_pTitleText1;
}
SDL_Texture* Engine::getFontTexture2()
{
	return m_pTitleText2;
}

void Engine::setFontTexture1(SDL_Texture* newFontTexture1)
{
	m_pTitleText1 = newFontTexture1;
}
void Engine::setFontTexture2(SDL_Texture* newFontTexture2)
{
	m_pTitleText2 = newFontTexture2;
}
