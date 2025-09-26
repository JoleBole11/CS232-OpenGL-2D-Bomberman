#pragma once
#include "GameState.h"
#include "TextRenderer.h"

class Intro
{
private:
	float duration = 3;
	TextRenderer* _introText;
	int previous_time;
	float delta_time = 0.0f;

	bool intro_finished = false;

	bool load_textures();
	void update(float dt);
	void init_game();
	void cleanup();
	void reshape(const GLsizei width, const GLsizei height);
	void init_glut();
	void game_loop();
	void callback_functions();

public:
	static Intro* intro_instance;

	Intro(int argc, char** argv,
		const char* title,
		const int WIDTH, const int HEIGHT,
		const int POSITION_X, const int POSITION_Y, const int WINDOW_WIDTH, const int WINDOW_HEIGHT);
	~Intro();

	void render();
	bool get_intro_finished() { return intro_finished; }
};
