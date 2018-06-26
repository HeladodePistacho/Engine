#include "ConsoleEngine.h"

class TetrisGame : public ConsoleEngine
{
private:
	class TetrisPiece
	{
	public:
		enum TETRIS_PIECE_TYPE
		{
			STRAIGHT = 0,
			J,
			L,
			T,
			QUAD,
			S,
			Z
		};

		TETRIS_PIECE_TYPE type;
		COLOR piece_color;

	public:
		TetrisPiece(TETRIS_PIECE_TYPE _type, int offset) : type(_type)
		{
			blocks = new Point[4];
			piece_color = (COLOR)((int)_type + 9);
			InitPiece(_type, offset);
		}

		Point GetBlock(int pos)
		{
			if (pos >= 0 && pos < 4)
				return blocks[pos];
		}

		void SetBlock(int pos, int x, int y)
		{
			if (pos >= 0 && pos < 4)
				blocks[pos].SetXY(x, y);
		}

		void SetBlock(int pos, Point other_point)
		{
			if (pos >= 0 && pos < 4)
				blocks[pos].SetXY(other_point);
		}

		void MoveDown()
		{
			for (int i = 0; i < 4; i++)
			{
				blocks[i].y++;
			}
		}

		void MoveHorizontal(int dir)
		{
			for (int i = 0; i < 4; i++)
			{
				blocks[i].x += dir;
			}
		}//dir -1 left +1 right

		//0 upper piece, 1 lefter piece, 2 righter piece, 3 downer piece
		Point GetFurtherPiece(int pos)
		{
			Point ret = blocks[0];

			switch (pos)
			{
			case 0:
				for (int i = 0; i < 4; i++)
				{
					if (blocks[i].y < ret.y)
						ret = blocks[i];
				}

				break;

			case 1:

				for (int i = 0; i < 4; i++)
				{
					if (blocks[i].x < ret.x)
						ret = blocks[i];
				}

				break;

			case 2:

				for (int i = 0; i < 4; i++)
				{
					if (blocks[i].x > ret.x)
						ret = blocks[i];
				}

				break;

			case 3:

				for (int i = 0; i < 4; i++)
				{
					if (blocks[i].y > ret.y)
						ret = blocks[i];
				}

				break;
			}

			return ret;
		}

		void Rotate(Point* fill_rotated_blocks)
		{
			if (type != QUAD)
			{
				//We will rotate using blocks[2] as the center
				//2d  rotation matrix looks like	[cos(alpha) -sin(alpha)] 
				//									[sin(alpha)  cos(alpha)]
				//
				//Rotating counterclock	-> positive degrees
				//Rotating clock		-> negateive degrees
				//
				// rotated_x = xcos(alpha) - ysin(alpha) -> alpha = -90º -> rotated_x =  y
				// rotated_y = xsin(alpha) + ycos(alpha) ->				 -> rotated_y = -x

				//Point rotated_blocks[4];

				for (int i = 0; i < 4; i++)
				{
					//Get position from pivot
					Point distance_to_pivot = blocks[i] - blocks[2];

					//rotate 90º using previous formula
					distance_to_pivot.SetXY(distance_to_pivot.y, -distance_to_pivot.x);

					//Save the new positions
					fill_rotated_blocks[i].SetXY(blocks[2] - distance_to_pivot);
				}

			}
		}

	private:
		Point* blocks;

		void InitPiece(TETRIS_PIECE_TYPE _type, int offset)
		{
			switch (_type)
			{
			case STRAIGHT:

				blocks[0].SetXY(offset + 4, 0);
				blocks[1].SetXY(offset + 5, 0);
				blocks[2].SetXY(offset + 6, 0);
				blocks[3].SetXY(offset + 7, 0);


				break;

			case J:

				blocks[0].SetXY(offset + 4, 0);
				blocks[1].SetXY(offset + 4, 1);
				blocks[2].SetXY(offset + 5, 1);
				blocks[3].SetXY(offset + 6, 1);

				break;

			case L:

				blocks[0].SetXY(offset + 6, 0);
				blocks[1].SetXY(offset + 4, 1);
				blocks[2].SetXY(offset + 5, 1);
				blocks[3].SetXY(offset + 6, 1);

				break;

			case T:

				blocks[0].SetXY(offset + 4, 0);
				blocks[1].SetXY(offset + 5, 1);
				blocks[2].SetXY(offset + 5, 0);		
				blocks[3].SetXY(offset + 6, 0);

				break;

			case QUAD:

				blocks[0].SetXY(offset + 5, 0);
				blocks[1].SetXY(offset + 6, 0);
				blocks[2].SetXY(offset + 5, 1);
				blocks[3].SetXY(offset + 6, 1);

				break;

			case S:

				blocks[0].SetXY(offset + 5, 1);
				blocks[1].SetXY(offset + 6, 1);
				blocks[2].SetXY(offset + 6, 0);
				blocks[3].SetXY(offset + 7, 0);

				break;

			case Z:

				blocks[0].SetXY(offset + 5, 0);
				blocks[1].SetXY(offset + 6, 0);
				blocks[2].SetXY(offset + 6, 1);
				blocks[3].SetXY(offset + 7, 1);

				break;

			}
		}

	};

public:
	TetrisGame(const wchar_t* name) : ConsoleEngine(name) {}

private:

