#pragma config(Sensor, S1,     back,           sensorEV3_Touch, modeEV3Bump)
#pragma config(Sensor, S2,     left_ultrasonic, sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     front_ultrasonic, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          L_motor,       tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          L_klepeto,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          R_klepeto,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          R_motor,       tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const float wheel_diameter = 70; // prumer kola
const float wheel_base = 148;	 // rozvor kol
const float speed = 50;			 // rychlost jizdy
short average_left;				 // konstanata pro plovouci prumer
int g_encoder;					 // promenne tady jen aby se vypisovaly
int g_front_sensor;				 // promenne tady jen aby se vypisovaly
int g_left_sensor = 100;		 // hodnota z leveho senzoru
int a1 = 0,
	a2 = 0,
	a3 = 0,
	a4 = 0,
	a5 = 0;
bool bear = false;
float distance_avg = 0;

typedef enum
{
	ESKO,
	FIELD1,
	FIELD2,
	FIELD3,
	FIELD4,
	WALL1,
	WALL2,
	WALL3,
	WALL4,
	ESKO_BACK,
	HOME1,
	HOME2
} sectors;
sectors state = ESKO;

void obloukright(short radius, float angle_oblouku)
{ // polomer oblouku pro vnejsi kolo
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, ((radius - wheel_base) * 2 * PI) * angle_oblouku / (PI * wheel_diameter), (speed * (radius - wheel_base)) / radius); // vypocet delky oblouku pro kazde kolo
	moveMotorTarget(R_motor, (radius * 2 * PI) * angle_oblouku / (PI * wheel_diameter), speed);													  // vypocet delky oblouku pro kazde kolo
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

