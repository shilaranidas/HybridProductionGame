#include <functional>
#include <iostream>
#include "FSM.h"
#include "Engine.h"
#include "Utilities.h"
#define BUTTON_W 200
#define BUTTON_H 70
using namespace std;

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin PauseState.
PauseState::PauseState() {}


void PauseState::Enter()
{
	cout << "Entering Pause..." << endl;
	m_vButtons.push_back(new ResumeButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 420, 270,BUTTON_W,BUTTON_H }, "Resume", 100, 30));
	// This exit button has a different size but SAME function as the one in title.
	m_vButtons.push_back(new ExitButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 420 ,360,BUTTON_W,BUTTON_H }, "Exit", 60, 30));
}

void PauseState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void PauseState::Render()
{
	//cout << "Rendering Pause..." << endl;
	Engine::Instance().GetFSM().GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_p(), NULL, NULL);
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting Pause..." << endl;

	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}
// End PauseState.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	cout << "Entering Game..." << endl;
	//{ width / 2 - m_pSrc.w / 2 - 300, height / 2 - m_pSrc.h / 2 + 50, m_pSrc.w, m_pSrc.h };
	Engine::Instance().m_player = new Player({ 0,0,61,46 }, { WIDTH/2-61/2-300, HEIGHT/2-46/2+50,61,46});
	//introduce enemy
	Engine::Instance().m_vEnemies.clear();
	Engine::Instance().m_vEnemies.reserve(3);
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 467,503,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 500, 328,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 630, 439,40,38 }));
	Engine::Instance().m_explosion = new Explosion({ 0,0,96,96 }, { 0,0,96,96 });
	Mix_PlayMusic(Engine::Instance().m_mBgMusic, -1);
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P) == 1)
		Engine::Instance().GetFSM().PushState(new PauseState());
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_X) == 1)
		Engine::Instance().GetFSM().ChangeState(new TitleState());
	else if (Engine::Instance().m_playerDie)
		Engine::Instance().GetFSM().ChangeState(new LoseState());
	else if (Engine::Instance().m_playerWinLevel1)
		Engine::Instance().GetFSM().ChangeState(new GameState1());
	

	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel1)
	{
	
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_W)) && Engine::Instance().m_player->GetDstP()->y > Engine::Instance().getSpeed())
	{
		/*Engine::Instance().m_bShootUp = true;
		Engine::Instance().m_bShootDown = false;
		Engine::Instance().m_bShootRight = false;
		Engine::Instance().m_bShootLeft = false;*/

		Engine::Instance().setAngle(270);
		if (Engine::Instance().m_player->GetDstP()->y > 248)
			Engine::Instance().m_player->GetDstP()->y -= Engine::Instance().getSpeed();

	
		cout << Engine::Instance().m_player->GetDstP()->y << endl;
		//here 248 is the building wall in top of background


	}
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_S)) && Engine::Instance().m_player->GetDstP()->y < HEIGHT - Engine::Instance().m_player->GetDstP()->h - Engine::Instance().getSpeed())
	{
	/*	Engine::Instance().m_bShootUp = false;
		Engine::Instance().m_bShootDown = true;
		Engine::Instance().m_bShootRight = false;
		Engine::Instance().m_bShootLeft = false;*/

		Engine::Instance().setAngle(90);
		Engine::Instance().m_player->GetDstP()->y += Engine::Instance().getSpeed();
		//cout << g_dst.y << " ";
	}

	if ((Engine::Instance().KeyDown(SDL_SCANCODE_A)) && Engine::Instance().m_player->GetDstP()->x > Engine::Instance().getSpeed())
	{
		/*Engine::Instance().m_bShootUp = false;
		Engine::Instance().m_bShootDown = false;
		Engine::Instance().m_bShootRight = false;
		Engine::Instance().m_bShootLeft = true;*/

		Engine::Instance().setAngle(180);
		Engine::Instance().m_player->GetDstP()->x -= Engine::Instance().getSpeed();
	}


	//if (keyDown(SDL_SCANCODE_D) && g_dst.x< WIDTH-g_dst.w- g_iSpeed)
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_D)) && Engine::Instance().m_player->GetDstP()->x < WIDTH - Engine::Instance().m_player->GetDstP()->w)
	{
		/*Engine::Instance().m_bShootUp = false;
		Engine::Instance().m_bShootDown = false;
		Engine::Instance().m_bShootRight = true;
		Engine::Instance().m_bShootLeft = false;*/

		Engine::Instance().setAngle(0);

		Engine::Instance().m_player->GetDstP()->x += Engine::Instance().getSpeed();
	}
	//---------------------------------------------------------------------------------------SHOOTING----------------------------------------------------------------------------------//



	/*if (Engine::Instance().KeyDown(SDL_SCANCODE_SPACE) && Engine::Instance().m_bCanShoot)
	{
		Engine::Instance().m_bCanShoot = false;
		Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x + 25,Engine::Instance().m_player->GetDstP()->y + 10 ,40,23 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
	}*/

	if (Engine::Instance().KeyDown(SDL_SCANCODE_RIGHT) && (Engine::Instance().m_bCanShoot == 6))
	{
		Engine::Instance().m_bCanShoot = 0;
		Engine::Instance().setAngle(0);
		Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y  ,40,23 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_LEFT) && (Engine::Instance().m_bCanShoot == 4))
	{
		Engine::Instance().m_bCanShoot = 0;
		Engine::Instance().setAngle(180);

		Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_UP) && (Engine::Instance().m_bCanShoot == 8))
	{
		Engine::Instance().m_bCanShoot = 0;
		Engine::Instance().setAngle(270);

		Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_DOWN) && (Engine::Instance().m_bCanShoot == 2))
	{
		Engine::Instance().m_bCanShoot = 0;
		Engine::Instance().setAngle(90);

		Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
	}


}

	// Update the bullets. Player's first.
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		Engine::Instance().m_vPBullets[i]->Update();
		if (Engine::Instance().m_vPBullets[i]->GetDstP()->x > WIDTH || Engine::Instance().m_vPBullets[i]->GetDstP()->y > HEIGHT
			|| Engine::Instance().m_vPBullets[i]->GetDstP()->x < 0 || Engine::Instance().m_vPBullets[i]->GetDstP()->y < 0)
		{
			delete Engine::Instance().m_vPBullets[i];
			Engine::Instance().m_vPBullets[i] = nullptr;
			Engine::Instance().m_bPBNull = true;
		}
	}
	if (Engine::Instance().m_bPBNull) CleanVector<Bullet*>(Engine::Instance().m_vPBullets, Engine::Instance().m_bPBNull);
	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel1)
		Engine::Instance().CheckCollision(1);
}

