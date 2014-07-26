#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#define MAX_STARS 10
  
// Ship: 11x11
const unsigned char PROGMEM ship [] = {
  4, 0, 4, 0, 4, 0, 4, 0, 46, 128, 46, 128, 59, 128, 145, 32, 245, 224, 191, 160, 132, 32
};    

// enemy 1: 13x9
const unsigned char PROGMEM enemy1 [] = {
  48, 96, 103, 48, 202, 152, 157, 200, 250, 248, 143, 136, 199, 8, 98, 24, 48, 96
};    

void setup()   {                
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(2000);

  // Clear the buffer.
  display.clearDisplay();
/*
  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
*/  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("BattleShip");
  display.display();  
  
  display.clearDisplay();

}

struct tPos {
   char x;
   char y; 
};

/*
  Ship class definition
*/
class Battleship
{
  public:
    Battleship();
    void update();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void setPosition(int x, int y);
    tPos getPosition();
    
  private:
    int _lastx;
    int _lasty;
    int _x;
    int _y;   
    int _velocity;   
};

Battleship::Battleship()
{
   _x=0;
   _y=0; 
   _lastx=0;
   _lasty=0;
   _velocity=2;
}

void Battleship::moveRight()   { _lastx=_x; _x += _velocity; }
void Battleship::moveLeft()    { _lastx=_x; _x -= _velocity; }
void Battleship::moveUp()      { _lasty=_y; _y -= _velocity; }
void Battleship::moveDown()    { _lasty=_y; _y += _velocity; }
tPos Battleship::getPosition() { tPos p; p.x = _x; p.y = _y; return p; }
void Battleship::setPosition(int x, int y) { _lastx=_x; _lasty=_y; _x=x; _y=y; }

void Battleship::update()
{
  
  display.drawBitmap(_lastx, _lasty,  ship, 11, 11, 0);
  display.drawBitmap(_x, _y,  ship, 11, 11, 1);
  /*
  display.drawBitmap(_lastx, _lasty,  enemy1, 13, 9, 0);
  display.drawBitmap(_x, _y,  enemy1, 13, 9, 1);  
  */
}

class Starfield 
{
  public:
    Starfield();
    void update();
    
 private:
   tPos stars[MAX_STARS];
};

Starfield::Starfield()
{
  randomSeed(analogRead(0));
  for (char i=0; i<MAX_STARS ; i++)
  {
      stars[i].y = random(0, 64);
      stars[i].x = random(0, 128);
  }
}

void Starfield::update()
{

    for (int i=0 ; i<MAX_STARS ; i++)
    {
      display.drawPixel(stars[i].x, stars[i].y, BLACK);
      stars[i].y+=1;    
      display.drawPixel(stars[i].x, stars[i].y, WHITE);
      
      if (stars[i].y > 64) { 
        stars[i].y=0;
        stars[i].x = random(0, 128);      
      }
    }
     
}

Battleship nave;
Starfield stars;

boolean flip=false;

float inc=0;
void loop() {
  
  char x=0;
  
  nave.setPosition(64+sin(inc)*35, 54);
  
  inc+=0.03;
      
     nave.update();    
     stars.update();
     
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.println("SCORE: 0");    
     
    display.display();    
  
}

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

