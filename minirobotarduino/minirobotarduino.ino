#include <Servo.h> //servo biblioteka
int ena=5;  //brzina motora A
int enb=6;  //brzina motora B
int in1=7;  //smjer kazaljke na satu-motor A
int in2=8;  //suprotan smjer-motor A
int in3=9;  //smjer kazaljke na satu-motor B
int in4=10; //suprotan smjer-motor B
int ehopin=4;//eho pin (ultrasonic)
int trigpin=3;//trig pin (ultrasonic)
int Nbrzina=0;//brzina naprijed
int Dbrzina=0;//brzina desno
int Lbrzina=0;//brzina lijevo
int smjer=0;//naprijed=8 nazad=2 lijevo=4 desno=6
Servo servomotor;//deklarisanje servo objekta
int delay_time=250;//kasnjenje nakon pomjeranja motora B
int Nidi=8;//pomjeranje naprijed
int Didi=6;//pomjeranje desno
int Lidi=4;//pomjeranje lijevo
int Nazadidi=2;//pomjeranje nazad
int desnisenzor=12;//definisanje pina 12 za desni ir
int lijevisenzor=13;//definisanje pina 13 za lijevi ir
int led=2;//definisanje pina 2 za led diode
int desnistanje;//stanje desnog ir senzora
int lijevistanje;//stanje lijevog ir senzora
int komanda=0;//komanda za blutu

void setup() {
  Serial.begin(9600);//pocetak serijske komunikacije
  //definisanje ulaza i izlaza
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ehopin,INPUT);
  pinMode(trigpin,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(desnisenzor,INPUT);
  pinMode(lijevisenzor,INPUT);
  //definisanje izlaznog pina za servo motor
  servomotor.attach(11);
}
void naprijed() {
  //kretanje motora naprijed
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena,255);
  analogWrite(enb,255);
}

void stoprobot() {
  //zaustavljanje robota
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  analogWrite(ena,0);
  analogWrite(enb,0);
}

void desno(){
  //kretanje motora desno
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena,255);
  analogWrite(enb,255);
}

void lijevo(){
  //kretanje motora lijevo
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena,255);
  analogWrite(enb,255);    
}

void nazad() {
  //kretanje motora nazad
  digitalWrite(in1,LOW); 
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena,255);
  analogWrite(enb,255); 
}

void naprijeddesno() {
  //kretanje motora naprijed-desno
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,80);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb,255);
}

void naprijedlijevo() {
  //kretanje motora naprijed-lijevo
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,255);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb,80);
}

void nazaddesno() {
  //kretanje motora nazad-desno
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,80);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,255);
}

void nazadlijevo() {
  //kretanje motora nazad-lijevo
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,255);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,80);
}

void detekcija() {//mjeri tri ugla(0,90,179)
  //vrijeme stabilizacije za servo motor nakon
  //kretanja robota nazad
  int delay_time=250;
  gledajPravo();//ocitava udaljenost ispred robota
  if(Nbrzina<10) {//petlja za udaljenost manju od 10 cm
    stoprobot();//ciscenje podataka
    delay(100);
    nazad();//pomjeranje robota nazad za 0.2s
    delay(200);
  }
  if(Nbrzina<25) {//petlja za udaljenost manju od 25 cm
    stoprobot();//ciscenje podataka
    delay(100);
    gledajLijevo();//ocitava udaljenost na lijevoj strani
    delay(delay_time);//vrijeme stabilizacije servo motora
    gledajDesno();//ocitava udaljenost na desnoj strani
    delay(delay_time);//vrijeme stabilizacije servo motora
    //petlja ukoliko je udaljenost-lijevo>udaljenost-desno
    if(Lbrzina>Dbrzina) {
      smjer=Lidi;//pomjeranje na lijevo
    }
    //petlja ukoliko je udaljenost-lijevo<=udaljenost-desno
    if(Lbrzina<=Dbrzina) {
      smjer=Didi;//pomjeranje na desno
    }
    //petlja ukoliko su obe udaljenosti <10 cm
    if(Lbrzina<10 && Dbrzina<10) {
      smjer=Nazadidi;//pomjeranje nazad
    }
  }
  else {
    smjer=Nidi;//pomjeranje naprijed
  }
}

