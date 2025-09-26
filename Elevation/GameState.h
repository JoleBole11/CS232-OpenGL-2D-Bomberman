#pragma once

#include "Player.h"
#include "Button.h"

class GameState
{
protected:
	Player* _player1;
	Player* _player2;

	vector<Button*> _buttons;

	bool _paused = false;
public:
	GameState();
	GameState(Player* player, Player* player2);
	~GameState();
	virtual void onStateEnter() = 0;
	virtual void onStateUpdate() = 0;
	virtual void onStateExit() = 0;
	virtual void render() = 0;

	void renderPauseMenu();
	void togglePause();

	void onClick(int x, int y);
};
