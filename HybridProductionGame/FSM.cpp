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
	m_vButtons.push_back(new Button("resume.png", { 0,0,200,80 }, { 412,200,200,80 },
		std::bind( &FSM::PopState, &Engine::Instance().GetFSM() )));
	m_vButtons.push_back(new Button("exit.png", { 0,0,400,100 }, { 412,400,200,80 },
		std::bind( &Engine::QuitGame, &Engine::Instance())) );
}

void PauseState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void PauseState::Render()
{
	cout << "Rendering Pause..." << endl;
	Engine::Instance().GetFSM().GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 128);
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
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P) == 1)
		Engine::Instance().GetFSM().PushState(new PauseState());
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_X) == 1)
		Engine::Instance().GetFSM().ChangeState(new TitleState());
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_W) || Engine::Instance().KeyDown(SDL_SCANCODE_UP)) && Engine::Instance().getDst()->y > Engine::Instance().getSpeed())
	{
		
		Engine::Instance().setAngle(270);
		
		Engine::Instance().getDst()->y -= Engine::Instance().getSpeed();

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
	if ((Engine::Instance().KeyDown(SDL_SCANCODE_D) || Engine::Instance().KeyDown(SDL_SCANCODE_RIGHT)) && Engine::Instance().getDst()->x < WIDTH / 2 - Engine::Instance().getDst()->w)
	{
		
		Engine::Instance().setAngle(0);

		Engine::Instance().getDst()->x += Engine::Instance().getSpeed();
	}
}

void GameState::Render()
{
	cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Render stuff.
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().getTexture_bg(), NULL, NULL);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().getTexturePR(), Engine::Instance().getSrc(), Engine::Instance().getDst(), Engine::Instance().getAngle(), nullptr, SDL_FLIP_NONE);

	

	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	
	// If GameState != current state.
	if (dynamic_cast<GameState*>(Engine::Instance().GetFSM().GetStates().back()))
		State::Render();
}

void GameState::Exit()
{ 
	cout << "Exiting Game..." << endl;
}

void GameState::Resume() { cout << "Resuming Game..." << endl; }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{ 
	cout << "Entering Title..." << endl;
	m_vButtons.push_back(new Button("button.png", { 0,0,400,100 }, { 312,100,400,100 },
		std::bind( &FSM::ChangeState, &Engine::Instance().GetFSM(), new GameState() )));
	// For the bind: what function, what instance, any parameters.
	m_vButtons.push_back(new Button("exit.png", { 0,0,400,100 }, { 312,300,400,100 },
		std::bind( &Engine::QuitGame, &Engine::Instance() )));
}

void TitleState::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
}

void TitleState::Render()
{
	cout << "Rendering Title..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
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