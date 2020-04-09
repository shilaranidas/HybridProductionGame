#include <iostream>
#include "Button.h"
#include "Engine.h"
#include "SDL.h"
#include "SDL_image.h"
using namespace std;

Button::Button(const char* s, SDL_Rect src, SDL_Rect dst, const char* text, int w, int h)
	: m_rSrc(src), m_rDst(dst), m_state(STATE_UP)
{
	cout << "Constructing button!" << endl;
	m_btnText = text;
	width = w;
	height = h;
	// Set the button image. You should have some fail checking just in case. 
	m_pText = IMG_LoadTexture(Engine::Instance().GetRenderer(), s);
}

Button::~Button()
{
	SDL_DestroyTexture(m_pText);
}

bool Button::MouseCollision()
{
	int mx = Engine::Instance().GetMousePos().x;
	int my = Engine::Instance().GetMousePos().y;
	return (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x &&
		    my < (m_rDst.y + m_rDst.h) && my > m_rDst.y);
}

void Button::Update()
{
	bool col = MouseCollision();
	switch (m_state)
	{
	case STATE_UP:
		if (col)   
			m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col)
			m_state = STATE_UP;
		else if (col && Engine::Instance().GetMouseState(0))
			m_state = STATE_DOWN;
		break;
	case STATE_DOWN:
		if (!Engine::Instance().GetMouseState(0))
		{
			if (col)
			{
				m_state = STATE_OVER;
				// Execute new "callback".
				Execute();
			}
			else 
				m_state = STATE_UP;
		}
		break;
	}
}

void Button::Render()
{
	//m_rSrc.x = m_rSrc.w (int)m_state;
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pText, &m_rSrc, &m_rDst);
	SDL_Color color = { 40, 8, 0 };	
	//int texW = 100;
	//int texH = 30;
	SDL_Rect m_fsrcrect;
	SDL_Rect m_fdstrect;
	m_fsrcrect = { 0,0,width,height };
	m_fdstrect = { m_rDst.x + 100-width/2, m_rDst.y + 24, width, height };
	TTF_SetFontOutline(Engine::Instance().m_font, 1);	
	Engine::Instance().m_surface = TTF_RenderText_Solid(Engine::Instance().m_font, m_btnText, color);
	Engine::Instance().m_ftexture = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), Engine::Instance().m_surface);
	SDL_QueryTexture(Engine::Instance().m_ftexture, NULL, NULL, &width, &height);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().m_ftexture, &m_fsrcrect, &m_fdstrect);
}

// Yes, the downside of the command pattern is we need a subclass for each unique type of button.

PlayButton::PlayButton(const char * s, SDL_Rect src, SDL_Rect dst, const char * text, int w, int h):Button(s, src, dst,text,w,h){}
void PlayButton::Execute()
{
	Engine::Instance().GetFSM().ChangeState(new GameState);
}

ExitButton::ExitButton(const char * s, SDL_Rect src, SDL_Rect dst, const char * text, int w, int h):Button(s, src, dst,text,w,h){}
void ExitButton::Execute()
{
	Engine::Instance().QuitGame();
}

ResumeButton::ResumeButton(const char * s, SDL_Rect src, SDL_Rect dst, const char * text,  int w, int h) :Button(s, src, dst,text,w,h) {}
void ResumeButton::Execute()
{
	Engine::Instance().GetFSM().PopState();
}
MainMenuButton::MainMenuButton(const char * s, SDL_Rect src, SDL_Rect dst, const char * text, int w, int h) : Button(s, src, dst, text, w, h) {}
void MainMenuButton::Execute()
{
	Engine::Instance().GetFSM().ChangeState(new TitleState());
}