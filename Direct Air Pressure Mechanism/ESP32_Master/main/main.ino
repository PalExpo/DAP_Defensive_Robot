/*
   Manual Code by Anuraag Pal
*/

#include "main.h"
#include "Define.h"

uint8_t pwm = 80;
uint8_t pwm_base1 = 80;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
uint8_t pwm_base2 = 120;
uint8_t pwm_max_aclk_clk = 70;
bool prev_pwm = false;
uint8_t prev_pwm_counter = 0;
uint8_t battery;
byte mapped_spd = 0;
bool pwm_decrease = false;
bool pwm_increase = false;
bool ljup_state = false;
bool ljdown_state = false;
bool rjclose_state = false;
bool rjopen_state = false;
bool triangle_state = false;
bool circle_state = false;
bool cross_state  = false;
bool square_state  = false;
bool select_state = false;
bool start_state = false;
bool r3_state = false;
bool l1_state = false;
bool r1_state = false;

bool ps_state = false;
bool can_control = true;
int ps_counter = 0;

long buttonTimer = 0;
long longPressTime = 1000;
long longPressTime2 = 750;
boolean buttonActive = false;
boolean longPressActive = false;
boolean buttonActive2 = false;
boolean longPressActive2 = false;

int pressed = 0;
int leave = 0;
bool print_counter = false;

int uart_S = 0;
int counter_while = 1;

bool swift_counter = true;
bool short_press_counter = true;

void setup() {

  //  Ps3.begin("11:11:11:11:11:11");
  //  Ps3.begin("22:22:22:22:22:22");
  Ps3.begin("33:33:33:33:33:33");
  //  Ps3.begin("44:44:44:44:44:44");
  SPI.begin();
  pinMode(5, OUTPUT);
  vspi = new SPIClass(VSPI);
  vspi->begin();
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(115200, SERIAL_8N1, 15, 27);
  Serial1.begin(115200, SERIAL_8N1, 13, 14);
  Serial2.begin(115200);
  delay(500);
  cytron_1.send_dummy_bit();
  delay(500);
  millis();
  bot.callibrate();
  pinMode(LIMIT, INPUT);
  pinMode(ESP_LED, OUTPUT);
  tfmini.begin(&Serial);
  ps_battery();
}

