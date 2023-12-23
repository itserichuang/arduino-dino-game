#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27,16,2);

int buttonPin = 12;
int buttonRead;

const int POINTS_TO_WIN = 10;

int mem;

float points = 0;

unsigned long long int jumpCounter;

int cactus1pos = 15;
float cactus1loopcounter = 15;
int cactusSpawn1 = 1;

int cactus2pos = 23;
float cactus2loopcounter = 23;
int cactusSpawn2 = 2;

int playerPos = 1;

bool gameover = false;
bool youWin = false;

// dino texture
byte dino[] = {
  B00000,
  B00111,
  B10101,
  B11111,
  B11100,
  B11111,
  B01010,
  B01010
};

// cactus1 texture
byte cactus1[] = {
  B00100,
  B10100,
  B10101,
  B10101,
  B11101,
  B00111,
  B00100,
  B00100
};

// cactus2 texture
byte cactus2[] = {
  B00100,
  B00101,
  B10101,
  B10101,
  B10111,
  B11100,
  B00100,
  B00100
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.backlight();

  randomSeed(analogRead(0));

  lcd.begin(16, 2);
  lcd.clear();

  lcd.createChar(0, dino);
  lcd.home();

  lcd.createChar(1, cactus1);
  lcd.createChar(2, cactus2);

  lcd.setCursor(3, 1);
  lcd.write((byte)0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(gameover == false and youWin == false) {
    // writing number of points onto screen
    lcd.setCursor(7, 0);
    lcd.print("points:");
    if (points < 10) {
      lcd.setCursor(14, 0);
      lcd.print(0);
      lcd.setCursor(15, 0);
      lcd.print(points);
    }
    else {
      lcd.setCursor(14, 0);
      lcd.print(points);
    }

    // putting a cactus onto screen
    lcd.setCursor(cactus1pos + 1, 1);
    lcd.print(" ");
    lcd.setCursor(cactus1pos, 1);
    if (cactusSpawn1 == 1) {
      lcd.write((byte)1);
    }
    else if (cactusSpawn1 == 2) {
      lcd.write((byte)2);
    }

    // putting another cactus onto screen
    lcd.setCursor(cactus2pos + 1, 1);
    lcd.print(" ");
    lcd.setCursor(cactus2pos, 1);
    if (cactusSpawn2 == 1) {
      lcd.write((byte)1);
    }
    else if (cactusSpawn2 == 2) {
      lcd.write((byte)2);
    }
    
    buttonRead = digitalRead(buttonPin);
    if(buttonRead == 0) {
      if(playerPos == 1) {
        playerPos = 0;
        mem = jumpCounter;
      }
   }

    cactus1loopcounter -= 0.5;
    if(cactus1loopcounter == round(cactus1loopcounter) and cactus1loopcounter >= 0) {
      cactus1pos = cactus1loopcounter;
    }
    if(cactus1pos == 3) {
      points += 0.5;
    }
    if(cactus1loopcounter < 0) {
      cactus1loopcounter = 15;
      cactus1pos = 15;
      cactusSpawn1 = random(1,3);
      Serial.println("Cactus Spawn 1: " + cactusSpawn1);
    }

    cactus2loopcounter -= 0.5;
    if(cactus2loopcounter == round(cactus2loopcounter) and cactus2loopcounter >= 0) {
      cactus2pos = cactus2loopcounter;  q
    }
    if(cactus2pos == 3) {
      points += 0.5;
    }
    if(cactus2loopcounter < 0) {
      cactus2loopcounter = 15;
      cactus2pos = 15;
      cactusSpawn2 = random(1,3);
      Serial.println("Cactus Spawn 2: " + cactusSpawn2);
    }

    if(jumpCounter >= mem+4) {
      playerPos = 1;
    }
 
    lcd.setCursor(0, 1);
    lcd.print(" ");
    delay(100);

    // checking if the dino crashed into a cactus
    if(cactus1pos == 3 and playerPos == 1) {
      gameover = true;
      Serial.println("game over");
    }
    // checking the player won the game
    if(points >= POINTS_TO_WIN) {
      youWin = true;
    }

    jumpCounter++;
    if(playerPos == 1) {
      lcd.setCursor(3, 0);
      lcd.print(" ");
      lcd.setCursor(3, 1);
      lcd.write((byte)0);
    }
    if(playerPos == 0) {
      lcd.setCursor(3, 1);
      lcd.print(" ");
      lcd.setCursor(3, 0);
      lcd.write((byte)0);
    }
  }
  else if(gameover == true) {
    lcd.setCursor(12, 0);
    lcd.print("    ");
    lcd.setCursor(3, 0);
    lcd.print("Game Over");
    lcd.setCursor(7, 1);
    lcd.print("points:");
    if (points < 10) {
      lcd.setCursor(14, 1);
      lcd.print(0);
      lcd.setCursor(15, 1);
      lcd.print(points);
    }
    else {
      lcd.setCursor(14, 1);
      lcd.print(points);
    }
  }
  else if(youWin == true) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.write((byte)0);
    lcd.print("You Win!");
    delay(4000);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Thanks");
    lcd.setCursor(2, 1);
    lcd.print("for Playing!");
    delay(4000);
  }
}
