#include <BluetoothSerial.h>
#include <Button2.h>
#include <ELMduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

//#include "FreeFonts.h"
//#include "OpelLogo.h"

#define ESP_BLUETOOTH_NAME "HornyBitch"

#define BUTTON_1_PIN 0
#define BUTTON_2_PIN 35

//#define SKIP_BT // <-- uncomment for demo mode

BluetoothSerial _bluetoothSerial;
ELM327 _elm327;
TFT_eSPI _tft = TFT_eSPI(135, 240);
uint8_t _rgu8MACAddress[6] = { 0x86, 0xDC, 0x52, 0xB5, 0xF6, 0xF1 };

enum tRequest
{
  RequestDemo,
  RequestDPFSoot,
  RequestDPFKilometer,
  RequestDPFBurn,
  RequestOilTemp,
  RequestOilPressure,
} _enRequest = RequestDPFSoot;

enum tPage
{
  Page1,
  Page2
} _enPage = Page1;

bool _bSwitchMode = false;
bool _bSwitchPage = false;

Button2 _button1(BUTTON_1_PIN);
Button2 _button2(BUTTON_2_PIN);

void setup()
{
  Serial.begin(115200);
  Serial.println("Attempting to connect to ELM327 ...");

  _bluetoothSerial.begin(ESP_BLUETOOTH_NAME, true);
  _bluetoothSerial.setPin("1234");

  _tft.init();
  _tft.setRotation(3);
  _tft.fillScreen(TFT_BLACK);
  _tft.setTextSize(2);
  //  _tft.setFreeFont(FF17);

  //  _tft.setSwapBytes(true);
  //  _tft.pushImage(0, 0, 240, 135, logo);
  //  delay(1000);

#ifdef SKIP_BT

  _enRequest = RequestDemo;

#else

  _tft.setTextColor(TFT_WHITE, TFT_BLACK);
  _tft.drawString("OBD 1", 10, 10);

  //  if (!_bluetoothSerial.connect("OBDII"))
  if (!_bluetoothSerial.connect(_rgu8MACAddress))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 1");

    _tft.fillScreen(TFT_BLACK);
    _tft.setTextColor(TFT_RED, TFT_BLACK);
    _tft.drawString("OBD 1 error", 10, 10);

    while (1);
  }

  _tft.fillScreen(TFT_BLACK);
  _tft.setTextColor(TFT_WHITE, TFT_BLACK);
  _tft.drawString("OBD 2", 10, 10);

  if (!_elm327.begin(_bluetoothSerial, false, 2000))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");

    _tft.fillScreen(TFT_BLACK);
    _tft.setTextColor(TFT_RED, TFT_BLACK);
    _tft.drawString("OBD 2 error", 10, 10);

    while (1);
  }

#endif

  Serial.println("Connected to ELM327");

  _tft.fillScreen(TFT_BLACK);
  _tft.setTextColor(TFT_GREEN, TFT_BLACK);
  _tft.drawString("Connected", 10, 10);
  delay(1000);
  _tft.fillScreen(TFT_BLACK);

  button_init();
}

void button_init()
{
  _button1.setTapHandler([](Button2 & b)
  {
    _bSwitchMode = true;
  });

  //  _button1.setLongClickHandler([](Button2 & b)
  //  {
  //    unsigned int time = b.wasPressedFor();
  //
  //    if (time >= 1000)
  //    {
  //      nav.doNav(enterCmd);
  //    }
  //  });

  _button2.setTapHandler([](Button2 & b)
  {
    _bSwitchPage = true;
  });
}

void button_loop()
{
  _button1.loop();
  _button2.loop();
}

