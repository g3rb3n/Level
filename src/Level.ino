#include "devices/SSD1306I2C.h"
#include "MPU9250.h"

using namespace g3rb3n;

MPU9250 mpu;
SSD1306I2C oled(0x3C);

void setup()
{
  //mpu.begin();
  oled.begin();
  oled.flipVertical(true);
  oled.flipHorizontal(true);
  oled.setContrast(127);
  mpu.setLowPassRate(LPR_10);
  mpu.setSampleRateDividerMode(RATE_DIVIDER_7);
  oled.print("Waterpas");
  oled.display();
}

uint8_t max = 64;
float scale = 64;
uint8_t offset = 32;
uint8_t radius = 5;
uint8_t hitStep = 0;
int last = 0;

void hit()
{
  for (uint8_t i = 0 ; i < hitStep && i < 16 ; ++i)
  {
    oled.drawCircle(2*offset, offset, 2*i);
  }
}

uint8_t rescale(float x)
{
  int16_t r = x * scale + offset;
  if (r < radius) r = radius;
  if (r > max - radius) r = max - radius;  
  return r;
}

void loop()
{  
  Point3D<float> a;
  mpu.acceleration(a);
  uint8_t x = rescale(-a.y) + 32;
  uint8_t y = rescale(-a.x);
  oled.clear();
  oled.fillCircle(x, y, radius);
  oled.drawCircle(2*offset, offset, radius + 2);
  if (a.y * a.y + a.x * a.x < .0001)
  {
    if (millis() - last > 20)
    {
      ++hitStep;
      last = millis();
    }
    hit();
  }
  else
    hitStep = 0;
  oled.display();
}
