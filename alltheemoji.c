#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        
#define SCREEN_ADDRESS 0x3C  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Adjustable
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

//current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32 + ref_eye_width + ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

int dir_x = 0;
int dir_y = 0;
int count = 0;

// -------- Function Declarations (NO DEFAULTS) --------
void draw_eyes(bool update);
void center_eyes(bool update);
void blink(int speed);
void sleep();
void wakeup();
void happy_eye();
void saccade(int direction_x, int direction_y);
void move_right_big_eye();
void move_left_big_eye();
void move_big_eye(int direction);

// -------- Eye Drawing Functions --------

void draw_eyes(bool update = true) {
  display.clearDisplay();
  //draw from center
  int x = int(left_eye_x - left_eye_width / 2);
  int y = int(left_eye_y - left_eye_height / 2);
  display.fillRoundRect(x, y, left_eye_width, left_eye_height, ref_corner_radius, SSD1306_WHITE);
  x = int(right_eye_x - right_eye_width / 2);
  y = int(right_eye_y - right_eye_height / 2);
  display.fillRoundRect(x, y, right_eye_width, right_eye_height, ref_corner_radius, SSD1306_WHITE);
  if (update) {
    display.display();
  }
}

void center_eyes(bool update = true) {
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;
  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
  left_eye_y = SCREEN_HEIGHT / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
  right_eye_y = SCREEN_HEIGHT / 2;
  draw_eyes(update);
}

void blink(int speed = 12) {
  draw_eyes();
  for (int i = 0; i < 3; i++) {
    left_eye_height -= speed;
    right_eye_height -= speed;
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height += speed;
    right_eye_height += speed;
    draw_eyes();
    delay(1);
  }
}

void sleep() {
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);
}

void wakeup() {
  sleep();
  for (int h = 0; h <= ref_eye_height; h += 2) {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
}

void happy_eye() {
  center_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++) {
    display.fillTriangle(left_eye_x - left_eye_width / 2 - 1, left_eye_y + offset, left_eye_x + left_eye_width / 2 + 1, left_eye_y + 5 + offset, left_eye_x - left_eye_width / 2 - 1, left_eye_y + left_eye_height + offset, SSD1306_BLACK);
    display.fillTriangle(right_eye_x + right_eye_width / 2 + 1, right_eye_y + offset, right_eye_x - left_eye_width / 2 - 1, right_eye_y + 5 + offset, right_eye_x + right_eye_width / 2 + 1, right_eye_y + right_eye_height + offset, SSD1306_BLACK);
    offset -= 2;
    display.display();
    delay(1);
  }
  display.display();
  delay(1000);
}

void saccade(int direction_x, int direction_y) {
  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;
  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    draw_eyes();
    delay(1);
  }
}

void move_right_big_eye() {
  move_big_eye(1);
}

void move_left_big_eye() {
  move_big_eye(-1);
}

void move_big_eye(int direction) {
  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  delay(1000);
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  center_eyes(true);
}

// -------- Setup & Main Loop --------

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);       
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);     
  display.println(F("by Ricardo Inc 2024"));
  display.println(F("  "));
  display.println(F("ricsistemas@gmail.com"));
  display.display();
  delay(5000);
}

void loop() {
  switch (count) {
    case 0:
      wakeup();
      Serial.println("Wake UP!");
      break;
    case 1:
      center_eyes(true);
      Serial.println("Center Eyes!");
      break;
    case 2:
      move_right_big_eye();
      Serial.println("Moving Right!");
      break;
    case 3:
      move_left_big_eye();
      Serial.println("Moving Left!");
      break;
    case 4:
      blink(10);
      Serial.println("Short Blink!");
      break;
    case 5:
      happy_eye();
      Serial.println("Happy Eye!");
      break;
    case 6:
      blink(20);
      Serial.println("Long Blink!");
      break;
    case 7:
      Serial.println("All Motion!");
      // BOTTOM LEFT
      dir_x = -1;
      dir_y = 1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // BOTTOM
      dir_x = 0;
      dir_y = 1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // BOTTOM RIGHT
      dir_x = 1;
      dir_y = 1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // RIGHT
      dir_x = 1;
      dir_y = 0;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // TOP RIGHT
      dir_x = 1;
      dir_y = -1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // TOP
      dir_x = 0;
      dir_y = -1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // TOP LEFT
      dir_x = -1;
      dir_y = -1;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);

      // LEFT
      dir_x = -1;
      dir_y = 0;
      saccade(dir_x, dir_y);
      delay(300);
      saccade(-dir_x, -dir_y);
      delay(300);
      break;
    case 8:
      sleep();
      break;
    default:
      Serial.println("Default!");
      break;
  }
  count += 1;
  if (count == 9) { count = 0; }
}
