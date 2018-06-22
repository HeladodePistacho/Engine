#include "ConsoleEngine.h"

class SnakeGame : public ConsoleEngine
{
public:
	SnakeGame(const wchar_t* game_name) : ConsoleEngine(game_name) {}

private:

	class Point
	{
	public:
		int x, y;

		Point() { x = 0; y = 0; }
		Point(int _x, int _y) : x(_x), y(_y) {}
		Point(const Point& new_point) : x(new_point.x), y(new_point.y) {}

		bool operator==(Point other_point)
		{
			return (other_point.x == x && other_point.y == y);
		}
	};

	enum PLAYER_DIR
	{
		UP, DOWN, LEFT, RIGHT
	};

	PLAYER_DIR player_dir = UP;		//0 -> up, 1 -> down, 2 -> left, 3 -> right
	Point player_pos;
	bool dir_asigned = false;

	int player_tail_num = 0;
	Point* player_tail_pos = new Point[GetScreenWidth() * GetScreenHeight()];

	int player_points = 0;
	int hight_score = 0;

	//objective
	Point objective_pos;
	bool win = false;

	//timer
	float update_timer = 0.5f;
	float player_timer = 0.0f;

	void HandleInput()
	{
		if (dir_asigned == false)
		{
			if ((GetKeyState(KEY::KEY_W) == KEY_STATE::KEY_DOWN || GetKeyState(KEY::KEY_ARROW_UP) == KEY_STATE::KEY_DOWN) && player_dir != DOWN)
			{
				player_dir = UP;
				dir_asigned = true;
			}

			if ((GetKeyState(KEY::KEY_S) == KEY_STATE::KEY_DOWN || GetKeyState(KEY::KEY_ARROW_DOWN) == KEY_STATE::KEY_DOWN) && player_dir != UP)
			{
				player_dir = DOWN;
				dir_asigned = true;
			}

			if ((GetKeyState(KEY::KEY_A) == KEY_STATE::KEY_DOWN || GetKeyState(KEY::KEY_ARROW_LEFT) == KEY_STATE::KEY_DOWN) && player_dir != RIGHT)
			{
				player_dir = LEFT;
				dir_asigned = true;
			}

			if ((GetKeyState(KEY::KEY_D) == KEY_STATE::KEY_DOWN || GetKeyState(KEY::KEY_ARROW_RIGHT) == KEY_STATE::KEY_DOWN) && player_dir != LEFT)
			{
				player_dir = RIGHT;
				dir_asigned = true;
			}
		}

		if (GetKeyState(KEY::KEY_ESC) == KEY_STATE::KEY_DOWN)
			CloseGame();

		if (GetKeyState(KEY::KEY_SPACE) == KEY_STATE::KEY_DOWN)
		{
			if(IsGamePaused())
				SetPauseGame(false);
			else SetPauseGame(true);
		}
	}

	void UpdatePlayer()
	{
		Point tmp_point(player_pos);

		if (player_dir == UP)
			player_pos.y--;

		if (player_dir == DOWN)
			player_pos.y++;

		if (player_dir == LEFT)
			player_pos.x--;

		if (player_dir == RIGHT)
			player_pos.x++;

		dir_asigned = false;

		//Not let player get out of the map
		if (player_pos.y < 0)
			player_pos.y = GetScreenHeight() - 3;

		if (player_pos.y >= GetScreenHeight() - 2)
			player_pos.y = 0;

		if (player_pos.x < 0)
			player_pos.x = GetScreenWidth() - 1;

		if (player_pos.x >= GetScreenWidth())
			player_pos.x = 0;

		//Check if player has eaten the objective
		if (player_pos  == objective_pos)
		{
			player_points += 100;
			player_tail_num++;

			if (update_timer > 0.5f)
				update_timer -= 0.05f;

			if (update_timer > 0.2f)
			{
				update_timer -= 0.025f;
				Clamp(update_timer, 0.5f, 0.05f);
			}

			

			MoveRandom();
		}

		//Check if player has collided with wall
		if (player_pos.x == 0 || player_pos.x == (GetScreenWidth() - 1))
		{
			GameplayStart();
		}

		if (player_pos.y == 0 || player_pos.y == (GetScreenHeight() - 3))
		{
			GameplayStart();
		}

		//Move Tail
		for (int i = player_tail_num; i > 0; i--)
		{
			player_tail_pos[i].x = player_tail_pos[i - 1].x;
			player_tail_pos[i].y = player_tail_pos[i - 1].y;

			//Check if player has eaten himself
			if (player_tail_pos[i] == player_pos)
				GameplayStart();

		}

		player_tail_pos[0].x = tmp_point.x;
		player_tail_pos[0].y = tmp_point.y;

		if (player_tail_num == (GetScreenHeight() * GetScreenWidth()) - 1)
		{
			GameplayStart();
			win = true;
		}

		//Update Map
		DrawMap();
	}

