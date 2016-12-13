#include "main.h"

int value[5] = {0};
float  pwm = 90;
int check = 0;
int threshold[5] = {2500, 200, 200, 200, 200};
TM_SERVO_t servo1;
TM_OneWire_t temperature;

int main(void)
{
	int i, srf = 0, lastStatus[5] = {0};
	char temp[16] = {"WELCOME"};
	int status[5] = {0};
	float angle[5] = {20, 50, 90, 130, 160};
	float pwm_safe = 180;

	init();

	TM_SERVO_SetDegrees(&servo1, 90);
//	delayMs(1000);
	Delay(1000);

//	lcd_clear()
//	sonarSetRange(0xE0, 6000);
//	sonarSetGain(0xE0, 14);
    while(1)
    {
    	sonarRange(0xE0);
    	srf = sonarGet(0xE0);
    	readFotodiode(value, status);
    	if(srf > 5000){
//    		kirim info ke user lewat localhost
        	TM_SERVO_SetDegrees(&servo1, pwm_safe);
    	}
    	else{
			if(check > 0){
				for(i = 0; i < 5; i++){
					lastStatus[i] = status[i];
				}
			}

			if(status[2] == 1){
				pwm = angle[2];
			}
			else if(status[1] == 1){
				pwm = angle[1];
			}
			else if(status[3] == 1){
				pwm = angle[3];
			}
			else if(status[0] == 1){
				pwm = angle[0];
			}
			else if (status[4] == 1 ){
				pwm = angle[4];
			}
			else{
				if(lastStatus[2] == 1){
					pwm = (angle[2] + angle[3]) / 2;
				}
				else if(lastStatus[1] == 1){
					pwm = (angle[1] + angle[2]) / 2;
				}
				else if(lastStatus[0] == 1){
					pwm = (angle[0] + angle[1]) / 2;
				}
				else if(lastStatus[3] == 1){
					pwm = (angle[3] + angle[4]) / 2;
				}
				else{
					//Solar panel doesn't get uv light from the sun
					pwm = 90;
				}
			}
    	}

    	if(pwm > MAX_ANGLE) pwm = MAX_ANGLE;
    	if(pwm < MIN_ANGLE) pwm = -1 * MIN_ANGLE;

    	//Set servo to the certain angle (according to pwm value)
    	TM_SERVO_SetDegrees(&servo1, pwm);


//    	delayMs(1000);
//    	while(1);
    }
}

void init(){
	adc1Init();
//	delayInit();
	i2c2Init();
//	lcd_init();
//	delayMs(1000);
	TM_DELAY_Init();
	uartInit();
	//Using PD14 as PWM, in channel 3 and Pinspack 2
	TM_SERVO_Init(&servo1, TIM4, TM_PWM_Channel_3, TM_PWM_PinsPack_2);
	TM_OneWire_Init(&temperature, GPIOA, 6);

}

void readFotodiode(int value[5], int status[5]){
	int i;
	for(i = 0; i < 5; i++){
		value[i] = adc1Read(i);
		if(value[i] > threshold[i]){
			status[i] = 1;
		}
		else{
			status[i] = 0;
		}
	}
}