void obloukleft(short radius, float angle_oblouku)
{ // polomer oblouku pro vnejsi kolo
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(R_motor, ((radius - wheel_base) * 2 * PI) * angle_oblouku / (PI * wheel_diameter), (speed * (radius - wheel_base)) / radius); // vypocet delky oblouku pro kazde kolo
	moveMotorTarget(L_motor, (radius * 2 * PI) * angle_oblouku / (PI * wheel_diameter), speed);													  // vypocet delky oblouku pro kazde kolo
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

// otevirani klepet
void open_klepeto(short open_deg_R, short open_deg_L)
{ // o kolik stupnu se otevre prave klepeto a o kolik leve
	resetMotorEncoder(R_klepeto);
	resetMotorEncoder(L_klepeto);
	moveMotorTarget(R_klepeto, open_deg_R, 20); // prave klepeto se otevira jako prvni aby se nenabouraly
	delay(50);
	moveMotorTarget(L_klepeto, open_deg_L, 20);
	waitUntilMotorStop(R_klepeto);
	waitUntilMotorStop(L_klepeto);
	delay(100);
}

// zavirani klepet
void close_klepeto()
{ // o kolik stupnu se zavre prave klepeto a o kolik leve
	resetMotorEncoder(R_klepeto);
	resetMotorEncoder(L_klepeto);
	setMotorSpeed(R_klepeto, -20); // leve klepeto se zavira jako prvni aby se nenabouraly
	setMotorSpeed(L_klepeto, -20);
	delay(100);
}

// otoceni robota doprava
void turn_right(short turnindeg, short speed)
{
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, turnindeg * wheel_base / wheel_diameter, speed);
	moveMotorTarget(R_motor, turnindeg * wheel_base / wheel_diameter, -speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

// otoceni robota doleva
void turn_left(short turnindeg, short speed)
{
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, turnindeg * wheel_base / wheel_diameter, -speed);
	moveMotorTarget(R_motor, turnindeg * wheel_base / wheel_diameter, speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

// jizda na enkodery dopredu
void forward1(short lenght)
{ // delka co ma robot ujet v mm
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, lenght * 360 / (PI * wheel_diameter), speed);
	moveMotorTarget(R_motor, lenght * 360 / (PI * wheel_diameter), speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

// jizda na encodery dozadu
void backward1(short lenght)
{ // delka co ma robot ujet v mm
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	moveMotorTarget(L_motor, lenght * 360 / (PI * wheel_diameter), -speed);
	moveMotorTarget(R_motor, lenght * 360 / (PI * wheel_diameter), -speed);
	waitUntilMotorStop(L_motor);
	waitUntilMotorStop(R_motor);
	delay(10);
}

// robot couva dokud se nezmackne tlacitko vzadu
void button_back()
{
	setMotorSpeed(R_motor, -speed);
	setMotorSpeed(L_motor, -speed);
	while (getTouchValue(back) == 0)
	{
	}
	setMotorSpeed(R_motor, 0);
	setMotorSpeed(L_motor, 0);
}

// hledani medveda bocnim senzorem
void field_search()
{
	setMotorSpeed(R_motor, speed);
	setMotorSpeed(L_motor, speed);
	short a1 = 0,
		  a2 = 0,
		  a3 = 0,
		  a4 = 0,
		  a5 = 0,
		  a6 = 0,
		  a7 = 0,
		  a8 = 0,
		  a9 = 0,
		  a10 = 0;
	for (int a = 0; a == 9; a++)
	{
		a1 = getUSDistance(S2);
		a2 = a1;
		a3 = a2;
		a4 = a3;
		a5 = a4;
		a6 = a5;
		a7 = a6;
		a8 = a7;
		a9 = a8;
		a10 = a9;
		delay(60);
	}
	do
	{
		a1 = getUSDistance(S2);
		a2 = a1;
		a3 = a2;
		a4 = a3;
		a5 = a4;
		a6 = a5;
		a7 = a6;
		a8 = a7;
		a9 = a8;
		a10 = a9;
		average_left = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10) / 10;

		delay(60);
	} while (average_left > 100 && average_left < 120);

	setMotorSpeed(R_motor, 0);
	setMotorSpeed(L_motor, 0);
}

void floating_average()
{
	int j = 0;
	int distance[6];
	int distance_sum;

	for (int i = 0; i < 6; i++)
	{ // pocatecni zaplneni pole hodnotami ze senzoru
		distance[i] = getUSDistance(S2);
		delay(20);
	}
	do
	{

		distance[j] = getUSDistance(S2);
		distance_sum = 0;
		for (int i = 0; i < 6; i++)
		{
			distance_sum += distance[i];
		}
		j++;
		distance_avg = distance_sum / 6;
		if (j > 5)
		{
			j = 0;
		}
		delay(100);
	} while (distance_avg < 60);
}

void bear_search(short measuring_speed, short lenght)
{
	int a1 = 0,
		a2 = 0,
		a3 = 0,
		a4 = 0,
		a5 = 0;
	setMotorSpeed(R_motor, measuring_speed);
	setMotorSpeed(L_motor, measuring_speed);
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	while (g_left_sensor >= 90 && g_encoder <= (lenght * 360 / (PI * wheel_diameter)))
	{
		g_encoder = getMotorEncoder(R_motor);
		a1 = getUSDistance(S2);
		delay(1);
		a2 = getUSDistance(S2);
		delay(1);
		a3 = getUSDistance(S2);
		delay(1);
		a4 = getUSDistance(S2);
		delay(1);
		a5 = getUSDistance(S2);
		delay(1);
		g_left_sensor = ((a1 + a2 + a3 + a4 + a5) / 5);
	}
	setMotorSpeed(R_motor, 0);
	setMotorSpeed(L_motor, 0);
	while(true){
	if (g_left_sensor <= 90)
	{
		state = HOME1;
		break;
	}
	if ( g_encoder = (lenght * 360 / (PI * wheel_diameter))) // nikdy nesmi byt true
	{
		state = WALL1;
		break;
	}
}
}
// jizda pro medveda
void go_for_bear(short lenght)
{ // delka po ktere robot zastavi
	resetMotorEncoder(L_motor);
	resetMotorEncoder(R_motor);
	setMotorSpeed(R_motor, speed);
	setMotorSpeed(L_motor, speed);
	do
	{
		g_encoder = getMotorEncoder(R_motor);
		g_front_sensor = getUSDistance(S3);
		delay(1);
	} while (g_front_sensor <= 245 && (g_front_sensor > 5 && g_encoder <= (lenght * 360 / (PI * wheel_diameter))));
	delay(1000);
	setMotorSpeed(R_motor, 0);
	setMotorSpeed(L_motor, 0);
}
void front_measuring()
{

	a1 = getUSDistance(S3);
	delay(1);
	a2 = getUSDistance(S3);
	delay(1);
	a3 = getUSDistance(S3);
	delay(1);
	a4 = getUSDistance(S3);
	delay(1);
	a5 = getUSDistance(S3);
	delay(1);
	g_front_sensor = ((a1 + a2 + a3 + a4 + a5) / 5);
	while(true){
	if (g_front_sensor>10)
	{
		state = HOME2;
		break;
	}
	//else
	//{
	//	state = FIELD2;
	//	break;
	//}
}
}

// tlacitka pro urceni polohy medveda
void buttons()
{
	while (true)
	{
		if (getButtonPress(buttonUp))
		{ // pokud se zmackne tlacitko nahoru zacne robot hledat v pasu jedna
			button_back();
			forward1(600);
			obloukleft(300, 180);
			forward1(50);
			obloukright(430, 164);
			open_klepeto(90, 90);
			setMotorSpeed(R_motor, speed - 2);
			setMotorSpeed(L_motor, speed);
			floating_average(); // hleda konec zdi
			setMotorSpeed(R_motor, 0);
			setMotorSpeed(L_motor, 0);
			forward1(100);

			bear_search(20, 1000);
			if (bear == false)
			{
				front_measuring();
				if (bear = true)
				{
					go_for_bear(200);
					button_back();
					forward1(100);
					turn_left(100, 40);
					button_back();
					forward1(500);
					obloukleft(300, 4);
					forward1(100);
					obloukright(300, 2);
					forward1(500);
				}
				if (bear = false)
				{
					forward1(100);
					obloukright(200, 90); // right znamena doleva
					button_back();
				}
			}
			if (bear == true)
			{
				forward1(110);
				turn_left(90, 40);
				button_back();
				go_for_bear(1200);
				close_klepeto();
				button_back();
				turn_right(110, 40);
				button_back();

				forward1(100);
				turn_left(100, 40);
				button_back();
				forward1(500);
				obloukleft(300, 4);
				forward1(100);
				obloukright(300, 2);
				forward1(500);
			}
		}

		if (getButtonPress(buttonRight))
		{
			delay(1);
		}

		if (getButtonPress(buttonDown))
		{
		}

		if (getButtonPress(buttonLeft))
		{ // pokud se zmackne tlacitko nahoru zacne robot hledat medveda v pasu tri
			while (true)
			{
				a1 = getUSDistance(S2);
				delay(1);
				a2 = getUSDistance(S2);
				delay(1);
				a3 = getUSDistance(S2);
				delay(1);
				a4 = getUSDistance(S2);
				delay(1);
				a5 = getUSDistance(S2);
				delay(1);
				g_left_sensor = ((a1 + a2 + a3 + a4 + a5) / 5);
			}
		}

		if (getButtonPress(buttonEnter))
		{
			while (true)
			{
				switch (state)
				{
				case ESKO:
					button_back();
					forward1(600);
					obloukleft(300, 180);
					forward1(50);
					obloukright(430, 164);
					open_klepeto(90, 90);
					setMotorSpeed(R_motor, speed - 2);
					setMotorSpeed(L_motor, speed);
					floating_average(); // hleda konec zdi
					setMotorSpeed(R_motor, 0);
					setMotorSpeed(L_motor, 0);
					forward1(100);
					state = FIELD1;
					break;
				///////////////////////
				case FIELD1:
					bear_search(20, 980);
					// state= WALL1
					// state = HOME1
					break;
				//////////////////
				case WALL1:
					front_measuring();
					//home2
					//field2
					break;
				//////////////////
				case HOME1:
					forward1(110);
					turn_left(90, 40);
					button_back();
					go_for_bear(1200);
					close_klepeto();
					delay(500);
					button_back();
					turn_right(110, 40);
					button_back();
					forward1(100);
					turn_left(100, 40);
					button_back();
					forward1(400);
					obloukleft(300, 100);
					forward1(300);
					obloukright(300, 180);
					forward1(500);
					break;

					case HOME2:
					go_for_bear(50);
					close_klepeto();
					delay(500);
					button_back();
					forward1(100);
					turn_left(100, 40);
					button_back();
					forward1(400);
					obloukleft(300, 100);
					forward1(300);
					obloukright(300, 180);
					forward1(500);
					break;
				}
			}
		}
	}
}

task main()
{
	buttons();
}