void GameState::Render()
{
	//cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Render stuff.
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_bg(), NULL, NULL);
	// Player.
	if (!Engine::Instance().m_playerDie)
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePR(), Engine::Instance().m_player->GetSrcP(), Engine::Instance().m_player->GetDstP(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);
	else
	{
		if (Engine::Instance().m_explosion->m_isAnimate) {
			SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureExp(), Engine::Instance().m_explosion->GetSrcP(), Engine::Instance().m_explosion->GetDstP());
			Engine::Instance().m_explosion->Animate1();

		}
	}
	// Player bullets.	
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePB(), Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP(), Engine::Instance().m_vPBullets[i]->GetAngle(), nullptr, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vPBullets[i]->GetDstP());*/
	}
	// Enemies.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().m_vEnemies[i]->GetSrcP(), Engine::Instance().m_vEnemies[i]->GetDstP());
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vEnemies[i]->GetDstP());*/
	}



	// If GameState != current state.
	if (dynamic_cast<GameState*>(Engine::Instance().GetFSM().GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	cout << "Exiting Game..." << endl;
	delete Engine::Instance().m_player;
	Engine::Instance().m_player = nullptr;
	Engine::Instance().m_vPBullets.clear();
	Engine::Instance().m_vEnemies.clear();
	//
	//Mix_FreeMusic(Engine::Instance().m_mBgMusic);
}

void GameState::Resume() { cout << "Resuming Game..." << endl; }
// End GameState.

// Begin GameState1.
GameState1::GameState1() {}

void GameState1::Enter()
{
	cout << "Entering Game1..." << endl;
	//{ width / 2 - m_pSrc.w / 2 - 300, height / 2 - m_pSrc.h / 2 + 50, m_pSrc.w, m_pSrc.h };
	Engine::Instance().m_player = new Player({ 0,0,61,46 }, { WIDTH / 2 - 61 / 2 - 250, HEIGHT / 2 - 46 / 2 + 50,61,46 });
	//introduce enemy
	Engine::Instance().m_vEnemies.clear();
	Engine::Instance().m_vEnemies.reserve(6);
	//fix enemy
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 277,140,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 667, 400,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 350, 600,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 567, 500,40,38 }));
	//moving enemy
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 40, 115,40,38 },1));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 770, 60,40,38 },2));
	Engine::Instance().m_explosion = new Explosion({ 0,0,96,96 }, { 0,0,96,96 });
	//Mix_PlayMusic(Engine::Instance().m_mBgMusic, -1);
}

