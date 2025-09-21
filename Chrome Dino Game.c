/**
 * Author:    Harsh Mittal
 * Created:   10.07.2021
 * Github: [www.github.com/harshmittal2210](https://www.github.com/harshmittal2210)
 * Email: [harshmittal2210@gmail.com](mailto:harshmittal2210@gmail.com)
 *
 * (c) Copyright by Harsh Mittal.
 **/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10     // Dino initial spawn location (x)
#define DINO_INIT_Y 29     // Dino initial spawn location (y, ground level)

#define BASE_LINE_X 0
#define BASE_LINE_Y 54
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 54

#define OBSTACLE_HEIGHT 8  // smaller line height
#define OBSTACLE_WIDTH 2   // line thickness (pixels)

#define OBSTACLE_Y (BASE_LINE_Y - OBSTACLE_HEIGHT)  // vertical position of obstacles (top of line)

#define JUMP_PIXEL 22  // Number of pixels dino will jump

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM dino1[]={
  // 'dino', 25x26px sprite - unchanged
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00,
  0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00,
  0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00,
  0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00,
  0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00,
  0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00
};

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();

  introMessage();
  delay(2000); // Show intro message for 2 seconds
  play();     // Start the game automatically
}

void loop() {
  // Not used because play() loops internally
}

void introMessage() {
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 5);
  display.println("Dino Game");

  display.setTextSize(1);
  display.setCursor(5, 45);
  display.println("Starting...");

  display.display();
}

void moveDino(int16_t *y) {
  display.drawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
}

void moveObstacle(int16_t *x) {
  // Draw smaller vertical line obstacle with thickness 2 pixels
  display.drawLine(*x, OBSTACLE_Y, *x, OBSTACLE_Y + OBSTACLE_HEIGHT, SSD1306_WHITE);
  display.drawLine(*x + 1, OBSTACLE_Y, *x + 1, OBSTACLE_Y + OBSTACLE_HEIGHT, SSD1306_WHITE);
}

void gameOver(int score = 0) {
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
  delay(3000); // Show game over for 3 seconds
  play();      // Restart game automatically
}

void displayScore(int score) {
  display.setTextSize(1);
  display.setCursor(64, 10);
  display.print("Score: ");
  display.print(score);
}

void play() {
  int16_t obstacle_x = 127;
  int16_t obstacle1_x = 195;

  int16_t dino_y = DINO_INIT_Y;
  int jump = 0;

  int score = 0;

  unsigned long currentTime = 0;

  for (;;) {
    display.clearDisplay();

    currentTime = millis();

    // Auto-jump when obstacle is close and dino is on ground
    if (jump == 0) {
      if ((obstacle_x > DINO_INIT_X) && (obstacle_x < (DINO_INIT_X + DINO_WIDTH + 5))) {
        jump = 1;
      } else if ((obstacle1_x > DINO_INIT_X) && (obstacle1_x < (DINO_INIT_X + DINO_WIDTH + 5))) {
        jump = 1;
      }
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
        Serial.println("Collision Happened");
        break;
      }
    }

    if (obstacle1_x <= (DINO_INIT_X + DINO_WIDTH) && obstacle1_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) {
      if (dino_y >= (DINO_INIT_Y - 3)) {
        Serial.println("Collision Happened");
        break;
      }
    }

    displayScore(score);
    moveObstacle(&obstacle_x);
    moveObstacle(&obstacle1_x);
    moveDino(&dino_y);
    display.drawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y1, SSD1306_WHITE);

    obstacle_x--;
    obstacle1_x--;

    if (obstacle_x <= 0) {
      obstacle_x = 127;
    }

    if (obstacle1_x <= 0) {
      obstacle1_x = 195;
    }

    display.display();
  }
  Serial.println("Game Over");
  gameOver(score); // Automatically restarts game after showing game over
}
