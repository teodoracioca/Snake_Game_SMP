#include <LedControl.h>

//structura pentru sarpe
typedef struct Sarpe Sarpe;
struct Sarpe{
  int cap[2];            //coordonatele pentru capul sarpelui: (randul, coloana)
  int corp[40][2];       //un vector care contine indecsii pentru (randul, coloana)
  int lungime;           //lungimea corpului sarpelui fara cap
  int directie[2];       //dir[0] este axa x (modific coloanele) si dir[1] este axa y (modific randurile)
};

//structura pentru hrana
typedef struct Hrana Hrana;
struct Hrana{
  int randul;  //pozitia de pe rand
  int coloana; //pozitia de pe coloana
};

//MAX7219 Matrice cu led-uri
const int DIN =12;
const int CS =11;
const int CLK = 10;
LedControl lc = LedControl(DIN, CLK, CS,1);

const int x_joystick = A3; //valoarea coordonatei x de pe joystick
const int y_joystick = A4;  //valoarea coordonatei x de pe joystick

byte pic[8] = {0,0,0,0,0,0,0,0}; //cele 8 randuri din matricea de led-uri

//initializare obiect de tip Sarpe
Sarpe sarpe = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};

//initializare obiect de tip Hrana
Hrana hrana = {(int)random(0,8),(int)random(0,8)};

//variabile pentru a retine timpul curent
float timp_vechi = 0;
float timer = 0;
float updateRate = 3;

int i,j;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  //setam pinii joystick-ului ca input
  pinMode(x_joystick, INPUT);
  pinMode(y_joystick, INPUT);
}

void loop() {
  
  float deltaTime = calculateDeltaTime();
  timer += deltaTime;

  int x = analogRead(x_joystick);
  int y = analogRead(y_joystick);
  
  if(x<100 && sarpe.directie[1]==0){ //daca vrem sa mergem in stanga si sarpele nu se misca pe axa x 
    sarpe.directie[0] = 0;              //atunci axa y ramane la fel, adica nu se va misca pe ea
    sarpe.directie[1] = -1;             //si se va deplasa la stanga pe axa x, adica ne deplasam pe coloane
  }else if(x >920 && sarpe.directie[1]==0){
    sarpe.directie[0] = 0;
    sarpe.directie[1] = 1;
  }else if(y<100 && sarpe.directie[0]==0){
    sarpe.directie[0] = -1;
    sarpe.directie[1] = 0;
  }else if(y >920 && sarpe.directie[0]==0){
    sarpe.directie[0] = 1;
    sarpe.directie[1] = 0;
  }
  
  //Actualizare
  if(timer > 1000/updateRate){
    timer = 0;
    Actualizare();
  }

  Setare();
  
}

float calculateDeltaTime(){
  float timp_curent = millis(); //masuram timpul in milisecunde de la momentul pornirii programului
  float durata = timp_curent - timp_vechi;  //calculam cat a durat sa parcurga o bucla
  timp_vechi = timp_curent; //salvam timpul de la trecerea in utlima bucla
  return durata;
}

void reset(){
  for(int j=0;j<8;j++){
    pic[j] = 0;
  }
}
void Actualizare(){
  reset(); //resetam matricea
  
  int sarpe_cap_nou[2] = {sarpe.cap[0]+sarpe.directie[0], sarpe.cap[1]+sarpe.directie[1]}; //noul cap al sarpelui devine capul vechi

  //daca sarpele iese din matrice
  if(sarpe_cap_nou[0]==8){ //daca capul sarpelui iese din matrice (intra in perete) prin dreapta, atunci acesta va reveni pe coloana 0
    sarpe_cap_nou[0]=0;     //de pe acelasi rand
  }else if(sarpe_cap_nou[0]==-1){
    sarpe_cap_nou[0] = 7;
  }else if(sarpe_cap_nou[1]==8){
    sarpe_cap_nou[1]=0;
  }else if(sarpe_cap_nou[1]==-1){
    sarpe_cap_nou[1]=7;
  }
  
  //verificam daca sarpele se loveste de el
   for(j=0; j<sarpe.lungime; j++){
    if(sarpe.corp[j][0] == sarpe_cap_nou[0] && sarpe.corp[j][1] == sarpe_cap_nou[1]){ //daca capul acestuia ajunge in locul ultimei parti din corp
      //atunci facem o pauza de 1 secunda si resetam jocul, deoarece am pierdut
      delay(1000);
      sarpe = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};       //reinitializam obiectul de tip sarpe
      hrana = {(int)random(0,8),(int)random(0,8)};  //reinitializam obiectul de tip sarpe
      return;
    }
  }

  //verificam daca sarpele s-a hranit
  if(sarpe_cap_nou[0] == hrana.randul && sarpe_cap_nou[1] == hrana.coloana){
    sarpe.lungime = sarpe.lungime+1; //daca da atunci ii crestem lungimea si ii dam alta pozitie random mancarii
    hrana.randul = (int)random(0,8);
    hrana.coloana = (int)random(0,8);
  }else{
    removeFirst(); //daca nu, mutam pozitia sarpelui spre stanga cu o pozitie
  }

  sarpe.corp[sarpe.lungime-1][0]= sarpe_cap_nou[0];
  sarpe.corp[sarpe.lungime-1][1]= sarpe_cap_nou[1];
  
  sarpe.cap[0] = sarpe_cap_nou[0];
  sarpe.cap[1] = sarpe_cap_nou[1];
  
  //actualizam noua matrice cu noua lungime a sarpelui si cu noua pozitie aleasa random a hranei prin  shiftarea cu 7
  for(j=0;j<sarpe.lungime;j++){
    pic[sarpe.corp[j][0]] |= 128 >> sarpe.corp[j][1]; //shiftam la dreapta pentru a aprinde led-urile pe pozitiile noi ale sarpelui+hrana
  }
  pic[hrana.randul] |= 128 >> hrana.coloana;
  
}

void Setare(){ //aici setam LED-urile de pe randuri
  
   for(i=0;i<8;i++){
    lc.setRow(0,i,pic[i]);
   }
}

void removeFirst(){
  for(j=1;j<sarpe.lungime;j++){
    sarpe.corp[j-1][0] = sarpe.corp[j][0];
    sarpe.corp[j-1][1] = sarpe.corp[j][1];
  }
}