void GameState1::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P) == 1)
		Engine::Instance().GetFSM().PushState(new PauseState());
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_X) == 1)
		Engine::Instance().GetFSM().ChangeState(new TitleState());
	else if (Engine::Instance().m_playerDie)
		Engine::Instance().GetFSM().ChangeState(new LoseState());
	else if (Engine::Instance().m_playerWinLevel2)
		Engine::Instance().GetFSM().ChangeState(new GameState2());
	////else if (Engine::Instance().m_playerWinFinal)
	//	//Engine::Instance().GetFSM().ChangeState(new WinState());

	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel2)
	{

		if ((Engine::Instance().KeyDown(SDL_SCANCODE_W)) && Engine::Instance().m_player->GetDstP()->y > Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(270);
			Engine::Instance().m_player->GetDstP()->y -= Engine::Instance().getSpeed();
		}
		if ((Engine::Instance().KeyDown(SDL_SCANCODE_S)) && Engine::Instance().m_player->GetDstP()->y < HEIGHT - Engine::Instance().m_player->GetDstP()->h - Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(90);
			Engine::Instance().m_player->GetDstP()->y += Engine::Instance().getSpeed();
		}

		if ((Engine::Instance().KeyDown(SDL_SCANCODE_A)) && Engine::Instance().m_player->GetDstP()->x > Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(180);
			Engine::Instance().m_player->GetDstP()->x -= Engine::Instance().getSpeed();
		}
		if ((Engine::Instance().KeyDown(SDL_SCANCODE_D)) && Engine::Instance().m_player->GetDstP()->x < WIDTH - Engine::Instance().m_player->GetDstP()->w)
		{
			Engine::Instance().setAngle(0);
			Engine::Instance().m_player->GetDstP()->x += Engine::Instance().getSpeed();
		}
		//---------------------------------------------------------------------------------------SHOOTING----------------------------------------------------------------------------------//
		if (Engine::Instance().KeyDown(SDL_SCANCODE_RIGHT) && (Engine::Instance().m_bCanShoot==6))
		{			
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(0);
			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y  ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_LEFT) && (Engine::Instance().m_bCanShoot==4))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(180);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_UP) && (Engine::Instance().m_bCanShoot==8))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(270);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_DOWN) && (Engine::Instance().m_bCanShoot==2))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(90);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}



	}

	// Update the bullets. Player's first.
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		Engine::Instance().m_vPBullets[i]->Update();
		if (Engine::Instance().m_vPBullets[i]->GetDstP()->x > WIDTH || Engine::Instance().m_vPBullets[i]->GetDstP()->y > HEIGHT
			|| Engine::Instance().m_vPBullets[i]->GetDstP()->x < 0|| Engine::Instance().m_vPBullets[i]->GetDstP()->y<0)
		{
			delete Engine::Instance().m_vPBullets[i];
			Engine::Instance().m_vPBullets[i] = nullptr;
			Engine::Instance().m_bPBNull = true;
		}
	}
	// Update the enemy.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		Engine::Instance().m_vEnemies[i]->Update();
		if (Engine::Instance().m_vEnemies[i]->GetDstP()->x > WIDTH)
		{
			Engine::Instance().m_vEnemies[i]->GetDstP()->x = 0;
		}
		if (Engine::Instance().m_vEnemies[i]->GetDstP()->y > HEIGHT)
		{
			Engine::Instance().m_vEnemies[i]->GetDstP()->y = 0;
		}
	}
	if (Engine::Instance().m_bPBNull) CleanVector<Bullet*>(Engine::Instance().m_vPBullets, Engine::Instance().m_bPBNull);
	//if (Engine::Instance().m_bENull) CleanVector<Enemy*>(Engine::Instance().m_vEnemies, Engine::Instance().m_bENull);
	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel2)
		Engine::Instance().CheckCollision(2);
}

