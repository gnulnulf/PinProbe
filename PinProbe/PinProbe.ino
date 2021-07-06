//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels


#define RXPIN 0
#define TXPIN 1
#define DIGITALIN 2
#define DIGITALOUT 3
#define IOSWITCH 4
//#define BUZZER 4
#define RED 5
#define YELLOW 6
#define GREEN 7
#define FLOAT 8

#define PWMPIN 9

//spi display:
//#define DC 9
//#define SPICS 10
//#define SPIMOSI 11
//#define SPIMISO 12
//#define SPICLK 13
#define LED 13


#define ANALOGIN A7
#define BUTTON1 14
#define BUTTON2 15
#define BUTTON3 16
#define BUTTON4 17
#define SDAPIN A4
#define SCLPIN A5

// pwm test
// PWM frequency from D3, D9, D10, and D11:
// 490.20 Hz (The DEFAULT)


// init oled screen 128x32
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ######################################################################
// vars
// ######################################################################

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

long freqtime = 0;
long millitime = 0;
volatile int pulses = 0;
volatile long freqcount = 0;
  long frequency;

String line1="PinProbe";
String line2="V0.1";
String line3="line3";
String line4="line4";
String line5="line5";

int inject =0; 
long injecttime;


// ######################################################################
// pulse_isr
// ######################################################################
void pulse_isr() {
  // to enable the pulse led
  pulses=8;
  // for a simple freq counter
  freqcount++;
}

// ######################################################################
// updatescreen 
// ######################################################################

void updateDisplay() {
  display.display();
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
  display.setCursor(0, 17);
  display.println(line2);
  display.display();    
  
}

void updateDisplay4() {
  display.display();
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
//  display.setCursor(0, 17);
  display.println(line2);
  display.println(line3);
  display.println(line4);
  display.println(line5);
  display.display();    
  
}


// ######################################################################
// Setup 
// ######################################################################
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
 

pinMode(BUTTON1, INPUT_PULLUP);
pinMode(BUTTON2, INPUT_PULLUP);
pinMode(BUTTON3, INPUT_PULLUP);
pinMode(BUTTON4, INPUT_PULLUP);
pinMode(IOSWITCH, INPUT_PULLUP);

pinMode(DIGITALIN, INPUT_PULLUP);


  pinMode(13, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(FLOAT, OUTPUT);
  pinMode(DIGITALOUT, OUTPUT);

pinMode(PWMPIN,OUTPUT);
pinMode(11,OUTPUT);

//TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
   TIMSK0 = 0;  // no interrupts
   Timer0::setMode (7, Timer0::PRESCALE_1, Timer0::CLEAR_B_ON_COMPARE);
   OCR0A = 3;   // count to 4, zero-relative
   OCR0B = 1;   // duty cycle

millitime = millis();
freqtime = millis();
injecttime = millis();
//attachInterrupt(digitalPinToInterrupt(2), pulse_isr, CHANGE);
attachInterrupt(digitalPinToInterrupt(DIGITALIN), pulse_isr, RISING);


}


// ######################################################################
// Main loop
// ######################################################################
void loop() {
  //pwm test
  analogWrite(PWMPIN,155);
  analogWrite(11,155);
  
  if ( digitalRead(IOSWITCH)== HIGH) {
    parseProbe();
  } else {
    parseInject();
  }
} //loop

void parseInject() {
    line1="INJECT";
    line2="";
    line3="";
    line4 = "BUT:" + String(  digitalRead(BUTTON1), BIN)+ String(  digitalRead(BUTTON2), BIN)+ String(  digitalRead(BUTTON3), BIN)+ String(  digitalRead(BUTTON4), BIN);

    if ( digitalRead(BUTTON1) == LOW ) {
        pinMode(DIGITALOUT, OUTPUT);
        digitalWrite( GREEN, LOW );
        digitalWrite( RED, HIGH );
        digitalWrite( YELLOW, HIGH );
        
        digitalWrite(DIGITALOUT, LOW);
    } else {
      if ( digitalRead(BUTTON2) == LOW ) {
          pinMode(DIGITALOUT, OUTPUT);
          digitalWrite( GREEN, HIGH );
          digitalWrite( RED, LOW );
          digitalWrite( YELLOW, HIGH );
          digitalWrite(DIGITALOUT, HIGH);
      } else {
        if ( digitalRead(BUTTON3) == LOW ) {
          pinMode(DIGITALOUT, OUTPUT);
          digitalWrite( GREEN, ( inject != 0 ) );
          digitalWrite( RED, ( inject == 0 ) );
          digitalWrite( YELLOW, LOW);
          digitalWrite(DIGITALOUT, ( inject == 0 ) );
        }
      }
    }


    
    if ( digitalRead(BUTTON1) == HIGH && digitalRead(BUTTON2) == HIGH && digitalRead(BUTTON3) == HIGH  ) {
        pinMode(DIGITALOUT, INPUT);
          digitalWrite( YELLOW, HIGH );
          digitalWrite( RED, HIGH );
          digitalWrite( GREEN, HIGH );
          digitalWrite( FLOAT, HIGH );
    }
      updateDisplay4();
  if ( millis() > injecttime + 500 ) {
    inject = ~inject;
    injecttime = millis();
  }
}


// ######################################################################
// parseProbe
// ######################################################################
void parseProbe() {
    line1="PROBE";
  //digitalWrite(LED,  digitalRead(BUTTON1) == LOW ) ;

  // put your main code here, to run repeatedly:
  int din = digitalRead(DIGITALIN);

  sensorValue = analogRead(ANALOGIN);
  //sprintf( line1 , "A:4%d F:%d",sensorValue,frequency)  ;  
  line2 = "A:"+String(sensorValue,DEC);
  line3 = "F:"+String(frequency,DEC);
  line4 = "BUT:" + String(  digitalRead(BUTTON1), BIN)+ String(  digitalRead(BUTTON2), BIN)+ String(  digitalRead(BUTTON3), BIN)+ String(  digitalRead(BUTTON4), BIN);

  //red LED
  digitalWrite(RED, (din == LOW) ); 

  //green LED
  //  digitalWrite(GREEN, (din == HIGH) ) ;
  if ( sensorValue < 280  ) {
    digitalWrite(GREEN,  LOW ) ;
  }else{
    digitalWrite(GREEN,  HIGH ) ;
  }

  //pulse LED
  if ( pulses > 1 ) {
    digitalWrite(YELLOW,  LOW ) ;
  } else {
    digitalWrite(YELLOW,  HIGH ) ;
  }
  // float LED
  if ( sensorValue > 310 and sensorValue < 340 ) {
    digitalWrite(FLOAT,  LOW ) ;
  }else{
    digitalWrite(FLOAT,  HIGH ) ;
  }
  updateDisplay4();

  if ( millis() > millitime + 50 ) {
    if ( pulses >0 ) {
      pulses--;
    }
    millitime = millis();
  }

  // every second show pulsecount ~ frequency 
  if ( millis() > freqtime + 1000 ) {
    frequency = (freqcount * 1000 )/ ( millis() - freqtime )  ;
    freqtime = millis();
    freqcount = 0;
  }
} // parseProbe
