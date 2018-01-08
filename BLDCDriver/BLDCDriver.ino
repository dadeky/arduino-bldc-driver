#include "Arduino.h"

int AA1=3;
int AA2=5;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=6;

int enable=2; //This is not used for now, The ESC is always enabled
int emfA=A0;
int emfB=A1;
int emfC=A2;

int fase=1;
int deltaA=0;
int emA=0;
int sum=0;

int IN=A3;
unsigned int Delay=4000;

int it=0;
int it2=1;

static int delta= 0;
static int Lastdelta= -1;

unsigned long previousMillis = 0;

void setup() {
	Serial.begin(250000);
	pinMode(AA1,OUTPUT);
	pinMode(AA2,OUTPUT);
	pinMode(BB1,OUTPUT);
	pinMode(BB2,OUTPUT);
	pinMode(CC1,OUTPUT);
	pinMode(CC2,OUTPUT);

	pinMode(enable,OUTPUT);

	pinMode(IN,INPUT);
	pinMode(emfA,INPUT);
	pinMode(emfB,INPUT);
	pinMode(emfC,INPUT);

  //digitalWrite(enable,HIGH);
  //previousMillis = micros();

}

void loop() {


	int emA = analogRead(emfA);
	int emB = analogRead(emfB);
	int emC = analogRead(emfC);
	int sum = (emA+emB+emC)/3;

	unsigned long currentMillis = micros();


	if(currentMillis - previousMillis >= Delay){

		previousMillis += Delay;

		//Phase1 C-B
		switch(fase){
		case 1:
		  digitalWrite(AA1,LOW);
		  digitalWrite(AA2,LOW);
		  digitalWrite(BB1,LOW);
		  digitalWrite(CC2,LOW);
		  digitalWrite(BB2,HIGH);
		  digitalWrite(CC1,HIGH);
		  delta = emA-sum;

		  break;


		//Phase2 A-B
		case 2:
		  digitalWrite(AA2,LOW);
		  digitalWrite(BB1,LOW);
		  digitalWrite(CC1,LOW);
		  digitalWrite(CC2,LOW);
		  digitalWrite(AA1,HIGH);
		  digitalWrite(BB2,HIGH);
		  delta = emC-sum;
		  break;

		//Phase3 A-C
		case 3:
		  digitalWrite(AA2,LOW);
		  digitalWrite(BB1,LOW);
		  digitalWrite(BB2,LOW);
		  digitalWrite(CC1,LOW);
		  digitalWrite(CC2,HIGH);
		  digitalWrite(AA1,HIGH);
		  delta = emB-sum;
		break;

		//Phase4 B-C
		case 4:
		  digitalWrite(AA1,LOW);
		  digitalWrite(AA2,LOW);
		  digitalWrite(BB2,LOW);
		  digitalWrite(CC1,LOW);
		  digitalWrite(BB1,HIGH);
		  digitalWrite(CC2,HIGH);
		  delta = emA-sum;
		  break;

		//Phase5 B-A
		case 5:
		  digitalWrite(AA1,LOW);
		  digitalWrite(BB2,LOW);
		  digitalWrite(CC1,LOW);
		  digitalWrite(CC2,LOW);
		  digitalWrite(AA2,HIGH);
		  digitalWrite(BB1,HIGH);
		  delta = emC-sum;
		  break;

		//Phase6 C-A
		case 6:
		  digitalWrite(AA1,LOW);
		  digitalWrite(BB1,LOW);
		  digitalWrite(BB2,LOW);
		  digitalWrite(CC2,LOW);
		  digitalWrite(CC1,HIGH);
		  digitalWrite(AA2,HIGH);
		  delta = emB-sum;
		  break;
		}

		if (Lastdelta < 0)
		{
		  if (delta > 0)
		  {
			  Lastdelta=delta; //save the last delta
			  fase= fase + 1;
			  if (fase > 6) {
				  fase = 1;
			  }
		  }
		}//Zero cross from - to +

		if (Lastdelta > 0)
		{
		  if (delta < 0)
		  {
			Lastdelta=delta;
			fase= fase + 1;
			if (fase > 6) {
			  fase = 1;
			  }
		  }
		}//Zero cross from + to -

  }//Case ends

  int t =analogRead(IN); //From the potentiometer
  Delay=map(t,0,1024,1,1000); //we obtain the delay speed using the potentiometer
  //we map the values from 1 to 1000 microseconds


} //loop ends
