#include "Explosion.h"
#include "Game.h"

Explosion::Explosion(const glm::vec2& pos, const glm::vec2& vel, Sprite* spr, float rad, std::vector<std::vector<int>>* _object_map)
{
	radius = rad;
	object_map = _object_map;
}

void Explosion::update(float dt)
{
	int tile_size = 64;
	int cols = 15;
	int rows = 13;

	int center_x = static_cast<int>(get_position().x / tile_size);
	int center_y = static_cast<int>(get_position().y / tile_size);

 	// Frame indices for explosion parts (adjust to your sprite sheet)
	const int frame_center = 6;
	const int frame_top = 0;
	const int frame_bottom = 1;
	const int frame_left = 2;
	const int frame_right = 3;
	const int frame_vertical = 4;
	const int frame_horizontal = 5;

	// Center
	if (center_x >= 0 && center_x < cols && center_y >= 0 && center_y < rows) {
		(*object_map)[center_y][center_x] = frame_center;
	}

	// Vertical (top and bottom)
	for (int i = 1; i <= static_cast<int>(radius); ++i) {
		int y_top = center_y - i;
		int y_bottom = center_y + i;

		// Top edge
		if (i == static_cast<int>(radius) && y_top >= 0 && center_x >= 0 && center_x < cols) {
			(*object_map)[y_top][center_x] = frame_top;
		}
		// Top between
		else if (y_top >= 0 && center_x >= 0 && center_x < cols) {
			(*object_map)[y_top][center_x] = frame_vertical;
		}

		// Bottom edge
		if (i == static_cast<int>(radius) && y_bottom < rows && center_x >= 0 && center_x < cols) {
			(*object_map)[y_bottom][center_x] = frame_bottom;
		}
		// Bottom between
		else if (y_bottom < rows && center_x >= 0 && center_x < cols) {
			(*object_map)[y_bottom][center_x] = frame_vertical;
		}
	}

	// Horizontal (left and right)
	for (int i = 1; i <= static_cast<int>(radius); ++i) {
		int x_left = center_x - i;
		int x_right = center_x + i;

		// Left edge
		if (i == static_cast<int>(radius) && x_left >= 0 && center_y >= 0 && center_y < rows) {
			(*object_map)[center_y][x_left] = frame_left;
		}
		// Left between
		else if (x_left >= 0 && center_y >= 0 && center_y < rows) {
			(*object_map)[center_y][x_left] = frame_horizontal;
		}

		// Right edge
		if (i == static_cast<int>(radius) && x_right < cols && center_y >= 0 && center_y < rows) {
			(*object_map)[center_y][x_right] = frame_right;
		}
		// Right between
		else if (x_right < cols && center_y >= 0 && center_y < rows) {
			(*object_map)[center_y][x_right] = frame_horizontal;
		}
	}
}