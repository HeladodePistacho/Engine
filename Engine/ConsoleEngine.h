#ifndef _CONSOLE_ENGINE_H_
#define _CONSOLE_ENGINE_H_

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <chrono>

enum COLOR
{
	//Intensity -> 8
	FG_BLACK = 0,
	FG_DARK_BLUE = 1,		//Main blue
	FG_DARK_GREEN = 2,		//Main green
	FG_DARK_CYAN = 3,		//Blue + Green
	FG_DARK_RED = 4,		//Main red
	FG_DARK_MAGENTA = 5,	//Blue + Red
	FG_DARK_YELLOW = 6,		//Green + Red
	FG_GREY = 7,			//Blue + Green + Red
	FG_DARK_GREY = 8,		//Black + Intensity
	FG_BLUE = 9,			//Blue + Intensity
	FG_GREEN = 10,			//Green + Intensity
	FG_CYAN = 11,			//Cyan + Intensity
	FG_RED = 12,			//Red + Intensity
	FG_MAGENTA = 13,		//Magenta + Intensity
	FG_YELLOW = 14,			//Yellow + Intensity
	FG_WHITE = 15			//Blue + Green + Red + Intensity
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

enum KEY
{
	//Numbers
	KEY_0 = 48,
	KEY_1 = 49,
	KEY_2 = 50,
	KEY_3 = 51,
	KEY_4 = 52,
	KEY_5 = 53,
	KEY_6 = 54,
	KEY_7 = 55,
	KEY_8 = 56,
	KEY_9 = 57,

	//Letters
	KEY_A = 65,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_Ñ = 192,
	KEY_O = 79,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	//Special
	KEY_ESC = 27,
	KEY_SPACE = 32,
	KEY_BACKWARDS = 8,

