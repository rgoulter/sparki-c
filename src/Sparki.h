#ifndef Sparki_h
#define Sparki_h

//#define NO_LCD    // disables the LCD, frees up 3088 Bytes Flash Memory, 1k RAM
//#define NO_ACCEL  // disables the Accelerometer, frees up 598 Bytes Flash Memory
//#define NO_MAG    // disables the Magnetometer, frees up 2500 Bytes Flash Memory

#include "Arduino.h"
// #include "Print.h" //R Print is CPP, so can't use for C-only.

#define SHIFTREG_LATCH      TXLED0   // PD5
#define STATUS_LED          13
#define BUZZER              11       // PB7
#define BUZZER_FREQ         2800
#define ULTRASONIC_ECHO     5        // PC6
#define ULTRASONIC_TRIG     10       // PB6
#define IR_RECEIVE          7        // PE6
#define IR_SEND             6        // PD7

#define SERVO               9        //
#define SERVO_LEFT          -80
#define SERVO_CENTER        0
#define SERVO_RIGHT         80
#define MAX_SERVO_OFFSET    30

// defining the MUX pins
#define MUX_ANALOG      A2 // PF5
#define MUX_A           A3 // PF4
#define MUX_B           A4 // PF1
#define MUX_C           A5 // PF0

// defining the IR line sensor pins (on the multiplexer)
#define IR_EDGE_RIGHT      LOW,  HIGH, LOW   // Mux A2
#define IR_LINE_RIGHT      HIGH, LOW,  LOW   // Mux A1
#define IR_LINE_CENTER     LOW,  LOW,  LOW   // Mux A0
#define IR_LINE_LEFT       HIGH, HIGH, LOW   // Mux A3
#define IR_EDGE_LEFT       HIGH, LOW,  HIGH  // Mux A5

// defining the light sensors
#define LIGHT_RIGHT         HIGH, HIGH, HIGH // Mux A7
#define LIGHT_CENTER        LOW,  HIGH, HIGH // Mux A6
#define LIGHT_LEFT          LOW,  LOW,  HIGH // Mux A4

// defining battery monitor constants
#define BATTERY_MONITOR     A10       // PB6
#define ADC_REFERENCE       2.56
#define VOLTAGE_DIVIDER     4.7    // using 47k and 10k resistors
#define MAX_BAT_V          (VOLTAGE_DIVIDER * ADC_REFERENCE)

// define the shift registers pin output values for the RGB arrays
#define RGB_R 0x01 // pin value of the Red LED on the RGB on the shift register
#define RGB_G 0x02 // pin value of the Green LED on the RGB on the shift register
#define RGB_B 0x04 // pin value of the Blue LED on the RGB on the shift register
#define RGB_SHIFT 1 // which shift register the RGB is on (starts at 0)

#define RGB_RED     100, 0,   0
#define RGB_ORANGE  90,  100, 0
#define RGB_YELLOW  60,  100, 0
#define RGB_GREEN   0,   100, 0
#define RGB_BLUE    0,   0,   100
#define RGB_PINK    90,  0,   100
#define RGB_INDIGO  20,  0,   100
#define RGB_VIOLET  60,  0,   100
#define RGB_WHITE   60,  100, 90
#define RGB_OFF     0,   0,   0

// properties about the robot in cm
#define STEPS_PER_REV 4096          // steps for wheels to revolve 360 degrees
#define WHEEL_DIAMETER_CM 5.00
#define TRACK_WIDTH_CM 8.51         //tire seperation in cm
//const float WHEEL_CIRCUMFERENCE_CM = WHEEL_DIAMETER_CM * PI;
//const float CM_PER_STEP            = WHEEL_CIRCUMFERENCE_CM / STEPS_PER_REV;
//const float CM_PER_DEGREE          = WHEEL_CIRCUMFERENCE_CM / 360.0;     // wheel movement per degree rotation of robot
const float STEPS_PER_CM;
//const float STEPS_PER_ROTATION     = (TRACK_WIDTH_CM / WHEEL_DIAMETER_CM) * STEPS_PER_REV ;  // robot rotation
//const float STEPS_PER_DEGREE       = STEPS_PER_ROTATION / 360.0;         // robot rotation
const float STEPS_PER_DEGREE;
const float STEPS_PER_ARM_CM;


#define DISTANCE_TIME_COSNTANT 222.222222
#define DEGREES_TIME_COSNTANT  21.388888

// defines for left and right motors
#define MOTOR_LEFT    0
#define MOTOR_RIGHT   1
#define MOTOR_GRIPPER 2

// defines for direction
#define DIR_CCW -1
#define DIR_CW   1

//includes for the LCD

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 0
#define WHITE 1

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x60
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

#define LCD_A0 4
#define LCD_RST 12
#define LCD_CS 17

// SPI definitions
#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR
#define SPI_CLOCK_DIV2 0x04

// Magnetometer definitions
#define Measurement_Continuous 0x00
#define DataRegisterBegin 0x03
#define HMC5883L_Address 0x1E
#define DataRegisterBegin 0x03
#define RawMagDataLength 6
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define M_SCALE 0.92

// Accelerometer definitions
#define MMA8452_ADDRESS 0x1D  // SA0 is high, 0x1C if low
#define ACCEL_SCALE 2  // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
#define ACCEL_DATARATE 0 //Set the output data rate: 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

#define SPEED_ARRAY_LENGTH 10  // uses an array to determine speed.
// increase this number (<255) to increase precision of speed control


// class SparkiClass : public Print {

