////////////////  //This is a test
/// SOLENOID ///  //Change this
////////////////

void solenoidInit(){
  pinMode(SOLENOID_PIN, OUTPUT);
}

void solenoidTest(){
  solenoidOpen();
  delay(1000);
  solenoidClose();
  delay(1000);
}
void solenoidOpen(){
  digitalWrite(SOLENOID_PIN, HIGH);
}

void solenoidClose(){
  digitalWrite(SOLENOID_PIN, LOW);
}
