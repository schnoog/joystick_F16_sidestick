#include "init.h"
#include "anaload.h"








///---setup
void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Setup");
  LED_setup();

    InitMCP();
    Joystick.setXAxisRange( 1023, 0);
    Joystick.setYAxisRange(0, 1023);

    GetCenter();
    delay(50); 
}


///----setup










void loop() {
GetButtons();
/////    GetAxis();
/////    Joystick.sendState();



ReadLoadCell();
LED_loop();

int VALUE_Z = analogRead(A2);
//1Serial.println( (String)CorrectedAxis1 + "      " + (String)CorrectedAxis2 + "     "  + (String)VALUE_X + "/"  + (String)VALUE_Y  + "      Voltage: " + (String)VALUE_Z  );
//Serial.println("Voltage: " + (String) AVERAGED_Z + " Axis 1 (raw/corrected)" + (String)AVERAGED_X + "/" + (String)CorrectedAxis1 + " Axis 2: " + (String) AVERAGED_Y + "/" + (String) CorrectedAxis2        );
        Joystick.setXAxis(CorrectedAxis1);
        Joystick.setYAxis(CorrectedAxis2); 
Joystick.sendState();
}
