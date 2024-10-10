#include <TMCStepper.h>

#define EN_PIN           27 // Enable
#define DIR_PIN          26 // Direction
#define STEP_PIN         9 // Step
#define CS_PIN 53  // Example chip select pin
#define SW_MOSI 51 // Software MOSI pin
#define SW_MISO 50 // Software MISO pin
#define SW_SCK 52  // Software SCK pin

#define R_SENSE 0.075f // Sense resistor value in ohms. ADJUST THIS TO YOUR ACTUAL VALUE!

// Create a TMC5160 object
TMC5160Stepper driver(CS_PIN, R_SENSE);

void setup() {
  Serial.begin(9600);
  
  SPI.begin();
  driver.begin(); // Initialize the driver
  
  // Configure your driver settings here
  driver.toff(5);

  driver.rms_current(800); // Set motor RMS current in mA
  driver.microsteps(64);   // Set microsteps
  
  // Enable StallGuard
  driver.TCOOLTHRS(0xFFFFF); // 20bit max
  
  // Configure COOLCONF register for StallGuard
  driver.COOLCONF(0); // Reset COOLCONF
  driver.sgt(0); // Set StallGuard threshold (range: -64 to 63)
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);  // Enable driver in hardware
}

void loop() {

 
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(10);


//  
//  // Print the StallGuard value
  uint32_t ms = millis();
  static uint32_t last_time = 0;
  if ((ms - last_time) > 100) {

      // Read the DRV_STATUS register
    uint32_t drv_status = driver.DRV_STATUS();
//  
//  // Extract the StallGuard result (bits 9:0 of DRV_STATUS)
    int sg_result = drv_status & 0x3FF;
    Serial.print("StallGuard value: ");
    Serial.println(sg_result);
    
    last_time = ms;
  }
  
}
