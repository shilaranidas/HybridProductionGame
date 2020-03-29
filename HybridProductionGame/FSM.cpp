#include <functional>
#include <iostream>
#include "FSM.h"
#include "Engine.h"
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
	m_vButtons.push_back(new Button("resume.png", { 0,0,200,100 }, { 412,200,200,100 },
		std::bind(&FSM::PopState, &Engine::Instance().GetFSM())));
	m_vButtons.push_back(new Button("exit.png", { 0,0,200,100 }, { 412,400,200,100 },
		std::bind(&Engine::QuitGame, &Engine::Instance())));
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
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 71, 57, 74, 250);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
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

	Mix_PlayMusic(Engine::Instance().m_mBgMusic, -1);
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P) == 1)
		Engine::Instance().GetFSM().PushState(new PauseState());
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_X) == 1)
		Engine::Instance().GetFSM().ChangeState(new TitleState());
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_W) || Engine::Instance().KeyDown(SDL_SCANCODE_UP)) && Engine::Instance().getDst()->y > Engine::Instance().getSpeed()

		)
	{

		Engine::Instance().setAngle(270);
		if (Engine::Instance().getDst()->y > 248)
			Engine::Instance().getDst()->y -= Engine::Instance().getSpeed();

		cout << Engine::Instance().getDst()->y << endl;
		//here 248 is the building wall in top of background




	}
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_S) || Engine::Instance().KeyDown(SDL_SCANCODE_DOWN)) && Engine::Instance().getDst()->y < HEIGHT - Engine::Instance().getDst()->h - Engine::Instance().getSpeed())
	{

		Engine::Instance().setAngle(90);
		Engine::Instance().getDst()->y += Engine::Instance().getSpeed();
		//cout << g_dst.y << " ";
	}

	if ((Engine::Instance().KeyDown(SDL_SCANCODE_A) || Engine::Instance().KeyDown(SDL_SCANCODE_LEFT)) && Engine::Instance().getDst()->x > Engine::Instance().getSpeed())
	{
		Engine::Instance().setAngle(180);
		Engine::Instance().getDst()->x -= Engine::Instance().getSpeed();
	}


	//if (keyDown(SDL_SCANCODE_D) && g_dst.x< WIDTH-g_dst.w- g_iSpeed)
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_D) || Engine::Instance().KeyDown(SDL_SCANCODE_RIGHT)) && Engine::Instance().getDst()->x < WIDTH - Engine::Instance().getDst()->w)
	{

		Engine::Instance().setAngle(0);

		Engine::Instance().getDst()->x += Engine::Instance().getSpeed();
	}
}

void GameState::Render()
{
	//cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Render stuff.
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_bg(), NULL, NULL);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePR(), Engine::Instance().getSrc(), Engine::Instance().getDst(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);


	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE1(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE2(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTextureE1(), Engine::Instance().getSrcE1(), Engine::Instance().getDstE3(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);




	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);

	// If GameState != current state.
	if (dynamic_cast<GameState*>(Engine::Instance().GetFSM().GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	cout << "Exiting Game..." << endl;
	Mix_FreeMusic(Engine::Instance().m_mBgMusic);
}

void GameState::Resume() { cout << "Resuming Game..." << endl; }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {

}

void TitleState::Enter()
{
	cout << "Entering Title..." << endl;
	//added background music
	Mix_PlayMusic(Engine::Instance().m_mBgMusicTitle, -1);

	m_vButtons.push_back(new Button("button.png", { 0,0,200,100 }, { (WIDTH / 2) - 100,250,200,100 },
		std::bind(&FSM::ChangeState, &Engine::Instance().GetFSM(), new GameState())));
	// For the bind: what function, what instance, any parameters.
	m_vButtons.push_back(new Button("exit.png", { 0,0,200,100 }, { (WIDTH / 2) - 100,400,200,100 },
		std::bind(&Engine::QuitGame, &Engine::Instance())));
}

void TitleState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void TitleState::Render()
{
	//cout << "Rendering Title..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 71, 57, 74, 255);
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
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
	Mix_FreeMusic(Engine::Instance().m_mBgMusicTitle);
}
// End TitleState.

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