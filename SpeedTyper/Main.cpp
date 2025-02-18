#include <iostream>
#include "raylib.h"
#include <chrono>
#include <ctime>

void processKeyInput(int key, char text[1024], int &cursor)
{
	if (key == 0) return;
		switch (key) {
		case KEY_BACKSPACE:
			if (cursor > 0) cursor--;
			break;
		case KEY_ENTER:
			text[cursor] = '\n';
			cursor++;
			break;
		default:
			text[cursor] = (char)key;
			cursor++;
			break;
		}
		text[cursor] = '\0';
}

int main()
{
	InitWindow(600, 400, "My game");
	char text[1024] = "\0";
	int cursor = 0;
	int input;
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	while (!WindowShouldClose())
	{
		input = GetKeyPressed();
		processKeyInput(input, text, cursor);
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(text, 200, 200, 20, WHITE);
		EndDrawing();
	}
	using namespace std::chrono_literals;
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	std::cout << "Total window runtime: " << (end - start) / 1ms << " s\n";
	CloseWindow();
	return 0;


}