#include <SPI.h>
#include <Pixy.h>
#include <ZumoMotors.h>

#define X_CENTER    160L
#define LED_PIN     13
#define MAX_SIZE    100

Pixy pixy;
ZumoMotors motors;
uint32_t lastDetected = 0;
int found_block = 0;

void setup() {
  Serial.begin(9600);
  pixy.init();
  Serial.print("Init started");
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {
  uint16_t blocks;
  // we find block here
  blocks = pixy.getBlocks();

  int tracked_block;
  
  motors.setRightSpeed(0);
  motors.setLeftSpeed(0);
  delay(2);
  if(found_block == 0){
      if(blocks){
        Serial.print("Block count:");
        Serial.println(blocks);

        digitalWrite(LED_PIN, HIGH);
        follow_block(tracked_block);
      }
    else{
      Serial.println("No block");
      digitalWrite(LED_PIN, LOW);
      find_block();
  
    }
    delay(2);
  }
  else{
    delay(5000);
    found_block = 0;
  }  

}

// signature is marked with 1 - 7 not 0
int signature = 0;
int old_x = 0;

int track_block(int n_blocks){
  // blocks is number of blocks
  // The API
  // pixy.blocks[i].signature 
  // pixy.blocks[i].x
  // pixy.blocks[i].y
  // pixy.blocks[i].width
  // pixy.blocks[i].height 

  int block_selected;

  for(int i = 0;i < n_blocks; i++){
    // if no signature just use or found old signature
    if((signature == 0) || (pixy.blocks[i].signature == signature)){
      block_selected = i;
      // Robot can only turn not tilt
      old_x = pixy.blocks[i].x;

    }
  }
  return block_selected;
}

void follow_block(int tracked_block){

    long block_size = pixy.blocks[tracked_block].height * pixy.blocks[tracked_block].width;  
    if(block_size < MAX_SIZE){
        Serial.print("Signature: ");
        Serial.println(pixy.blocks[tracked_block].signature);
        Serial.print("Not close enough, size is ");
        Serial.println(block_size);
        motors.setRightSpeed(400);
        motors.setLeftSpeed(400);
    }
    else{
      Serial.println("Alright close enough");
      found_block = 1;
    }
  
}

int roam = 0;
void find_block(){
  // TODO: Create roam mode a.k.a hide and seek mode
  if(roam == 0){
    motors.setRightSpeed(-200);
    motors.setLeftSpeed(200);
  }
  else if(roam == 1){
    motors.setRightSpeed(200);
    motors.setLeftSpeed(-200);
  }
  else if(roam == 2){
    motors.setRightSpeed(400);
    motors.setLeftSpeed(400);
  }
  else{
    motors.setRightSpeed(-400);
    motors.setLeftSpeed(-400);
  }
  roam = random(0,3);
  delay(500);

}