//mjerenje udaljenosti ispred robota(servo-90 stepeni)
void gledajPravo() {
  servomotor.write(90);//servo postavljen pod uglom od 90 stepeni
  //ultrasonic odašilje signal niskog nivoa 2 mikrosekunde
  digitalWrite(trigpin,LOW); 
  delayMicroseconds(2);
  //ultrasonic odašilje signal visokog nivoa 10 mikrosekundi
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  //nastavljanje odašiljanja signala niskog nivoa
  digitalWrite(trigpin,LOW);
  //cita ehopin, vraca zvucni talas koji putuje u mikrosekundama
  float Ndaljina=pulseIn(ehopin,HIGH);
  Ndaljina=Ndaljina/5.8/10;//konverzija mikrosekundi u cm
  Nbrzina=Ndaljina;//cita udaljenost u Nbrzina
  Serial.print("Nbrzina=");
  Serial.print(Nbrzina);
  Serial.print(" cm");
}

//mjerenje udaljenosti sa lijeve strane robota(servo-5 stepeni)
void gledajLijevo() {
  servomotor.write(5);//servo postavljen pod uglom od 5 stepeni
  delay(delay_time);
  //ultrasonic odašilje signal niskog nivoa 2 mikrosekunde
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  //ultrasonic odašilje signal visokog nivoa 10 mikrosekundi
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  //nastavljanje odašiljanja signala niskog nivoa
  digitalWrite(trigpin,LOW);
  //cita ehopin, vraca zvucni talas koji putuje u mikrosekundama
  float Ldaljina=pulseIn(ehopin,HIGH);
  Ldaljina=Ldaljina/5.8/10;//konverzija mikrosekundi u cm
  Lbrzina=Ldaljina;//cita udaljenost u Lbrzina
  Serial.print("Lbrzina = ");
  Serial.print(Lbrzina);
  Serial.print(" cm ");
}

//mjerenje udaljenosti sa desne strane robota(servo-177 stepeni)
void gledajDesno() 
 {
   servomotor.write(177);//servo postavljen pod uglom od 177 stepeni
   delay(delay_time);
   //ultrasonic odašilje signal niskog nivoa 2 mikrosekunde
   digitalWrite(trigpin,LOW); 
   delayMicroseconds(2);
   //ultrasonic odašilje signal visokog nivoa 10 mikrosekundi
   digitalWrite(trigpin,HIGH); 
   delayMicroseconds(10);
   //nastavljanje odašiljanja signala niskog nivoa
   digitalWrite(trigpin,LOW); 
   //cita ehopin, vraca zvucni talas koji putuje u mikrosekundama
   float Ddaljina = pulseIn(ehopin, HIGH); 
   Ddaljina=Ddaljina/5.8/10; //konverzija mikrosekundi u cm
   Dbrzina=Ddaljina;//cita udaljenost u Dbrzina
   Serial.print("Dbrzina=");
   Serial.print(Dbrzina);
   Serial.println(" cm");
 }