void GameState1::Render()
{
	//cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Render stuff.
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_bg1(), NULL, NULL);
	// Player.
	if (!Engine::Instance().m_playerDie)
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePR(), Engine::Instance().m_player->GetSrcP(), Engine::Instance().m_player->GetDstP(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);
	else
	{
		if (Engine::Instance().m_explosion->m_isAnimate) {
			SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureExp(), Engine::Instance().m_explosion->GetSrcP(), Engine::Instance().m_explosion->GetDstP());
			Engine::Instance().m_explosion->Animate1();

		}
	}
	// Player bullets.	
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		//SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePB(), Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP());
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePB(), Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP(), Engine::Instance().m_vPBullets[i]->GetAngle(), nullptr, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vPBullets[i]->GetDstP());*/
	}
	// Enemies.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().m_vEnemies[i]->GetSrcP(), Engine::Instance().m_vEnemies[i]->GetDstP(), Engine::Instance().m_vEnemies[i]->GetAngle(), nullptr, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vEnemies[i]->GetDstP());*/
	}

	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE1());
	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE2());
	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE3());




		//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);

		// If GameState != current state.
	if (dynamic_cast<GameState1*>(Engine::Instance().GetFSM().GetStates().back()))
		State::Render();
}

void GameState1::Exit()
{
	cout << "Exiting Game..." << endl;
	delete Engine::Instance().m_player;
	Engine::Instance().m_player = nullptr;
	Engine::Instance().m_vPBullets.clear();
	Engine::Instance().m_vEnemies.clear();
		//SDL_DestroyTexture(Engine::Instance().getTexture_bg1());
	//Mix_FreeMusic(Engine::Instance().m_mBgMusic);
}

void GameState1::Resume() { cout << "Resuming Game..." << endl; }
// End GameState1.



// Begin GameState2.
GameState2::GameState2() {}

void GameState2::Enter()
{
	cout << "Entering Game2..." << endl;
	//{ width / 2 - m_pSrc.w / 2 - 300, height / 2 - m_pSrc.h / 2 + 50, m_pSrc.w, m_pSrc.h };
	Engine::Instance().m_player = new Player({ 0,0,61,46 }, { WIDTH / 2 - 61 / 2 - 250, HEIGHT / 2 - 46 / 2 + 50,61,46 });
	//introduce enemy
	Engine::Instance().m_vEnemies.clear();
	Engine::Instance().m_vEnemies.reserve(6);
	//fix enemy
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 277,140,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 667, 400,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 350, 600,40,38 }));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 567, 500,40,38 }));
	//moving enemy
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 40, 115,40,38 }, 1));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 770, 60,40,38 }, 2));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 340, 115,40,38 }, 1));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 770, 260,40,38 }, 2));
	Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,0,40,38 }, { 570, 260,40,38 }, 2));
	Engine::Instance().m_explosion = new Explosion({ 0,0,96,96 }, { 0,0,96,96 });
	//Mix_PlayMusic(Engine::Instance().m_mBgMusic, -1);
}