	KEY_ARROW_LEFT = 37,
	KEY_ARROW_UP,
	KEY_ARROW_RIGHT,
	KEY_ARROW_DOWN
};

enum KEY_STATE
{
	KEY_NONE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ConsoleEngine
{
private:
	HANDLE console_output_handle;
	HANDLE console_input_handle;

	int screen_width;
	int screen_height;
	CHAR_INFO* screen_buffer = nullptr;
	SMALL_RECT screen_window;

	bool in_game = true;

	//Keys
	KEY_STATE keyboard[256];

public:
	//---------------Console Settings---------------
	ConsoleEngine()
	{
		console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
		console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		screen_width = 50;
		screen_height = 50;

		memset(keyboard, KEY_NONE, sizeof(KEY_STATE) * 256);
	}

	//To set the name write L"game name"
	ConsoleEngine(const wchar_t* game_name)
	{
		console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
		console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		screen_width = 50;
		screen_height = 50;

		SetConsoleTitle(game_name);
	}

	~ConsoleEngine()
	{
		delete[] screen_buffer;
	}

	bool CreateConsoleWindow(int new_screen_width, int new_screen_height, int font_width, int font_height)
	{
		//Set Window to min so the buffer can shrink
		screen_window = { 0,0,1,1 };

		if (!SetConsoleWindowInfo(console_output_handle, TRUE, &screen_window))
			return false;

		//Check if new width and height are able
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
		if (!GetConsoleScreenBufferInfo(console_output_handle, &screen_buffer_info))
			return 1;

		if (new_screen_width > screen_buffer_info.dwMaximumWindowSize.X)
			screen_width = screen_buffer_info.dwMaximumWindowSize.X;
		else screen_width = new_screen_width;

		if (new_screen_height > screen_buffer_info.dwMaximumWindowSize.Y)
			screen_height = screen_buffer_info.dwMaximumWindowSize.Y;
		else screen_height = new_screen_height;

		//Set buffer size
		COORD size = { screen_width, screen_height };

		if (!SetConsoleScreenBufferSize(console_output_handle, size))
			return false;

		// Assign screen buffer to the console
		if (!SetConsoleActiveScreenBuffer(console_output_handle))
			return false;

		//Set the font options
		CONSOLE_FONT_INFOEX font_info;
		font_info.cbSize = sizeof(font_info);
		font_info.nFont = 0;
		font_info.dwFontSize.X = font_width;
		font_info.dwFontSize.Y = font_height;
		font_info.FontFamily = FF_DONTCARE;
		font_info.FontWeight = FW_NORMAL;

		if (!SetCurrentConsoleFontEx(console_output_handle, false, &font_info))
			return false;

		//Set Window to normal size
		screen_window = { 0,0,(short)screen_width - 1, (short)screen_height - 1 };

		if (!SetConsoleWindowInfo(console_output_handle, TRUE, &screen_window))
			return false;

		if (!SetConsoleMode(console_input_handle, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT))
			return false;

		//Set the writting buffer
		screen_buffer = new CHAR_INFO[screen_width * screen_height];
		memset(screen_buffer, 0, sizeof(CHAR_INFO) * screen_width * screen_height);

		//Hide the cursor
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.dwSize = 100;
		cursor_info.bVisible = FALSE;

		if (!SetConsoleCursorInfo(console_output_handle, &cursor_info))
			return false;

		

		return true;
	}

	//---------------Gameplay Loop---------------
	void StartGame()
	{
		Update();
	}

	void CloseGame() { in_game = false; }

	KEY_STATE GetKeyState(KEY asked_key)
	{
		return keyboard[asked_key];
	}

	//Override this with gameplay code inside
	virtual void GameplayStart() = 0;

	virtual void GameplayUpdate(float delta_time) = 0;

	virtual void CleanGameMemory() = 0;

	//---------------Functionality----------------
	int GetScreenWidth() { return screen_width; }

	int GetScreenHeight() { return screen_height; }

	char GetCharAtScreenBuffer(int x, int y)
	{
		Cut(x, y);
		return screen_buffer[x + (screen_width * y)].Char.AsciiChar;
	}

	char GetCharAtScreenBuffer(int pos)
	{
		Cut(pos);
		return screen_buffer[pos].Char.AsciiChar;
	}

	void Cut(int& x, int& y)
	{
		if(x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= screen_width) x = screen_width - 1;
		if (y >= screen_height) y = screen_height - 1;
	}

	void Cut(int& pos)
	{
		if (pos < 0) pos = 0;
		if (pos >= (screen_width * screen_height)) pos = (screen_width * screen_height) - 1;
	}

	void Clamp(int& value, int max, int min)
	{
		if (value < min) value = min;
		if (value > max) value = max;
	}

	void Clamp(float& value, float max, float min)
	{
		if (value < min) value = min;
		if (value > max) value = max;
	}


	//---------------Draw functions---------------
	void DrawPixel(int x, int y, COLOR color = FG_BLACK, PIXEL_TYPE pixel_type = PIXEL_SOLID)
	{
		if (x >= 0 && y >= 0 && x < screen_width && y < screen_height)
		{
			screen_buffer[x + (screen_width * y)].Char.UnicodeChar = pixel_type;
			screen_buffer[x + (screen_width * y)].Attributes = color;
		}
	}

	void DrawPixel(int pos, COLOR color = FG_BLACK, PIXEL_TYPE pixel_type = PIXEL_SOLID)
	{
		if (pos >= 0 && pos < (screen_width * screen_height))
		{
			screen_buffer[pos].Char.UnicodeChar = pixel_type;
			screen_buffer[pos].Attributes = color;
		}
	}

	void DrawChar(int x, int y, char character, COLOR color = FG_BLACK)
	{
		if (x >= 0 && y >= 0 && x < screen_width && y < screen_height)
		{
			screen_buffer[x + (screen_width * y)].Char.AsciiChar = character;
			screen_buffer[x + (screen_width * y)].Attributes = color;
		}
	}

	void DrawChar(int pos, char character, COLOR color = FG_BLACK)
	{
		if (pos >= 0 && pos < (screen_width * screen_height))
		{
			screen_buffer[pos].Char.AsciiChar = character;
			screen_buffer[pos].Attributes = color;
		}
	}

private:

	void Update()
	{	
		std::chrono::time_point<std::chrono::system_clock> actual_time = std::chrono::system_clock::now();
		std::chrono::time_point<std::chrono::system_clock> last_frame_time = std::chrono::system_clock::now();

		GameplayStart();

		while (in_game)
		{
			//Time
			actual_time = std::chrono::system_clock::now();
			std::chrono::duration<float> delta_time = actual_time - last_frame_time;
			last_frame_time = actual_time;

			PreUpdate();

			GameplayUpdate(delta_time.count());

			Render();
		}
	
		CleanGameMemory();

	}

	void PreUpdate()
	{
		//Get input events: Mouse, Keyboard...
		INPUT_RECORD events_buffer[32];
		DWORD num_events = 0;
		GetNumberOfConsoleInputEvents(console_input_handle, &num_events);

		if (num_events > 0)
			ReadConsoleInput(console_input_handle, events_buffer, num_events, &num_events);

		//Update Keyboard
		for (int i = 0; i < 256; i++)
		{
			if (keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_REPEAT;

			if (keyboard[i] == KEY_UP)
				keyboard[i] = KEY_NONE;
		}

		//Handle Events
		for (int i = 0; i < num_events; i++)
		{
			switch (events_buffer[i].EventType)
			{
			case KEY_EVENT:
				//events_buffer[i].Event.KeyEvent.

				if (events_buffer[i].Event.KeyEvent.bKeyDown)
				{
					keyboard[events_buffer[i].Event.KeyEvent.wVirtualKeyCode] = KEY_DOWN;
				}
				else keyboard[events_buffer[i].Event.KeyEvent.wVirtualKeyCode] = KEY_UP;
					

				break;

			default:
				break;
			}
		}
	}

	void Render()
	{
		WriteConsoleOutput(console_output_handle, screen_buffer, { (short)screen_width, (short)screen_height }, { 0,0 }, &screen_window);
	}

};


#endif // !_CONSOLE_ENGINE_H_

