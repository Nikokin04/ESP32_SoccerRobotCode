#include <Ps3Controller.h>

// Mot1
int Mot1Pin1 = 4;
int Mot1Pin2 = 2;
// Mot2
int Mot2Pin1 = 14;
int Mot2Pin2 = 12;

const int PWMFreq = 50;
const int PWMResolution = 8;
const int PWM_IO2 = 0;
const int PWM_IO4 = 1;
const int PWM_IO12 = 2;
const int PWM_IO14 = 3;

int joystickL = 0;
int joystickR = 0;
int R2 = 0;
int L2 = 0;
int V = 255;
int Inertia = 25;  //Ruptura de inercia;

void notify() {

  joystickL = Ps3.data.analog.stick.ly;
  joystickR = Ps3.data.analog.stick.rx;
  R2 = Ps3.data.analog.button.r2;
  L2 = Ps3.data.analog.button.l2;

  if (Ps3.data.analog.stick.ly >= Inertia) { //Movimiento delantero
    joystickL = map(Ps3.data.analog.stick.ly, 0, 128, 0, 100);
    ledcWrite(PWM_IO14, joystickL);
    ledcWrite(PWM_IO12, 0);
    ledcWrite(PWM_IO4, 0);
    ledcWrite(PWM_IO2, joystickL);
  } else {
    if (Ps3.data.analog.stick.ly <= -Inertia) {
      joystickL = map(Ps3.data.analog.stick.ly, 0, 128, 0, 100);
      ledcWrite(PWM_IO14, 0);
      ledcWrite(PWM_IO12, -joystickL);
      ledcWrite(PWM_IO4, -joystickL);
      ledcWrite(PWM_IO2, 0);
    } else {
      if (Ps3.data.analog.stick.rx >= Inertia) {
        joystickR = map(Ps3.data.analog.stick.rx, 0, 128, 0, 70);
        ledcWrite(PWM_IO14, 0);
        ledcWrite(PWM_IO12, joystickR/2);
        ledcWrite(PWM_IO4, joystickR);
        ledcWrite(PWM_IO2, 0);
        Serial.println("3");
      } else {
        if (Ps3.data.analog.stick.rx <= -Inertia) {
          joystickR = map(Ps3.data.analog.stick.rx, 0, 128, 0, 70);
          ledcWrite(PWM_IO14, 0);
          ledcWrite(PWM_IO12, -joystickR);
          ledcWrite(PWM_IO4, -joystickR/2);
          ledcWrite(PWM_IO2, 0);
        } else {
          if (Ps3.data.analog.button.r2 >= 128) {
            R2 = 255;
            ledcWrite(PWM_IO14, 0);
            ledcWrite(PWM_IO12, R2);
            ledcWrite(PWM_IO4, R2);
            ledcWrite(PWM_IO2, 0);
            Serial.println("5");
          } else {
            if (Ps3.data.analog.button.l2 >= 128) {
              L2 = 255;
              ledcWrite(PWM_IO14, L2);
              ledcWrite(PWM_IO12, 0);
              ledcWrite(PWM_IO4, 0);
              ledcWrite(PWM_IO2, L2);
              Serial.println("6");
            } else {
              ledcWrite(PWM_IO14, 0);
              ledcWrite(PWM_IO12, 0);
              ledcWrite(PWM_IO4, 0);
              ledcWrite(PWM_IO2, 0);
              Serial.println("7");
            }
          }
        }
      }
    }
  }
}

void onConnect() {
  Serial.println("Connected!.");
}

void onDisConnect() {
  Serial.println("Disconnected!.");
}

void setUpPinModes() {
  ledcAttachPin(Mot1Pin1, PWM_IO4);
  ledcAttachPin(Mot1Pin2, PWM_IO2);
  ledcAttachPin(Mot2Pin1, PWM_IO14);
  ledcAttachPin(Mot2Pin2, PWM_IO12);
  ledcSetup(PWM_IO14, PWMFreq, PWMResolution);
  ledcSetup(PWM_IO12, PWMFreq, PWMResolution);
  ledcSetup(PWM_IO4, PWMFreq, PWMResolution);
  ledcSetup(PWM_IO2, PWMFreq, PWMResolution);
}

void setup() {
  setUpPinModes();
  Serial.begin(115200);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisConnect);
  Ps3.begin();
  Serial.println("Ready.");
}

void loop() {
  Ps3.attach(notify);
}