void loop() {
  mpu6050.update();
  //  print_sensor_data();
  //  Serial.println(Serial2.read());
  if (Ps3.isConnected()) {
    digitalWrite(ESP_LED, HIGH);
    if (can_control)
    {
      if (!swift_counter)
      {
        if (Ps3.data.button.l1 && Ps3.data.button.r1 && !prev_pwm)
        {
          prev_pwm = true;
          prev_pwm_counter++;
          if (prev_pwm_counter % 2 == 0)
          {
            pwm = pwm_base1;
            send_data(COLOR_SLOW);
            Ps3.setRumble(100.0);
            delay(100);
            Ps3.setRumble(0);
          }
          else if (prev_pwm_counter % 2 == 1)
          {
            pwm = pwm_base2;
            send_data(COLOR_FAST);
          }
        }
        if (Ps3.data.button.l1 && Ps3.data.button.r1)
        {

        }
        else if (Ps3.data.button.l2)
        {
          mapped_spd = map(Ps3.data.analog.button.l2, 0, 255, 0, pwm_max_aclk_clk);
          bot.base_anti_clockwise(mapped_spd, mapped_spd, mapped_spd, mapped_spd);
        }
        else if (Ps3.data.button.r2)
        {
          mapped_spd = map(Ps3.data.analog.button.r2, 0, 255, 0, pwm_max_aclk_clk);
          bot.base_clockwise(mapped_spd, mapped_spd, mapped_spd, mapped_spd);
        }
        else if (Ps3.data.button.up && Ps3.data.button.right)
        {
          bot.up_right(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.up && Ps3.data.button.left)
        {
          bot.up_left(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down && Ps3.data.button.right)
        {
          bot.down_right(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down && Ps3.data.button.left)
        {
          bot.down_left(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.up)
        {
          bot.forward(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down)
        {
          bot.backward(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.left)
        {
          bot.left(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.right)
        {
          bot.right(pwm, pwm, pwm, pwm);
        }
        else if ((Ps3.data.analog.stick.lx == -128) &&
                 (Ps3.data.analog.stick.ly > -127 && Ps3.data.analog.stick.ly <= 127))
        {
          bot.down_left(pwm, pwm, pwm, pwm);
        }
        else
        {
          prev_pwm = false;
          bot.base_brake();
          //          send_data(0);
        }
        if (Ps3.data.button.l1 && !l1_state)
        {
          send_data(DES_PRESSURE_1to1);
          pressed = millis();
          l1_state = true;
        }
        else if (Ps3.data.button.l1)
        {
          leave = millis();
          if ((leave - pressed) > 750)
          {
            send_data(DES_PRESSURE);
          }
        }
        else if (Ps3.data.button.r1 && !r1_state)
        {
          send_data(INC_PRESSURE_1to1);
          pressed = millis();
          r1_state = true;
        }
        else if (Ps3.data.button.r1)
        {
          leave = millis();
          if ((leave - pressed) > 750)
          {
            send_data(INC_PRESSURE);
          }
        }
        else if (Ps3.data.button.triangle && !triangle_state)
        {
          send_data(SHOOT1);
          triangle_state = true;
        }
        else if (Ps3.data.button.triangle)
        {

        }
        else if (Ps3.data.button.circle && !circle_state)
        {
          send_data(SHOOT2);
          circle_state = true;
        }
        else if (Ps3.data.button.circle)
        {

        }
        else if (Ps3.data.button.cross && !cross_state)
        {
          send_data(SHOOT3);
          cross_state  = true;
        }
        else if (Ps3.data.button.cross)
        {

        }
        else if (Ps3.data.button.square && !square_state)
        {
          send_data(SHOOT4);
          square_state  = true;
        }
        else if (Ps3.data.button.square)
        {

        }
        else if (Ps3.data.button.start && !start_state)
        {
          send_data(SHOOT5);
          start_state = true;
        }
        else if (Ps3.data.button.start)
        {

        }
        else if (Ps3.data.button.select && !select_state)
        {
          if (buttonActive2 == false) {
            buttonActive2 = true;
            buttonTimer = millis();
          }
          if ((millis() - buttonTimer > longPressTime2) && (longPressActive2 == false)) {
            longPressActive2 = true;
            send_data(PRINT_DATA);
            digitalWrite(ESP_LED, LOW);
            delay(100);
          }
        }
        //      else if (Ps3.data.button.r3 && !r3_state)
        //      {
        //        send_data(ROBOLOCK);
        //        r3_state = true;
        //      }
        //      else if (Ps3.data.button.r3)
        //      {
        //        bot.base_free();
        //      }
        else if ((Ps3.data.analog.stick.lx >= -128  && Ps3.data.analog.stick.lx < 127) &&
                 (Ps3.data.analog.stick.ly == -128) && !ljup_state)
        {
          send_data(LOAD_ARROW);
          ljup_state = true;
        }
        else if ((Ps3.data.analog.stick.lx >= -128  && Ps3.data.analog.stick.lx < 127) &&
                 (Ps3.data.analog.stick.ly == -128))
        {

        }
        else if ((Ps3.data.analog.stick.lx > -128  && Ps3.data.analog.stick.lx <= 127) &&
                 (Ps3.data.analog.stick.ly == 127) && !ljdown_state)
        {
          bot.callibrate();
          ljdown_state = true;
        }
        else if ((Ps3.data.analog.stick.lx > -128  && Ps3.data.analog.stick.lx <= 127) &&
                 (Ps3.data.analog.stick.ly == 127))
        {

        }
        else if ((Ps3.data.analog.stick.lx == 127) &&
                 (Ps3.data.analog.stick.ly <= 127 && Ps3.data.analog.stick.ly > -128))
        {
          send_data(SERVO_ACLK);
        }
        else if ((Ps3.data.analog.stick.rx >= -128  && Ps3.data.analog.stick.rx < 127) &&
                 (Ps3.data.analog.stick.ry == -128))
        {
          send_data(AL_CLOCKWISE);
        }
        else if ((Ps3.data.analog.stick.rx > -128  && Ps3.data.analog.stick.rx <= 127) &&
                 (Ps3.data.analog.stick.ry == 127))
        {
          send_data(AL_ANTICLOCKWISE);
        }
        else if ((Ps3.data.analog.stick.rx == -128) &&
                 (Ps3.data.analog.stick.ry > -127 && Ps3.data.analog.stick.ry <= 127) && !rjopen_state)
        {
          send_data(PNEU_OPEN);
          rjopen_state = true;
        }
        else if ((Ps3.data.analog.stick.rx == -128) &&
                 (Ps3.data.analog.stick.ry > -127 && Ps3.data.analog.stick.ry <= 127))
        {

        }
        else if ((Ps3.data.analog.stick.rx == 127) &&
                 (Ps3.data.analog.stick.ry <= 127 && Ps3.data.analog.stick.ry > -128) && !rjclose_state)
        {
          send_data(PNEU_CLOSE);
          rjclose_state = true;
        }
        else if ((Ps3.data.analog.stick.rx == 127) &&
                 (Ps3.data.analog.stick.ry <= 127 && Ps3.data.analog.stick.ry > -128))
        {

        }
        else if (Ps3.data.button.ps && !ps_state)
        {
          if (buttonActive == false) {
            buttonActive = true;
            buttonTimer = millis();
          } if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
            longPressActive = true;
            can_control = false;
            ps_state = true;
            send_data(PAUSE);
            digitalWrite(ESP_LED, LOW);
            delay(100);
            Ps3.setRumble(100.0);
            delay(150);
            Ps3.setRumble(0);
            delay(150);
            Ps3.setRumble(100.0);
            delay(150);
            Ps3.setRumble(0);
          }
        }
        else if (Ps3.data.button.ps)
        {
          bot.base_free();
        }
        else
        {
          prev_pwm = false;
          pwm_increase = false;
          pwm_decrease = false;
          ljup_state = false;
          ljdown_state = false;
          rjclose_state = false;
          rjopen_state = false;
          triangle_state = false;
          circle_state = false;
          cross_state  = false;
          square_state = false;
          ps_state = false;
          select_state = false;
          start_state = false;
          r3_state = false;
          print_counter = false;
          l1_state = false;
          r1_state = false;
          //-----
          if (buttonActive == true) {
            if (longPressActive == true) {
              longPressActive = false;
            } else {
              if (!short_press_counter)
              {
                swift_counter = true;
                short_press_counter = true;
                Ps3.setRumble(100.0);
                delay(150);
                Ps3.setRumble(0);
              }
              else if (short_press_counter)
              {
                swift_counter = false;
                short_press_counter = false;
                Ps3.setRumble(100.0);
                delay(150);
                Ps3.setRumble(0);
              }
            } buttonActive = false;
          }
          //-----
          if (buttonActive2 == true) {
            if (longPressActive2 == true) {
              longPressActive2 = false;
            } else {
              send_data(JUST_TO_SEE);
              delay(100);
            } buttonActive2 = false;
          }
          //-----
          send_data(0);
        }
      }
      else if (swift_counter)
      {
        if (Ps3.data.button.l1 && Ps3.data.button.r1 && !prev_pwm)
        {
          prev_pwm = true;
          prev_pwm_counter++;
          if (prev_pwm_counter % 2 == 0)
          {
            pwm = pwm_base1;
            send_data(COLOR_SLOW);
            Ps3.setRumble(100.0);
            delay(100);
            Ps3.setRumble(0);
          }
          else if (prev_pwm_counter % 2 == 1)
          {
            pwm = pwm_base2;
            send_data(COLOR_FAST);
          }
        }
        if (Ps3.data.button.l1 && Ps3.data.button.r1)
        {

        }
        else if (Ps3.data.button.l2)
        {
          mapped_spd = map(Ps3.data.analog.button.l2, 0, 255, 0, pwm_max_aclk_clk);
          bot.base_anti_clockwise(mapped_spd, mapped_spd, mapped_spd, mapped_spd);
        }
        else if (Ps3.data.button.r2)
        {
          mapped_spd = map(Ps3.data.analog.button.r2, 0, 255, 0, pwm_max_aclk_clk);
          bot.base_clockwise(mapped_spd, mapped_spd, mapped_spd, mapped_spd);
        }
        else if (Ps3.data.button.up && Ps3.data.button.right)
        {
          bot.down_left(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.up && Ps3.data.button.left)
        {
          bot.down_right(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down && Ps3.data.button.right)
        {
          bot.up_left(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down && Ps3.data.button.left)
        {
          bot.up_right(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.up)
        {
          bot.backward(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.down)
        {
          bot.forward(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.left)
        {
          bot.right(pwm, pwm, pwm, pwm);
        }
        else if (Ps3.data.button.right)
        {
          bot.left(pwm, pwm, pwm, pwm);
        }
        else
        {
          prev_pwm = false;
          bot.base_brake();
          //          send_data(0);
        }
        if (Ps3.data.button.l1 && !l1_state)
        {
          send_data(DES_PRESSURE_1to1);
          pressed = millis();
          l1_state = true;
        }
        else if (Ps3.data.button.l1)
        {
          leave = millis();
          if ((leave - pressed) > 750)
          {
            send_data(DES_PRESSURE);
          }
        }
        else if (Ps3.data.button.r1 && !r1_state)
        {
          send_data(INC_PRESSURE_1to1);
          pressed = millis();
          r1_state = true;
        }
        else if (Ps3.data.button.r1)
        {
          leave = millis();
          if ((leave - pressed) > 750)
          {
            send_data(INC_PRESSURE);
          }
        }
        else if (Ps3.data.button.triangle && !triangle_state)
        {
          send_data(SHOOT1);
          triangle_state = true;
        }
        else if (Ps3.data.button.triangle)
        {

        }
        else if (Ps3.data.button.circle && !circle_state)
        {
          send_data(SHOOT2);
          circle_state = true;
        }
        else if (Ps3.data.button.circle)
        {

        }
        else if (Ps3.data.button.cross && !cross_state)
        {
          send_data(SHOOT3);
          cross_state  = true;
        }
        else if (Ps3.data.button.cross)
        {

        }
        else if (Ps3.data.button.square && !square_state)
        {
          send_data(SHOOT4);
          square_state  = true;
        }
        else if (Ps3.data.button.square)
        {

        }
        else if (Ps3.data.button.start && !start_state)
        {
          send_data(SHOOT5);
          start_state = true;
        }
        else if (Ps3.data.button.start)
        {

        }
        else if (Ps3.data.button.select && !select_state)
        {
          if (buttonActive2 == false) {
            buttonActive2 = true;
            buttonTimer = millis();
          }
          if ((millis() - buttonTimer > longPressTime2) && (longPressActive2 == false)) {
            longPressActive2 = true;
            send_data(PRINT_DATA);
            digitalWrite(ESP_LED, LOW);
            delay(100);
          }
        }
        //      else if (Ps3.data.button.r3 && !r3_state)
        //      {
        //        send_data(ROBOLOCK);
        //        r3_state = true;
        //      }
        //      else if (Ps3.data.button.r3)
        //      {
        //        bot.base_free();
        //      }
        else if ((Ps3.data.analog.stick.lx >= -128  && Ps3.data.analog.stick.lx < 127) &&
                 (Ps3.data.analog.stick.ly == -128) && !ljup_state)
        {
          send_data(LOAD_ARROW);
          ljup_state = true;
        }
        else if ((Ps3.data.analog.stick.lx >= -128  && Ps3.data.analog.stick.lx < 127) &&
                 (Ps3.data.analog.stick.ly == -128))
        {

        }
        else if ((Ps3.data.analog.stick.lx > -128  && Ps3.data.analog.stick.lx <= 127) &&
                 (Ps3.data.analog.stick.ly == 127) && !ljdown_state)
        {
          bot.callibrate();
          ljdown_state = true;
        }
        else if ((Ps3.data.analog.stick.lx > -128  && Ps3.data.analog.stick.lx <= 127) &&
                 (Ps3.data.analog.stick.ly == 127))
        {

        }
        else if ((Ps3.data.analog.stick.lx == -128) &&
                 (Ps3.data.analog.stick.ly > -127 && Ps3.data.analog.stick.ly <= 127))
        {
          send_data(SERVO_CLK);
        }
        else if ((Ps3.data.analog.stick.lx == 127) &&
                 (Ps3.data.analog.stick.ly <= 127 && Ps3.data.analog.stick.ly > -128))
        {
          send_data(SERVO_ACLK);
        }
        else if ((Ps3.data.analog.stick.rx >= -128  && Ps3.data.analog.stick.rx < 127) &&
                 (Ps3.data.analog.stick.ry == -128))
        {
          send_data(AL_CLOCKWISE);
        }
        else if ((Ps3.data.analog.stick.rx > -128  && Ps3.data.analog.stick.rx <= 127) &&
                 (Ps3.data.analog.stick.ry == 127))
        {
          send_data(AL_ANTICLOCKWISE);
        }
        else if ((Ps3.data.analog.stick.rx == -128) &&
                 (Ps3.data.analog.stick.ry > -127 && Ps3.data.analog.stick.ry <= 127) && !rjopen_state)
        {
          send_data(PNEU_OPEN);
          rjopen_state = true;
        }
        else if ((Ps3.data.analog.stick.rx == -128) &&
                 (Ps3.data.analog.stick.ry > -127 && Ps3.data.analog.stick.ry <= 127))
        {

        }
        else if ((Ps3.data.analog.stick.rx == 127) &&
                 (Ps3.data.analog.stick.ry <= 127 && Ps3.data.analog.stick.ry > -128) && !rjclose_state)
        {
          send_data(PNEU_CLOSE);
          rjclose_state = true;
        }
        else if ((Ps3.data.analog.stick.rx == 127) &&
                 (Ps3.data.analog.stick.ry <= 127 && Ps3.data.analog.stick.ry > -128))
        {

        }
        else if (Ps3.data.button.ps && !ps_state)
        {
          if (buttonActive == false) {
            buttonActive = true;
            buttonTimer = millis();
          } if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
            longPressActive = true;
            can_control = false;
            ps_state = true;
            send_data(PAUSE);
            digitalWrite(ESP_LED, LOW);
            delay(100);
            Ps3.setRumble(100.0);
            delay(150);
            Ps3.setRumble(0);
            delay(150);
            Ps3.setRumble(100.0);
            delay(150);
            Ps3.setRumble(0);
          }
        }
        else if (Ps3.data.button.ps)
        {
          bot.base_free();
        }
        else
        {
          prev_pwm = false;
          pwm_increase = false;
          pwm_decrease = false;
          ljup_state = false;
          ljdown_state = false;
          rjclose_state = false;
          rjopen_state = false;
          triangle_state = false;
          circle_state = false;
          cross_state  = false;
          square_state = false;
          ps_state = false;
          select_state = false;
          start_state = false;
          r3_state = false;
          print_counter = false;
          l1_state = false;
          r1_state = false;
          //-----
          if (buttonActive == true) {
            if (longPressActive == true) {
              longPressActive = false;
            } else {
              if (!short_press_counter)
              {
                swift_counter = true;
                short_press_counter = true;
                Ps3.setRumble(100.0);
                delay(150);
                Ps3.setRumble(0);
              }
              else if (short_press_counter)
              {
                swift_counter = false;
                short_press_counter = false;
                Ps3.setRumble(100.0);
                delay(150);
                Ps3.setRumble(0);
              }
            } buttonActive = false;
          }
          //-----
          if (buttonActive2 == true) {
            if (longPressActive2 == true) {
              longPressActive2 = false;
            } else {
              send_data(JUST_TO_SEE);
              delay(100);
            } buttonActive2 = false;
          }
          //-----
          send_data(0);
        }
      }
    }
    else
    {
      if (Ps3.data.button.ps && !ps_state)
      {
        if (buttonActive == false) {
          buttonActive = true;
          buttonTimer = millis();
        }
        if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
          longPressActive = true;
          can_control = true;
          ps_state = true;
          send_data(RUN);
          digitalWrite(ESP_LED, LOW);
          delay(100);
          Ps3.setRumble(100.0);
          delay(150);
          Ps3.setRumble(0);
          delay(150);
          Ps3.setRumble(100.0);
          delay(150);
          Ps3.setRumble(0);
        }
      }
      else if (Ps3.data.button.ps)
      {

      }
      else
      {
        ps_state = false;
        if (buttonActive == true) {
          if (longPressActive == true) {
            longPressActive = false;
          }
          else {
            digitalWrite(ESP_LED, LOW);
            delay(100);
          }
          buttonActive = false;
        }
      }
      bot.base_free();
    }
  }
  else {
    bot.base_free();
    digitalWrite(ESP_LED, LOW);
    send_data(0);
  }
}

void ps_battery()
{
  if (battery != Ps3.data.status.battery)
  {
    battery = Ps3.data.status.battery;
    if (battery == ps3_status_battery_charging)
      Serial.println("charging");
    else if (battery == ps3_status_battery_full)
      Ps3.setPlayer(10);
    else if (battery == ps3_status_battery_high)
      Ps3.setPlayer(9);
    else if (battery == ps3_status_battery_low)
      Ps3.setPlayer(7);
    else if (battery == ps3_status_battery_dying)
      Ps3.setPlayer(4);
    else if (battery == ps3_status_battery_shutdown)
      Ps3.setPlayer(0);
  }
}

void print_sensor_data()
{
  Serial.print("TFmini: ");            //TFmini's Reading:
  Serial.print(tfmini.get_distance());
  Serial.print("  ||  Encoder: ");     //Encoder's Reading:
  Serial.print(encoder_1.read());
  Serial.print("  ");
  Serial.print(encoder_2.read());
  Serial.print("  ");
  Serial.print(encoder_3.read());
  Serial.print("  ");
  Serial.print(encoder_4.read());
  Serial.print("  ||  AngleZ : ");    //MPU6050's Reading:
  Serial.println(mpu6050.getAngleZ());
}

//========================== send_data(int x) ===========================
void send_data(int x)
{
  vspi -> beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  vspi -> transfer(x);
  digitalWrite(5, HIGH); //pull ss high to signify end of Data transfer
  vspi->endTransaction();
}
