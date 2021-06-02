#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define pot A0
#define button 2
#define pinSignal 3 
#define segMax 120

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void boot();
void config();
void printHoras();


bool fConfig = 1;

int value;
int segundos, minutos, horas, dias,
    uniSeg, uniMin, uniHoras, uniDias;
int periodoSelect;

unsigned long timer;
unsigned long photos;
unsigned long picTimer;
unsigned long seg;

void setup() 
{
  pinMode (button, INPUT);
  pinMode (pinSignal, OUTPUT);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.setTextColor(WHITE); 
  display.setTextSize(2); 
  display.clearDisplay();
  display.dim(0);
  display.display();
  delay (100); 

  boot();
  config();
}

void loop() 
{  
  printHoras();
  atualizaTimer();
  tiraFoto();
}

void boot()
{ 
  display.setCursor(10, 10); 
  display.print("Iniciando");    
  display.display();
  
  display.setCursor(10, 40);
  
  display.print(".");
  display.display();
  delay (500);
  
  display.print(".");
  display.display();
  delay (500);
  
  display.print(".");
  display.display();
  delay (500);

  display.clearDisplay();
  display.display();
}

void config()
{
  while (!digitalRead(button))
  {
    display.clearDisplay();
  
    display.setCursor (10, 10);
    display.print ("Periodo: ");
  
    display.setCursor (10, 40);
    display.print (value);
  
    display.print('\'');
    display.display();
  
    display.display();

    int val = analogRead(pot);
    value = map (val, 0, 1023, 1, segMax);
  }
  periodoSelect = value;
  display.clearDisplay();
  display.display();
  delay (100);
}


void atualizaTimer()
{
  if ((millis() - timer) > 1000)
  {
    uniSeg++;
    seg++;
    timer = millis();
  }
  
  if (uniSeg == 10)
  {
    uniSeg = 0;
    segundos++;
  }
  
  if (segundos == 6)
  {
    segundos = 0;
    uniMin++;
  }

  if (uniMin == 10)
  {
    uniMin = 0;
    minutos++;
  }
  
  if (minutos == 6)
  {
    minutos = 0;
    uniHoras++;
  }


  if (uniHoras == 10)
  {
    uniHoras = 0;
    horas++;
  }

  if (horas == 10)
  {
    uniSeg = 0;
    segundos = 0;
    uniMin = 0;
    minutos = 0;
    uniHoras = 0;
    horas = 0;
    uniDias = 0;
    dias = 0;
  }
}

void printHoras()
{
  display.clearDisplay();
  
  display.setCursor(15, 5);
  display.print (horas);
  display.print (uniHoras);
  display.print (':');
  display.print (minutos);
  display.print (uniMin);
  display.print (':');
  display.print (segundos);
  display.print (uniSeg);

  display.setCursor(15, 25);
  display.print ("1:");
  display.print (periodoSelect);
  display.print ('\'');

  display.setCursor (15, 45);
  display.print ("Pic:");
  display.print (photos);
  
  display.display();
}

void tiraFoto()
{
  if (seg == periodoSelect)
  {
    photos++;
    seg = 0;
    digitalWrite (pinSignal, 1);
    delay (2);
    digitalWrite (pinSignal, 0);
  }
}