void loop()
{
  static bool bForceDisplayUpdate = true;

  static int32_t i32Temp;

  static int32_t i32Soot;
  static int32_t i32SootOld;
  static int32_t i32Kilometer;
  static int32_t i32KilometerOld;
  static int32_t i32Burn;
  static int32_t i32BurnOld;
  static int32_t i32OilTemp;
  static int32_t i32OilTempOld;
  static float fOilPressure;
  static float fOilPressureOld;

  button_loop();

  if (bForceDisplayUpdate)
  {
    bForceDisplayUpdate = false;

    UpdateDisplay(i32Soot, i32Kilometer, i32Burn, i32OilTemp, fOilPressure);
  }

  if (_bSwitchMode)
  {
    _bSwitchMode = false;

    if (_enRequest == RequestDemo)
    {
      _tft.fillScreen(TFT_BLACK);

      _tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      _tft.drawString("Live mode", 10, 10);

      delay(1000);

      _tft.fillScreen(TFT_BLACK);

      i32Soot = 0;
      i32SootOld = 0;
      i32Kilometer = 0;
      i32KilometerOld = 0;
      i32Burn = 0;
      i32BurnOld = 0;
      i32OilTemp = 0;
      i32OilTempOld = 0;
      fOilPressure = 0;
      fOilPressureOld = 0;

      _enRequest = RequestDPFSoot;
    }
    else
    {
      _tft.fillScreen(TFT_BLACK);

      _tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      _tft.drawString("Demo mode", 10, 10);

      delay(1000);

      _tft.fillScreen(TFT_BLACK);

      _enRequest = RequestDemo;
    }
  }

  if (_bSwitchPage)
  {
    _bSwitchPage = false;
    bForceDisplayUpdate = true;

    switch (_enPage)
    {
      case Page1:
        {
          _tft.fillScreen(TFT_BLACK);

          _tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          _tft.drawString("Page 2", 10, 10);

          delay(1000);

          _enPage = Page2;
        }
        break;

      case Page2:
        {
          _tft.fillScreen(TFT_BLACK);

          _tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          _tft.drawString("Page 1", 10, 10);

          delay(1000);

          _enPage = Page1;
        }
        break;
    }
  }

  switch (_enRequest)
  {
    case RequestDemo:
      {
        static int32_t soot = 10;
        static int32_t km = 150;
        static int32_t burn = 0;
        static int32_t oilTemp = 20;
        static float oilPressure = 1.2345;

        UpdateDisplay(soot, km, burn, oilTemp, oilPressure);

        delay(500);

        oilTemp = random(20, 90);
        oilPressure = random(0, 10) * 1.2345;

        if (burn)
        {
          int32_t tempSoot = soot - random(5, 10);

          if (tempSoot < 0)
          {
            tempSoot = random(0, 9);
          }

          soot = tempSoot;

          if (soot < 10)
          {
            km = 0;
            burn = 0;
          }
        }
        else
        {
          soot += random(5, 10);
          burn = soot > 80 + random(5, 10) ? 7 : 0;
        }

        km += 5;
      }
      break;

    case RequestDPFSoot:
      {
        // read DPF soot
        i32Temp = (int32_t)_elm327.processPID(0x22, 0x336A, 1, 1);

        if (i32Temp)
        {
          i32Soot = i32Temp;

          _enRequest = RequestDPFKilometer;
        }
      }
      break;

    case RequestDPFKilometer:
      {
        // read DPF km since last regen
        i32Temp = (int32_t)_elm327.processPID(0x22, 0x3039, 1, 2);

        if (i32Temp)
        {
          i32Kilometer = _elm327.responseByte_1 << 8 | _elm327.responseByte_0;

          _enRequest = RequestDPFBurn;
        }
      }
      break;

    case RequestDPFBurn:
      {
        // read DPF burn status
        i32Temp = (int32_t)_elm327.processPID(0x22, 0x20F2, 1, 1);

        if (i32Temp)
        {
          i32Burn = i32Temp;

          _enRequest = RequestOilTemp;
        }
      }
      break;

    case RequestOilTemp:
      {
        // read oil temperatur
        i32Temp = (int32_t)_elm327.processPID(0x22, 0x1154, 1, 1);

        if (i32Temp)
        {
          i32OilTemp = i32Temp - 40;

          _enRequest = RequestOilPressure;
        }
      }
      break;

    case RequestOilPressure:
      {
        // read oil pressure
        i32Temp = (int32_t)_elm327.processPID(0x22, 0x115C, 1, 1);

        if (i32Temp)
        {
          fOilPressure = ((i32Temp * 0.65) - 17.5) / 14.5037738;

          _enRequest = RequestDPFSoot;
        }
      }
      break;
  }

  if ((bForceDisplayUpdate) || (i32Soot != i32SootOld) || (i32Kilometer != i32KilometerOld) || (i32Burn != i32BurnOld) || (i32OilTemp != i32OilTempOld) || (fOilPressure != fOilPressureOld))
  {
    bForceDisplayUpdate = false;

    UpdateDisplay(i32Soot, i32Kilometer, i32Burn, i32OilTemp, fOilPressure);

    i32SootOld = i32Soot;
    i32KilometerOld = i32Kilometer;
    i32BurnOld = i32Burn;
    i32OilTempOld = i32OilTemp;
    fOilPressureOld = fOilPressure;
  }
}

