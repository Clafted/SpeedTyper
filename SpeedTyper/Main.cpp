#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "raylib.h"

class Game
{
public:
	int deleteCount;
	int mistakes;

	void reset(void)
	{
		deleteCount = 0;
		mistakes = 0;
	}
};

void processKeyInput(int key, char text[1024], char sample[1024], int &cursor, int sampleLength, Game &game)
{
	if (key == 0) return;

	if (key == KEY_BACKSPACE)
	{
		if (cursor > 0) cursor--;
		text[cursor] = sample[cursor];
		game.deleteCount++;
		return;
	}

	if (cursor == sampleLength) return; // Reached end of sample
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
		if (text[cursor] != sample[cursor]) game.mistakes++;
		cursor++;
		break;
	}
}

void differentiateCorrectText(char userText[1024], char sample[1024], char correctText[1024], char incorrectText[1024], int cursor)
{
	for (int i = 0; i < cursor; i++)
	{
		if (userText[i] == sample[i])
		{
			correctText[i] = userText[i];
			incorrectText[i] = ' ';
		}
		else
		{
			incorrectText[i] = (userText[i] == ' ') ? '-' : userText[i];
			correctText[i] = ' ';
		}
	}
	correctText[cursor] = '\0';
	incorrectText[cursor] = '_';
	incorrectText[cursor + 1] = '\0';
}

using namespace std::chrono_literals;

int main()
{
	float aspectRatio = 0.6f;
	int screenWidth = 700;
	int screenHeight = (int) (screenWidth * aspectRatio);
	float textSize = screenWidth * 0.05f;
	float textSpacing = textSize + 4.0f;

	InitWindow(screenWidth, (int) (screenWidth * aspectRatio), "SpeedTyper");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	// Text variables
	Font consolata = LoadFontEx("./Inconsolata-VariableFont_wdth,wght.ttf", (int)textSize, NULL, 0);
	char sample[1024] = "This is the sample text.";
	char userTypedText[1024];
	char correctText[1024];
	char incorrectText[1024];
	int cursor = 0;
	int input;
	strcpy_s(userTypedText, sample);

	// Zero-out char array
	for (int i = 0; i < 1024; i++)
	{
		correctText[i] = 0;
		incorrectText[i] = 0;
	}

	// Performance variables
	Game game;

	// Timer variables
	std::chrono::steady_clock::duration countDown = std::chrono::seconds(10);
	std::chrono::time_point<std::chrono::steady_clock> timer(countDown);
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> deltaTime;
	std::string displayTimer;
	bool endRound;

	game.reset();
	Color ui_color{ 200, 200, 250, 255 };

	while (!WindowShouldClose())
	{
		// Check for window resize
		if (IsWindowResized()) 
		{
			screenWidth = GetScreenWidth();
			screenHeight = (int)(screenWidth * aspectRatio);
			textSize = screenWidth * 0.05f;
			SetWindowSize(screenWidth, screenHeight);
			consolata = LoadFontEx("./Inconsolata-VariableFont_wdth,wght.ttf", (int)textSize, NULL, 0);
		}

		// Process input
		input = GetKeyPressed();
		processKeyInput(input, userTypedText, sample, cursor, 24, game);
		differentiateCorrectText(userTypedText, sample, correctText, incorrectText, cursor);
		endRound = (strcmp(correctText, sample) == 0);

		BeginDrawing();
		
		displayTimer = std::to_string(deltaTime.count() / 1000.0f);

		if (endRound)
		{
			ClearBackground(Color{ 140, 140, 255, 255 });
			DrawTextEx(consolata,
						("You completed in: " + displayTimer + " sec").c_str(),
						Vector2{ 20.0f, 20.0f },
						textSize*1.2f, 1.0f, 
						ui_color);
			DrawTextEx(consolata,
						("Delete count: " + std::to_string(game.deleteCount)).c_str(),
						Vector2{ 20.0f, 20.0f + textSpacing },
						textSize * 1.2f, 1.0f,
						ui_color);
			DrawTextEx(consolata,
						("Mistakes: " + std::to_string(game.mistakes)).c_str(),
						Vector2{ 20.0f, 20.0f + textSpacing*2 },
						textSize * 1.2f, 1.0f,
						ui_color);
			DrawTextEx(consolata,
						("Accuracy: " + std::to_string((int)(100 * (float)(cursor - 1 - game.mistakes)/(cursor-1))) + "%").c_str(),
						Vector2{ 20.0f, 20.0f + textSpacing * 3 },
						textSize * 1.2f, 1.0f,
						ui_color);
		}
		else
		{
			ClearBackground(Color{ 100, 100, 220, 255 });
			currentTime = std::chrono::steady_clock::now();
			deltaTime = (timer - currentTime) + startTime.time_since_epoch();
			// Shapes
			DrawRectangleRounded(Rectangle{ screenWidth * 0.1f, screenHeight * 0.1f , screenWidth * 0.8f, textSize*3.0f }, 0.5, 5, Color{ 85, 85, 205, 255 });
			// Text
			DrawTextEx(consolata, userTypedText, Vector2{ screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{ 10, 10, 10, 180 });
			DrawTextEx(consolata, correctText, Vector2{ screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{ 200, 200, 250, 255 });
			DrawTextEx(consolata, incorrectText, Vector2{ screenWidth * 0.1f, screenHeight * 0.1f }, textSize, 1.0f, Color{ 255, 120, 120, 255 });
			// Timer
			DrawTextEx(consolata, displayTimer.c_str(), Vector2{ screenWidth * 0.5f, screenHeight * 0.5f }, textSize, 1.0f, Color{ 255, 200, 250, 255 });
		}
	
		EndDrawing();
	}
	CloseWindow();
	return 0;
}