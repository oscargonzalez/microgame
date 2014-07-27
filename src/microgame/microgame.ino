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

#define MAX_STARS          10 // Starfield config. The more, the poor...
#define SCOREBOARD_HEIGHT  8
#define MAX_FIRES          3 // Max player fires at the same time
#define BATTESHIP_WIDTH    10
#define BATTESHIP_HEIGHT   7
#define BATTESHIP_FIRE_WIDTH    3  //Width of the shot
#define BATTESHIP_FIRE_SPEED    3 

// Define enemy types
#define ENEMY_TYPE_1    0
#define ENEMY_TYPE_2    1
#define ENEMY_TYPE_3    2
#define ENEMY_TYPE_4    3
#define ENEMY_TYPE_5    4

#define MAX_ENEMIES     6 // More enemies = more fun and less RAM...
char max_enemies  = 1; // This will change in realtime (more enemies, more dificult!)

// Splash logo 128x64
const unsigned char PROGMEM splash_logo [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 48, 192, 0, 0, 0, 0, 15, 224, 0, 0, 0, 0, 0, 0, 0, 224, 112, 0, 0, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 224, 240, 0, 0, 0, 0, 0, 96, 0, 0, 0, 0, 0, 0, 0, 1, 225, 224, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 1, 243, 96, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 1, 182, 97, 131, 240, 254, 7, 192, 192, 3, 248, 31, 248, 15, 128, 0, 3, 60, 195, 6, 1, 140, 28, 193, 135, 128, 28, 49, 156, 24, 192, 0, 3, 60, 195, 12, 1, 128, 48, 97, 129, 128, 12, 49, 140, 48, 192, 0, 3, 56, 195, 12, 1, 128, 48, 97, 129, 128, 12, 49, 140, 48, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 0, 24, 99, 24, 96, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 7, 248, 99, 24, 127, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 12, 24, 99, 24, 96, 0, 0, 12, 3, 12, 48, 6, 0, 193, 134, 6, 24, 48, 198, 48, 192, 0, 0, 12, 3, 12, 48, 6, 0, 193, 135, 6, 24, 48, 198, 48, 192, 0, 0, 12, 3, 12, 56, 6, 0, 231, 3, 6, 24, 48, 198, 48, 192, 0, 0, 24, 6, 24, 31, 12, 0, 124, 1, 252, 15, 225, 140, 96, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 14, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 17, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 65, 56, 227, 148, 32, 156, 243, 231, 39, 62, 0, 0, 0, 0, 0, 0, 65, 69, 20, 88, 32, 34, 136, 72, 168, 132, 0, 0, 0, 0, 0, 0, 65, 49, 3, 208, 35, 162, 136, 135, 175, 136, 0, 0, 0, 0, 0, 0, 65, 9, 4, 80, 32, 162, 136, 136, 168, 8, 0, 0, 0, 0, 0, 0, 34, 69, 20, 208, 17, 34, 137, 9, 168, 144, 0, 0, 0, 0, 0, 0, 28, 56, 227, 80, 14, 28, 139, 230, 167, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 7, 28, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 8, 162, 96, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 34, 104, 0, 162, 161, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 34, 152, 0, 162, 33, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 34, 136, 1, 34, 34, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 34, 136, 2, 34, 35, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 166, 152, 4, 34, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 154, 105, 15, 156, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    
  
// Ship: 10x7
const unsigned char PROGMEM ship [] = { 112, 0, 243, 0, 121, 128, 63, 192, 121, 128, 243, 0, 112, 0 };    

// enemies: 5x5
const unsigned char PROGMEM enemy1 [] = { 32, 168, 248, 112, 136 }; 
const unsigned char PROGMEM enemy2 [] = { 24, 48, 248, 48, 24 };    
const unsigned char PROGMEM enemy3 [] = { 216, 112, 80, 112, 168 };    
const unsigned char PROGMEM enemy4 [] = { 136, 168, 248, 112, 216 };    
const unsigned char PROGMEM enemy5 [] = { 80, 248, 248, 112, 168 };    

void setup()   {                
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
/*  
  // Splash logo!
  display.clearDisplay();
  display.drawBitmap(0, 0,  splash_logo, 128, 64, 1);
  display.display();    
  
  for (int i=0 ; i<10 ; i++)
  {
    display.invertDisplay(true);
    delay(100); 
    display.invertDisplay(false);
    delay(100);   
  }
  
  delay(2000);
  
  display.startscrollleft(0x00, 0x0F);
  delay(5000);
  display.stopscroll();  
*/
  display.clearDisplay();
  

}

struct tPos {
   char x;
   char y; 
};

/*======================================================================
  BattleShip class definition
  ======================================================================*/
class Battleship
{
  public:
    Battleship();
    void update();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void Fire();
    void setPosition(int x, int y);
    tPos getPosition();
    
  private:
    int _lastx;
    int _lasty;
    int _x;
    int _y;   
    int _velocity;   
    char fire_count;
    tPos fires[MAX_FIRES];
};

Battleship::Battleship()
{
    int i;
    
   _x=0;
   _y=0; 
   _lastx=0;
   _lasty=0;
   _velocity=2;
   fire_count=0;
   for (i=0;i<MAX_FIRES;i++) { fires[i].x=0; }
}

void Battleship::moveRight()   { _lastx=_x; _x += _velocity; }
void Battleship::moveLeft()    { _lastx=_x; _x -= _velocity; }
void Battleship::moveUp()      { _lasty=_y; _y -= _velocity; }
void Battleship::moveDown()    { _lasty=_y; _y += _velocity; }
tPos Battleship::getPosition() { tPos p; p.x = _x; p.y = _y; return p; }
void Battleship::setPosition(int x, int y) { _lastx=_x; _lasty=_y; _x=x; _y=y; }
void Battleship::Fire()
{
  if (fire_count < MAX_FIRES)
  {
     for (int i=0 ; i<MAX_FIRES; i++)
     {
       if (fires[i].x == 0)
       {
         fires[i].x = _x + BATTESHIP_WIDTH;
         fires[i].y = _y + (BATTESHIP_HEIGHT/2); // Divide in realtime in bad ;)
         fire_count++;
         break;
       }
     }
  }
}

void Battleship::update()
{
  
  int i,j;
  
  // Update position
  display.drawBitmap(_lastx, _lasty,  ship, 10, 7, 0);
  display.drawBitmap(_x, _y,  ship, 10, 7, 1);

  // Update fire
  for (i=0 ; i<MAX_FIRES ; i++)
  {
    //BATTESHIP_FIRE_WIDTH     
    if (fires[i].x > 0)
    { 
      for (j=0 ; j<BATTESHIP_FIRE_WIDTH ; j++) { display.drawPixel(fires[i].x-j, fires[i].y, BLACK); } // Draw 3 pixels back for the full shot            
      fires[i].x += BATTESHIP_FIRE_SPEED;
      for (j=0 ; j<BATTESHIP_FIRE_WIDTH ; j++) { display.drawPixel(fires[i].x-j, fires[i].y, WHITE); } // Draw 3 pixels back for the full shot      
      
      //Shot is dead
      if (fires[i].x > (128-BATTESHIP_FIRE_WIDTH))
      {
        for (j=0 ; j<BATTESHIP_FIRE_WIDTH ; j++) { display.drawPixel(fires[i].x-j, fires[i].y, BLACK); } // Erase shot
        fires[i].x=0; 
        fire_count--;
      }
      
    }    
  }
}

/*======================================================================
  Enemy class definition
  ======================================================================*/
class Enemy {
  public:
    
  private:
    int type;
    int _lastx;
    int _lasty;
    int _x;
    int _y;      
};

/*======================================================================
  Starfield class definition
  ======================================================================*/
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
      stars[i].x = random(0, 127);
      stars[i].y = random(SCOREBOARD_HEIGHT, 63);      
  }
}

