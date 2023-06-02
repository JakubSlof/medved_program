#pragma config(Motor,  motorA,          motorL,       tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          klepetoR,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          klepetoL,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          motorR,       tmotorEV3_Large, PIDControl, encoder)


#include "JoystickDriver.c" // hned pod #pragma config

// Set the sensor port 1 (S1) to be a custom I2C sensor
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// verze pro rizeni ovladacem simulatoru z Robotarny
// nastaveni joysticku = ovladace simulatoru -  viz jpg soubor

// klepeta jsou vepredu; lev� motor - A, prav� motor - C

// TODO zkontrolovat I2C bez poijmu dat

// Address is 0x04 on the ESP32: (Binary) 0100
// Bit shifted out with one 0, that becomes: (Binary) 1000
// Which is 0x08
#define ESP_ADR	0x08    // ESP: 0x04


int prsty = 70; // poloha obou prstu pro 'baterie' v Roadside
int prsty_data = 60; // data pocitana jako poloha serv z prsty
char Pbt1 = 0, Pbt2 = 0, Pbt4 = 0, Pbt5 = 0; // puvodni hodnoty prepinacu
char Nbt1 = 0, Nbt2 = 0, Nbt4 = 0, Nbt5 = 0; // nove hodnoty prepinacu
float enL = 0, enR = 0; // hodnoty z leveho a praveho enkoderu klepet
float pozL = 0, pozR = 0, poz60 = 60; // chtena pozice klepet
int rychle = 30, pomalu = 3; // pro velke zmeny polohy mohou byt klepeta rychlejsi
int doba_pomalu = 100;
int zpomal = 15; // hranice, od kdy se pohyb klepet zpomaluje
long cnt = 0; // pocitadlo - kontrola, ze smycky bezi
int puvodni = 0, novy = 2, stav60 =0; // stavy klepet
int zmena = 0; // indikuje, ze prave doslo ke zmene

byte I2Cmessage[22];
byte I2Creply[4];
ubyte servo1 = 100, servo2 = 100; //servo1, servo2 - pozice serv + 100;

void i2c_msg(byte ard_address, int message_size, int return_size, ubyte byte0, ubyte byte1, ubyte byte2, ubyte byte3 ,ubyte byte4)
{
	memset(I2Creply, 0, sizeof(I2Creply));
	message_size = message_size+3;

	I2Cmessage[0] = message_size; // Messsage Size
	I2Cmessage[1] = ard_address;

	I2Cmessage[2] = byte0;
	I2Cmessage[3] = byte1;
	I2Cmessage[4] = byte2;
	I2Cmessage[5] = byte3;
	I2Cmessage[6] = byte4; // max is 99 only for I2Cmessage[6]
	//// can't add more than 5 Bytes
	sendI2CMsg(S1, &I2Cmessage[0], return_size);
	while (nI2CStatus[S1] == 1 ) // STAT_COMM_PENDING - Wait for I2C bus to be ready
	{ // do nothing
	}
	readI2CReply(S1, &I2Creply[0], return_size);
	wait1Msec(35);
}

task I2C(){

	int h = 10; // hlavicka

	while(true){
		prsty_data = prsty*65/80; // mapov�n� vstupu z joysticku na maxim�ln� rozsah serv na medvedici
		if( prsty_data < (-44) ) // vyssi hodnota znamena vyssi minimalni pozice serva
			prsty_data =  -44;
		servo1 = prsty_data + 100;
	  servo2 = prsty_data + 100;
	  //displayBigTextLine(3, ">>00E: %d", nI2CStatus[S1]);

		if (!(Nbt2))
			  i2c_msg(ESP_ADR, 3, 4, h, servo1, servo2, 0, 0);  //servo1, servo2 - pozice serv + 100;
			 // displayBigTextLine(3, ">>00E: %d", nI2CStatus[S1]);
			 // displayBigTextLine(5, "a: %i", I2Creply[0]);
				//displayBigTextLine(7, "b: %i", I2Creply[1]);
				//displayBigTextLine(9, "c: %i", I2Creply[2]);
				wait1Msec(500);
		//}
		delay(10);
	}
}

void pozice60(){
        if ((abs(enL-poz60))>15) {
	  			if ((enL-poz60)>15)
		  			setMotorSpeed(klepetoL, -rychle);
		  		else
		  			setMotorSpeed(klepetoL, rychle);
		    }
		  	else
			  	setMotorSpeed(klepetoL, 0);

		    if ((abs(enR-poz60))>15) {
		  		if ((poz60-enR)>5)
		  			setMotorSpeed(klepetoR, rychle);
		  		else
		  			setMotorSpeed(klepetoR, -rychle);
		    }
		  	else
			  	setMotorSpeed(klepetoR, 0);

				if (((abs(enL-poz60))<=15) && (abs(enR-poz60))<=5)
					stav60 = 0;
}

task moc_otevrene(){
	while(true){
			if ( enL > 105 )
				setMotorTarget(klepetoL, 100, pomalu);
			if ( enR > 105)
				setMotorTarget(klepetoR, 100, pomalu);
			delay(10);
	}
}

