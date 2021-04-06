const int RELAY1_PIN = 3;
const int RELAY2_PIN = 4;
const int DRAINAGE_RELAY_PIN = 5;
const int PUMP_RELAY_PIN = 6;
const int TURBIDITY_PIN = A1;


void setup() 
{
    relay_init();
    serial_init();
    turbidity_init();
    pump_init();
    drainage_init();
}

void loop() 
{
    /*int value = turbidity_read();
    Serial.println(value);*/

    drainage_open();
    Serial.println("open");
    delay(2000);
    drainage_close();
    Serial.println("close");
    delay(2000);
}


void serial_init()
{
  Serial.begin(9600);
}


void relay_init()
{
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
}
void relay_open(int relayPin)
{
    digitalWrite(relayPin, HIGH);
}
void relay_close(int relayPin)
{
    digitalWrite(relayPin, LOW);
}

//////////////////////////
//      DRAINAGE        //
//////////////////////////

void drainage_init()
{
    pinMode(DRAINAGE_RELAY_PIN, OUTPUT);
}
void drainage_open()
{
    relay_open(DRAINAGE_RELAY_PIN);
}
void drainage_close()
{
    relay_close(DRAINAGE_RELAY_PIN);
}

//////////////////////////
//      TURBIDITY       //
//////////////////////////
void turbidity_init()
{
    pinMode(TURBIDITY_PIN, INPUT);
}
int turbidity_read()
{
  int val = analogRead(TURBIDITY_PIN);
  return val * (5.0 / 1024.0);
}


//////////////////////////
//       PUMP           //
//////////////////////////

void pump_init()
{
    pinMode(PUMP_RELAY_PIN, OUTPUT);
}
void pump_open()
{
    relay_open(PUMP_RELAY_PIN);
}
void pump_close()
{
    relay_close(PUMP_RELAY_PIN);
}
