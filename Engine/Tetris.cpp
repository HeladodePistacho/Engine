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

		void MoveDown()
		{
			for (int i = 0; i < 4; i++)
			{
				blocks[i].y++;
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
				blocks[1].SetXY(offset + 5, 0);
				blocks[2].SetXY(offset + 5, 1);
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

	float update_time = 0.8f;
	float timer = 0.0f;

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
			for (int x = 0; x < scenery_width; x++)
			{
				if (x == 0 || x == (scenery_width - 1))
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


	void GameplayStart()
	{
		//init for random starts
		std::chrono::system_clock::time_point now_in_ms = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::system_clock::now());
		auto value = now_in_ms.time_since_epoch();
		int number = value.count();

		srand(number);
		current_piece = new TetrisPiece((TetrisPiece::TETRIS_PIECE_TYPE)(rand() % 7), 0);
	}

	void GameplayUpdate(float delta_time)
	{
		if (timer >= update_time)
		{
			current_piece->MoveDown();
			timer = 0.0f;
		}
		else timer += delta_time;

		DrawMap();
	}

	void CleanGameMemory() 
	{

	}

private:
	

};

int main()
{

	TetrisGame game(L"Tetris");

	if (game.CreateConsoleWindow(25, 25, 20, 20))
		game.StartGame();

	return 0;
}