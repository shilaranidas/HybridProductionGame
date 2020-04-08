#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "FSM.h"
#include "Sprites.h"
#define WIDTH 924
#define HEIGHT 703
#define FPS 60
// Keep header clean of initializaions and function definitions.
// Method prototypes only.

class Engine
{
private: // Properties.
	bool m_bRunning; // Loop control flag.
	int m_iSpeed;
	const Uint8* m_iKeystates; // Keyboard state container.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

	// New variables for sprite.
	//SDL_Rect m_pSrc, m_pDst;
	
	//SDL_Rect m_pSrcE1, m_pDstE1;
	//SDL_Rect m_pSrcE2, m_pDstE2;
	//SDL_Rect m_pSrcE3, m_pDstE3;
	SDL_Texture* m_pTexturePR;
	SDL_Texture* m_pTexturePB;
	SDL_Texture* m_pTextureE1;
	SDL_Texture* m_pTextureExp;
	SDL_Texture* m_pTexture_bg;
	SDL_Texture* m_pTexture_bg1;
	SDL_Texture* m_pTexture_bg2;


	FSM* m_pFSM; // Pointer to the StateMachine object created dynamically.
	SDL_Point m_MousePos;
	bool m_MouseState[3] = { 0,0,0 }; // Button up/down. Left, Middle, Right.
	//Player animation
	int angle = 0;
	TTF_Font* m_Font;
	SDL_Color m_cTextColor = { 255, 255, 255, 0 }; // White text.
	SDL_Texture* m_pTitleText1;
	SDL_Texture* m_pTitleText2;
	

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
	//bool keyDown(SDL_Scancode c);
	void update();
	void render();
	void clean();
public:
	bool m_bENull, // These three flags check if we need to clear the respective vectors of nullptrs.
		m_bPBNull,		
		
		m_playerDie, m_playerWinLevel1, m_playerWinLevel2, m_playerWinLevel3, m_playerWinFinal;
	int m_bCanShoot; // 0 for no shoot, 8 for up, 2 for down, 6 for right, 4 for left
		
	Player* m_player;
	vector<Enemy*> m_vEnemies;
	vector<Bullet*> m_vPBullets;
	Mix_Music* m_mBgMusic;
	Mix_Music* m_mBgMusicTitle;
	// Audio members.
	Mix_Chunk* m_mPlayerBullet;
	Mix_Chunk* m_mPlayerExplode;
	Explosion* m_explosion;
	bool fontShowed = true;
	TTF_Font* m_font;

	SDL_Surface* m_surface;
	SDL_Texture* m_ftexture;
	int texW = 600;
	int texH = 50;
	SDL_Rect m_fsrcrect;
	SDL_Rect m_fdstrect;
	Engine(); // Constructor.
	~Engine();
	int run();
	static Engine& Instance(); // This static method creates the static instance that can be accessed 'globally'
	bool KeyDown(SDL_Scancode c);
	bool KeyUp(SDL_Scancode c);
	SDL_Renderer* GetRenderer();
	FSM& GetFSM();
	SDL_Point& GetMousePos();
	bool GetMouseState(int idx);
	void QuitGame();
	void CheckCollision(int level);
	//void PrintMessage(string msg, bool shaded);
	//SDL_Rect* getDst();
	//SDL_Rect* getDstE1();
	//SDL_Rect* getDstE2();
	//SDL_Rect* getDstE3();
	//void setDst(SDL_Rect& newDst);
	//void setDst(SDL_Rect dst);
	//SDL_Rect* getSrc();
	//SDL_Rect* getSrcE1();

	//void setSrc(SDL_Rect& newSrc);
	SDL_Texture* getTexturePR();
	SDL_Texture* getTexturePB();
	SDL_Texture* getTexture_bg();
	SDL_Texture* getTexture_bg1();
	SDL_Texture* getTexture_bg2();
	SDL_Texture* getTextureE1();
	SDL_Texture* getTextureExp();
	
	int getAngle();
	void setAngle(int newAngle);
	int getSpeed();
	TTF_Font* getFont();
	SDL_Color getColor();
	SDL_Texture* getFontTexture1();
	SDL_Texture* getFontTexture2();
	void setFontTexture1(SDL_Texture* newFontTexture1);
	void setFontTexture2(SDL_Texture* newFontTexture2);
	//void setSrc(SDL_Rect src);
};