task klepeta() {
	while(true) {
		if ( enL > 105 )
				setMotorTarget(klepetoL, 100, pomalu);
		if ( enR > 105)
				setMotorTarget(klepetoR, 100, pomalu);

		switch (10*puvodni+novy) {

		case 01: // zavrit -> otevrit
			pozL = 100;
	  	pozR = 100;  // tady nema byt break
		case 02: // zavri -> do V
			if (novy == 2) {
		  	pozL = 45;
	    	pozR = 45;
			}

	  	if ((pozL-enL)>zpomal) // aby to fungovalo, musi byt delay na konci switche dostatecne male, napr. 5
				setMotorSpeed(klepetoL, rychle);
		  else
		  	setMotorSpeed(klepetoL, 0);

			if (((pozR-enR)>zpomal) && (enL>35)  ) // pocka na otevreni leveho klepeta
				setMotorSpeed(klepetoR, rychle);
				//if (novy == 3)
				//  setMotorSpeed(klepetoR, -rychle);
				//else
				//	setMotorSpeed(klepetoR, rychle);
		  else
		  	setMotorSpeed(klepetoR, 0);
	  break;

		case 12: // otevri -> do V
			pozL = 45;
	  	pozR = 45;
			if ((enL-pozL)>zpomal)
				setMotorSpeed(klepetoL, -rychle);
		  else
		  	setMotorSpeed(klepetoL, 0);
			if ((enR-pozR)>zpomal)
				setMotorSpeed(klepetoR, -rychle);
		  else
		  	setMotorSpeed(klepetoR, 0);
		break;

		case 10: // otevrit -> zavrit
		case 20: // do V -> zavrit
		  pozL = 0;
	  	pozR = 0;
		  if ((stav60 == 0) && zmena) { // aby se nesrazila klepeta, tak je napred otevru na 60
		  	stav60 = 1;
		  	zmena = 0;
		  }
			if (stav60)
				pozice60();
	  	else {
				if ((abs(enL-pozL)>zpomal) && (enR<5))
					setMotorSpeed(klepetoL, -rychle);
			  else
			  	setMotorSpeed(klepetoL, 0);
				if ((abs(enR-pozR))>zpomal)
					setMotorSpeed(klepetoR, -rychle);
			  else
			  	setMotorSpeed(klepetoR, 0);
		  }
		break;

  	case 21: // do V -> otevrit
			pozL = 100;
	  	pozR = 100;
			if (abs(enL-pozL)>zpomal) // aby to fungovalo, musi byt delay na konci switche dostatecne male, napr. 5
				setMotorSpeed(klepetoL, rychle);
		  else
		  	setMotorSpeed(klepetoL, 0);
			if ((abs(enR-pozR))>zpomal)
				setMotorSpeed(klepetoR, rychle);
		  else
		  	setMotorSpeed(klepetoR, 0);
	  break;

	  case 03: // zavri -> motor
	  case 13: // otevri -> motor - klepeta se zaviraji v opacnem poradi
	  case 23: // do V -> motor
	   pozL = -20;
	  	pozR = 60;
		  if ((stav60 == 0) && zmena) { // aby se nesrazila klepeta, tak je napred otevru na 60
		  	stav60 = 1;
		  	zmena = 0;
		  }
			if (stav60)
				pozice60();
	  	else {
				if ((abs(enL-pozL)>2))
					setMotorSpeed(klepetoL, -rychle);
			  else
			  	setMotorSpeed(klepetoL, 0);
				if ((enR-pozR)>zpomal)
					setMotorSpeed(klepetoR, -rychle);
			  else
			  	setMotorSpeed(klepetoR, 0);
		  }
		break;

		case 30: // ze vsech stavu, kdy tocil motorem, se vraci do pozice V
		case 31:
		case 32:
			pozL = 45;
	  	pozR = 45;
			if ((abs(enL-pozL))>zpomal)
					setMotorSpeed(klepetoL, rychle);
			  else
			  	setMotorSpeed(klepetoL, 0);

		if ((enR-pozR)>zpomal)
					setMotorSpeed(klepetoR, -rychle);
			  else
			  	setMotorSpeed(klepetoR, 0);
		break;
		} // konec switch

 		delay(5);
  } // konec while(true)
} // konec klepeta

void zacatek(){
	setLEDColor(ledRed);
	displayBigTextLine(1, "TCA-B  RA-B");
	displayBigTextLine(3, "Jsou klepeta");
	displayBigTextLine(5, "zavrena?!?");
	displayTextLine(8, "Pokud ano, stiskni LEVE");
	displayTextLine(9, "tlacitko na kostce.");
	displayTextLine(11, "Pokud NE, zavri klepeta");
	displayTextLine(12, "a spust program znovu.");
	do
		delay(1);
	while (getButtonPress(buttonLeft) == 0);
	//waitForButtonPress(); // pocka na potvrzujici stisk tlacitka
	resetMotorEncoder(klepetoL);
  resetMotorEncoder(klepetoR);
  //clearTimer(T1);
  //unsigned long varPgmTime = nPgmTime;
  eraseDisplay();
  displayBigTextLine(3, "Vyzkousej");
	displayBigTextLine(5, "jizdu, klepeta");
	displayBigTextLine(7, "a valec.");
	displayTextLine(11, "Stiskni PRAVE");
	displayTextLine(12, "tlacitko na kostce.");
	do
		delay(1);
	while (getButtonPress(buttonRight) == 0);
	setLEDColor(ledGreen);  // pripraven ke startu
}

