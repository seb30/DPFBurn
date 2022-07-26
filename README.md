# DPFBurn

1. Download and install [Arduino IDE](https://www.arduino.cc/en/software)

2. Add support for TTGO T-Display

![DPFBurn_Arduino_Preferences](https://user-images.githubusercontent.com/28293949/181024539-7d9efc1f-12d3-4ebe-b1f6-a0da42f8b190.jpg)

Enter the URL `https://dl.espressif.com/dl/package_esp32_index.json` under `Additional Boards Manager URLs`

![DPFBurn_Arduino_Preferences_Settings](https://user-images.githubusercontent.com/28293949/181024589-3fd241a0-11bd-4c6b-9538-60bc4cd9dae7.jpg)

The board can now be selected.

![DPFBurn_Arduino_Board_Select](https://user-images.githubusercontent.com/28293949/181024652-bf9652f7-1390-4ca1-9690-bea03e1e259e.jpg)

3. Install necessary libraries

![DPFBurn_Arduino_Libraries](https://user-images.githubusercontent.com/28293949/181024684-117aba2a-5304-4f25-9371-fd4de61eb18d.jpg)

- Button2

![DPFBurn_Button2](https://user-images.githubusercontent.com/28293949/181024700-a07a658c-0e44-4b60-86ca-8d864cb815b1.jpg)

- ELMDuino

![DPFBurn_ELMDuino](https://user-images.githubusercontent.com/28293949/181024714-b3dbfcea-d6b4-467b-b7f1-cda4b3d97b62.jpg)

- TFT_eSPI

![DPFBurn_TFT](https://user-images.githubusercontent.com/28293949/181024724-a0c17af4-0936-4d95-bf48-a89e4bfc4bf3.jpg)

4. Change TFT_eSPI library to handle TTGo T-Display

Edit file `%userprofile%\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h` and change

Line 22 from

`#include <User_Setup.h>           // Default setup is root library folder`

to

`//#include <User_Setup.h>           // Default setup is root library folder`

and line 53 from

`//#include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT`

to

`#include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT`

5. Download the files [DPF.ino](DPF.ino) and [FreeFonts.h](FreeFonts.h)

6. Open the `DPF.ino` in Arduino IDE.

7. Change the MAC address to your OBD II adapter MAC address on [line 20](https://github.com/seb30/DPFBurn/blob/main/DPF.ino#L20) (my MAC is 86:DC:52:B5:F6:F1)

https://user-images.githubusercontent.com/28293949/181034516-2630e44f-c084-4aaf-9e08-255ecdbc3262.mp4

`uint8_t _rgu8MACAddress[6] = { 0x86, 0xDC, 0x52, 0xB5, 0xF6, 0xF1 };`

8. Open the `DPF.ino` in Arduino IDE and press `CTRL + R` to compile and check for errors.

![DPFBurn_Arduino_Compile](https://user-images.githubusercontent.com/28293949/181036190-5c348f22-97b3-4194-b9ec-9da280b8d395.jpg)

9. Press `CTRL + U` to flash the TTGO T-Display.

10. Go to your car and check, whether a connection can be established.
