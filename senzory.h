//g_distance2 > min_distance && g_distance2 < max_distance && g_distance2!=225
const short max_distance=120;
const short min_distance=100;
int g_distance2;
int g_distance;
void bearserch(){
	setMotorSpeed(motorL, speed);
	setMotorSpeed(motorR, speed);
	while(g_distance > 80 && g_distance<150){
		g_distance = getUSDistance(S2);
		displayBigTextLine(3, "vzdalenost: %.2f" , g_distance);
				delay(1);
		}
stopAllMotors();


		}








void bear_search_left_sensor(){//vzdalenost od robota ke zdi
	int delayos = 1;
	short g_v_distance = 101,//levej senzor
				distance1 = 0,
				distance2 = 0,
				distance3 = 0,
				distance4 = 0,
				distance5 = 0,
				distance6 = 0,
				distance7 = 0,
				distance8 = 0,
				distance9 = 0,
				distance10 = 0;

			while(g_v_distance > 100 && g_v_distance<150){
				distance1 = getUSDistance(S1);
				delay(delayos);
				distance2 = getUSDistance(S1);
				delay(delayos);
				distance3 = getUSDistance(S1);
				delay(delayos);
				distance4 = getUSDistance(S1);
				delay(delayos);
				distance5 = getUSDistance(S1);
				delay(delayos);
				distance6 = getUSDistance(S1);
				delay(delayos);
				distance7 = getUSDistance(S1);
				delay(delayos);
				distance8 = getUSDistance(S1);
				delay(delayos);
				distance9 = getUSDistance(S1);
				delay(delayos);
				distance10 = getUSDistance(S1);
				delay(delayos);
				g_v_distance = ((distance1+distance2+distance3+distance4+distance5+distance6+distance7+distance8+distance9+distance10)/10);
				distance1 = getUSDistance(S1);
				distance2 = distance3;
				distance3 = distance4;
				distance4 = distance5;
				distance5 = distance6;
				distance6 = distance7;
				distance7 = distance8;
				distance8 = distance9;
				distance9 = distance10;
				distance10 = distance2;


				//writeDebugStreamLine("jizda: time1: %i vL: %i vR: %i EncL: %i EncR: %i EncRD: %4.2f err: %4.2f corr: %4.2f", time1[T1], actualL - lastL, actualR - lastR, actualL , actualR, actualRD, err, corr );
				displayBigTextLine(3, "vzdalenost: %.2f" , g_v_distance);
				delay(1);
			}
	stopAllMotors();

}


void bear_search_front_sensor(){//vzdalenost od robota ke zdi
	 g_distance2 = getUSDistance(S2);//predni senzor
	setMotorSpeed(motorL, speed);
	setMotorSpeed(motorR, speed);
			while(g_distance2 > 8 && g_distance2 < 120){
				g_distance2 = getUSDistance(S2);
				displayBigTextLine(3, "vzdalenost: %.2f" , g_distance2);
				delay(1);
			}
	stopAllMotors();
	close_klepeto(45);

}
void search_wall_end(){//vzdalenost od robota ke zdi
	 g_distance2 = getUSDistance(S2);//bocni senzor
	setMotorSpeed(motorL, speed);
	setMotorSpeed(motorR, speed);
			while(g_distance2 < 50){
				g_distance2 = getUSDistance(S2);
				displayBigTextLine(3, "vzdalenost: %.2f" , g_distance2);
				delay(1);
			}
		stopAllMotors();
			}
