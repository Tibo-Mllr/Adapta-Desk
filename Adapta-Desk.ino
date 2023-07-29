//C:\Users\Thiba\Desktop\Adapta-Desk\Simulation\Adapta-Desk.ino.hex
const int CPB = A0;
const int MHS = A1;
const int MPS = A2;
const int BPH = A3;
const int BPB = A4;
const int CP = A5;
const int CB = 4;
const int E = 3;
const int CH = 2;
const int BB = 6;
const int BH = 7;
const int En = 9;
const int Data = 10;
const int Res = 11;
const int Clock = 12;
int pivot;                  //position pivot
int hauteur;                //position hauteur
volatile byte compt;        //encodage hauteur
volatile byte donnee;       //montée ou descente
int piv;                    //position pivot enregistrement
int haut;                   //position hauteur enregistrement
int Bluet;                  //bluetooth
String Str;                 //enregistrement


void setup() {
  reagir(0);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(E), compte, CHANGE);
  set_reset();
}


void loop() {

  //sur-courant
  if (analogRead(MHS) >= 650 or analogRead(MPS) >= 650) {
    while (analogRead(MHS) >= 650 or analogRead(MPS) >= 650) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 2);
      digitalWrite(Res, HIGH);
      com(1);
    }
    delay(2000);
  }

  else {

    while ((Serial.available() < 0 or ( (not isAlphaNumeric(Bluet)) and (not isDigit(Bluet)))) and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      reagir(0);
      Bluet = Serial.read();
    }

    //haut et bas
    while (digitalRead(BH) == 1 and digitalRead(BB) == 1 and digitalRead(BPB) == 0 and digitalRead(BPH) == 0) {
      reagir(2);
    }

    //haut et bas pivot
    while (digitalRead(BPH) == 1 and digitalRead(BPB) == 1 and digitalRead(BH) == 0 and digitalRead(BB) == 0) {
      reagir(2);
    }

    //capteur et bouton haut
    while (digitalRead(CH) == 1 and (digitalRead(BH) == 1 or Serial.read() == "H")) {
      reagir(2);
    }

    //capteur et bouton bas
    while (digitalRead(CB) == 1 and (digitalRead(BB) == 1 or Serial.read() == "h")) {
      reagir(2);
    }

    //capteur et bouton haut pivot
    while (analogRead(CP) >= 650 and (digitalRead(BPH) == 1 or Serial.read() == "P")) {
      reagir(2);
    }

    //capteur et bouton bas pivot
    while (digitalRead(CPB) == 1 and (digitalRead(BPB) == 1 or Serial.read() == "p")) {
      reagir(2);
    }

    //haut et haut
    while (digitalRead(BH) == 1 and digitalRead(BPH) == 1 and digitalRead(BB) == 0 and digitalRead(BPB) == 0 and digitalRead(CH) == 0 and analogRead(CP) <= 650 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      digitalRead(CH);
      analogRead(CP);
      reagir(181);
    }

    //haut et haut capteur
    while (digitalRead(BH) == 1 and digitalRead(BPH) == 1 and (digitalRead(CH) == 1 or analogRead(CP) >= 650) and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      reagir(2);
    }

    //haut et bas
    while (digitalRead(BH) == 1 and digitalRead(BPB) == 1 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(CH) == 0 and digitalRead(CPB) == 0  and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      digitalRead(CH);
      digitalRead(CPB);
      reagir(173);

    }

    //haut et bas capteur
    while (digitalRead(BH) == 1 and digitalRead(BPB) == 1 and (digitalRead(CH) == 1 or digitalRead(CPB) == 1)  and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      reagir(2);
    }

    //bas et haut
    while (digitalRead(BB) == 1 and digitalRead(BPH) == 1 and digitalRead(BH) == 0 and digitalRead(BPB) == 0 and digitalRead(CB) == 0 and analogRead(CP) <= 650 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      digitalRead(CB);
      analogRead(CP);
      reagir(117);
    }

    //bas et haut capteur
    while (digitalRead(BB) == 1 and digitalRead(BPH) == 1 and (digitalRead(CB) == 1 or analogRead(CP) >= 650) and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      reagir(2);
    }

    //bas et bas
    while (digitalRead(BB) == 1 and digitalRead(BPB) == 1 and digitalRead(BH) == 0 and digitalRead(BPH) == 0 and digitalRead(CB) == 0 and digitalRead(CPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      digitalRead(CB);
      digitalRead(CPB);
      reagir(109);
    }

    //bas et bas capteur
    while (digitalRead(BB) == 1 and digitalRead(BPB) == 1 and (digitalRead(CB) == 1 or digitalRead(CPB) == 1) and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
      reagir(2);
    }

    //haut bouton
    while (digitalRead(BH) == 1 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650 and digitalRead(CH) == 0) {
      digitalRead(CH);
      reagir(161);
    }

    if (Serial.available() > 0) {
      Bluet = Serial.read();
    }
    reagir(0);

    //haut Bluetooth
    while (Bluet == 72) {
      Bluet = Serial.read();
      while (Bluet != 83 and digitalRead(CH) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
        Bluet = Serial.read();
        digitalRead(CH);
        digitalRead(BH);
        digitalRead(BB);
        digitalRead(BPH);
        digitalRead(BPB);
        reagir(161);
        if (Bluet == 83) {
          break;
        }
      }
      //break;
    }

    //bas bouton
    while (digitalRead(BH) == 0 and digitalRead(BB) == 1 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650 and digitalRead(CB) == 0) {
      digitalRead(CB);
      reagir(97);
    }

    //bas Bluetooth
    while (Bluet == 104) {
      Bluet = Serial.read();
      while (Bluet != 83 and digitalRead(CB) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
        Bluet = Serial.read();
        digitalRead(CB);
        digitalRead(BH);
        digitalRead(BB);
        digitalRead(BPH);
        digitalRead(BPB);
        reagir(97);
        if (Bluet == 83) {
          break;
        }
      }
    }

    //haut pivot bouton
    while (digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 1 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650 and analogRead(CP) <= 650) {
      analogRead(CP);
      reagir(21);
    }

    //haut pivot Bluetooth
    while (Bluet == 80) {
      Bluet = Serial.read();
      while (Bluet != 83 and analogRead(CP) <= 650 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
        Bluet = Serial.read();
        analogRead(CP);
        digitalRead(BH);
        digitalRead(BB);
        digitalRead(BPH);
        digitalRead(BPB);
        reagir(21);
        if (Bluet == 83) {
          break;
        }
      }
    }

    //bas pivot bouton
    while (digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 1 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650 and digitalRead(CPB) == 0) {
      digitalRead(CPB);
      reagir(13);
    }

    //bas pivot Bluetooth
    while (Bluet == 112) {
      Bluet = Serial.read();
      while (Bluet != 83 and digitalRead(CPB) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0 and analogRead(MHS) <= 650 and analogRead(MPS) <= 650) {
        Bluet = Serial.read();
        digitalRead(CPB);
        digitalRead(BH);
        digitalRead(BB);
        digitalRead(BPH);
        digitalRead(BPB);
        reagir(13);
        if (Bluet == 83) {
          break;
        }
      }
    }

    //enregistrements de positions

    if (Serial.available() != 0 and Bluet != 72 and Bluet != 104 and Bluet != 80 and Bluet != 112 and Bluet != 83) {
      Str = Serial.readStringUntil(83);
      Str = Serial.readStringUntil(83);
      int haut = Str.substring(0, 3).toInt();
      int piv = Str.substring(3).toInt();
      Serial.print("Haut :");
      Serial.println(haut);
      Serial.print("Piv :");
      Serial.println(piv);
      Serial.println("ok");
      enregistre(haut, piv);
    }

    //rangement
    while (digitalRead(BH) == 1 and digitalRead(BB) == 1 and digitalRead(BPB) == 1 and digitalRead(BPH) == 0) {
      range();
    }
  }
}
/*Serial enregistrement ???
  pour calibrer : print compt et non hauteur(map)
  Serial.println(tt le temps le même nombre de caractères)*/
