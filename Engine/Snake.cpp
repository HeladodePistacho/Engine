/*#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>*/
//#include <Windows.h>

//#include <chrono>

//---------------------------Variables-------------------------------------
//map
/*bool in_game = true;
bool win = false;
int screen_width = 20;
int screen_height = 20;

char* map = new char[screen_width * screen_height];
wchar_t* screen = new wchar_t[screen_width * screen_height];

//player
int player_dir = 0;		//0 -> up, 1 -> down, 2 -> left, 3 -> right
bool dir_asigned = false;
int player_pos_x = 0;
int player_pos_y = 0;

int player_tail_num = 0;
int* player_tail_pos_x = new int[screen_height * screen_width];
int* player_tail_pos_y = new int[screen_height * screen_width];

int player_points = 0;
int hight_score = 0;

//objective
int objective_pos_x = 7;
int objective_pos_y = 7;

//Time
int update_time = 11000;

//---------------------------Functions-------------------------------------
int FromArray2Dto1D(int x, int y, int height)
{
	return (x + (y * height));
}

int FromArray1Dto2DX(int pos, int y, int height)
{
	return (pos - (y * height));
}

int FromArray1Dto2DY(int pos, int height)
{
	return pos / height;
}

/*void Draw()
{
	//Draw game
	for (int y = 0; y < screen_height; y++)
	{
		for (int x = 0; x < screen_width; x++)
		{
			screen[y + (x * screen_height)] = map[x + (y * screen_height)];
			//printf("%c", map[x + (y * screen_height)]);
		}

		//printf("\n");
	}

	//Draw puntuation
	//printf("Points: %i", player_points);

	if (win)
		printf("\n---------------YOU WIN :D---------------");
}*/

