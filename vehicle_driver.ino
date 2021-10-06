char pom='1'; //usunąć

int i;
int kL = 0;   //pomiar odleglosci
int skL = -1;  //poprzedni pomiar
int pkL = 0;  //sprawdzenie stanu wysokiego

int kP = 0; 
int skP = -1;
int pkP = 0;

int s = 0;      //kierunek jazdy
int ps = 2;     //poprzedni kierunek

int pasy;
int pasy_k=0;
int kolo = 0;

int s_prze=0;

int lokal = 0;

int k = 0; //kierunek jazdy 0 przód 1 tył

int dok = 10;  //dokladnosc pomiaru
//int dok_l=0;


int limit = 0;
int pok = -1; // pokonana odleglosc
int prze = 0;


int skret = 40;
int pros = 80;
int szer_poj = 50;
int dl, sz;
int krok;

//char pom;
float p_p;
int p, pp;
String tab_1; //tabela przy pierwszym przejezdzie
String tab_2; // tabela przy 2 przejezzdzie
String s_pom = "0";

char tab[10];  //pomiary pasa

String start="0";

int parametry_b=0;

char tab_p;

#define CYCLES_PER_SIGNAL 5000


unsigned long lastSignalTime = 0;
unsigned long signalTimeDelta = 0;

boolean firstSignal = true;
unsigned long storedTimeDelta = 0;

SIGNAL(TIMER1_COMPA_vect)
{
  unsigned long currentTime = micros();
  signalTimeDelta =  currentTime - lastSignalTime;
  lastSignalTime = currentTime;

  if (firstSignal)
  {
    firstSignal = false;
  }
  else if (storedTimeDelta == 0)
  {
    storedTimeDelta = signalTimeDelta;
  }

  // Reset OCR1A
  OCR1A += CYCLES_PER_SIGNAL;
}

void setup() {
  
pinMode(3, OUTPUT);     //PWM silnikow P
pinMode(5, OUTPUT);     //L
pinMode(7, OUTPUT);     //kierunek sinika Prawy    
pinMode(8, OUTPUT);     //kierunek sinika Prawy     A4
pinMode(12, OUTPUT);    //kierunek silnika Lewy
pinMode(13, OUTPUT);    //kierunek silnika Lewy     A5  //78.5mm obrot enkodera 1pik to 2 mm

analogWrite(3, 240);    //PWM P
analogWrite(6, 240);    //PWM L

Serial.begin(9600);

  // Set WGM(Waveform Generation Mode) to 0 (Normal)
  TCCR1A = 0b00000000;
  
  // Set CSS(Clock Speed Selection) to 0b111 (External clock source on T0 pin
  // (ie, pin 5 on UNO). Clock on rising edge.)
  TCCR1B = 0b00000111;

  // Enable timer compare interrupt A (ie, SIGNAL(TIMER1_COMPA_VECT))
  TIMSK1 |= (1 << OCIE1A);

  // Set OCR1A (timer A counter) to 1 to trigger interrupt on next cycle
  OCR1A = 1;
}




void jazdaprzod() {                        //funkcja przypisująca parametry do jazdy do przodu
digitalWrite(7, LOW);
digitalWrite(8, HIGH);

digitalWrite(12, LOW);
digitalWrite(13, HIGH);     
limit=dl;                                 //zmienna zapamiętująca odległość jaka ma zostać przejechaną
}

void jazdatyl() {
digitalWrite(7, HIGH);
digitalWrite(8, HIGH);  //low

digitalWrite(12, HIGH);
digitalWrite(13, HIGH);  //low

limit = kL;
}

void skretprawo() {
digitalWrite(7, HIGH);
digitalWrite(8, LOW); //low

digitalWrite(12, LOW); //low
digitalWrite(13, HIGH);

limit=skret;
}

void skretlewo() {
digitalWrite(7, LOW); //low
digitalWrite(8, HIGH);

digitalWrite(12, HIGH);
digitalWrite(13, LOW); //low

limit=skret;
}

void wyrprawo() {
digitalWrite(7, HIGH);
digitalWrite(8, LOW);

digitalWrite(12, HIGH);
digitalWrite(13, HIGH);
}

void wyrlewo() {
digitalWrite(7, HIGH);
digitalWrite(8, HIGH);

digitalWrite(12, HIGH);
digitalWrite(13, LOW);
}

