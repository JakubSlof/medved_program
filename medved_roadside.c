#pragma config(Sensor, S1,     back,           sensorEV3_Touch, modeEV3Bump)
#pragma config(Sensor, S2,     left_ultrasonic, sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     front_ultrasonic, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          L_motor,       tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          L_klepeto,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          R_klepeto,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          R_motor,       tmotorEV3_Large, PIDControl, encoder)
//////////////////////////////////////////////////////////////////////////////////////////////////////
const float wheel_diameter = 70; //prumer kola
const float wheel_base = 148; //rozvor kol
const float speed = 50; //rychlost jizdy
////////////////////////////////////////////
int g_front_sensor1 =60;
int g_front_sensor2 =60;
task detection()
{
	while((g_front_sensor2 +g_front_sensor1)/2 >40){
	g_front_sensor1 = getUSDistance(S3);
	g_front_sensor2 = getUSDistance(S2);
  }
  stopAllTasks();
}








//program pro roadside na robot medved
//jizda na encodery dozadu

//jizda na encodery dozadu
void backward(short lenght){ //delka co ma robot ujet v mm
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, lenght*360/(PI*wheel_diameter), -speed);
	moveMotorTarget(R_motor, lenght*360/(PI*wheel_diameter), -speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}
//otoceni robota doprava
void turn_right(short turnindeg, short speed){
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, turnindeg*wheel_base/wheel_diameter, speed);
	moveMotorTarget(R_motor, turnindeg*wheel_base/wheel_diameter, -speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

//otoceni robota doleva
void turn_left(short turnindeg, short speed){
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, turnindeg*wheel_base/wheel_diameter, -speed);
	moveMotorTarget(R_motor, turnindeg*wheel_base/wheel_diameter, speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}
void go_forward(short lenght){ //delka co ma robot ujet v mm
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, lenght*360/(PI*wheel_diameter), speed);
	moveMotorTarget(R_motor, lenght*360/(PI*wheel_diameter), speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}


void back_button(){
setMotorSpeed(R_motor, -speed);
setMotorSpeed(L_motor, -speed);
	while(getTouchValue(back) == 0){
    }
    delay(100);
    setMotorSpeed(R_motor, 0);
		setMotorSpeed(L_motor, 0);
}
task main()
{


while(true){
		if(getButtonPress(buttonUp)){ //pokud se zmackne tlacitko nahoru zacne robot hledat v pasu jedna
			startTask(detection);
		back_button();
go_forward(500);
turn_right(100,20);
go_forward(1000);
backward(100);
turn_right(190,20);
go_forward(890);
turn_left(90,20);
go_forward(400);

		}

	}
///////////////////////

		}


