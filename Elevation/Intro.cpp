#include "Intro.h"

Intro::Intro(int argc, char** argv,
	const char* title,
	const int WIDTH, const int HEIGHT,
	const int POSITION_X, const int POSITION_Y, const int WINDOW_WIDTH, const int WINDOW_HEIGHT)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(POSITION_X, POSITION_Y);
	glutCreateWindow(title);

	init_glut();
	init_game();

	callback_functions();

	glutMainLoop();

	_introText = new TextRenderer(glm::vec2(30, 700),
		new Sprite("Resources/font.png", glm::vec2(64), 1, glm::vec2(15, 8), true), 54, 32);
	_introText->setText("BOMBERMAN_____BY__BOGDAN_CVETKOVIC_5934");
}

Intro::~Intro()
{
}

void Intro::update(float dt)
{
	duration -= dt;
	if (duration <= 0) {
		
	}
}

void Intro::init_game()
{
	load_textures();
}

void Intro::init_glut()
{
	glClearColor(0.5f, 0.2f, 0.8f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);
}

void Intro::reshape(const GLsizei w, const GLsizei h)
{
	GLsizei width = w;
	GLsizei height = (h == 0) ? 1 : h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Intro::game_loop()
{
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (float)(current_time - previous_time) / 1000.0f;
	previous_time = current_time;

	update(delta_time);
	render();

	glutPostRedisplay();
}

void Intro::callback_functions()
{
	glutDisplayFunc([]() {
		if (intro_instance) {
			intro_instance->game_loop();
		}
		});

	glutReshapeFunc([](int w, int h) {
		if (intro_instance) {
			intro_instance->reshape(w, h);
		}
		});
}

void Intro::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	_introText->render();

	glutSwapBuffers();
}