#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define GRID_SIZE 4                 // Moderate size blocks (4x4 pixels)
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)
#define MAX_SNAKE_LENGTH 150        // Max length for snake segments

struct Point {
  int x;
  int y;
};

Point snake[MAX_SNAKE_LENGTH];
int snakeLength = 4;
int directionX = 1;  // Start moving right
int directionY = 0;
Point food;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 150;  // Control snake speed

bool gameOver = false;

void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  randomSeed(analogRead(0));

  int startX = GRID_WIDTH / 2;
  int startY = GRID_HEIGHT / 2;
  for(int i = 0; i < snakeLength; i++) {
    snake[i].x = startX - i;
    snake[i].y = startY;
  }

  placeFood();

  display.clearDisplay();
  display.display();
  // No startup text displayed
}

void loop() {
  if (gameOver) {
    displayGameOver();
    delay(3000);
    resetGame();
  }

  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime > updateInterval) {
    lastUpdateTime = currentTime;
    moveSnake();
    checkCollision();
    drawGame();
  }
}

void moveSnake() {
  for(int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i-1];
  }

  snake[0].x += directionX;
  snake[0].y += directionY;

  if (snake[0].x < 0) snake[0].x = GRID_WIDTH - 1;
  else if (snake[0].x >= GRID_WIDTH) snake[0].x = 0;

  if (snake[0].y < 0) snake[0].y = GRID_HEIGHT - 1;
  else if (snake[0].y >= GRID_HEIGHT) snake[0].y = 0;

  if (snake[0].x == food.x && snake[0].y == food.y) {
    if (snakeLength < MAX_SNAKE_LENGTH) {
      snakeLength++;
      snake[snakeLength - 1] = snake[snakeLength - 2];
    }
    placeFood();
  }

  autoAdjustDirectionToFood();
}

void autoAdjustDirectionToFood() {
  if (snake[0].x != food.x) {
    if ((food.x > snake[0].x && directionX != -1) || (snake[0].x == GRID_WIDTH-1 && food.x == 0)) {
      directionX = 1; directionY = 0;
    } else if ((food.x < snake[0].x && directionX != 1) || (snake[0].x == 0 && food.x == GRID_WIDTH-1)) {
      directionX = -1; directionY = 0;
    }
  } else if (snake[0].y != food.y) {
    if ((food.y > snake[0].y && directionY != -1) || (snake[0].y == GRID_HEIGHT-1 && food.y == 0)) {
      directionX = 0; directionY = 1;
    } else if ((food.y < snake[0].y && directionY != 1) || (snake[0].y == 0 && food.y == GRID_HEIGHT-1)) {
      directionX = 0; directionY = -1;
    }
  }
}

void checkCollision() {
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameOver = true;
      break;
    }
  }
}

void placeFood() {
  bool valid = false;
  while (!valid) {
    food.x = random(0, GRID_WIDTH);
    food.y = random(0, GRID_HEIGHT);
    valid = true;
    for (int i = 0; i < snakeLength; i++) {
      if (food.x == snake[i].x && food.y == snake[i].y) {
        valid = false;
        break;
      }
    }
  }
}

void drawGame() {
  display.clearDisplay();

  // Draw food block
  display.fillRect(food.x * GRID_SIZE, food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);

  // Draw snake blocks
  for(int i = 0; i < snakeLength; i++) {
    display.fillRect(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);
  }

  display.display();
}

void displayGameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 20);
  display.print("GAME");
  display.setCursor(15, 40);
  display.print("OVER");
  display.display();
}

void resetGame() {
  snakeLength = 4;
  int startX = GRID_WIDTH / 2;
  int startY = GRID_HEIGHT / 2;
  for(int i = 0; i < snakeLength; i++) {
    snake[i].x = startX - i;
    snake[i].y = startY;
  }
  directionX = 1;
  directionY = 0;
  gameOver = false;
  placeFood();
}