void GameState2::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P) == 1)
		Engine::Instance().GetFSM().PushState(new PauseState());
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_X) == 1)
		Engine::Instance().GetFSM().ChangeState(new TitleState());
	else if (Engine::Instance().m_playerDie)
		Engine::Instance().GetFSM().ChangeState(new LoseState());
	else if (Engine::Instance().m_playerWinLevel3)
		Engine::Instance().GetFSM().ChangeState(new WinState());
	

	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel3)
	{

		if ((Engine::Instance().KeyDown(SDL_SCANCODE_W)) && Engine::Instance().m_player->GetDstP()->y > Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(270);
			Engine::Instance().m_player->GetDstP()->y -= Engine::Instance().getSpeed();
		}
		if ((Engine::Instance().KeyDown(SDL_SCANCODE_S)) && Engine::Instance().m_player->GetDstP()->y < HEIGHT - Engine::Instance().m_player->GetDstP()->h - Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(90);
			Engine::Instance().m_player->GetDstP()->y += Engine::Instance().getSpeed();
		}

		if ((Engine::Instance().KeyDown(SDL_SCANCODE_A)) && Engine::Instance().m_player->GetDstP()->x > Engine::Instance().getSpeed())
		{
			Engine::Instance().setAngle(180);
			Engine::Instance().m_player->GetDstP()->x -= Engine::Instance().getSpeed();
		}
		if ((Engine::Instance().KeyDown(SDL_SCANCODE_D)) && Engine::Instance().m_player->GetDstP()->x < WIDTH - Engine::Instance().m_player->GetDstP()->w)
		{
			Engine::Instance().setAngle(0);
			Engine::Instance().m_player->GetDstP()->x += Engine::Instance().getSpeed();
		}
		//---------------------------------------------------------------------------------------SHOOTING----------------------------------------------------------------------------------//
		if (Engine::Instance().KeyDown(SDL_SCANCODE_RIGHT) && (Engine::Instance().m_bCanShoot == 6))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(0);
			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y  ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_LEFT) && (Engine::Instance().m_bCanShoot == 4))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(180);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_UP) && (Engine::Instance().m_bCanShoot == 8))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(270);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_DOWN) && (Engine::Instance().m_bCanShoot == 2))
		{
			Engine::Instance().m_bCanShoot = 0;
			Engine::Instance().setAngle(90);

			Engine::Instance().m_vPBullets.push_back(new Bullet({ 0,0,40,23 }, { Engine::Instance().m_player->GetDstP()->x ,Engine::Instance().m_player->GetDstP()->y ,40,23 }, 30));
			Mix_PlayChannel(-1, Engine::Instance().m_mPlayerBullet, 0);
		}



	}

	// Update the bullets. Player's first.
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		Engine::Instance().m_vPBullets[i]->Update();
		if (Engine::Instance().m_vPBullets[i]->GetDstP()->x > WIDTH || Engine::Instance().m_vPBullets[i]->GetDstP()->y > HEIGHT
			|| Engine::Instance().m_vPBullets[i]->GetDstP()->x < 0 || Engine::Instance().m_vPBullets[i]->GetDstP()->y < 0)
		{
			delete Engine::Instance().m_vPBullets[i];
			Engine::Instance().m_vPBullets[i] = nullptr;
			Engine::Instance().m_bPBNull = true;
		}
	}
	// Update the enemy.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		Engine::Instance().m_vEnemies[i]->Update();
		if (Engine::Instance().m_vEnemies[i]->GetDstP()->x > WIDTH)
		{
			Engine::Instance().m_vEnemies[i]->GetDstP()->x = 0;
		}
		if (Engine::Instance().m_vEnemies[i]->GetDstP()->y > HEIGHT)
		{
			Engine::Instance().m_vEnemies[i]->GetDstP()->y = 0;
		}
	}
	if (Engine::Instance().m_bPBNull) CleanVector<Bullet*>(Engine::Instance().m_vPBullets, Engine::Instance().m_bPBNull);
	//if (Engine::Instance().m_bENull) CleanVector<Enemy*>(Engine::Instance().m_vEnemies, Engine::Instance().m_bENull);
	if (!Engine::Instance().m_playerDie && !Engine::Instance().m_playerWinLevel3)
		Engine::Instance().CheckCollision(3);
}

void GameState2::Render()
{
	//cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Render stuff.
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_bg3(), NULL, NULL);
	// Player.
	if (!Engine::Instance().m_playerDie)
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePR(), Engine::Instance().m_player->GetSrcP(), Engine::Instance().m_player->GetDstP(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);
	else
	{
		if (Engine::Instance().m_explosion->m_isAnimate) {
			SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureExp(), Engine::Instance().m_explosion->GetSrcP(), Engine::Instance().m_explosion->GetDstP());
			Engine::Instance().m_explosion->Animate1();

		}
	}
	// Player bullets.	
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		//SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePB(), Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP());
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePB(), Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP(), Engine::Instance().m_vPBullets[i]->GetAngle(), nullptr, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vPBullets[i]->GetDstP());*/
	}
	// Enemies.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().m_vEnemies[i]->GetSrcP(), Engine::Instance().m_vEnemies[i]->GetDstP(), Engine::Instance().m_vEnemies[i]->GetAngle(), nullptr, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vEnemies[i]->GetDstP());*/
	}

	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE1());
	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE2());
	//	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE3());




		//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);

		// If GameState != current state.
	if (dynamic_cast<GameState2*>(Engine::Instance().GetFSM().GetStates().back()))
		State::Render();
}

