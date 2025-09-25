#include "Game.h"
#include "Input.h"
#include "Bomb.h"
#include "Explosion.h"

Game* Game::game_instance = nullptr;
const float tile_size = 64.0f;

Game::Game(int argc, char** argv,
	const char* title,
	const int WIDTH, const int HEIGHT,
	const int POSITION_X, const int POSITION_Y) :
	width(WIDTH), height(HEIGHT)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(POSITION_X, POSITION_Y);
	glutCreateWindow(title);

	init_glut();
	init_game();

	game_instance = this;
	callback_functions();

	glutMainLoop();
}

Game::~Game() {
	cleanup();
}

bool Game::load_textures()
{
	for (auto& m : mapV) delete m;
	mapV.clear();
	for (auto& t : tiles) delete t;
	tiles.clear();
	for (auto& o : objects) delete o;
	objects.clear();

	Player* player1 = new Player(
		glm::vec2(64, 64),
		glm::vec2(0),
		new Sprite(
			"resources/charWhite.png",
			glm::vec2(tile_size),
			1,
			glm::vec2(4, 3)
		),
		height_map,
		&object_map
	);
	player1->get_sprite()->set_current_frame(0);
	players.push_back(player1);

	GameObject* map = new GameObject(
		glm::vec2(0, 0),
		glm::vec2(0),
		new Sprite(
			"resources/level.png",
			glm::vec2(width, height),
			1,
			glm::vec2(1)
		)
	);
	map->get_sprite()->set_current_frame(0);
	mapV.push_back(map);

	for (int row = 0; row < tile_map.size(); ++row) {
		for (int col = 0; col < tile_map[row].size(); ++col) {
			float origin_x = (width - tile_map[0].size() * tile_size) / 2.0f;
			float origin_y = (height - tile_map.size() * tile_size) / 2.0f;
			int flipped_row = tile_map.size() - 1 - row;

			glm::vec2 pos(
				col * tile_size + origin_x,
				flipped_row * tile_size + origin_y
			);

			int wall_type = tile_map[row][col];

			if (wall_type != 0) {
				int frame = (wall_type == 1) ? 1 : 0;
				GameObject* tile = new GameObject(
					pos,
					glm::vec2(0),
					new Sprite(
						"resources/walls.png",
						glm::vec2(tile_size),
						1,
						glm::vec2(2, 1)
					)
				);
				tile->get_sprite()->set_current_frame(frame);
				tiles.push_back(tile);
			}
		}
	}

	return true;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& m : mapV) {
		m->render();
	}

	for (auto& t : tiles) {
		t->render();
	}

	for (auto& o : objects) {
		o->render();
	}

	for (auto& p : players) {
		p->render();
	}

	glutSwapBuffers();
}

void Game::update(float delta_time) 
{
	for (auto& p : players) {
		p->update(delta_time);
	}
	for (auto& o : objects) {
		o->update(delta_time);
	}
}

void Game::init_game()
{
    Input::setCallbackFunctions();

    srand(static_cast<unsigned int>(time(nullptr)));
    tile_map = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    height_map = new int[195]{
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    };

    object_map = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

	int rows = int(tile_map.size());
	int cols = int(tile_map[0].size());
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			int flipped_row = rows - 1 - row;
			if (tile_map[row][col] > 0) {
				int index = flipped_row * cols + col;
				height_map[index] = 2;
			}
		}
	}
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			int flipped_row = rows - 1 - row;
			if (object_map[row][col] > 0) {
				int index = flipped_row * cols + col;
				height_map[index] = 2;
			}
		}
	}

    load_textures();
}

void Game::cleanup()
{
	for (auto& object : objects) {
		delete object;
	}
	for (auto& tile : tiles) {
		delete tile;
	}

	for (auto& map : mapV) {
		delete map;
	}
}

void Game::addBomb(int tile_x, int tile_y) {
	if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
		return;
	}

	int array_y = 12 - tile_y;

	if (object_map[array_y][tile_x] != 0) {
		return;
	}

	object_map[array_y][tile_x] = 1;

	const float tile_size = 64.0f;
	float origin_x = (width - 15 * tile_size) / 2.0f;
	float origin_y = (height - 13 * tile_size) / 2.0f;

	glm::vec2 bomb_pos(
		tile_x * tile_size + origin_x,
		tile_y * tile_size + origin_y
	);

	Bomb* new_bomb = new Bomb(
		bomb_pos,
		glm::vec2(0),
		new Sprite(
			"resources/bombBlack.png",
			glm::vec2(tile_size),
			1,
			glm::vec2(3, 1)
		),
		3.0f,
		&object_map,    
		tile_x,         
		array_y         
	);
	new_bomb->get_sprite()->set_current_frame(0);
	objects.push_back(new_bomb);

	std::cout << "Bomb created at world tile (" << tile_x << ", " << tile_y << ") array pos (" << tile_x << ", " << array_y << ")" << std::endl;
}

void Game::addExplosion(int tile_x, int tile_y)
{
	if (tile_x < 0 || tile_x >= 15 || tile_y < 0 || tile_y >= 13) {
		return;
	}

	int array_y = 12 - tile_y;

	if (object_map[array_y][tile_x] != 0) {
		return;
	}

	object_map[array_y][tile_x] = 1;

	const float tile_size = 64.0f;
	float origin_x = (width - 15 * tile_size) / 2.0f;
	float origin_y = (height - 13 * tile_size) / 2.0f;

	glm::vec2 bomb_pos(
		tile_x * tile_size + origin_x,
		tile_y * tile_size + origin_y
	);

	Explosion* new_explosion = new Explosion(
		bomb_pos,
		glm::vec2(0),
		new Sprite(
			"resources/bombBlack.png",
			glm::vec2(tile_size),
			1,
			glm::vec2(3, 1)
		),
		2.0f,
		&object_map,
		&tile_map
	);
	new_explosion->get_sprite()->set_current_frame(0);
	objects.push_back(new_explosion);

	std::cout << "Explosion created at world tile (" << tile_x << ", " << tile_y << ") array pos (" << tile_x << ", " << array_y << ")" << std::endl;
}

void Game::init_glut()
{
	glClearColor(0.5f, 0.2f, 0.8f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);
}

void Game::reshape(const GLsizei w, const GLsizei h)
{
	GLsizei width = w;
	GLsizei height = (h == 0) ? 1 : h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Game::game_loop()
{
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (float)(current_time - previous_time) / 1000.0f;
	previous_time = current_time;

	update(delta_time);
	render();

	glutPostRedisplay();
}

void Game::callback_functions()
{
	glutDisplayFunc([]() {
		if (game_instance) {
			game_instance->game_loop();
		}
		});

	glutReshapeFunc([](int w, int h) {
		if (game_instance) {
			game_instance->reshape(w, h);
		}
		});
}
