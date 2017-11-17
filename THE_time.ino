// Illustrates how to use hardware serial ports with Thunderbolt
//#define IEEE754_4BYTE_PRECISION	// comment out if there is double precision support on the device 
#include "Thunderbolt.h"

// ---------------------------
// Devices 
// ---------------------------
Thunderbolt tbolt(Serial); // Use HardwareSerial 
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


  
// ---------------------------
// Globals
// ---------------------------
GPSStatus prevStatus;
GPSTime prevTime;

// Display version info
void display_version(GPSVersion ver) {
	char buf[50];
	sprintf(buf, "%d.%02d %d.%02d", ver.app.major_ver, ver.app.minor_ver, ver.core.major_ver, ver.core.minor_ver);
lcd.clear();	
lcd.print(buf);
delay(2000);
}

// Display status
void display_status(GPSStatus s) {
	lcd.clear();
  //lcd.print("Status: ");
	lcd.print(s.rcvr_status);
	//lcd.print(" Mode = ");
	lcd.print(s.rcvr_mode);
}

// Display UTC time
void displayTime(GPSTime t) {
	char buf[20];
	sprintf(buf, "UTC: %02d:%02d:%02d", t.Hour, t.Minute, t.Second);
	lcd.clear();
        lcd.print(buf);
        lcd.setCursor(0, 1) ;
        //sprintf(buf, "%02d-%02d-%02d--%02d", t.Day, t.Month, t.Year, t.week_no);
        setTime(t.Hour,t.Minute,t.Second,t.Day,t.Month,t.Year);
        
        long int nowww = now();
        long int newnow = nowww + 619315200;
        setTime(newnow);
        int d = day();
        int m = month();
        int y = year();
        
        sprintf(buf, "%02d-%02d-%02d--%02d", d, m, y, t.week_no);
        lcd.print(buf);
}

void setup() {
  
	Serial.begin(TSIP_BAUD_RATE);
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(1000);
	tbolt.flush();
	if (tbolt.getSoftwareVersionInfo() == false) // this call is synchronous (waits for a response - but will timeout)
	{
	    lcd.clear();	
            lcd.print("comms fail");
            lcd.setCursor(0, 1) ;
            lcd.print("Hang on...");
		delay (1000);
                 setup();
                
	}
	display_version(tbolt.getVersion());
	tbolt.setPacketBroadcastMask(PBM_TIMING_ONLY);
	tbolt.setFixMode(RPT_FIX_POS_LLA_64, RPT_FIX_VEL_ENU, ALT_MSL, PPS_FIX, TME_UTCTIME);
}

void loop()
{
        GPSTime time = tbolt.getGPSTime();
	if (prevTime != time) {
		displayTime(time);
		prevTime = time;
	
	}

	
}

// Use handy built-in serial event (hardware serial only), NOT interrupt driven, called once each loop when serial data is available
void serialEvent() {
	tbolt.readSerial(); // Thunderbolt class serial in
}
