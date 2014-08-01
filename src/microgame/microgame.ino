#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
*/
// Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


#define MAX_STARS          10 // Starfield config. The more, the poor...
#define SCOREBOARD_HEIGHT  8
#define MAX_FIRES          3 // Max player fires at the same time
#define BATTESHIP_WIDTH    10
#define BATTESHIP_HEIGHT   7
#define BATTESHIP_FIRE_WIDTH    3  //Width of the shot
#define BATTESHIP_FIRE_SPEED    3 

// Define enemy
#define ENEMY_WIDTH       5
#define ENEMY_HEIGHT      5
#define ENEMY_MAX_FIRES   3
#define ENEMY_FIRES_WIDTH 3
#define ENEMY_FIRES_SPEED 3

#define ENEMY_TYPE_1    0
#define ENEMY_TYPE_2    1
#define ENEMY_TYPE_3    2
#define ENEMY_TYPE_4    3
#define ENEMY_TYPE_5    4

#define MAX_ENEMIES     6 // More enemies = more fun and less RAM...
char max_enemies  = 3; // This will change in realtime (more enemies, more dificult!)
char total_enemies  = 0; // This will change in realtime (more enemies, more dificult!)

// Splash logo 128x64
const unsigned char PROGMEM splash_logo [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 48, 192, 0, 0, 0, 0, 15, 224, 0, 0, 0, 0, 0, 0, 0, 224, 112, 0, 0, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 224, 240, 0, 0, 0, 0, 0, 96, 0, 0, 0, 0, 0, 0, 0, 1, 225, 224, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 1, 243, 96, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 1, 182, 97, 131, 240, 254, 7, 192, 192, 3, 248, 31, 248, 15, 128, 0, 3, 60, 195, 6, 1, 140, 28, 193, 135, 128, 28, 49, 156, 24, 192, 0, 3, 60, 195, 12, 1, 128, 48, 97, 129, 128, 12, 49, 140, 48, 192, 0, 3, 56, 195, 12, 1, 128, 48, 97, 129, 128, 12, 49, 140, 48, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 0, 24, 99, 24, 96, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 7, 248, 99, 24, 127, 192, 0, 6, 1, 134, 24, 3, 0, 96, 195, 3, 12, 24, 99, 24, 96, 0, 0, 12, 3, 12, 48, 6, 0, 193, 134, 6, 24, 48, 198, 48, 192, 0, 0, 12, 3, 12, 48, 6, 0, 193, 135, 6, 24, 48, 198, 48, 192, 0, 0, 12, 3, 12, 56, 6, 0, 231, 3, 6, 24, 48, 198, 48, 192, 0, 0, 24, 6, 24, 31, 12, 0, 124, 1, 252, 15, 225, 140, 96, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 14, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 17, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 65, 56, 227, 148, 32, 156, 243, 231, 39, 62, 0, 0, 0, 0, 0, 0, 65, 69, 20, 88, 32, 34, 136, 72, 168, 132, 0, 0, 0, 0, 0, 0, 65, 49, 3, 208, 35, 162, 136, 135, 175, 136, 0, 0, 0, 0, 0, 0, 65, 9, 4, 80, 32, 162, 136, 136, 168, 8, 0, 0, 0, 0, 0, 0, 34, 69, 20, 208, 17, 34, 137, 9, 168, 144, 0, 0, 0, 0, 0, 0, 28, 56, 227, 80, 14, 28, 139, 230, 167, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 7, 28, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 8, 162, 96, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 34, 104, 0, 162, 161, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 34, 152, 0, 162, 33, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 34, 136, 1, 34, 34, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 34, 136, 2, 34, 35, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 166, 152, 4, 34, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 154, 105, 15, 156, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    

// Numbers font (4x5)
const unsigned char PROGMEM number_0 [] = { 96, 144, 144, 144, 96 }; // 0
const unsigned char PROGMEM number_1 [] = { 32, 96, 32, 32, 32 }; // 1    
const unsigned char PROGMEM number_2 [] = { 96, 144, 32, 64, 240 }; // 2    
const unsigned char PROGMEM number_3 [] = { 96, 16, 48, 16, 96 }; // 3    
const unsigned char PROGMEM number_4 [] = { 128, 160, 224, 32, 32 }; // 4    
const unsigned char PROGMEM number_5 [] = { 240, 128, 240, 16, 240 }; // 5    
const unsigned char PROGMEM number_6 [] = { 240, 128, 240, 144, 240  }; // 6   
const unsigned char PROGMEM number_7 [] = { 240, 16, 96, 64, 64 }; // 7    
const unsigned char PROGMEM number_8 [] = { 240, 144, 240, 144, 240  }; // 8    
const unsigned char PROGMEM number_9 [] = { 240, 144, 240, 16, 112 }; // 9    
  
// Ship: 10x7
const unsigned char PROGMEM ship [] = { 112, 0, 243, 0, 121, 128, 63, 192, 121, 128, 243, 0, 112, 0 };    

// enemies: 5x5
const unsigned char PROGMEM enemy1 [] = { 32, 168, 248, 112, 136 }; 
const unsigned char PROGMEM enemy2 [] = { 24, 48, 248, 48, 24 };    
const unsigned char PROGMEM enemy3 [] = { 216, 112, 80, 112, 168 };    
const unsigned char PROGMEM enemy4 [] = { 136, 168, 248, 112, 216 };    
const unsigned char PROGMEM enemy5 [] = { 80, 248, 248, 112, 168 };    

//Enemy explosion
const unsigned char PROGMEM enemy_explode1 [] = { 0, 0, 32, 0, 0 };    
const unsigned char PROGMEM enemy_explode2 [] = { 0, 80, 32, 80, 0 };    
const unsigned char PROGMEM enemy_explode3 [] = { 136, 80, 32, 80, 136 };    
const unsigned char PROGMEM enemy_explode4 [] = { 136, 80, 0, 80, 136 };    
const unsigned char PROGMEM enemy_explode5 [] = {  136, 0, 0, 0, 136 };    

void setup()   {                
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  
  randomSeed(analogRead(0));
  
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
    tPos getTpos(int x, int y);
    boolean isInsideRect(tPos point, tPos rectpos, int width, int height);
    
    // Internals (also public...)
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
         fires[i].y = _y + (BATTESHIP_HEIGHT/2); // Divide in realtime is bad ;)
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

// Helper return tPos from coordinates
tPos Battleship::getTpos(int x, int y)
{
  tPos p;
  p.x = x; p.y= y;
  return p;
}

// Helper:
// Return TRUE if the point is inside a rectagle by given by width and height
boolean Battleship::isInsideRect(tPos point, tPos rectpos, int width, int height)
{
  if ( (point.x >= rectpos.x) && 
       (point.y >= rectpos.y) &&
       (point.x <  (rectpos.x+width)) &&
       (point.y <  (rectpos.y+height)) )
       {
        return true; 
       }
       else { return false; }
}

/*======================================================================
  Enemy class definition
  ======================================================================*/
class Enemy {
  public:
    Enemy();
    void update();
    tPos getPosition();
    void create();
    void destroy();
    
    // Internals (also public...)
    boolean islive; // Enemy is alive!
    int _type;        // Type of enemy
    int _lastx;
    int _lasty;
    tPos position;
    int _x;
    int _y;    
    unsigned long last_millis;  
    unsigned long fire_delay_ms;  
    const unsigned char *sprite;
};

Enemy::Enemy()
{
    islive=false;
    _type=0;
    _lastx=0;
    _lasty=0;
    _x=0;
    _y=0;
    position.x=0;
    position.y=0;
    fire_delay_ms=0;
    last_millis=0;    
}

void Enemy::create()
{
    if (total_enemies < MAX_ENEMIES)
    {
    
      islive=true;
      _type= random(1, 5);    
      position.x = 100;
      position.y = random(10, 50);
      _lastx=_x;
      _lasty=_y;
      
      last_millis = millis();
      fire_delay_ms = random(200,2500);
      
      total_enemies++;
    }
}


void Enemy::update()
{
  
  if (!islive){return;}
    
  float animx;
  float animy;
  
  switch (_type)
  {
     case 1: {
       sprite=enemy1; 
       animx = (0.001*(millis()-last_millis));
       animy = (0.003*(millis()-last_millis));
       _x = position.x+sin(animx)*20;
       _y = position.y+cos(animx)*16;       
     } break;
     case 2: {
       sprite=enemy2; 
       animx = (0.002*(millis()-last_millis));
       animy = (0.009*(millis()-last_millis));
       _x = position.x+sin(animx)*20;
       _y = position.y+cos(animx)*16;              
     } break;
     case 3: { 
       sprite=enemy3; 
       animx = (0.0005*(millis()-last_millis));
       animy = (0.009*(millis()-last_millis));
       _x = position.x+cos(animx)*20;
       _y = position.y+sin(animx)*16;              
     } break;
     case 4: { 
       sprite=enemy4; 
       animx = (0.003*(millis()-last_millis));
       animy = (0.005*(millis()-last_millis));
       _x = position.x+sin(animx)*20;
       _y = position.y+cos(animx)*16;              
     } break;
     case 5: {
       sprite=enemy5; 
       animx = (0.008*(millis()-last_millis));
       animy = (0.0007*(millis()-last_millis));
       _x = position.x+cos(animx)*20;
       _y = position.y+sin(animx)*16;              
     } break;
  }
  
  display.drawBitmap(_lastx, _lasty,  sprite, 5, 5, 0); // Erase
  
  display.drawBitmap(_x, _y,  sprite, 5, 5, 1); // Draw
  _lastx = _x;
  _lasty = _y;  
  
}

void Enemy::destroy()
{
   display.drawBitmap(_lastx, _lasty,  sprite, 5, 5, 0); // Erase
   islive=false; 
   total_enemies--;
}

/*======================================================================
  Collider class
  ======================================================================*/
class Collider 
{
  public:
    void check(Battleship m_nave, Enemy *m_enemy);
};

//Check for collisions
void Collider::check(Battleship m_nave, Enemy *m_enemy)
{
  
   //Get all our battleship shots
   for (int i=0 ; i<MAX_FIRES; i++)
   {
     if (m_nave.fires[i].x > 0)
     {
       
       // Check all enemies position for collision
       for (int j=0;j<MAX_ENEMIES ; j++)
       {
         //fires[i].x
         //fires[i].y     
         if (m_enemy[j].islive)
         {
           if (m_nave.isInsideRect(m_nave.getTpos(m_nave.fires[i].x, m_nave.fires[i].y), 
                            m_nave.getTpos(m_enemy[j]._x, m_enemy[j]._y), 
                            ENEMY_WIDTH, ENEMY_HEIGHT))  
                          {
                            // Alien collided!
                            m_enemy[j].destroy();
                            m_nave.fires[i].x=0;
                          }  
         }
       }
     }
   }
}

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
Enemy enemies[MAX_ENEMIES];
Collider collisions;

float inc=0;
unsigned long last_fire=millis();
unsigned long last_enemy=millis();
unsigned long last_time=millis();
int fps=0;

void printNumbers(int x, int y, char* str) {
  
  for (int i=0; i<strlen(str) ;i++)
  {
            if (str[i] == 0) { display.drawBitmap(30, 30,  number_0, 4, 5, 1);  }
            if (str[i] == 1) { display.drawBitmap(30, 30,  number_1, 4, 5, 1);  }
            if (str[i] == 2) { display.drawBitmap(30, 30,  number_2, 4, 5, 1);  }
            if (str[i] == 3) { display.drawBitmap(30, 30,  number_3, 4, 5, 1);  }
            if (str[i] == 4) { display.drawBitmap(30, 30,  number_4, 4, 5, 1);  }
            if (str[i] == 5) { display.drawBitmap(30, 30,  number_5, 4, 5, 1);  }
            if (str[i] == 6) { display.drawBitmap(30, 30,  number_6, 4, 5, 1);  }
            if (str[i] == 7) { display.drawBitmap(30, 30,  number_7, 4, 5, 1);  }
            if (str[i] == 8) { display.drawBitmap(30, 30,  number_8, 4, 5, 1);  }
            if (str[i] == 9) { display.drawBitmap(30, 30,  number_9, 4, 5, 1);  }

  }
}

/*======================================================================
  Main loop
  ======================================================================*/
void loop() {
  
  char x=0;
  
  nave.setPosition(0, 32+sin(inc)*20);
  
  // Create fake fire
  if (millis()-last_fire > 400)
  {
     nave.Fire(); 
     last_fire=millis();
  }
  
  // Create enemies
  if (millis()-last_enemy > 1000)
  {
      if (total_enemies < max_enemies)
      {
       enemies[total_enemies].create();        
       last_enemy=millis();
      }
  }  
  
  inc+=0.06;
           
     stars.update();
     nave.update();  
     collisions.check(nave, enemies);
     
     for(x=0;x<MAX_ENEMIES;x++)
     {
       enemies[x].update();
     }
     
    display.drawBitmap(20, 20,  enemy_explode1, 5, 5, 1);     
    display.drawBitmap(20, 30,  enemy_explode2, 5, 5, 1);     
    display.drawBitmap(20, 40,  enemy_explode3, 5, 5, 1);     
    display.drawBitmap(20, 50,  enemy_explode4, 5, 5, 1);     
    display.drawBitmap(20, 59,  enemy_explode5, 5, 5, 1);   
  
    printNumbers(20, 20, "1234");
    
     
    // Draw framebuffer
    display.display();    
   
  //FPS counter 
  if (millis()-last_time > 1000)
  {
     last_time=millis();
     Serial.print(fps);
     Serial.println(" FPS");
     fps=0;
  }    
  else
  {
     fps++; 
  }
  
}


