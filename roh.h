


void roh(){
open_klepeto(90);	//roh1
forward1(1400);
short fd_1 = 0;
short fd_2 = 0;
short fd_3 = 0;
short fd_4 = 0;
short fd_v = 0;
fd_1 = getUSDistance(S2);//frontdistance1
fd_2 = getUSDistance(S2);//frontdistance2
fd_3 = getUSDistance(S2);//frontdistance3
fd_4 = getUSDistance(S2);//frontdistance4
fd_v = ((fd_1 + fd_2 + fd_3 + fd_4)/4);
	if(fd_v<10&&fd_v>100){
	close_klepeto(45);//funkce co se musi vyrobit
	//cesta ypatkz
	}

		else{
		close_klepeto(90); //roh2
		forward1(-50);
		turn(90,20,-20)
		forward1(1400);
		short fd_1 = 0;
		short fd_2 = 0;
		short fd_3 = 0;
		short fd_4 = 0;
		short fd_v = 0;
		fd_1 = getUSDistance(S2);//frontdistance1
		fd_2 = getUSDistance(S2);//frontdistance2
		fd_3 = getUSDistance(S2);//frontdistance3
		fd_4 = getUSDistance(S2);//frontdistance4
		fd_v = ((fd_1 + fd_2 + fd_3 + fd_4)/4);
			if(fd_v<10&&fd_v>100){
			//cesta zpatkz
			}


				else{
				close_klepeto(90); //roh3
				forward1(-50);
				turn(90,20,-20)
				forward1(1400);
				short fd_1 = 0;
				short fd_2 = 0;
				short fd_3 = 0;
				short fd_4 = 0;
				short fd_v = 0;
				fd_1 = getUSDistance(S2);//frontdistance1
				fd_2 = getUSDistance(S2);//frontdistance2
				fd_3 = getUSDistance(S2);//frontdistance3
				fd_4 = getUSDistance(S2);//frontdistance4
				fd_v = ((fd_1 + fd_2 + fd_3 + fd_4)/4);
					if(fd_v<10&&fd_v>100){
					//cesta domu
					}
				}
	 }
}