task main()
{
	startTask(I2C);

	int kalX = -1, kalY = 2;  // softwarova kalibrace Ovladace simulatoru ?
	float coef = 1, coef_speed = 40; // korekce rychlosti leveho motoru, min. rychlost, kdy korekce nastava
	float X = 0, Y = 0, X1 = 0, Y1 = 0, xVst = 0, yVst = 0 ; // X - vpravo-vlevo, Y - vpred-vzad
	float exp = 1.6; // mocninna konstanta pro rizeni

	zacatek();
	startTask(klepeta);
	// startTask(moc_otevrene);
	while(true){
		// writeDebugStreamLine("time: %u, timeTotal: %i", nPgmTime - varPgmTime, time1[T1] );
		enL = getMotorEncoder(klepetoL);
		enR = getMotorEncoder(klepetoR);

		getJoystickSettings(joystick); // nacita aktualni hodnoty z joysticku -> na zacatek cyklu
		xVst = - joystick.joy1_x1;
		yVst = joystick.joy1_x2;
		prsty = joystick.joy1_y2;
		X = (xVst + kalX)/85.0;
		Y = (yVst + kalY)/75.0;
		Pbt1 = Nbt1; Pbt2 = Nbt2; Pbt4 = Nbt4; Pbt5 = Nbt5;
		Nbt1 = joy1Btn(01);
		Nbt2 = joy1Btn(02);
		Nbt4 = joy1Btn(04); // zavri 4
		Nbt5 = joy1Btn(05); // otevri 5, obe vyple -> do V
		if (!( (Pbt1 == Nbt1) && (Pbt2 == Nbt2) && (Pbt4 == Nbt4) && (Pbt5 == Nbt5) )) {
			puvodni = novy;
			zmena = 1;
		}

		if (Nbt4 == 1) novy = 0; // zavri
		if (Nbt5 == 1) novy = 1; // otevri
		if ((Nbt4 == 0) && (Nbt5 == 0)) novy = 2; // do V
		if (Nbt1 == 1) novy = 3; // motor
		if ((Nbt1 == 0) && (Pbt1 == 1)) novy = 2; // po skonceni motoru jdi do V - kam ma jit po skonceni motoru

		if (Y>0)
			Y1 = pow(Y, exp) * 100; // mocninny tvar krivky rizeni
		else
			Y1 = -pow(-Y, exp) * 100;
	  if (X>0)
			X1 = pow(X, exp) * 100;
		else
			X1 = -pow(-X, exp) * 100;

		if (abs(Y1) < coef_speed)
			setMotorSpeed(motorL, (Y1*coef) - X1); // levy motor pri vysokych rychlostech jezdi rychleji
		else
			setMotorSpeed(motorL, Y1 - X1);
		setMotorSpeed(motorR, Y1 + X1);
  //  int aaa = 45;  // hranicni rychlost
		//if (abs(Y1) > aaa) {
		//	setMotorSpeed(motorL, aaa - X1); // levy motor pri vysokych rychlostech ma vetsi zrychleni, musi se rozjizdet pomaleji
	 //   setMotorSpeed(motorR, aaa + X1);
	 //   delay(300);
	 //   setMotorSpeed(motorL, Y1 - X1); // levy motor pri vysokych rychlostech ma vetsi zrychleni, musi se rozjizdet pomaleji
	 //   setMotorSpeed(motorR, Y1 + X1);
	 // }
		//else {
		//	setMotorSpeed(motorL, Y1 - X1);
		//	setMotorSpeed(motorR, Y1 + X1);
	 // }
		// zobrazeni na displeji
	  eraseDisplay();
		displayBigTextLine(1, "Pripraven,V:%f", getBatteryVoltage());
		displayBigTextLine(4, "enL/poL:%i/%i", enL, pozL);
		displayBigTextLine(6, "enR/poR:%i/%i", enR, pozR);
		displayTextLine(10, "pr: %i -> %i, %i", prsty, prsty_data, servo1);
		displayTextLine(11, "xVst: %f, yVst %f", xVst, yVst);
		displayTextLine(12, "X: %f -> %f", X, X1);
		displayTextLine(13, "Y: %f -> %f", Y, Y1);
		displayTextLine(14, "btns: %i %i %i %i, cnt:%i", Nbt1, Nbt2, Nbt4, Nbt5, cnt++);
		displayTextLine(15, "p->n: %i->%i, 60: %i", puvodni, novy, stav60);
		delay(10);

	}
	stopAllTasks();
}  // konec programu