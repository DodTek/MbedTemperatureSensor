# MbedTemperatureSensor
A sensor that uses the temperature sensor on an mbed application board to flash leds based on the temperature range which can be set by the potentiometers attached to the board and when the temperature reaches a certain range a buzzer goes off 
-if the temperature is less than the lower threshold the led will be green
-if the temperature is between the lower and higher threshold the led will flash orange and a buzzer will sound
-if the temperature goes beyond the higher threshold then the led will flash red rapidly and the buzzer will sound with a higher pitch
-each potentiometer changes the value for the lower and upper threshold respectively
