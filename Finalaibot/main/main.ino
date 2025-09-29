#include "SnakeGame.h"
#include "BreakoutGame.h"
#include "DinoGame.h"

// Create instances
SnakeGame snakeGame;
BreakoutGame breakoutGame;
DinoGame dinoGame;

enum GameType { SNAKE, BREAKOUT, DINOGAME };
GameType currentGame;
GameType previousGame = SNAKE;  // Initialize arbitrarily

// Pick a random game different from previous
GameType pickNewGame(GameType prev) {
  GameType newGame;
  do {
    int choice = random(0, 3);
    newGame = static_cast<GameType>(choice);
  } while(newGame == prev);
  return newGame;
}

void switchGame() {
  currentGame = pickNewGame(previousGame);
  previousGame = currentGame;

  if (currentGame == SNAKE) {
    snakeGame.begin();
    Serial.println("Playing Snake Game");
  } else if (currentGame == BREAKOUT) {
    breakoutGame.begin();
    Serial.println("Playing Breakout Game");
  } else {
    dinoGame.begin();
    Serial.println("Playing Dino Game");
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  switchGame();
}

void loop() {
  if (currentGame == SNAKE) {
    snakeGame.update();
    if (snakeGame.gameOver) {  // Detect end, no delay, immediate switch
      switchGame();
    }
  } else if (currentGame == BREAKOUT) {
    breakoutGame.update();
    if (!breakoutGame.gameRunning) {
      switchGame();
    }
  } else {  // DINOGAME
    dinoGame.update();
    if (dinoGame.gameOver) {
      switchGame();
    }
  }
}
