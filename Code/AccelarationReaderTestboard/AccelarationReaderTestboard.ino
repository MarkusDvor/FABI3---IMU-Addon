#include <Wire.h>

#define IMU_addr 0x53 // ADXL345 I2C address

float prevValue = 0;

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  Wire1.setSDA(26);
  Wire1.setSCL(27);
  Wire1.begin();
  Serial.begin(115200);

  writeToRegister(IMU_addr, 0x2D, 0x08); // Enable measurement mode by writing 0x08 to the POWER_CTL register (register address 0x2D)
  writeToRegister(IMU_addr, 0x31, 0x08); // Set the data format to full resolution (10 bits) by writing 0x08 to the DATA_FORMAT register (register address 0x31)
}

void loop() {
  float x = readAxis(0x32); // Read X-axis -> 0x32 register address for XDATA0
  float y = readAxis(0x34); // Read Y-axis -> 0x34 Register address for YDATA0 
  float z = readAxis(0x36); // Read Z-axis -> 0x36 Register address for ZDATA0

  float Value = sqrt(x*x+y*y+z*z);  // resulting vector from x,y,z
  
  /*
  Serial.print("Value: ");
  Serial.println(Value);
  */

/*
  float accValue = Value - prevValue; // subtracting previous vector from current one to get the acceleration
  if(accValue < 0)
    accValue = accValue * (-1);
  prevValue = Value;
*/

/*
  if(accValue >= 10000) // threshhold to count as movement
  {
    Serial.println("Movement detected!");
    Serial.println();
  }
*/

  /*
  Serial.print("acceleration: ");
  Serial.println(accValue);
  Serial.println();
  */

  
  // Print the raw values
  Serial.print("X: ");
  Serial.print(x);
  Serial.print("\tY: ");
  Serial.print(y);
  Serial.print("\tZ: ");
  Serial.println(z);
  

  delay(50);
}

int readAxis(byte axis_addr) {
  // Read the 2 bytes of data for the specified axis
  Wire1.beginTransmission(IMU_addr);
  Wire1.write(axis_addr);
  Wire1.endTransmission(false);
  Wire1.requestFrom(IMU_addr, 2, true);

  // Combine the two bytes into a 10-bit signed integer
  int16_t axisValueHB = Wire1.read();
  int16_t axisValueLB = Wire1.read() << 8;
  int16_t axisValue = axisValueHB | axisValueLB;

  return axisValue;
}

void writeToRegister(byte deviceAddress, byte registerAddress, byte data) {
  // Write data to a specific register on the device
  Wire1.beginTransmission(deviceAddress);
  Wire1.write(registerAddress);
  Wire1.write(data);
  Wire1.endTransmission(true);
}
