#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

//Define buttons
#define BUTTON_UP     A0
#define BUTTON_DOWN   A1
#define BUTTON_LEFT   2
#define BUTTON_RIGHT  A3
#define BUTTON_A      4
#define BUTTON_B      5

#define MAX_STARS          10 // Starfield config. The more, the poor...
#define SCOREBOARD_HEIGHT  8
#define MAX_FIRES          3 // Max player fires at the same time
#define BATTESHIP_WIDTH    10
#define BATTESHIP_HEIGHT   7
#define BATTESHIP_FIRE_WIDTH    3  //Width of the shot
#define BATTESHIP_FIRE_SPEED    3 
#define BATTESHIP_FIRE_DELAY    90 //delay between shots in ms

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
int max_enemies  = 3; // This will change in realtime (more enemies, more dificult!)
int total_enemies  = 0; // This will change in realtime (more enemies, more dificult!)
int current_score = 0; // Current player score
int max_lives     = 3; // Total lives to play

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
const unsigned char PROGMEM ship [] = {  240, 0, 249, 0, 48, 128, 127, 192, 48, 128, 249, 0, 240, 0 };    

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
  
  //Randomize!
  randomSeed(analogRead(0));
  
  //Setup controls  
  pinMode(BUTTON_UP, INPUT);  
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);  

  // Set default text size
  display.setTextSize(1);
  display.setTextColor(WHITE);  
    
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

//Define a 2D point
struct tPos {
   int x;
   int y; 
};

// define a button
struct control_def {
    boolean currentState;
    boolean lastState;
    boolean debouncedState;
    int debounceInterval;
    unsigned long timeOfLastButtonEvent;  
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
    int fire_count;
    tPos fires[MAX_FIRES];
    unsigned long last_fire;
};

Battleship::Battleship()
{
    int i;
    
   _x = 0;
   _y = 32 - (BATTESHIP_HEIGHT/2); 
   _velocity=2;
   fire_count=0;
   last_fire=0;
   for (i=0;i<MAX_FIRES;i++) { fires[i].x=0; }
}

void Battleship::moveRight()   { if (_x < (128-BATTESHIP_WIDTH)-2)  { _x ++; } }
void Battleship::moveLeft()    { if (_x > 0)                        { _x --; } }
void Battleship::moveUp()      { if (_y > 8)                        { _y --; } }
void Battleship::moveDown()    { if (_y < (64-BATTESHIP_HEIGHT))    { _y ++; } }
void Battleship::setPosition(int x, int y) { _x=x; _y=y; }
void Battleship::Fire()
{
  Serial.print("fire_count: ");
  Serial.println(fire_count, DEC);

  if (fire_count < MAX_FIRES)
  {
    if (millis()-last_fire > BATTESHIP_FIRE_DELAY)
    {
     for (int i=0 ; i<MAX_FIRES; i++)
     {
        Serial.print(fires[i].x); Serial.print("-");
       if (fires[i].x == 0)
       {
         fires[i].x = _x + BATTESHIP_WIDTH;
         fires[i].y = _y + (BATTESHIP_HEIGHT/2); // Divide in realtime is bad ;)
         fire_count++;
         last_fire=millis();
         break;
       }
     }
    }
  }
}

