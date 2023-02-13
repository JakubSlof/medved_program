float distance;
task measure(){
    SensorType[S1] = sensorEV3_Ultrasonic;
	while(true){
		distance = getUSDistance(S1);
		displayBigTextLine(3, "vzdalenost: %.2f" , distance);
		delay(1);}
}
task main()
{
    startTask measure();
}