void stop() {
digitalWrite(7, HIGH);
digitalWrite(8, HIGH);

digitalWrite(12, HIGH);
digitalWrite(13, HIGH);

Serial.write("wynik_1 ");
Serial.println(tab_1);
Serial.write("wynik_2 ");
Serial.println(tab_2);

while (1)
  delay (100);
}

/*void cofanie()
{
  k=1;
  jazdatyl();
    
  while(kL >0)
  { 
    pomiarlewe();
    pomiarprawe();
    
  if (testlewy() && testprawy() )
    continue;
  else
    stop();
  }
      
  k=0;

  zerowanie();
}*/void przypisywanie()
{tab[8]='2';
  if (pasy_k <= ( (pasy+1)/2) )
  {
    if (ps == 2)
      for (i=0; i<krok; i++)
        {tab_1 [i+((pasy_k-1)*10)] = tab[i];
Serial.write("wynik_11 ");
Serial.println(tab_1);
        }
    else
      for (i=0; i<krok; i++)
        {tab_1 [i+((pasy_k-1)*10)] = tab[krok-i-1]; //czemu tu było razy 10 ??????????
        Serial.write("wynik_12 ");
Serial.println(tab_1);
        }
  }
  else
  {
    if (ps == 2)
      for (i=0; i<krok; i++)
       { tab_2 [tab_2.length() - i-((pasy_k-1) - ((pasy+1)/2)  )*10 -1] = tab[i];
       Serial.write("wynik_21 ");
Serial.println(tab_2);
       }
    else
      for (i=0; i<krok; i++)
        {tab_2 [tab_2.length() - i-((pasy_k-1) - ((pasy+1)/2) )*10 -1] = tab[krok-i-1];
    Serial.write("wynik_2 ");
Serial.println(tab_2);
       }
  }  

  lokal=0;
}

void testprze_g()
{     
  prze=0;
  s_prze=1;
  Serial.println( ps);
      for (i=lokal;i<krok;i++)
        tab[i]='4';

      przypisywanie();
    
        jazdatyl();
      k=1;
Serial.write("tyl");
    while (kL != 0)
    {
      pomiarlewe();
    }
      k=0;
      zerowanie();

    if (ps ==1)
    {
      skretlewo();
  Serial.write("lewo");
  while(kL < limit)
  {pomiarlewe();

  } 
  zerowanie();

jazdaprzod();
limit = pros ;

  while(kL < limit)
  pomiarlewe();

skretprawo();
zerowanie();

 Serial.write("prawo");
  while(kL < limit)
  {pomiarlewe();

  }
  zerowanie();
/*
  if (ps == 1)
  ps=2;
  else if (ps == 2)
  ps=1;
*/
  s=0;
  zerowanie();
    }
    else if (ps ==2)
    {
skretprawo(); 
Serial.write("prawo");
  while(kL < limit)
  {pomiarlewe();

  }

    
  zerowanie();

jazdaprzod();
limit = pros;

  while(kL < limit)
  pomiarlewe();

skretlewo();
zerowanie();

Serial.write("lewo"); 
  while(kL < limit)
  {pomiarlewe();

  } 

  
  zerowanie();

  /*if (ps == 1)
  ps=2;
  else if (ps == 2)
  ps=1;
*/
  s=0;
  zerowanie();
    }

    k=0;
    
}

/*void testwspol_g()
{
        if(kL < kP)
      {Serial.write("blad prawo ");
        k = 1;
        wyrprawo();

        while(kL != kP)
        {pomiarprawe();
          }
  k=0;
      }
       else
       {Serial.write("blad lewo");
        k = 1;
        wyrlewo();

        while(kL != kP)
        {pomiarlewe();
          }
          k=0;
       }
   
*/
void pomiarlewe() {                        //funkcja odpowiadająca za pomiar przejechanej odległości przez lewe koło                   
  if (analogRead(A5) > 200 && pkL == 1) {  //sprawdzenie czy wystąpiło zbocze narastające
    skL=kL;                                //zapisanie wartości poprzedniej przejechanej odległości 
    pkL=0;                                 //zapisanie wystąpienia zbocza narastającego 
    
    if(k == 0)                             //sprawdzenie czy pojazd porusza się do przodu k=0 czy do tyłu k=1 
      kL++;
    else
      kL--;
  }
  else if (analogRead(A5) < 200 && pkL == 0)  {
    skL=kL;
    pkL=1;
    
    if(k == 0)
      kL++;
    else
      kL--;
  }
  else
    skL=kL;
}


