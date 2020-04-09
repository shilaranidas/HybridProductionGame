#include "Engine.h"
#include <iostream>
#include <ctime>
#include "Utilities.h"

using namespace std;



//Engine::Engine():m_bRunning(false) // Class initializer list.
//{ }
Engine::Engine()
{
	m_bRunning = false;
	m_bCanShoot =6,
		
		m_playerDie = false;
	m_iSpeed = 5;
}
Engine::~Engine() { delete m_pFSM; }

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
					m_pTexture_p = IMG_LoadTexture(m_pRenderer, "pause_bg.png");
					m_pTexture_bg = IMG_LoadTexture(m_pRenderer, "level_1.png");
					m_pTexture_bg1 = IMG_LoadTexture(m_pRenderer, "level_2.png");
					m_pTexture_bg2 = IMG_LoadTexture(m_pRenderer, "level_2.png");
					m_pTexture_bg3 = IMG_LoadTexture(m_pRenderer, "level_3.png");
					//Player shtuff
					m_pTexturePR = IMG_LoadTexture(m_pRenderer, "playerRight.png");
					m_pTexturePB = IMG_LoadTexture(m_pRenderer, "bullet.png");
					//Enemy shtuff
					m_pTextureE1 = IMG_LoadTexture(m_pRenderer, "enemy1.png");
					m_pTextureExp = IMG_LoadTexture(m_pRenderer, "exp.png");

					



					//sound init
					if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
					{
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
						Mix_AllocateChannels(16);
						m_mPlayerBullet = Mix_LoadWAV("Fire.wav");
						m_mPlayerExplode = Mix_LoadWAV("explode.wav");

						m_mBgMusicTitle = Mix_LoadMUS("bg.mp3");
						m_mBgMusic = Mix_LoadMUS("gamebg.mp3");
						Mix_VolumeChunk(m_mPlayerBullet, 25);

						Mix_VolumeMusic(32);
						if (TTF_Init() == 0) // Font init success.
						{
							m_Font = TTF_OpenFont("LTYPE.TTF", 18); // 18 is our desired font size.
							m_font = TTF_OpenFont("30pakgirl_bold_TBS.ttf", 25);
						}
						else return false; // Font init fail.
					}
					else return false; // Mixer init fail.


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

	//added background music
	//Mix_PlayMusic(m_mBgMusicTitle, -1);
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
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP)
				m_bCanShoot = 8;
			else if (event.key.keysym.sym == SDLK_DOWN)
				m_bCanShoot = 2;
			else if (event.key.keysym.sym == SDLK_LEFT)
				m_bCanShoot = 4;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				m_bCanShoot = 6;
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

bool Engine::KeyUp(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 0)
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
	Mix_FreeMusic(m_mBgMusicTitle);
	Mix_FreeMusic(m_mBgMusic);
	Mix_FreeChunk(m_mPlayerBullet);
	Mix_FreeChunk(m_mPlayerExplode);
	SDL_DestroyTexture(m_pTexturePR);
	SDL_DestroyTexture(m_pTexturePB);
	SDL_DestroyTexture(m_pTextureE1);
	SDL_DestroyTexture(getTexture_bg());
	SDL_DestroyTexture(getTexture_bg1());
	SDL_DestroyTexture(getTexture_bg2());
	SDL_DestroyTexture(m_pTitleText1);
	SDL_DestroyTexture(m_pTitleText2);
	
	GetFSM().Clean();
	SDL_DestroyTexture(m_ftexture);
	SDL_FreeSurface(m_surface);
	TTF_CloseFont(m_font);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

