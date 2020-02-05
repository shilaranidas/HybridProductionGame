#pragma once
#include <vector>
#include "Button.h"
using namespace std;

class State // This is the abstract base class for all specific states.
{
protected:
	State() {}
public:
	virtual void Enter() = 0;
	virtual void Update() = 0; 
	virtual void Render();     
	virtual void Exit() = 0;
	virtual void Resume(); 
};

class PauseState : public State
{
private:
	vector<Button*> m_vButtons;
public:
	PauseState();
	void Enter();
	void Update();
	void Render();
	void Exit(); 
};

class GameState : public State
{
public:
	GameState();
	void Enter();
	void Update();
	void Render();
	void Exit();
	void Resume();
};

class TitleState : public State
{
private:
	vector<Button*> m_vButtons;
public:
	TitleState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

class FSM
{
private:
	vector<State*> m_vStates;
public:
	void Update();
	void Render();
	void ChangeState(State* pState);
	void PushState(State* pState);
	void PopState();
	void Clean();
	vector<State*>& GetStates();
};