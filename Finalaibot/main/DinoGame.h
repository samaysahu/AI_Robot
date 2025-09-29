// DinoGame.h
#ifndef DINOGAME_H
#define DINOGAME_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C

#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10
#define DINO_INIT_Y 29

#define BASE_LINE_X 0
#define BASE_LINE_Y 54
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 54

#define OBSTACLE_HEIGHT 8
#define OBSTACLE_WIDTH 2
#define OBSTACLE_Y (BASE_LINE_Y - OBSTACLE_HEIGHT)
#define JUMP_PIXEL 22

class DinoGame {
public:
  Adafruit_SSD1306 display;
  int16_t obstacle_x, obstacle1_x;
  int16_t dino_y;
  int jump;
  int score;
  bool gameOver;

  static const unsigned char PROGMEM dino1[];

  DinoGame() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    obstacle_x(127), obstacle1_x(195), dino_y(DINO_INIT_Y), jump(0), score(0), gameOver(false) {}

  void begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      for (;;) {}
    }
    display.clearDisplay();
    introMessage();
    delay(2000);
    resetGame();
  }

  void update() {
    if (gameOver) {
      gameOverScreen();
      resetGame();
      return;
    }
    playStep();
  }

private:
  void introMessage() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 5);
    display.println("Dino Game");
    display.setTextSize(1);
    display.setCursor(5, 45);
    display.println("Starting...");
    display.display();
  }

  void resetGame() {
    obstacle_x = 127;
    obstacle1_x = 195;
    dino_y = DINO_INIT_Y;
    jump = 0;
    score = 0;
    gameOver = false;
  }

  void moveDino(int16_t y) {
    display.drawBitmap(DINO_INIT_X, y, dino1, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
  }

  void moveObstacle(int16_t x) {
    display.drawLine(x, OBSTACLE_Y, x, OBSTACLE_Y + OBSTACLE_HEIGHT, SSD1306_WHITE);
    display.drawLine(x + 1, OBSTACLE_Y, x + 1, OBSTACLE_Y + OBSTACLE_HEIGHT, SSD1306_WHITE);
  }

  void displayScore(int score) {
    display.setTextSize(1);
    display.setCursor(64, 10);
    display.print("Score: ");
    display.print(score);
  }

  void gameOverScreen() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 5);
    display.println("Game Over");
    display.setTextSize(1);
    display.setCursor(10, 30);
    display.print("Score: ");
    display.print(score);
    display.setCursor(1, 45);
    display.println("Restarting...");
    display.display();
    delay(3000);
  }

  void playStep() {
    display.clearDisplay();

    // Auto jump trigger
    if (jump == 0) {
      if ((obstacle_x > DINO_INIT_X) && (obstacle_x < (DINO_INIT_X + DINO_WIDTH + 5))) jump = 1;
      else if ((obstacle1_x > DINO_INIT_X) && (obstacle1_x < (DINO_INIT_X + DINO_WIDTH + 5))) jump = 1;
    }

    // Jump logic
    if (jump == 1) {
      dino_y--;
      if (dino_y == (DINO_INIT_Y - JUMP_PIXEL)) {
        jump = 2;
        score++;
      }
    } else if (jump == 2) {
      dino_y++;
      if (dino_y == DINO_INIT_Y) {
        jump = 0;
      }
    }

    // Collision detection
    if (obstacle_x <= (DINO_INIT_X + DINO_WIDTH) && obstacle_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) {
      if (dino_y >= (DINO_INIT_Y - 3)) {
        gameOver = true;
      }
    }
    if (obstacle1_x <= (DINO_INIT_X + DINO_WIDTH) && obstacle1_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) {
      if (dino_y >= (DINO_INIT_Y - 3)) {
        gameOver = true;
      }
    }

    displayScore(score);
    moveObstacle(obstacle_x);
    moveObstacle(obstacle1_x);
    moveDino(dino_y);
    display.drawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y1, SSD1306_WHITE);

    obstacle_x--;
    obstacle1_x--;

    if (obstacle_x <= 0) obstacle_x = 127;
    if (obstacle1_x <= 0) obstacle1_x = 195;

    display.display();
  }
};

// Bitmap data for dino sprite (refer to your original byte array)
const unsigned char DinoGame::dino1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00,
  0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00,
  0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00,
  0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00,
  0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00,
  0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00
};

#endif // DINOGAME_H