void loop(){
  //ocitavanje desnog ir senzora
  desnistanje=digitalRead(desnisenzor);
  //ocitavanje lijevog ir senzora
  lijevistanje=digitalRead(lijevisenzor);
  //provjera da li podaci dolaze sa serijskog porta
  if(Serial.available()>0) {
      komanda=Serial.read();//cita podatke sa serijskog porta
    }
    
  if(komanda=='0') {//ukoliko sa serijskog porta dolazi '0'
      //vozi naprijed
       naprijed();
       komanda='0';
    }
    
    else if(komanda=='1') {//ukoliko sa serijskog porta dolazi '1'
      //vozi nazad
      nazad();
      komanda='1';
    }
    
    else if(komanda=='2') {//ukoliko sa serijskog porta dolazi '2'
      //vozi desno
      desno();
      komanda='2';
    }
    
    else if(komanda=='3') {//ukoliko sa serijskog porta dolazi '3'
      //vozi lijevo
      lijevo();
      komanda='3';
    }
    
    else if(komanda=='4') {//ukoliko sa serijskog porta dolazi '4'
      //zaustavljanje robota
      stoprobot();
      komanda='4';
    }
    
    else if(komanda=='5') {//ukoliko sa serijskog porta dolazi '5'
      //vozi naprijed-desno
      naprijeddesno();
      komanda='5';
    }
    
    else if(komanda=='6') {//ukoliko sa serijskog porta dolazi '6'
      //vozi naprijed-lijevo
      naprijedlijevo();
      komanda='6';
    }
    
    else if(komanda=='7') {//ukoliko sa serijskog porta dolazi '7'
      //vozi nazad-desno
      nazaddesno();
      komanda='7';
    }
    
    else if(komanda=='8') {//ukoliko sa serijskog porta dolazi '8'
      //vozi nazad-lijevo
      nazadlijevo();
      komanda='8';
    }
    
    else if(komanda=='9') {//ukoliko sa serijskog porta dolazi '9'
      if((lijevistanje==LOW)&&(desnistanje==LOW)) {
        //pravo
        analogWrite(ena,120);
        analogWrite(enb,120);
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
      }
       if((lijevistanje==LOW)&&(desnistanje==HIGH)) {
        //desno
        analogWrite(ena,150);
        analogWrite(enb,150);
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
        
      }
       if((lijevistanje==HIGH)&&(desnistanje==LOW)) {
        //lijevo
        analogWrite(ena,150);
        analogWrite(enb,150);
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
        
      }
       
      if((lijevistanje==HIGH)&&(desnistanje==HIGH)) {
        //stop
        analogWrite(ena,0);
        analogWrite(enb,0);
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,LOW);
        
      }
  komanda='9';
    }

    //ukoliko sa serijskog porta dolazi 'a' ili broj 97 sa mit app inventora
    else if(komanda=='a'){
      //postavljanje servo motora pod uglom od 90 stepeni na pocetku rada
       servomotor.write(90); 
       //mjerenje uglova i odredjivanje u kojem smjeru se treba kretati robot
       detekcija(); 
       if(smjer==2) // petlja za smjer=2(nazad)
       {
       nazad();//kretanje robota nazad 0.8 sekundi
       delay(800);
       lijevo() ;//kretanje robota lijevo 0.2 sekundi
                 //(kako ne bi doslo do zaglavljenja u slijepom uglu)
       delay(200); 
       }
       if(smjer==6) //petlja za smjer=6(desno)
       { 
       nazad();//kretanje robota nazad 0.1 sekundi
       delay(100);
       desno();//kretanje robota lijevo 0.6 sekundi
       delay(600); 
       }
       if(smjer==4) // petlja za smjer=4(lijevo)
       { 
       nazad();//kretanje robota nazad 0.6 sekundi
       delay(600);
       lijevo();//kretanje robota lijevo 0.6 sekundi
       delay(600); 
       }
       if(smjer==8) //petlja za smjer=8(naprijed)
       { 
       naprijed();//kretanje robota naprijed 0.1 sekundi
       delay(100);
       }
    komanda='a';
  }

  //ukoliko sa serijskog porta dolazi 'b' ili broj 98 sa mit app inventora
  else if(komanda=='b') {
    //svjetla-on
    digitalWrite(led,HIGH);//postavljanje pina na HIGH
    komanda='b';
  }

  //ukoliko sa serijskog porta dolazi 'c' ili broj 99 sa mit app inventora
  else if(komanda=='c'){
    //svjetla-off
    digitalWrite(led,LOW);//postavljanje pina na LOW
    komanda='c';
  }
}