void pomiarlewe1()
{
if (analogRead(A5) > 200 && pkL == 1)
{ 
  skL=kL;
  pkL=0;
  
  if(k == 0)
    {
      kL++;
      Serial.write("kL ");
Serial.println(kL);
//Serial.write("skL ");
//Serial.println(skL);
Serial.write("ks ");
Serial.println(s);
    }
  else
  {
    kL--;
Serial.write("kL ");
Serial.println(kL);
//Serial.write("skL ");
//Serial.println(skL);
Serial.write("ks ");
Serial.println(s);
  }
}
else if (analogRead(A5) < 200 && pkL == 0)
{
  skL=kL;
  pkL=1;

  if(k == 0)
    {
      kL++;
      Serial.write("kL ");
Serial.println(kL);
//Serial.write("skL ");
//Serial.println(skL);
Serial.write("ks ");
Serial.println(s);
    }
  else
  {
    kL--;
Serial.write("kL ");
Serial.println(kL);
//Serial.write("skL ");
//Serial.println(skL);
Serial.write("ks ");
Serial.println(s);
  }
}
else
skL=kL;
}

void pomiarprawe()
{
  //skP=kP;
  //skL=kL;
if (analogRead(A4) > 200 && pkP == 1)
{skP=kP;
  
  pkP=0;
  
  if(k == 0)
    kP++;
  else
    kP--;
}
else if (analogRead(A4) < 200 && pkP == 0)
{
  skP=kP;
  pkP=1;

  if(k == 0)
  {
  kP++;
Serial.write("kP ");
Serial.println(kP);
Serial.write("s ");
Serial.println(s);
  }
  else
  {
  kP--;
Serial.write("kP ");
Serial.println(kP);
Serial.write("s ");
Serial.println(s);
  }
}
else
skP=kP;

}

bool testwspol()
{
  if ( abs(kL - kP) < 10 )
    return 1;
  else
    return 0;
}

bool testprze()
{
  if(prze == 10000)
    return 0;
  else
    return 1;
}

void przeszkoda()
{
  if (kL == skL /*&& kP == skP*/)
    {        

      prze++;
    }
  else
    prze=0;

          //  Serial.write("prze ");
//Serial.println(prze);
}

/*bool testwspol()
{
  if(kL - skL < dok)
    return 1;
  else
    return 0;
}*/

bool bez_przesz()
{
  for (i=0; i<pasy*krok; i++)
    if (tab_1[i] == '9' || tab_1[i] == '0') //0 do usuniecia
      return 0;
  
  return 1;
     
  
}

void zerowanie()
{
kL=0;
skL=0;
kP=0; 
skP=0; 
}

void parametry()
{


dl=100;
sz=500;



krok=10;

pasy = ceil(sz/szer_poj)*2 - 1;
for (i = 0; i < (pasy+1)/2 * krok; i++)
 tab_1=tab_1+s_pom;
for (i = 0; i < (pasy-1)/2 * krok; i++)
 tab_2=tab_2+s_pom;
  
parametry_b=-1;

Serial.write("pas ");
Serial.println(pasy);

Serial.write("wynik_1 ");
Serial.println(tab_1);
Serial.write("wynik_2 ");
}


void pomiar()
{
 p_p= round(kL/(krok*5));
p=p_p;


 if (p > pp)
 {
  Serial.write("                  p ");
Serial.println(p);
Serial.write("                     l");
Serial.println(lokal);

  pomiar_r();
  tab[lokal] = pom;
 pp=p;
lokal++;


 }
 else
 {
  
  pp=p;

 }

//if()  //zerowanie po kolejnych pasach - nie musi być dla char

//if (pasy_k == (pasy-1/2))
 
 
 //Serial.write("tab ");
//Serial.println(tab);
}

float mapFloat(int input, int inMin, int inMax, float outMin, float outMax)
{
  float scale = (float)(input - inMin) / (inMax - inMin);
  return ((outMax - outMin) * scale) + outMin;
}

