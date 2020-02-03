#pragma once
#include <functional>
#include "SDL.h"

class Button
{
private:
	enum state { MOUSE_UP, MOUSE_OVER, MOUSE_DOWN };
	int m_iFrame;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst;
	SDL_Texture* m_pText;
	
	std::function<void()> m_callback; // The button callback!

public:
	Button(const char* s, SDL_Rect src, SDL_Rect dst, std::function<void()> cb);
	~Button();
	bool MouseCollision();
	void Update();
	void Render();
};