void GameState2::Exit()
{
	cout << "Exiting Game..." << endl;
	delete Engine::Instance().m_player;
	Engine::Instance().m_player = nullptr;
	Engine::Instance().m_vPBullets.clear();
	Engine::Instance().m_vEnemies.clear(); 
	//	SDL_DestroyTexture(Engine::Instance().getTexture_bg2());
	//Mix_FreeMusic(Engine::Instance().m_mBgMusic);
}

void GameState2::Resume() { cout << "Resuming Game..." << endl; }
// End GameState2.



// Begin TitleState.
TitleState::TitleState() {

}

void TitleState::Enter()
{
	cout << "Entering Title..." << endl;
	//added background music
	Mix_PlayMusic(Engine::Instance().m_mBgMusicTitle, -1);

	//reset player,enemy for after lose
	Engine::Instance().m_playerDie = false;
	//reset score in title state
	Engine::Instance().m_playerWinFinal = false;
	Engine::Instance().m_playerWinLevel1 = false;
	Engine::Instance().m_playerWinLevel2 = false;
	Engine::Instance().m_playerWinLevel3 = false;
	/*Engine::Instance().m_iESpawn = 0;
	Engine::Instance().m_iESpawnMax = 60;*/
	Engine::Instance().texW = 300;
	Engine::Instance().texH = 50;
	Engine::Instance().m_fsrcrect = { 0,0,Engine::Instance().texW,Engine::Instance().texH };
	Engine::Instance().m_fdstrect = { 200, 10, Engine::Instance().texW, Engine::Instance().texH };
	m_vButtons.push_back(new PlayButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,300,BUTTON_W,BUTTON_H }, "Start Game", 100, 90));
	// For the bind: what function, what instance, any parameters.
	m_vButtons.push_back(new ExitButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,450,BUTTON_W,BUTTON_H }, "Exit", 60, 30));
}

void TitleState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void TitleState::Render()
{
	//cout << "Rendering Title..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	string title1 = "WELCOME TO";
	string title2 = "ALONE";
	SDL_Surface* titleSurf1 = TTF_RenderText_Solid(Engine::Instance().getFont(), title1.c_str(), Engine::Instance().getColor());
	SDL_Surface* titleSurf2 = TTF_RenderText_Solid(Engine::Instance().getFont(), title2.c_str(), Engine::Instance().getColor());
	SDL_Texture* titleText1 = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), titleSurf1);
	SDL_Texture* titleText2 = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), titleSurf2);
	Engine::Instance().setFontTexture1(titleText1);
	Engine::Instance().setFontTexture2(titleText2);
	SDL_Rect titleRect1 = { WIDTH / 2 - (titleSurf1->w * 1.5f), 30, titleSurf1->w * 3, titleSurf1->h * 3 };
	SDL_Rect titleRect2 = { WIDTH / 2 - (titleSurf2->w * 1.5f), 130, titleSurf2->w * 3, titleSurf2->h * 3 };
	SDL_RenderCopy(Engine::Instance().GetRenderer(), titleText1, 0, &titleRect1);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), titleText2, 0, &titleRect2);
	SDL_FreeSurface(titleSurf1);
	SDL_FreeSurface(titleSurf2);
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting Title..." << endl;
	//Mix_FreeMusic(Engine::Instance().m_mBgMusicTitle);
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
	
}
// End TitleState.
// Begin WinState.
WinState::WinState() {}

void WinState::Enter()
{
	cout << "Entering Win..." << endl;
	m_vButtons.push_back(new MainMenuButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,300,BUTTON_W,BUTTON_H }, "Main Menu", 100, 30));
	// For the bind: what function, what instance, any parameters.
	m_vButtons.push_back(new ExitButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,400,BUTTON_W,BUTTON_H }, "Exit", 60, 30));
}