void UpdateDisplay(int32_t i32Soot, int32_t i32Kilometer, int32_t i32Burn, int32_t i32OilTemp, float fOilPressure)
{
  static int32_t i32BurnLast;

  switch (_enPage)
  {
    case Page1:
      {
        uint32_t u32ColorSoot;
        uint32_t u32ColorBurn;

        u32ColorSoot = i32Soot > 79 ? TFT_RED : TFT_WHITE;
        u32ColorBurn = i32Burn > 0 ? TFT_RED : TFT_WHITE;

        if (i32Burn > 0)
        {
          String sText = String(i32Soot) + " %";

          _tft.setTextSize(7);

          _tft.fillScreen(TFT_RED);
          _tft.setTextColor(TFT_WHITE, TFT_RED);
          _tft.drawString(sText, _tft.width() / 2 - _tft.textWidth(sText) / 2, _tft.height() / 2 - _tft.fontHeight() / 2);

          delay(2000);

          _tft.fillScreen(TFT_BLACK);
        }
        else
        {
          if (i32BurnLast > 0)
          {
            String sText = "Done";

            _tft.setTextSize(7);

            _tft.fillScreen(TFT_GREEN);
            _tft.setTextColor(TFT_WHITE, TFT_GREEN);
            _tft.drawString(sText, _tft.width() / 2 - _tft.textWidth(sText) / 2, _tft.height() / 2 - _tft.fontHeight() / 2);

            delay(5000);

            _tft.fillScreen(TFT_BLACK);
          }
        }

        i32BurnLast = i32Burn;

        _tft.setTextSize(2);

        _tft.setCursor(10, 10);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.print("DPF soot:");

        _tft.setCursor(130, 10);
        _tft.setTextColor(u32ColorSoot, TFT_BLACK);
        _tft.printf("%i %%     ", i32Soot);

        _tft.setCursor(10, 31);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.print("DPF dist:");

        _tft.setCursor(130, 31);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.printf("%i km     ", i32Kilometer);

        _tft.setCursor(10, 52);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.print("DPF burn:");

        _tft.setTextSize(3);
        _tft.setCursor(130, 52);
        _tft.setTextColor(u32ColorBurn, TFT_BLACK);
        _tft.printf("%s     ", i32Burn > 0 ? "Active" : "Idle");
        _tft.setTextSize(2);

        _tft.drawRect(10, 85, 202, 50, TFT_WHITE);
        _tft.fillRect(11 + 2 * i32Soot, 86, 200 - 2 * i32Soot, 48, TFT_BLACK);
        _tft.fillRect(11, 86, 2 * i32Soot, 48, u32ColorSoot);
      }
      break;

    case Page2:
      {
        _tft.setCursor(10, 10);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.print("Oil temp:");

        _tft.setCursor(130, 10);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.printf("%i °C     ", i32OilTemp);

        _tft.setCursor(10, 31);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.print("Oil bar:");

        _tft.setCursor(130, 31);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.printf("%.1f bar     " , fOilPressure);

        // show red stripe in case of DPF burn
        _tft.fillRect(0, _tft.height() - 10, _tft.width(), _tft.height(), i32Burn > 0 ? TFT_RED : TFT_BLACK);
      }
      break;
  }
}