void Battleship::update()
{
  
  int i,j;
  
  // Update position
  display.drawBitmap(_x, _y,  ship, BATTESHIP_WIDTH, BATTESHIP_HEIGHT, 1);

  // Update fire
  for (i=0 ; i<MAX_FIRES ; i++)
  {
    //BATTESHIP_FIRE_WIDTH     
    if (fires[i].x > 0)
    { 
      fires[i].x += BATTESHIP_FIRE_SPEED;
      for (j=0 ; j<BATTESHIP_FIRE_WIDTH ; j++) { display.drawPixel(fires[i].x-j, fires[i].y, WHITE); } // Draw 3 pixels back for the full shot      
      
      //Shot is dead
      if (fires[i].x > (128-BATTESHIP_FIRE_WIDTH))
      {        
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
    boolean isdead; // Enemy died!
    int _type;        // Type of enemy
    int _deadanim_num;
    tPos position;
    int _x;
    int _y;    
    unsigned long last_millis;  
    unsigned long fire_delay_ms;  
    const unsigned char *sprite;
    int reward_points;
};

Enemy::Enemy()
{
    islive=false;
    isdead=false;
    _type=0;
    _x=0;
    _y=0;

    _deadanim_num=-1;

    position.x=0;
    position.y=0;
    fire_delay_ms=0;
    last_millis=0;    

    reward_points=0;
}

void Enemy::create()
{
    if (total_enemies < MAX_ENEMIES)
    {
    
      islive=true;
      isdead=false;
      _type= random(1, 5);    

      switch (_type)
      {
        case 1: reward_points=1; break;
        case 2: reward_points=2; break;
        case 3: reward_points=3; break;
        case 4: reward_points=4; break;
        case 5: reward_points=5; break;
      }

      position.x = 100;
      position.y = random(10, 50);

      _deadanim_num=-1;
      
      last_millis = millis();
      fire_delay_ms = random(200,2500);
      
      total_enemies++;
    }
}


void Enemy::update()
{
  
  if ( (!islive) ) {return;}
    
  float animx;
  float animy;

  if (_deadanim_num == -1)      
  {
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

    display.drawBitmap(_x, _y,  sprite, 5, 5, 1); // Draw
  }
  else
  {

      // Enemy dead animation
      if (_deadanim_num == 0) { display.drawBitmap(_x, _y,  enemy_explode1, 5, 5, 1); }
      if (_deadanim_num == 1) { display.drawBitmap(_x, _y,  enemy_explode2, 5, 5, 1); }
      if (_deadanim_num == 2) { display.drawBitmap(_x, _y,  enemy_explode3, 5, 5, 1); }
      if (_deadanim_num == 3) { display.drawBitmap(_x, _y,  enemy_explode4, 5, 5, 1); }
      if (_deadanim_num == 4) { display.drawBitmap(_x, _y,  enemy_explode5, 5, 5, 1); }

      if ( (millis()-last_millis) > 90) { 
        _deadanim_num++;
        last_millis=millis();
        if (_deadanim_num > 4) {
           // End of dead animation
           _deadanim_num=0;
           islive=false; 
           total_enemies--;                     
        }
      }      
  }
  
}

void Enemy::destroy()
{   
   _deadanim_num = 0;
   last_millis=millis();
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
           // Check if we hit some alien
           if (m_nave.isInsideRect(m_nave.getTpos(m_nave.fires[i].x, m_nave.fires[i].y), 
                            m_nave.getTpos(m_enemy[j]._x, m_enemy[j]._y), 
                            ENEMY_WIDTH, ENEMY_HEIGHT))  
                          {                            

                              m_nave.fires[i].x = 0;   // Stop fire                         
                              m_nave.fires[i].y = 0;   // Stop fire                                                       

                            if (m_enemy[j]._deadanim_num == -1) // If not allready in dead mode
                            {
                              // Alien collided!                            

                              current_score += m_enemy[j].reward_points; // Increase current points based on enemy value

                              m_enemy[j].destroy(); // Destroy enemy
                              m_nave.fires[i].x=0;
                            }
                          }  
         }
       }
     }
   }

   // Check if some alien hit the battelship
   for (int j=0;j<MAX_ENEMIES ; j++)
   {
     //fires[i].x
     //fires[i].y     
     if (m_enemy[j].islive)
     {   
         if (m_nave.isInsideRect(m_nave.getTpos(m_enemy[j]._x, m_enemy[j]._y), 
                                 m_nave.getTpos(m_nave._x, m_nave._y), 
                                 ENEMY_WIDTH, ENEMY_HEIGHT))
                                 {
                                    m_nave._x = 0;
                                    m_nave._y = 32;                                    
                                    max_lives--;
                                                                       
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
  for (int i=0; i<MAX_STARS ; i++)
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

/*======================================================================
  Controls class definition
  ======================================================================*/
class Controls {
  public:
    Controls();
    boolean getButton(int button_pin);
    
    control_def _ctrl[6];
};  

Controls::Controls()
{
    for(int i=0;i<6;i++)
    {
      _ctrl[i].currentState = LOW;//stroage for current measured button state
      _ctrl[i].lastState = LOW;//storage for last measured button state
      _ctrl[i].debouncedState = LOW;//debounced button state      
      _ctrl[i].debounceInterval = 20;//wait 20 ms for button pin to settle
      _ctrl[i].timeOfLastButtonEvent = 0;//store the last time the button state changed        
    }
}

boolean Controls::getButton(int button_pin)
{
  
  //---
  int pin;
  
  if (button_pin == BUTTON_UP) { pin=0; }
  if (button_pin == BUTTON_DOWN) { pin=1; }
  if (button_pin == BUTTON_LEFT) { pin=2; }
  if (button_pin == BUTTON_RIGHT) { pin=3; }
  if (button_pin == BUTTON_A) { pin=4; }
  if (button_pin == BUTTON_B) { pin=5; }
  
  _ctrl[pin].currentState = digitalRead(button_pin);
  unsigned long currentTime = millis();
  
  if (_ctrl[pin].currentState != _ctrl[pin].lastState){
    _ctrl[pin].timeOfLastButtonEvent = currentTime;
  }

  if (currentTime - _ctrl[pin].timeOfLastButtonEvent > _ctrl[pin].debounceInterval){//if enough time has passed
    if (_ctrl[pin].currentState != _ctrl[pin].debouncedState){//if the current state is still different than our last stored debounced state
      _ctrl[pin].debouncedState = _ctrl[pin].currentState;//update the debounced state
      
      //trigger an event
      if (_ctrl[pin].debouncedState == HIGH){
        return true;
      } else {
        return false;
      }
    }
  }

  _ctrl[pin].lastState = _ctrl[pin].currentState;
  
}

/*======================================================================
  Scores class definition
  ======================================================================*/
class Scores {

  public:
    Scores();
    void draw();    

};

Scores::Scores()
{

}

void Scores::draw()
{    

    // Display score
    display.setCursor(0,0);
    display.print("SCORE: ");     
    display.print(current_score);       

    for (int i=0 ; i<max_lives ; i++)
    {
        display.drawBitmap(118-((i*(BATTESHIP_WIDTH+3))), 0,  ship, BATTESHIP_WIDTH, BATTESHIP_HEIGHT, 1);
    }    

    display.drawLine(0, 8, 128, 8, WHITE);    
}

// Main games object clases
Battleship  nave;
Starfield   stars;
Enemy       enemies[MAX_ENEMIES];
Collider    collisions;
Controls    controls;
Scores      score;

float inc=0;
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
  
  int x=0;
  
  display.clearDisplay();
  
  //nave.setPosition(0, 32+sin(inc)*20);
  if (controls.getButton(BUTTON_UP)) { nave.moveUp(); }
  if (controls.getButton(BUTTON_DOWN)) { nave.moveDown(); }
  if (controls.getButton(BUTTON_LEFT)) { nave.moveLeft(); }
  if (controls.getButton(BUTTON_RIGHT)) { nave.moveRight(); }    
  if (controls.getButton(BUTTON_A)) { nave.Fire(); }
  
  // Create enemies
  if (millis()-last_enemy > 1000)
  {
      if (total_enemies < max_enemies)
      {
       enemies[total_enemies].create();        
       last_enemy=millis();
      }
  }  

  if (digitalRead(BUTTON_UP)) { Serial.println("UP");  }  
  if (digitalRead(BUTTON_DOWN)) { Serial.println("DOWN");  }  
  if (digitalRead(BUTTON_LEFT)) { Serial.println("LEFT");  }
  if (digitalRead(BUTTON_RIGHT)) { Serial.println("RIGHT");  }  
  if (digitalRead(BUTTON_A)) { Serial.println("A");  }
  if (digitalRead(BUTTON_B)) { Serial.println("B");  }
  inc+=0.06;
           
     stars.update();
     nave.update();  
     collisions.check(nave, enemies);
     
     for(x=0;x<MAX_ENEMIES;x++)
     {
       enemies[x].update();
     }

     score.draw();     
          
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


