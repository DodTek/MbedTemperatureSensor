
#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"


//Create an LM75B object at the default address (ADDRESS_0)
LM75B sensor(p28, p27);
//rgb leds
PwmOut r(p23);
PwmOut g(p24);
PwmOut b(p25);

AnalogIn pot1(p19);
AnalogIn pot2(p20);

//set screen pins 
C12832 lcd(p5, p7, p6, p8, p11);
//set speaker pin
PwmOut speaker(p26);

Ticker flipper;
Ticker orange_flipper;
Ticker screen_flipper;

float lower_threshold = 10.0;
float higher_threshold = 50.0;
int room_temp;
int flashing_red = 0;
int flashing_orange = 0;

void led_red(){
    r = 0;
    g = 1;
    b = 1;
}

void led_green(){
    r = 1;
    g = 0;
    b = 1;
}

void led_orange(){
    r = 0;
    g = 0;
    b = 1;
}

void led_off(){
    r = 1;
    g = 1;
    b = 1;
}

void print_lcd(){
    lcd.cls();
    lcd.locate(0,1);
    lcd.printf("Lower T= %.1f\n", lower_threshold);
    lcd.locate(0,12);
    lcd.printf("Higher T = %.1f\n", higher_threshold);
    lcd.locate(0,20);
    lcd.printf("Temp = %.1f\n", (float)sensor);
}

void flip_red() {
    flashing_red = !flashing_red;
}

void flip_orange(){
    flashing_orange = !flashing_orange;
    
}

void screen_flip(){
    print_lcd();
}

float map(float x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void set_thresholds(){
    //maps the pots to the thresholds
    lower_threshold = map(pot1,0,1,room_temp,int(higher_threshold));
    higher_threshold = map(pot2,0,1,int(lower_threshold),50);
            
}
int main()
{
    room_temp = (float)sensor;
    flipper.attach(&flip_red, 2.0); // setup flipper to call flip after 2 seconds
     // setup flipper to call flip after 2 seconds
    screen_flipper.attach(&screen_flip, 2.0);
    orange_flipper.attach(&flip_orange, 2.0);
    //pot1 = map(lower_threshold, room_temp, 1023, 0, 255);
    //Try to open the LM75B
        while (1) {
            
            if(((float)sensor < lower_threshold) && (lower_threshold != higher_threshold)){
                led_green();//makes led green r = 1, g = 0, b = 1
            
                set_thresholds();
                speaker =0.0;
                //print_lcd();//prints temperature to screen   
            }
            else if((float)sensor >= higher_threshold){
                set_thresholds();
                speaker.period(0.001);
                if(flashing_red == 1){
                    led_red();
                    speaker =0.95; //95% duty cycle
                    
                }
                else{
                    led_off(); //turns off led
                    speaker =0.95; //95% duty cycle
                    
                }
            }
            else if(((float)sensor >= lower_threshold )&& ((float)sensor < higher_threshold) && (lower_threshold != higher_threshold)){
               
                set_thresholds();
                speaker.period(0.003);
                if(flashing_orange == 1){
                    led_orange();
                    speaker =0.95; //95% duty cycle
                }
                else{
                    led_off(); //turns off led 
                    speaker =0.95; //95% duty cycle
                }   
            }
        }
 } 

