[![Telegram](https://img.shields.io/badge/join-telegram-blue.svg?style=for-the-badge)](https://t.me/+W4rVVa0_VLEzYmI0)
 [![WhatsApp](https://img.shields.io/badge/join-whatsapp-green.svg?style=for-the-badge)](https://chat.whatsapp.com/FkNC7u83kuy2QRA5sqjBVg) 
 [![Donate](https://img.shields.io/badge/donate-$-brown.svg?style=for-the-badge)](http://paypal.me/mtpsilva)
 [![Say Thanks](https://img.shields.io/badge/Say%20Thanks-!-yellow.svg?style=for-the-badge)](https://saythanks.io/to/mtpsilva)
![](https://img.shields.io/github/last-commit/aeonSolutions/aeonlabs-ESP32-C-Base-Firmware-Libraries?style=for-the-badge)
<a href="https://trackgit.com">
<img src="https://us-central1-trackgit-analytics.cloudfunctions.net/token/ping/lgj908xjlweccmzynhbl" alt="trackgit-views" />
</a>
![](https://views.whatilearened.today/views/github/aeonSolutions/aeonlabs-ESP32-C-Base-Firmware-Libraries.svg)
[![Open Source Love svg1](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](#)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat&label=Contributions&colorA=red&colorB=black	)](#)
[<img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" data-canonical-src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" height="30" />](https://www.buymeacoffee.com/migueltomas)

[Open Software Caralog](https://github.com/aeonSolutions/aeonlabs-open-software-catalogue)  >>  ESP32 C++ Base Firmware Libraries

<p align="right">
 <a href="https://github-com.translate.goog/aeonSolutions/aeonlabs-ESP32-C-Base-Firmware-Libraries?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=en&_x_tr_pto=wapp">Change Language</a> <br>
Last update: 3-1-2024
</p>

# [![CodeFactor](https://www.codefactor.io/repository/github/aeonsolutions/aeonlabs-esp32-c-base-firmware-libraries/badge)](https://www.codefactor.io/repository/github/aeonsolutions/aeonlabs-esp32-c-base-firmware-libraries) ESP32 C++ Base Firmware Libraries
**current project Total: > 520 work.h**

These are simple C++ class libraries I maintain to speed up firmware code development on all my ESP32 PCB electronics.
This is still an early code development. Is memory intensive and lacks many code optimizations.

<br>

Currently, it includes:

- TFT LCD Display Library (uses [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI))
- Geolocation over WIFI 
- External sensor measurements (channels 1 & 2)
   - DS18B20 Temperature sensor
   - AHT2x Temperature & Humidity sensor
   - SHT3x Temperature & Humidity sensor
   - Built-in Ohmmeter
   - VL6180x Laser sensor
   - LSM6DS3 motion sensor
- Onboard sensors 
   - AHT2x/ SHT3x Temperature & Humidity sensor
   - LSM6DS3 motion sensor
- FFat File management (uses [LittleFs](https://github.com/littlefs-project/littlefs) )
- WIFI connectivity management
- NPT Internet Time Synchronization
- $GBRL command style communication
   - over Serial USB-C interface
   - over Bluetooth BLE interface
- Microprocessor Core Frequency Management
- Some simple Math functions
- CSV style Files for settings and configuration
- AT204SHA IC for unique data fingerprint identification in open environments 
- Dataset upload to a dataverse data repository. More info at www.dataverse.org and also on my [C++ API library](https://github.com/aeonSolutions/OpenScience-Dataverse-API-C-library) .


**Most recent code bugs corrected:** <br>
1-1-2024
- WiFi connectivity after light sleep is now working
- onboard RGB LED is now turning ON the exact RGB color 

View the full [reviion history](https://github.com/aeonSolutions/aeonlabs-ESP32-C-Base-Firmware-Libraries/wiki/revision-history). 

<br>

## How to use this Library
If you're using Arduino Studio for windows, download this repository and unzip it to the "Arduino/libraries" folder located in "Documents". <br>
Next rename the unziped folder "aeonlabs-ESP32-C-Base-Firmware-Libraries-main" to "AeonLabs". <br>
Restart Arduino Studio, and the Aeonlabs library is now ready to be used. 


<br>

## Library Dependencies
- ✓ DallasTemperature [view](https://www.arduino.cc/reference/en/libraries/dallastemperature/)
- ✓ Onewire [view]( [view]())
- ✓ Semphr [view](https://github.com/FreeRTOS/FreeRTOS-Kernel)
- ✓ max6675 [view](https://github.com/adafruit/MAX6675-library)
- ✓ arduinoJSON [view](https://arduinojson.org)
- ✓ Adafruit_VL6180X [view](https://github.com/adafruit/Adafruit_VL6180X)
- ✓ esp32FOTA [view](https://www.arduino.cc/reference/en/libraries/esp32fota/)
- ✓ UniversalTelegramBot [view](https://www.arduino.cc/reference/en/libraries/esp32fota/)
- ✓ AHT20 [view](https://www.arduino.cc/reference/en/libraries/aht20/)
- ✓ SparkFunLSM6DS3 [view](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library/blob/master/src/SparkFunLSM6DS3.h)
- ✓ ESP32Ping [view](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library/blob/master/src/SparkFunLSM6DS3.h)
- ✓ Adafruit_SHT31 [view](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library/blob/master/src/SparkFunLSM6DS3.h)
- ✓ TFT_eSPI [view](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library/blob/master/src/SparkFunLSM6DS3.h)
- ✓ ESP32Ping [view](https://github.com/marian-craciunescu/ESP32Ping)


<br>

### Smart Devices I prototyped and using this code on their OEM firmware versions

- [Smart Concrete Curing Monitoring Device](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity)
- [Validation of Experimental Data Origins: A Swarm of DAQ devices able to Deliver Unique Experimental Data using Blockchain-like Fingerprint ID to a Data Repository](https://github.com/aeonSolutions/openScience-Smart-DAQ-to-Upload-Live-Experimental-Data-to-a-Data-Repository)
- [Smart Power Switch](https://github.com/aeonSolutions/AeonLabs-Smart-Power-Switch-Outlet/blob/main/README.md)
- [Smart Coffee Machine Add-on](https://github.com/aeonSolutions/AeonLabs-Home-Automation-Smart-Coffee-MAchine-Addon)


<br>

### Useful utilities

[Arduino ESP8266/ESP32 Exception Stack Trace Decoder](https://github.com/me-no-dev/EspExceptionDecoder)

<br />
<br />

## Author

You can get in touch with me on my LinkedIn Profile:

#### Miguel Tomas

[![LinkedIn Link](https://img.shields.io/badge/Connect-Miguel--Tomas-blue.svg?logo=linkedin&longCache=true&style=social&label=Connect)](https://www.linkedin.com/in/migueltomas/)

<a href="https://stackexchange.com/users/18907312/miguel-silva"><img src="https://stackexchange.com/users/flair/18907312.png" width="208" height="58" alt="profile for Miguel Silva on Stack Exchange, a network of free, community-driven Q&amp;A sites" title="profile for Miguel Silva on Stack Exchange, a network of free, community-driven Q&amp;A sites" /></a>

<a href="https://app.userfeel.com/t/2f6cb1e0" target="_blank"><img src="https://app.userfeel.com/tester/737648/image?.png" width="257" class="no-b-lazy"></a>

You can also follow my GitHub Profile to stay updated about my latest projects: [![GitHub Follow](https://img.shields.io/badge/Connect-Miguel--Tomas-blue.svg?logo=Github&longCache=true&style=social&label=Follow)](https://github.com/aeonSolutions)

<br>

**Hire me** <br>
See [here](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/How-to-Hire-AeonLabs) how to hire AeonLabs.

<br>

### Be supportive of my dedication and work towards technology education and buy me a cup of coffee
The PCB design Files I provide here for anyone to use are free. If you like this Smart Device or use it, please consider buying me a cup of coffee, a slice of pizza or a book to help me study, eat and think new PCB design files.

<p align="center">
    <a href="https://www.buymeacoffee.com/migueltomas">
        <img height="35" src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png">
    </a>
</p>


### Make a donation on PayPal
Make a donation on PayPal and get a TAX refund*.

<p align="center">
    <a href="http://paypal.me/mtpsilva">
        <img height="35" src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/media/paypal_small.png">
    </a>
</p>

### Support all these open hardware projects and become a GitHub sponsor  
Liked any of my PCB KiCad Designs? Help and Support my open work to all by becoming a GitHub sponsor.

<p align="center">
    <a href="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/become_a_sponsor/aeonlabs-github-sponsorship-agreement.docx">
        <img height="50" src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/media/want_to_become_a_sponsor.png">
    </a>
    <a href="https://github.com/sponsors/aeonSolutions">
        <img height="50" src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/media/become_a_github_sponsor.png">
    </a>
</p>

# 

### License

Before proceeding to download any of AeonLabs software solutions for open-source development and/or PCB hardware electronics development make sure you are choosing the right license for your project. See [AeonLabs Solutions for Open Hardware & Source Development](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development) for more information. 