void WinState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void WinState::Render()
{
	cout << "Rendering Win..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());


	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	Engine::Instance().m_fdstrect.x = WIDTH / 2 - 200;
	Engine::Instance().m_fdstrect.y = 100;
	Engine::Instance().m_fdstrect.w = 400;
	Engine::Instance().m_fdstrect.h = 70;
	//Engine::Instance().PrintMessage("   Y O U     W O N  !!", true);
	string msg = "   Y O U     W O N  !!";
	SDL_Color color = { 255, 255, 255 };
	SDL_Color colorbg = { 0, 0, 0 };
	TTF_SetFontOutline(Engine::Instance().m_font, 1);
	Engine::Instance().m_surface = TTF_RenderText_Shaded(Engine::Instance().m_font, msg.c_str(), color, colorbg);

	Engine::Instance().m_ftexture = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), Engine::Instance().m_surface);
	SDL_QueryTexture(Engine::Instance().m_ftexture, NULL, NULL, &Engine::Instance().texW, &Engine::Instance().texH);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().m_ftexture, &Engine::Instance().m_fsrcrect, &Engine::Instance().m_fdstrect);
	State::Render();
}

void WinState::Exit()
{
	cout << "Exiting Win..." << endl;
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}
// End WinState.

// Begin LosState.
LoseState::LoseState() {}

void LoseState::Enter()
{
	cout << "Entering Lose..." << endl;
	m_vButtons.push_back(new MainMenuButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,300, BUTTON_W,BUTTON_H }, "Main Menu", 100, 30));
	// For the bind: what function, what instance, any parameters.
	m_vButtons.push_back(new ExitButton("btn.png", { 0,0,BUTTON_W,BUTTON_H }, { 360,400,BUTTON_W,BUTTON_H }, "Exit", 60, 30));
}

void LoseState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void LoseState::Render()
{
	//cout << "Rendering Los..." << endl;	

	//cout << "Rendering Title..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0 , 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());


	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	Engine::Instance().m_fdstrect.x = WIDTH / 2 - 200;
	Engine::Instance().m_fdstrect.y = 100;
	Engine::Instance().m_fdstrect.w = 400;
	Engine::Instance().m_fdstrect.h = 70;
	//Engine::Instance().PrintMessage("   G A M E     O V E R    !!", true);
	string msg = "   G A M E     O V E R    !!";
	SDL_Color color = { 255, 255, 255 };
	SDL_Color colorbg = { 0, 0, 0 };
	TTF_SetFontOutline(Engine::Instance().m_font, 1);
	Engine::Instance().m_surface = TTF_RenderText_Shaded(Engine::Instance().m_font, msg.c_str(), color, colorbg);
	
	Engine::Instance().m_ftexture = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), Engine::Instance().m_surface);
	SDL_QueryTexture(Engine::Instance().m_ftexture, NULL, NULL, &Engine::Instance().texW, &Engine::Instance().texH);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().m_ftexture, &Engine::Instance().m_fsrcrect, &Engine::Instance().m_fdstrect);
	State::Render();

}

void LoseState::Exit()
{
	cout << "Exiting Lose..." << endl;
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}
// End LoseState.


// Begin StateMachine.
void FSM::Update()
{
	if (!m_vStates.empty()) // empty() and back() are methods of the vector type.
		m_vStates.back()->Update();
}

void FSM::Render()
{
	if (!m_vStates.empty())
		m_vStates.back()->Render();
}

void FSM::ChangeState(State* pState)
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();	// De-allocating the state in the heap.
		m_vStates.back() = nullptr; // Nullifying pointer to the de-allocated state.
		m_vStates.pop_back();		// Removes the now-null pointer from the vector.
	}
	PushState(pState); // Invokes method below.
}

void FSM::PushState(State* pState)
{
	m_vStates.push_back(pState);
	m_vStates.back()->Enter();
}

void FSM::PopState()
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
	m_vStates.back()->Resume();
}


void FSM::Clean()
{
	while (!m_vStates.empty()) // Because we can exit the game in the pause state with the window's 'X'.
	{						   // Ensures that ALL states left in the vector are cleaned up.
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}

vector<State*>& FSM::GetStates() { return m_vStates; }
// End StateMachine.