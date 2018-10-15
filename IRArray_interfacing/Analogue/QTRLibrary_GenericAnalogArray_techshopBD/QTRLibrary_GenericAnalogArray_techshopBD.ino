#include <QTRSensors.h>

//library link: https://www.pololu.com/docs/0J19/all
//tested with: https://techshopbd.com/product-categories/sensors/2494/analog-ir-sensor-array-10b-techshop-bangladesh

/**
  LFR Codes
  Name: QTRLibrary_GenericAnalogArray_techshopBD.ino
  Purpose: Interfacing generic analogue 10 IR array with QTR library

  @author Sakib Ahmed (#ahmadSum1)
  @version 1.0 15/10/18


   This example is modied from "QTRAExample" to work with a generic analogue 10 IR array.
   These reflectance sensors should be connected to analog inputs (A0 to A9).

   The setup phase of this example calibrates the sensor for ten seconds and turns on
   the LED built in to the Arduino on pin 13 while calibration is going on.
   During this phase, you should expose each reflectance sensor to the lightest and
   darkest readings they will encounter.
   For example, if you are making a line follower, you should slide the sensors across the
   line during the calibration phase so that each sensor can get a reading of how dark the
   line is and how light the ground is.  Improper calibration will result in poor readings.
   If you want to skip the calibration phase, you can get the raw sensor readings
   (analog voltage readings from 0 to 1023) by calling qtra.read(sensorValues)
   instead of qtra.readLine(sensorValues, QTR_EMITTERS_ON, true).

   The main loop of the example reads the calibrated sensor values and uses them to
   estimate the position of a line.  You can test this by taping a piece of 3/4" black
   electrical tape to a piece of white paper and sliding the sensor across it.  It
   prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance)
   to 1000 (minimum reflectance) followed by the estimated location of the line as a number
   from 0 to 9000.  1000 means the line is directly under sensor 1, 2000 means directly
   under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
   sensor 0 before being lost.  9000 means the line is directly under sensor 9 or was
   last seen by sensor 9 before being lost.
*/

#define NUM_SENSORS             10  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // Since there is no emitter(ir LED) control option in this Sensor

// sensors 0 through 9 are connected to analog inputs A0 through A9, respectively (right to left)
QTRSensorsAnalog qtra( (unsigned char[]) {
  A9, A8, A7, A6, A5, A4, A3, A2, A1, A0
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];   //array to store Sensor readings


void setup()
{
  delay(500);
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission

  calibrateSensors();  // read the description above for details on calibrating

  delay(1000);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 9000 (right to left)
  // To get raw sensor values, call:
  //  qtra.read(sensorValues); instead of unsigned int position = qtra.readLine(sensorValues);
  unsigned int position = qtra.readLine(sensorValues, QTR_EMITTERS_ON, true);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.print('\t');
  Serial.println(position); // comment this line out if you are using raw values

  delay(100);
}


void calibrateSensors() {
  Serial.print("Calibrating. ");
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
    Serial.print('.');
  }
  Serial.println();
  Serial.println("Calibration complete ");
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
  // print the calibration maximum values measured
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  // print the calibration maximum values measured
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
}
