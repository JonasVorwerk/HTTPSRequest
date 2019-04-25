
#define NUM_DOTS 3

//uint8_t hue_dot[NUM_DOTS];
//uint8_t sat_dot[NUM_DOTS];
//uint8_t val_dot[NUM_DOTS];
int age_dot[NUM_DOTS];
float spd_dot[NUM_DOTS];
float pos_dot[NUM_DOTS];

uint8_t global_speed_dots =     20; //0-100
uint8_t speed_dots_difference = 30;
uint8_t min_speed_dot =         40;
uint8_t max_speed_dot =         90;

void move_a_dot() {
  
  //calculate all dots
  for (int i = 0 ; i < 2 ; i++) {

    //show the dot
    leds[ int(pos_dot[i]) ] = CHSV(hue, sat, val);

    //change dot pos_position
    pos_dot[i] = pos_dot[i] + spd_dot[i];

    //increase age
    age_dot[i] = age_dot[i] + 1;
    if(age_dot[i] > 3000) init_dot(i);

    //if dot at end, move to beginning 
    if( pos_dot[i] >= NUM_LEDS && spd_dot[i] > 0) pos_dot[i] = 0;  
    //other way around, if at beginning, move to the end
    if( pos_dot[i] < 0 && spd_dot[i] < 0) pos_dot[i] = NUM_LEDS - 1;

  }

  fadeToBlackBy(leds, NUM_LEDS, fadeout);
}

//init one dot
void init_dot(int i) {
//  Serial.print("init_dot:" + String(i));

  min_speed_dot = global_speed_dots;
  max_speed_dot = global_speed_dots + speed_dots_difference;
  max_speed_dot = constrain(max_speed_dot, 0, 100);
  spd_dot[i] = map(random(100), 0, 100, min_speed_dot, max_speed_dot) / 100.0 * (random(2) - 1 | 1);
  
  pos_dot[i] = random(NUM_LEDS-1);
  age_dot[i] = random(int(3000/10));
  //hue_dot[i] = random(256); 
}

//init all dots
void init_all_dots() {
  //Serial.println("init_all_dots"); 
  for (int i = 0 ; i < NUM_DOTS ; i++) {
    init_dot(i);
  }
}