int Engine::run()
{
	if (m_bRunning) // What does this do and what can it prevent?
		return -1;
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
/*void Engine::PrintMessage(string msg, bool shaded = false) {

	SDL_Color color = { 255, 255, 255 };
	SDL_Color colorbg = { 72,58,75 };
	TTF_SetFontOutline(m_font, 2);
	if (shaded)
		m_surface = TTF_RenderText_Shaded(m_font, msg.c_str(), color, colorbg);
	else
		m_surface = TTF_RenderText_Solid(m_font, msg.c_str(), color);
	m_ftexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	SDL_QueryTexture(m_ftexture, NULL, NULL, &texW, &texH);
	SDL_RenderCopy(m_pRenderer, m_ftexture, &m_fsrcrect, &m_fdstrect);

}*/


void Engine::CheckCollision(int level)
{
	if (m_player != nullptr)
	{
		// Player vs. Enemy.
		SDL_Rect p = { m_player->GetDstP()->x, m_player->GetDstP()->y, 61, 46 };
		for (int i = 0; i < (int)m_vEnemies.size(); i++)
		{
			SDL_Rect e = { m_vEnemies[i]->GetDstP()->x, m_vEnemies[i]->GetDstP()->y , 40, 38 };
			if (SDL_HasIntersection(&p, &e))
			{
				// Game over!
				cout << "Player goes boom!" << endl;
				m_playerDie = true;
				Mix_PlayChannel(-1, m_mPlayerExplode, 0);
				break;
			}
		}
		// Player bullets vs. Enemies.
		for (int i = 0; i < (int)m_vPBullets.size(); i++)
		{
			SDL_Rect b = { m_vPBullets[i]->GetDstP()->x - 40, m_vPBullets[i]->GetDstP()->y, 40, 23 };
			for (int j = 0; j < (int)m_vEnemies.size(); j++)
			{
				if (m_vEnemies[j] == nullptr) continue;
				SDL_Rect e = { m_vEnemies[j]->GetDstP()->x, m_vEnemies[j]->GetDstP()->y , 40, 38 };
				if (SDL_HasIntersection(&b, &e))
				{
					m_player->m_pCurrentScore++;

					Mix_PlayChannel(-1, m_mPlayerExplode, 0);
					delete m_vEnemies[j];
					m_vEnemies[j] = nullptr;
					delete m_vPBullets[i];
					m_vPBullets[i] = nullptr;
					m_bENull = true;
					m_bPBNull = true;
					if (level == 1)
					{
						if (m_player->m_pCurrentScore == m_player->m_pWinScoreLevel1)
						{
							m_playerWinLevel1 = true;
							m_player->m_pCurrentScore = 0;
						}
					}
					else if (level == 2)
					{
						if (m_player->m_pCurrentScore == m_player->m_pWinScoreLevel2)
						{
							m_playerWinLevel2 = true;
							m_player->m_pCurrentScore = 0;
						}
					}
					else if (level == 3)
					{
						if (m_player->m_pCurrentScore == m_player->m_pWinScoreLevel3)
						{
							m_playerWinLevel3 = true;
							m_player->m_pCurrentScore = 0;
						}
					}
					break;
				}
			}
		}
		if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);
		if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);

		if (m_playerDie)
		{
			//m_explosion = new Explosion({ 0,156,96,96 }, { p.x,p.y,96,96 });
			m_explosion->m_isAnimate = true;
			m_explosion->SetDstP({ p.x,p.y,96,96 });
			delete m_player;
			m_player = nullptr;
		}
	}
}



SDL_Texture* Engine::getTexturePR()
{
	return m_pTexturePR;
}
SDL_Texture* Engine::getTexturePB()
{
	return m_pTexturePB;
}

SDL_Texture* Engine::getTextureE1()
{
	return m_pTextureE1;
}
SDL_Texture* Engine::getTextureExp()
{
	return m_pTextureExp;
}


SDL_Texture* Engine::getTexture_p()
{
	return m_pTexture_p;
}
SDL_Texture* Engine::getTexture_bg()
{
	return m_pTexture_bg;
}
SDL_Texture* Engine::getTexture_bg1()
{
	return m_pTexture_bg1;
}
SDL_Texture* Engine::getTexture_bg2()
{
	return m_pTexture_bg2;
}

SDL_Texture* Engine::getTexture_bg3()
{
	return m_pTexture_bg3;
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
