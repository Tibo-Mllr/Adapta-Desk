void reagir(int Ordre) {          //envoyer
  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, Ordre);
  digitalWrite(Res, HIGH);
  analogWrite(En, 180);

  //vérifier
  digitalRead(CB);
  digitalRead(CH);
  digitalRead(CPB);
  analogRead(CP);
  donnee = Ordre;

  //envoyer positions
  com(0);
}


void compte() {                   //encodeur

  //montée
  if (donnee >= 160) {
    compt++;
  }

  //descente
  if (110 >= donnee >= 96) {
    compt--;
  }

  //autres
  if (donnee <= 95) {
    compt = compt;
  }
}


void set_reset() {                //reset
  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 2);
  digitalWrite(Res, HIGH);
  delay(100);

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 0);
  digitalWrite(Res, HIGH);
  delay(1000);

  if (digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0) {
    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 161);
    digitalWrite(Res, HIGH);
    delay(100);

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);

    //descend
    while (digitalRead(CB) == 0 and analogRead(MHS) <= 650 and digitalRead(BH) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 97);
      digitalWrite(Res, HIGH);
      digitalRead(CB);
      analogRead(MHS);
    }

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);
    delay(500);

    //monte
    while (digitalRead(CH) == 0 and analogRead(MHS) <= 650 and digitalRead(BB) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 161);
      digitalWrite(Res, HIGH);
      digitalRead(CH);
      analogRead(MHS);
    }

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);
    delay(1000);
    compt = 365;

    //monte pivot
    while (analogRead(CP) <= 679 and analogRead(MPS) <= 650 and digitalRead(BPB) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 21);
      digitalWrite(Res, HIGH);
      analogRead(CP);
      analogRead(MPS);
    }

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);
    delay(500);

    //descent pivot
    while (digitalRead(CPB) == 0 and analogRead(MPS) <= 650 and digitalRead(BPH) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 13);
      digitalWrite(Res, HIGH);
      digitalRead(CPB);
      analogRead(MPS);
    }

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);
    delay(500);

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 2);
    digitalWrite(Res, HIGH);
    delay(100);

    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 0);
    digitalWrite(Res, HIGH);
  }
}

void com(int alerte) {
  pivot = map(analogRead(CP), 479, 650, 0, 100);
  hauteur = map(compt, 0, 365, 0, 100);

  //sur-tension
  if (alerte == 1) {
    Serial.println("Il y a un problème");
  }
  else {
    if (pivot <= 9 and hauteur <= 9) {
      Serial.print("Pivot : ");
      Serial.print(pivot);
      Serial.print("% ; Hauteur : ");
      Serial.print(hauteur);
      Serial.println("%");
    }
    if ((pivot >= 10 and hauteur <= 9) or (pivot <= 9 and hauteur >= 10) and pivot != 100 and hauteur != 100) {
      Serial.print("Pivot: ");
      Serial.print(pivot);
      Serial.print("% ; Hauteur : ");
      Serial.print(hauteur);
      Serial.println("%");
    }
    if (pivot >= 10 and hauteur >= 10) {
      Serial.print("Pivot: ");
      Serial.print(pivot);
      Serial.print("% ; Hauteur: ");
      Serial.print(hauteur);
      Serial.println("%");
    }
    if (pivot == 100 and hauteur == 100) {
      Serial.print("Pivot:");
      Serial.print(pivot);
      Serial.print("% ; Hauteur:");
      Serial.print(hauteur);
      Serial.println("%");
    }
  }
}

void enregistre(int H, int P) {
  H = map(H, 130, 160, 0, 1500);
  P = map(P, 0, 450, 479, 650);
  Serial.print("Hauteur :");
  Serial.println(H);
  Serial.print("Pivot :");
  Serial.println(P);
  Serial.println("Veuillez Patienter...");

  //monte
  while (compt <= H and digitalRead(CH) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0) {
    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 161);
    digitalWrite(Res, HIGH);
    digitalRead(CH);
    digitalRead(BH);
    digitalRead(BB);
    digitalRead(BPH);
    digitalRead(BPB);
    Serial.println("Ca marche");
  }

  //descend
  while (compt >= H and digitalRead(CB) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0) {
    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 97);
    digitalWrite(Res, HIGH);
    digitalRead(CB);
    digitalRead(BH);
    digitalRead(BB);
    digitalRead(BPH);
    digitalRead(BPB);
  }

  if (P != 0) {
    //monte pivot
    while (analogRead(CP) <= P and analogRead(CP) <= 650 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 21);
      digitalWrite(Res, HIGH);
      analogRead(CP);
      digitalRead(BH);
      digitalRead(BB);
      digitalRead(BPH);
      digitalRead(BPB);
    }

    //descend pivot
    while (analogRead(CP) >= P and digitalRead(CPB) == 0 and digitalRead(BH) == 0 and digitalRead(BB) == 0 and digitalRead(BPH) == 0 and digitalRead(BPB) == 0) {
      digitalWrite(Res, LOW);
      shiftOut(Data, Clock, LSBFIRST, 13);
      digitalWrite(Res, HIGH);
      digitalRead(CPB);
      digitalRead(BH);
      digitalRead(BB);
      digitalRead(BPH);
      digitalRead(BPB);
    }
  }

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 2);
  digitalWrite(Res, HIGH);
  delay(100);

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 0);
  digitalWrite(Res, HIGH);
  Serial.println("Votre position a été mise en place !");
  delay(500);

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 0);
  digitalWrite(Res, HIGH);
}

void range() {

  //descend
  while (digitalRead(CB) == 0 and analogRead(MHS) <= 650 and digitalRead(BPH) == 0) {
    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 97);
    digitalWrite(Res, HIGH);
    digitalRead(CB);
    digitalRead(BPH);
    analogRead(MHS);
  }

  //descend pivot
  while (digitalRead(CPB) == 0 and analogRead(MPS) <= 650 and digitalRead(BPH) == 0) {
    digitalWrite(Res, LOW);
    shiftOut(Data, Clock, LSBFIRST, 13);
    digitalWrite(Res, HIGH);
    digitalRead(CPB);
    digitalRead(BPH);
    analogRead(MPS);
  }

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 2);
  digitalWrite(Res, HIGH);
  delay(100);

  digitalWrite(Res, LOW);
  shiftOut(Data, Clock, LSBFIRST, 0);
  digitalWrite(Res, HIGH);
}