	int scenery_width = 12;
	int scenery_height = 22;

	int offset = 6;

	float update_time = 1.8f;
	float update_key_repeat = 0.15f;

	float timer = 0.0f;
	float key_repeat_timer = 0.0f;

	bool get_down = true;

	TetrisPiece* current_piece = nullptr;

	void DrawMap()
	{
		//Reset map
		for (int i = 0; i < GetScreenSize(); i++)
		{
			DrawPixel(i);
		}

		//Draw scenary
		for (int y = 0; y < scenery_height; y++)
		{
			for (int x = offset; x < (offset + scenery_width); x++)
			{
				if (x == offset || x == (offset + scenery_width - 1))
					DrawPixel(x, y, COLOR::FG_GREY, PIXEL_TYPE::PIXEL_HALF);

				if (y == (scenery_height - 1))
					DrawPixel(x, y, COLOR::FG_GREY, PIXEL_TYPE::PIXEL_HALF);
			}
		}

		//Draw falling piece
		for (int i = 0; i < 4; i++)
		{
			Point tmp = current_piece->GetBlock(i);
			DrawPixel(tmp.x, tmp.y, current_piece->piece_color);
		}

	}

	void HandleInput()
	{
		if (current_piece->GetFurtherPiece(1).x > (offset + 1) && GetKeyState(KEY::KEY_ARROW_LEFT) == KEY_STATE::KEY_REPEAT)
		{
			if (key_repeat_timer >= update_key_repeat)
			{
				current_piece->MoveHorizontal(-1);
				key_repeat_timer = 0.0f;
			}
		}

		if (current_piece->GetFurtherPiece(2).x < (offset + scenery_width - 2) && GetKeyState(KEY::KEY_ARROW_RIGHT) == KEY_STATE::KEY_REPEAT)
		{
			if (key_repeat_timer >= update_key_repeat)
			{
				current_piece->MoveHorizontal(1);
				key_repeat_timer = 0.0f;
			}
		}

		if (GetKeyState(KEY::KEY_SPACE) == KEY_STATE::KEY_DOWN)
		{
			Point rotated_blocks[4];
			current_piece->Rotate(rotated_blocks);


			//Check if the rotated blocks are valid
			bool valid = true;
			for (int i = 0; i < 4; i++)
			{

				int tmp_x = rotated_blocks[i].x;

				if (tmp_x < (offset + 1) || tmp_x > (offset + scenery_width - 2))
					valid = false;
			}

			//if valid rotate
			if (valid)
			{
				for (int i = 0; i < 4; i++)
					current_piece->SetBlock(i, (rotated_blocks + i)->x, (rotated_blocks + i)->y);
			}

		}
	}

	void GameplayStart()
	{
		//init for random starts
		std::chrono::system_clock::time_point now_in_ms = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::system_clock::now());
		auto value = now_in_ms.time_since_epoch();
		int number = value.count();

		srand(number);
		current_piece = new TetrisPiece((TetrisPiece::TETRIS_PIECE_TYPE)(6), offset);
	}

	void GameplayUpdate(float delta_time)
	{
		HandleInput();

		if (timer >= update_time)
		{
			current_piece->MoveDown();
			timer = 0.0f;
		}
		else timer += delta_time;

		key_repeat_timer += delta_time;

		DrawMap();
	}

	void CleanGameMemory() 
	{

	}	

};

int main()
{

	TetrisGame game(L"Tetris");

	if (game.CreateConsoleWindow(25, 25, 20, 20))
		game.StartGame();

	return 0;
}