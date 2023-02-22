#pragma config(Sensor, S1,     left,           sensorEV3_Ultrasonic)
#pragma config(Sensor, S2,     front,          sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          motorL,        tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          klepetoR,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorR,        tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorD,          klepetoL,      tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const float wheel_base = 200; //rozvor kol robota
const float wheel_diameter = 140; //prumer kol robota
int speed = 20; //rychlost motoru
int speed_k = 20;



#include "senzory.h"

//tento program je pro autonomni bear rescue pro robata 'medved'


void open_klepeto(short open_deg){//o kolik stupnu se otevre klepeto
resetMotorEncoder(klepetoR);
resetMotorEncoder(klepetoL);
moveMotorTarget(klepetoR, open_deg, speed_k);
delay(50);
moveMotorTarget(klepetoL, open_deg, speed_k);
waitUntilMotorStop(klepetoR);
waitUntilMotorStop(klepetoL);
}

void turn(float turnindeg, short speedL, short speedR)//o kolik se ma otocit robot ve stupnich
{
	resetMotorEncoder(motorL);
	resetMotorEncoder(motorR);
	moveMotorTarget(motorL, turnindeg*wheel_base/wheel_diameter, speedL);
	moveMotorTarget(motorR, turnindeg*wheel_base/wheel_diameter, speedR);
	waitUntilMotorStop(motorL);
	waitUntilMotorStop(motorR);
}

void forward1(short lenght) //delka co ma robot ujet v mm
{
	resetMotorEncoder(motorL);
	resetMotorEncoder(motorR);
	moveMotorTarget(motorL, lenght*360/(PI*wheel_diameter), speed+5);
	moveMotorTarget(motorR, lenght*360/(PI*wheel_diameter), speed);
	waitUntilMotorStop(motorL);
	waitUntilMotorStop(motorR);
}

#include "roh.h"





void esko(){//program na projeti eska
forward1(900);
turn(90,20,-20);
forward1(400);
turn(90,20,-20);
forward1(500);
turn(90,-20,20);
forward1(450);
turn(90,-20,20);
forward1(1100);
}



void buttons(){
	while(true){
		if(getButtonPress(buttonUp)==1)//pokud se zmackne tlacitko nahoru zacne robot hledat medveda v hristy
			{
			esko();
			//open_klepeto(90);
			bear_search_left_sensor();
			//turn(90,-20,20);


			}
				if(getButtonPress(buttonDown)==1)//pokud se zmackne tlacitko dolu zacne robot hledat medveda v rohu
				{
				esko();
				roh();



				}
					if(getButtonPress(buttonLeft)==1)//pokud se zmackne tlacitko nahoru zacne robot hledat medveda u zdi
				{
				esko();


				}
	}
}














task main()
{
buttons();
}
