#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprites.h"
#include "Engine.h"
using namespace std;

Sprite::Sprite() {}
Sprite::Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
SDL_Rect* Sprite::GetSrcP() { return &m_rSrc; }
SDL_Rect* Sprite::GetDstP() { return &m_rDst; }
double Sprite::GetAngle() { return m_angle; }

AnimatedSprite::AnimatedSprite(int a, int fm, int sm, SDL_Rect s, SDL_Rect d) :
	Sprite(s, d), m_iFrameMax(fm), m_iSpriteMax(sm)
{
	// Added this here (and not in header line) just because. Note: initializers ARE resolved before body statements.
	m_angle = a;
	m_iSprite = m_iFrame = 0; // Chaining assignments to the same value.
}

void AnimatedSprite::Animate()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}
	}
	m_rSrc.x = m_rSrc.w * m_iSprite;
}

Player::Player(SDL_Rect s, SDL_Rect d) : AnimatedSprite(90, 8, 4, s, d) { m_pCurrentScore = 0; m_pWinScoreLevel1 = 3;
m_pWinScoreLevel2 = 6;
m_pWinScoreLevel3 = 9;
}
Explosion::Explosion(SDL_Rect s, SDL_Rect d) : AnimatedSprite(0, 12, 4, s, d) {  }

void Explosion::SetDstP(SDL_Rect r)
{
	*(&m_rDst) = r;
	//m_rSrc.y = r.y;
}

void Explosion::Animate1()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
			m_isAnimate = false;
		}
	}
	m_rSrc.x = m_rSrc.w * m_iSprite;
}

Bullet::Bullet(SDL_Rect s, SDL_Rect d, int spd) :
	Sprite(s, d), m_speed(spd), m_active(true) {
	
}

void Bullet::Update()
{
	if (Engine::Instance().m_bCanShoot == 5)
	{
		m_rDst.x += m_speed;
		m_angle = 0;
	}
	else if (Engine::Instance().m_bCanShoot == 8)
	{
		/*Engine::Instance().m_bShootDown = false;
		Engine::Instance().m_bShootRight = false;
		Engine::Instance().m_bShootLeft = false;*/

		m_rDst.y -= m_speed;
		m_angle = 270;
	}

	else if (Engine::Instance().m_bCanShoot == 2)
	{
		
		m_angle = 90;
		m_rDst.y += m_speed;
	}

	else if (Engine::Instance().m_bCanShoot == 6)
	{
		
		m_angle = 0;
		m_rDst.x += m_speed;
	}

	else if (Engine::Instance().m_bCanShoot == 4)
	{
		
		m_angle = 180;
		m_rDst.x -= m_speed;
	}
}


Enemy::Enemy(SDL_Rect s, SDL_Rect d, int m) : AnimatedSprite(-90, 4, 4, s, d) {
	moving = m; 
	if (m == 1)
		m_angle = 180;
	else if (m == 2)
		m_angle = 270;
	else m_angle = 0;
}

Enemy::Enemy(SDL_Rect s, SDL_Rect d, vector<Bullet*>* bVec, Mix_Chunk* c, int fr) : AnimatedSprite(-90, 4, 4, s, d),
m_pBulletVec(bVec), m_pPew(c), m_bulletTimer(0), m_timerMax(fr) {}

void Enemy::Update()
{
	//Animate();
	if(moving==1)
		m_rDst.x += 1;
	else if (moving == 2)
		m_rDst.y += 1;
	
	/*if (m_bulletTimer++ == m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBulletVec->push_back(new Bullet({ 160,100,14,14 }, { m_rDst.x,m_rDst.y - 28,14,14 }, -10));
		Mix_PlayChannel(-1, m_pPew, 0);
	}*/
}