	void MoveRandom()
	{
		objective_pos.x = rand() % (GetScreenWidth() - 2);
		objective_pos.y = rand() % (GetScreenHeight() - 4);

		Clamp(objective_pos.x, (GetScreenWidth() - 2), 1);
		Clamp(objective_pos.y, (GetScreenHeight() - 4), 1);

		char pos_ok = GetCharAtScreenBuffer(objective_pos.x, objective_pos.y);

		if (pos_ok != '.')
		{
			for (int iter = 0; iter < GetScreenHeight() * GetScreenWidth(); iter++)
				if (GetCharAtScreenBuffer(iter) == '.')
				{
					objective_pos.y = iter / GetScreenHeight();
					objective_pos.x = (iter - (objective_pos.y * GetScreenWidth()));
				}
		}
	}

	void GameplayStart()
	{
		//Set Player
		player_pos.x = GetScreenWidth() * 0.5f;
		player_pos.y = GetScreenHeight() * 0.5f;

		player_dir = UP;
		player_tail_num = 0;

		hight_score = player_points;
		player_points = 0;
		update_timer = 0.5f;

		//Set objective
		objective_pos.x = rand() % (int)((GetScreenWidth() * 0.5f) - 1);
		objective_pos.y = rand() % (int)((GetScreenHeight() * 0.5f) - 1);

		DrawMap();
	}

	void DrawMap()
	{
		//Draw all .
		for (int iter = 0; iter < (GetScreenWidth() * (GetScreenHeight() - 2)); iter++)
		{
			int y = iter / GetScreenWidth();
			int x = iter - (y * GetScreenWidth());

			if (x == 0 || x == (GetScreenWidth() - 1))
			{
				DrawChar(iter, '|', COLOR::FG_BLUE);
				continue;
			}

			if (y == 0 || y == (GetScreenHeight() - 3))
			{
				DrawChar(iter, '-', COLOR::FG_BLUE);
				continue;
			}
			else DrawChar(iter, '.', COLOR::FG_DARK_GREY);
		}

		//Print objective
		DrawChar(objective_pos.x, objective_pos.y, '$', COLOR::FG_RED);

		//Print player
		DrawChar(player_pos.x, player_pos.y, '@', COLOR::FG_YELLOW);

		for (int i = 0; i < player_tail_num; i++)
		{
			DrawChar(player_tail_pos[i].x, player_tail_pos[i].y, '#', COLOR::FG_GREEN);	
		}

		//Print points
		char puntuation[20];
		sprintf_s(puntuation, "Points: %i", player_points);
		int lenght = strlen(puntuation);
		int point_iter = 0;

		for (int i = (GetScreenWidth() * (GetScreenHeight() - 2)); i < (GetScreenWidth() * (GetScreenHeight() - 1)); i++)
		{
			if (lenght <= point_iter)
				DrawChar(i, ' ');
			else DrawChar(i, puntuation[point_iter], COLOR::FG_GREY);

			point_iter++;
		}

		//Print high score
		char max_points[20];
		sprintf_s(max_points, "Record: %i", hight_score);
		lenght = strlen(max_points);
		point_iter = 0;

		for (int i = (GetScreenWidth() * (GetScreenHeight() - 1)); i < (GetScreenWidth() * GetScreenHeight()); i++)
		{
			if (lenght <= point_iter)
				DrawChar(i, ' ');
			else DrawChar(i, max_points[point_iter], COLOR::FG_GREY);

			point_iter++;
		}
	}

	void GameplayUpdate(float delta_time)
	{
		HandleInput();

		if (IsGamePaused())
			return;

		if (player_timer >= update_timer)
		{
			UpdatePlayer();
			player_timer = 0.0f;
		}
		else player_timer += delta_time;

	}

	void CleanGameMemory()
	{
		delete[] player_tail_pos;
	}

	

};


int main()
{
	
	SnakeGame game(L"SNAKE");

	if(game.CreateConsoleWindow(20, 20, 17, 17))
		game.StartGame();

	return 0;
}