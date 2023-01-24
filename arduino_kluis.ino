#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
char* passWord = "0512";

int position = 0;
int wrong = 0;

char newCode[4] = "";
int newCodePos = 0;

int buzzer = 11;
int ButtonValue = 0;
int Button = A0;

char keys[4][4]= {    //hoe ik het keypad moest gebruiken vond ik hier: https://arduinogetstarted.com/tutorials/arduino-keypad
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {9,8,7,6};
byte colPins[4] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

#define RED_LED 12
#define GREEN_LED 13

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  setLocked(false);
  servo.attach(10);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(Button, INPUT);
}

void loop() {
  ButtonValue = digitalRead(Button);
  if(ButtonValue != 0)
  {
    programFunction();
  }
  else
  {
    mainFunction();
  }
  delay(200);
}


void programFunction()
{
  char key = keypad.getKey();
  if(key >= '0' && key <= '9')
  {
    newCode[newCodePos] = key;
    position++;
    lcd.setCursor (position,0);
    lcd.print(key);
    newCodePos++;

    if(position == 4)
    {
      passWord = newCode;
      changedSound();
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("Pincode changed!");
      position = 0;
    }
  }
}

void mainFunction()
{
  char key = keypad.getKey();

  if((key >= '0' && key <= '9') || (key == '#'))
  {
    if(key == '#')
    {
      position = 0;
      wrong = 0;
      setLocked(true);
    }

    else if(key == passWord[position])
    {
      position++;
      wrong = 0;
      lcd.setCursor (position,0);
      lcd.print(key);
    }
    
    else if(key != passWord[position])
    {
      position = 0;
      wrong++;
      lcd.setCursor (wrong,0);
      lcd.print(key);
    }

    if(wrong == 4)
    {
      deniedSound();
      wrong = 0;
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("locked");
    }

    if(position == 4)
    {
      accesSound();
      setLocked(false);
      position = 0;
    }
  }
}

void accesSound()
{
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
}

void changedSound()
{
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
}

void deniedSound()
{
  tone(buzzer, 450);
  delay(300);
  noTone(buzzer);
}

void setLocked(int locked)
{
  if(locked)
  {
    delay(500);
    servo.write(180);
    lcd.clear();
    lcd.setCursor (0,1);
    lcd.print("locked");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }

  else
  {
    delay(500);
    servo.write(120);
    lcd.clear();
    lcd.setCursor (0,1);
    lcd.print("opened");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}
