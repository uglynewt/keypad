struct pin_details {
  const int pin;
  char good_state, last_state; /* high or low */
  unsigned int last_change; /* ms */
  const char name;
};

struct pin_details all_pins[] = {
  { 2, 1,1,0, '0' },
  { 3, 1,1,0, '1' },
  { 4, 1,1,0, '2' },
  { 5, 1,1,0, '3' },
  { 6, 1,1,0, '4' },
  { 7, 1,1,0, '5' },
  { 8, 1,1,0, '6' },
  { 9, 1,1,0, '7' },
  { 10, 1,1,0, '8' },
  { 11, 1,1,0, '9' },
  { 12, 1,1,0, 'a' },
  { 13, 1,1,0, 'b' } /* D13 needs an external pullup resistor */
};

const int debounceDelay = 40;
const int pollDelay = 10;

void setup() {
  Serial.begin(9600);

  
  /* set all digital pins to read, with internal pullup */
  int i;
  for (i=0;i < sizeof(all_pins)/sizeof(all_pins[0]); i++) {
    pinMode(all_pins[i].pin,INPUT_PULLUP);
  }
}

void loop() {
  int state,i;
  struct pin_details *p;
  char message[3];

    for (i=0;i < sizeof(all_pins)/sizeof(all_pins[0]); i++)
    {

      p = all_pins+i;
      state=digitalRead(p->pin);
      if (p->good_state==state)
      {
        /* no change; clear any flickers and move on to next pin */
        p->last_state = state;
        continue;
      }

      /* change detected */

      if (state!=p->last_state)
      {
        /* this is new; start the clock */
        p->last_state = state;
        p->last_change=millis();
        continue;
      }

      /* already seen, check how long ago */
      if (millis() - p->last_change > debounceDelay)
      {
        /* state change detected, announce it */
        message[0] = p->name;
        message[1] = state?'r':'p'; /* release / press */
        message[2] = 0;
        Serial.println(message);
        p->good_state = state;
      }

    }

    delay(pollDelay);
}