/*void HandleInput()
{
	//If input detected
	if (_kbhit())
	{
		int key = _getch();

		if (dir_asigned == false)
		{
			if ((key == 119 || key == 72) && player_dir != 1)
			{
				player_dir = 0;
				dir_asigned = true;
			}

			if ((key == 115 || key == 80) && player_dir != 0)
			{
				player_dir = 1;
				dir_asigned = true;
			}

			if ((key == 97 || key == 75) && player_dir != 3)
			{
				player_dir = 2;
				dir_asigned = true;
			}

			if ((key == 100 || key == 77) && player_dir != 2)
			{
				player_dir = 3;
				dir_asigned = true;
			}
		}

		if (key == 32)
			in_game = false;
	}
}

void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void MoveRandom()
{	
	objective_pos_x = rand() % screen_width;
	objective_pos_y = rand() % (screen_height - 2);
	
	char pos_ok = map[FromArray2Dto1D(objective_pos_x, objective_pos_y, screen_height)];
	
	if (pos_ok == '@' || pos_ok == '#')
	{
		for(int iter = 0; iter < screen_height * screen_width; iter++)
			if (map[iter] == '.')
			{
				objective_pos_y = FromArray1Dto2DY(iter, screen_height);
				objective_pos_x = FromArray1Dto2DX(iter, objective_pos_y, screen_height);
			}
	}

}

void UpdateMap()
{
	//Draw all .
	for (int iter = 0; iter < (screen_width * (screen_height - 1)); iter++)
	{
		screen[iter] = '.';	
	}

	//Print objective
	screen[FromArray2Dto1D(objective_pos_x, objective_pos_y, screen_height)] = '$';

	//Print player
	screen[FromArray2Dto1D(player_pos_x, player_pos_y, screen_height)] = '@';

	for (int i = 0; i < player_tail_num; i++)
	{
		screen[FromArray2Dto1D(player_tail_pos_x[i], player_tail_pos_y[i], screen_height)] = '#';
	}

	//Print points
	char puntuation[20];
	sprintf_s(puntuation, "Points: %i", player_points);
	int lenght = strlen(puntuation);
	int point_iter = 0;

	for (int i = FromArray2Dto1D(0, screen_height - 2, screen_height); i < (screen_width * (screen_height - 1)); i++)
	{
		if (lenght <= point_iter)
			screen[i] = ' ';
		else screen[i] = puntuation[point_iter];

		point_iter++;
	}

	//Print high score
	char max_points[20];
	sprintf_s(max_points, "Hight Score: %i", hight_score);
	lenght = strlen(max_points);
	point_iter = 0;

	for (int i = FromArray2Dto1D(0, screen_height - 1, screen_height); i < (screen_width * screen_height); i++)
	{
		if (lenght <= point_iter)
			screen[i] = ' ';
		else screen[i] = max_points[point_iter];

		point_iter++;
	}

}

void ReStart()
{
	player_dir = 0;		
	player_pos_x = 5;
	player_pos_y = 5;

	if(player_points > hight_score)
		hight_score = player_points;

	player_points = 0;
	player_tail_num = 0;

	update_time = 11000;
}

void UpdatePlayer()
{
	int tmp_x = player_pos_x;
	int tmp_y = player_pos_y;

	if (player_dir == 0)
		player_pos_y--;

	if (player_dir == 1)
		player_pos_y++;

	if (player_dir == 2)
		player_pos_x--;

	if (player_dir == 3)
		player_pos_x++;

	dir_asigned = false;

	//Not let player get out of the map
	if (player_pos_y < 0)
		player_pos_y = screen_height - 3;

	if (player_pos_y >= screen_height - 2)
		player_pos_y = 0;

	if (player_pos_x < 0)
		player_pos_x = screen_width - 1;

	if (player_pos_x >= screen_width)
		player_pos_x = 0;

	//Check if player has eaten the objective
	if (player_pos_x == objective_pos_x && player_pos_y == objective_pos_y)
	{
		player_points += 100;
		player_tail_num++;

		if (update_time >= 3500)
			update_time -= 1500;

		MoveRandom();
	}

	//Move Tail
	for (int i = player_tail_num; i > 0; i--)
	{
		player_tail_pos_x[i] = player_tail_pos_x[i - 1];
		player_tail_pos_y[i] = player_tail_pos_y[i - 1];

		//Check if player has eaten himself
		if (player_tail_pos_x[i] == player_pos_x && player_tail_pos_y[i] == player_pos_y)
			ReStart();		
		
	}

	player_tail_pos_x[0] = tmp_x;
	player_tail_pos_y[0] = tmp_y;

	if (player_tail_num == (screen_height * screen_width) - 1)
	{
		ReStart();
		win = true;
	}

	//Update Map
	UpdateMap();

}

void StartConsole()
{
	//Create Screen buffer
	wchar_t* screen = new wchar_t[screen_width * screen_height];
	HANDLE console_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console_handle);
	DWORD bytes_written = 0;
}

int main()
{
	//wchar_t* screen = new wchar_t[screen_width * screen_height];
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);//CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SMALL_RECT console_size;
	console_size.Top = (short)0;
	console_size.Left = (short)0;
	console_size.Bottom = (short)screen_height - 1;
	console_size.Right = (short)screen_width - 1;

	if (SetConsoleWindowInfo(console_handle, true, &console_size) == 0)
	{
		DWORD paco = GetLastError();

		if (paco == 0)
			return 1;
	}

	COORD size;
	size.X = screen_width;
	size.Y = screen_height;

	if (SetConsoleScreenBufferSize(console_handle, size) == 0)
	{
		DWORD paco = GetLastError();

		if (paco == 0)
			return 1;
	}

	// Set the font size now that the screen buffer has been assigned to the console
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 15;
	cfi.dwFontSize.Y = 15;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	if (!SetCurrentConsoleFontEx(console_handle, false, &cfi))
		return 0;

	//SetConsoleActiveScreenBuffer(console_handle);
	DWORD bytes_written = 0;

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(console_handle, &info);

	wchar_t snake[10];
	swprintf_s(snake, 10, L"Snake");
	SetConsoleTitle(snake);

	

	//HideCursor();
	UpdateMap();

	//Time
	int player_timer = 0;

	while (in_game)
	{	
		HandleInput();

		if (player_timer >= update_time)
		{
			UpdatePlayer();
			player_timer = 0;
		}
		player_timer++;

		//Draw();

		//UpdateMap();

		//screen[screen_width * screen_height - 1].Char.UnicodeChar = '\0';
		WriteConsoleOutputCharacter(console_handle, screen, screen_width * screen_height, { 0,0 }, &bytes_written);
		//WriteConsoleOutput(console_handle, screen, { (short)screen_width * (short)screen_height }, { 0,0 }, &console_size);



		//system("cls");	
	}
	
	delete[] screen;
	delete[] map;
	delete[] player_tail_pos_x;
	delete[] player_tail_pos_y;

	return 0;
}*/

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
			if (GetKeyState(KEY::KEY_W) == KEY_STATE::KEY_DOWN && player_dir != DOWN)
			{
				player_dir = UP;
				dir_asigned = true;
			}

			if (GetKeyState(KEY::KEY_S) == KEY_STATE::KEY_DOWN && player_dir != UP)
			{
				player_dir = DOWN;
				dir_asigned = true;
			}

			if (GetKeyState(KEY::KEY_A) == KEY_STATE::KEY_DOWN && player_dir != RIGHT)
			{
				player_dir = LEFT;
				dir_asigned = true;
			}

			if (GetKeyState(KEY::KEY_D) == KEY_STATE::KEY_DOWN && player_dir != LEFT)
			{
				player_dir = RIGHT;
				dir_asigned = true;
			}
		}
		if (GetKeyState(KEY::KEY_ESC) == KEY_STATE::KEY_DOWN)
			CloseGame();
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

		if (pos_ok == '@' || pos_ok == '#')
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

	if(game.CreateConsoleWindow(15, 15, 20, 20))
		game.StartGame();

	return 0;
}