void pomiar_r()
{

int ii=0;
    
    
    for (i=0; i <10 ; i++)
    {float sensitivity;
    sensitivity = mapFloat(1000, 0, 1023, 0.5, 10.0);
    int storedTimeDeltaDifference = (storedTimeDelta - signalTimeDelta) * sensitivity;

    if (storedTimeDeltaDifference > 5000)
      ii++;
    }


if(ii>8)
  pom='1';
else
  pom='0';
}



void odwrot()
{
for (i=0;i< tab_2.length()/2; i++)
   {
    tab_p=tab_2[i];
    tab_2[i]=tab_2[tab_2.length()-i-1];
    tab_2[tab_2.length()-i-1]=tab_p;
   }
   
  // swap(tab_2[i], tab_2[tab[2].length-i-1])
}

void loop() {




if (parametry_b == 0)
parametry();      //przypisanie parametrow
//pomiarlewe();     

switch ( s )
{
  
case 0: {                                  //case odpowiadający za jazdę do przodu 
  pasy_k++;                                //zwiększenie ilości przejechanych pasów
  jazdaprzod();                            //przypisanie parametrów                

  while(kL <= limit)                       //pętla odpowiadająca za wykonywanie ruchu na odpowiednią odległość
  { 
    pomiar();                              //funkcja wykonująca pomiar czujnikiem metalu
    pomiarlewe();                          //funkcja wykonująca pomiar przejechanej odległości lewego koła
    pomiarprawe();                         //funkcja wykonująca pomiar przejechanej odległości prawego koła
    przeszkoda();                          //funkcja sprawdzająca czy oba koła są zablokowane
    
  /*  if (testwspol() && testprze() )        //warunek sprawdzający czy wystąpiła przeszkoda w ruchu
      continue;
    else {
      if( !testwspol() ) {                 //warunek sprawdzający czy przeszkoda dotyczy jednego koła
         testwspol_g();                   //funkcja wykonująca korektę położenia jednego z kół
          break;
          }
      else if ( !testprze() ) {            //warunek sprawdzający czy przeszkoda dotyczy obu kół
        testprze_g();                      //funkcja wykonująca korektę położenia pojazdu
      }
    }*/
  }

  if(ps == 1)                              //warunek sprawdzający, w którą ze stron ma skręcić pojazd
    s=2;
  else if (ps == 2)
    s=1;
  

  
  if (pasy_k == ( (pasy+1)/2) ) {          //warunek sprawdzający cze należy rozszerzyć 
    if(ps == 1) {
      s=1;
      ps=2;
      }
  else if (ps == 2)  {
    s=2;
    ps=1;
    }
  }
  
  if (s_prze!=1)                          //
    przypisywanie();
  else  {
    s=0;
    s_prze=0;
    }
  
  zerowanie();

  break;
}

case 1:
{

//if (pasy_k == ( (pasy+1)/2) ) {          //warunek sprawdzający cze należy rozszerzyć   

skretprawo();                            //funkcja przypisująca parametry do skrętu w prawo
  
  while(kL < limit)
    pomiarlewe();
    
  zerowanie();                           //funkcja zerująca przejechaną odległość
  jazdaprzod();                          //funkcja przypisująca parametry do jazdy prosto o odległość równą długości pojazdu 
  limit = pros;


  while(kL < limit)
    pomiarlewe();
  
  zerowanie();
  skretprawo(); 
  
  while(kL < limit)
    pomiarlewe();

  zerowanie();

  if (ps == 1)                            //zapamiętanie kierunku wykonanego skrętu
    ps=2;
  else if (ps == 2)
    ps=1;

  s=0;                                  
  zerowanie();

  break;
}

case 2:
{

  
  
skretlewo();
  
  while(kP < limit)
  pomiarprawe();

  zerowanie();
  
jazdaprzod();
limit = pros;

  while(kP < limit)
  pomiarprawe();
  
zerowanie();
skretprawo(); 

  while(kP < limit)
  pomiarprawe();

  if (ps == 1)
  ps=2;
  else if (ps == 2)
  ps=1;
  
  s=0;
  zerowanie();

  break;
}


}

if (pasy_k == pasy /*|| bez_przesz()*/)
{ //if (!bez_przeszkod())
 // odwrot();
  stop();
  }



  //Serial.println(analogRead(A5));
 //Serial.println(kL);
  //Serial.println(s);
  //Serial.println(analogRead(A4));
  //delay(500);



}