void Starfield::update()
{

    for (int i=0 ; i<MAX_STARS ; i++)
    {
      display.drawPixel(stars[i].x, stars[i].y, BLACK);
      stars[i].x-=1;    
      display.drawPixel(stars[i].x, stars[i].y, WHITE);
      
      if (stars[i].x <= 1) { 
        display.drawPixel(stars[i].x, stars[i].y, BLACK);
        stars[i].x = 127;      
        stars[i].y = random(SCOREBOARD_HEIGHT, 63);      
      }
    }
     
}

Battleship nave;
Starfield stars;

float inc=0;
unsigned long last_fire=millis();

/*======================================================================
  Main loop
  ======================================================================*/
void loop() {
  
  char x=0;
  
  nave.setPosition(0, 32+sin(inc)*20);
  
  if (millis()-last_fire > 400)
  {
     nave.Fire(); 
     last_fire=millis();
  }
  
  inc+=0.06;
      
     nave.update();    
     stars.update();
     
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.println("PTS: 875");    
    
    display.drawBitmap(64, 10, enemy1, 5, 5, 1);
    display.drawBitmap(64, 20, enemy2, 5, 5, 1);
    display.drawBitmap(64, 30, enemy3, 5, 5, 1);
    display.drawBitmap(64, 40, enemy4, 5, 5, 1);
    display.drawBitmap(64, 50, enemy5, 5, 5, 1);
     
    display.display();    
  
}


