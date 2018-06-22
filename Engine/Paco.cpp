#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include <iostream>

#include "ConsoleEngine.h"

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. Thanks! For now, I'll try enabling it for you - Javidx9
#define UNICODE
#define _UNICODE
#endif

enum COLORo
{
	//Intensity -> 8

	FGo_BLACK = 0,
	FGo_DARK_BLUE = 1,		//Main blue
	FGo_DARK_GREEN = 2,		//Main green
	FGo_DARK_CYAN = 3,		//Blue + Green
	FGo_DARK_RED = 4,		//Main red
	FGo_DARK_MAGENTA = 5,	//Blue + Red
	FGo_DARK_YELLOW = 6,		//Green + Red
	FGo_GREY = 7,			//Blue + Green + Red
	FGo_DARK_GREY = 8,		//Black + Intensity
	FGo_BLUE = 9,			//Blue + Intensity
	FGo_GREEN = 10,			//Green + Intensity
	FGo_CYAN = 11,			//Cyan + Intensity
	FGo_RED = 12,			//Red + Intensity
	FGo_MAGENTA = 13,		//Magenta + Intensity
	FGo_YELLOW = 14,			//Yellow + Intensity
	FGo_WHITE = 15			//Blue + Green + Red + Intensity
};

enum PIXEL_TYPEo
{
	PIXELo_SOLID = 0x2588,
	PIXELo_THREEQUARTERS = 0x2593,
	PIXELo_HALF = 0x2592,
	PIXELo_QUARTER = 0x2591,
};
bool in_game = true;

HANDLE console_output_handle;
HANDLE console_input_handle;

int screen_width = 50;
int screen_height = 50;
CHAR_INFO* screen_buffer;
SMALL_RECT screen_window;

using namespace std;

void Draw()
{
	for (int y = 0; y < screen_height; y++)
	{
		for (int x = 0; x < screen_width; x++)
		{
			screen_buffer[x + (y * screen_width)].Char.AsciiChar = '#'; // PIXEL_SOLID;

			if (y % 2 == 0)
				screen_buffer[x + (y * screen_width)].Attributes = FGo_WHITE;
			else 	screen_buffer[x + (y * screen_width)].Attributes = FGo_DARK_GREEN;
		}
	}
}

void Render()
{
	while (in_game)
	{
		Draw();

		if (_kbhit())
		{
			int key = _getch();

			if (key == 32)
				in_game = false;
		}
		short lol = (short)screen_height;
		WriteConsoleOutput(console_output_handle, screen_buffer, { (short)screen_width, (short)screen_height}, { 0,0 }, &screen_window);
	}
}

/*int main()
{

	//Start console
	console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);


	wchar_t title[] = { 'C','o','n','s','o','l','e',' ','E','n','g','i','n','e','\0' };
	SetConsoleTitle(title);

	//Set Window to min so the buffer can shrink
	screen_window = { 0,0,1,1 };

	if (!SetConsoleWindowInfo(console_output_handle, TRUE, &screen_window))
		return 1;

	//Set Window to normal size
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	if (!GetConsoleScreenBufferInfo(console_output_handle, &screen_buffer_info))
		return 1;

	if (screen_width > screen_buffer_info.dwMaximumWindowSize.X)
		screen_width = screen_buffer_info.dwMaximumWindowSize.X;

	if (screen_height > screen_buffer_info.dwMaximumWindowSize.Y)
		screen_height = screen_buffer_info.dwMaximumWindowSize.Y;

	//Set buffer size
	COORD size;
	size.X = (short)screen_width;
	size.Y = (short)screen_height;

	if (!SetConsoleScreenBufferSize(console_output_handle, size))
		return 1;

	// Assign screen buffer to the console
	if (!SetConsoleActiveScreenBuffer(console_output_handle))
		return 1;


	//Set the font options
	CONSOLE_FONT_INFOEX font_info;
	font_info.cbSize = sizeof(font_info);
	font_info.nFont = 0;
	font_info.dwFontSize.X = 15;
	font_info.dwFontSize.Y = 15;
	font_info.FontFamily = FF_DONTCARE;
	font_info.FontWeight = FW_NORMAL;

	if (!SetCurrentConsoleFontEx(console_output_handle, false, &font_info))
		return 1;

	

	screen_window = { 0,0,(short)screen_width - 1, (short)screen_height - 1 };

	
	if (!SetConsoleWindowInfo(console_output_handle, TRUE, &screen_window))
		return 1;

	if (!SetConsoleMode(console_input_handle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
		return 1;

	//Set the buffer
	screen_buffer = new CHAR_INFO[screen_width * screen_height];
	memset(screen_buffer, 0, sizeof(CHAR_INFO) * screen_width * screen_height);

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(console_output_handle, &info);

	//thread print_thread = thread(&Render);
	//print_thread.join();

	while (in_game)
	{
		Render();
	}

	return 0;
}*/