// SparkiClass(); //R `begin()` instead
float sparki_systemVoltage();
int sparki_ping_single();
int sparki_ping();
void sparki_begin();
void sparki_beep();
void sparki_beep_freq(int);
void sparki_beep_freq_time(int, int);
void sparki_noBeep();
void sparki_RGB(uint8_t,uint8_t,uint8_t);

void sparki_setMux(uint8_t, uint8_t, uint8_t);

// Light level sensors
int sparki_lightRight();
int sparki_lightCenter();
int sparki_lightLeft();

// Infrared reflectance sensors
int sparki_edgeRight();
int sparki_lineRight();
int sparki_lineCenter();
int sparki_lineLeft();
int sparki_edgeLeft();

int sparki_readSensorIR(int);
int sparki_readBlindSensorIR(int,int,int);
int sparki_diffIR(int,int,int);


// Infrared Remote sensor
int sparki_readIR();

// Infrared Remote sensor
void sparki_sendIR(uint8_t);
void sparki_irPulse(uint16_t, uint16_t);

// Wire Functions
void sparki_WireWrite(int, int);
uint8_t* sparki_WireRead(int, int);

// Compass
float sparki_compass();

// Magnetometer
float sparki_readMag();
float sparki_xAxisMag;
float sparki_zAxisMag;
float sparki_yAxisMag;
float sparki_magX();
float sparki_magY();
float sparki_magZ();

// Accelerometer
// int accelGood; //R unused??
float sparki_xAxisAccel;
float sparki_zAxisAccel;
float sparki_yAxisAccel;
float sparki_accelX();
float sparki_accelY();
float sparki_accelZ();

void sparki_readAccelData();
int sparki_initAccelerometer();

// I2C functions //R Used in Accel
void    sparki_readi2cRegisters(uint8_t, int, uint8_t *, uint8_t);
uint8_t sparki_readi2cRegister_read(uint8_t, uint8_t);
void    sparki_readi2cRegister_write(unsigned char, unsigned char, uint8_t);

// Servo Functions
void sparki_startServoTimer();
void sparki_servo(int);
int8_t sparki_servo_deg_offset;

// high-level move functions
void sparki_moveForward_cm(float);
void sparki_stepForward(unsigned long);
void sparki_moveForward();

void sparki_moveBackward_cm(float);
void sparki_stepBackward(unsigned long);
void sparki_moveBackward();

void sparki_moveLeft_deg(float);
void sparki_stepLeft(unsigned long);
void sparki_moveLeft();

void sparki_moveRight_deg(float);
void sparki_stepRight(unsigned long);
void sparki_moveRight();

void sparki_gripperOpen();
void sparki_gripperOpen_cm(float);
void sparki_gripperClose();
void sparki_gripperClose_cm(float);
void sparki_gripperStop();

void sparki_moveStop();
void sparki_speed(uint8_t);

// individual motor control (non-blocking)
// speed range is percent 0-100
void sparki_motorRotate( int motor, int direction,  int speed);
void sparki_motorRotate_steps( int motor, int direction,  int speed, long steps);
void sparki_motorStop(int motor);

// returns true if one or both motors a still stepping
bool sparki_areMotorsRunning();

void sparki_onIR();
void sparki_offIR();

// Display Functions
void sparki_progmem_lcd_logo(void);
void sparki_ensure_lcd_init(void);
void sparki_beginDisplay(void);
void sparki_st7565_init(void);
void sparki_st7565_command(uint8_t c);
void sparki_st7565_data(uint8_t c);
void sparki_st7565_set_brightness(uint8_t val);
void sparki_clear_display(void);

void sparki_clearLCD();
void sparki_updateLCD();
void sparki_setPixelColor(uint8_t);

void sparki_moveUpLine();

uint8_t sparki_readPixel(uint8_t x, uint8_t y);

void sparki_textWrite(const char* buffer, uint16_t len);

// Helper utils, since C can't use the Print class' println(), etc.
void sparki_textWrite_sz(const char* buffer);
void sparki_textWrite_i(int i);
void sparki_textWriteln();

/* Play nice with Arduino's Print class */ //R Which we don't need
// virtual size_t write(uint8_t b) {
//     textWrite((const char *)&b, 1);
//     return 1;
// }
// virtual size_t write(const uint8_t *buffer, size_t size) {
//     textWrite((const char *)buffer, size);
//     return size;
// }

void sparki_drawPixel(uint8_t x, uint8_t y);
void sparki_drawCircleFilled(uint8_t x0, uint8_t y0, uint8_t r);
void sparki_drawCircle(uint8_t x0, uint8_t y0, uint8_t r);
void sparki_drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void sparki_drawRectFilled(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void sparki_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void sparki_drawChar(uint8_t x, uint8_t line, char c);
void sparki_drawString(uint8_t x, uint8_t line, char *c);
void sparki_drawString_P(uint8_t x, uint8_t line, const char *c);
void sparki_drawBitmap(uint8_t x, uint8_t y,
        const uint8_t *bitmap, uint8_t w, uint8_t h);

void sparki_updateBoundingBox(uint8_t , uint8_t , uint8_t , uint8_t );
//    private:
//static void scheduler();

// Display Functions
// int8_t sid, sclk, a0, rst, cs; //R These aren't used?
void sparki_startSPI();
void sparki_spiwrite(uint8_t c);
void sparki_my_setpixel(uint8_t x, uint8_t y, uint8_t color);
// };

// extern SparkiClass sparki;

#endif

