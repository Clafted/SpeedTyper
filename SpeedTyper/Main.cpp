#include <iostream>
#include "raylib.h"
#include <chrono>
#include <ctime>
#include <string>

void processKeyInput(int key, char text[1024], char sample[1024], int &cursor, int sampleLength)
{
	if (key == 0) return;
	if (key == KEY_BACKSPACE)
	{
		if (cursor > 0) cursor--;
		text[cursor] = sample[cursor];
		return;
	}
	if (cursor == sampleLength) return;
	switch (key) {
	case KEY_ENTER:
		text[cursor] = '\n';
		cursor++;
		break;
	case KEY_LEFT_SHIFT:
		break;
	default:
		if (key < KEY_A || KEY_Z < key || (KEY_A <= key && key <= KEY_Z && IsKeyDown(KEY_LEFT_SHIFT))) text[cursor] = (char)key;
		else text[cursor] = (char)(key + 32);
		cursor++;
		break;
	}
}


using namespace std::chrono_literals;

int main()
{
	float aspectRatio = 0.6f;
	int screenWidth = 700;
	int screenHeight = (int) (screenWidth * aspectRatio);
	int textSize = (int)(screenWidth * 0.05f);

	InitWindow(screenWidth, (int) (screenWidth * aspectRatio), "SpeedTyper");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	// Text variables
	Font consolata = LoadFontEx("./Inconsolata-VariableFont_wdth,wght.ttf", textSize, NULL, 0);
	char sample[1024] = "This is the sample text.";
	char userTypedText[1024];
	char correctText[1024];
	char incorrectText[1024];
	int cursor = 0;
	int input;
	strcpy_s(userTypedText, sample);

	// Timer variables
	std::chrono::steady_clock::duration countDown = std::chrono::seconds(10);
	std::chrono::time_point<std::chrono::steady_clock> timer(countDown);
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
	std::string displayTimer;

	for (int i = 0; i < 1024; i++)
	{
		correctText[i] = 0;
		incorrectText[i] = 0;
	}

	while (!WindowShouldClose())
	{
		if (IsWindowResized()) {
			screenWidth = GetScreenWidth();
			screenHeight = (int)(screenWidth * aspectRatio);
			textSize = (int)(screenWidth * 0.05f);
			SetWindowSize(screenWidth, screenHeight);
			consolata = LoadFontEx("./Inconsolata-VariableFont_wdth,wght.ttf", textSize, NULL, 0);
		}
		input = GetKeyPressed();
		processKeyInput(input, userTypedText, sample, cursor, 24);

		for (int i = 0; i < cursor; i++)
		{
			if (userTypedText[i] == sample[i])
			{	
				correctText[i] = userTypedText[i];
				incorrectText[i] = ' ';
			}
			else
			{
				incorrectText[i] = (userTypedText[i] == ' ') ? '-' : userTypedText[i];
				correctText[i] = ' ';
			}
		}
		correctText[cursor] = '_';
		correctText[cursor + 1] = '\0';
		incorrectText[cursor] = ' ';
		incorrectText[cursor+1] = '\0';

		currentTime = std::chrono::steady_clock::now();
		displayTimer = (timer - currentTime + startTime.time_since_epoch()) / 1s;

		BeginDrawing();

		ClearBackground(Color{100, 100, 220, 255});
		DrawTextEx(consolata, userTypedText, Vector2 { screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{ 10, 10, 10, 180 });
		DrawTextEx(consolata, correctText, Vector2{ screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{200, 200, 250, 255});
		DrawTextEx(consolata, incorrectText, Vector2{ screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{255, 120, 120, 255});
		DrawTextEx(consolata, displayTimer.c_str(), Vector2{screenWidth * 0.5f, screenHeight * 0.5f}, textSize, 1.0f, Color{255, 200, 250, 255});
	
		EndDrawing();
	}
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	std::cout << "Total window runtime: " << (end - currentTime) / 1ms << " s\n";
	CloseWindow();
	return 0;


}