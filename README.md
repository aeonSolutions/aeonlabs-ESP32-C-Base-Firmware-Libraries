[Open Software Caralog](https://github.com/aeonSolutions/aeonlabs-open-software-catalogue)  >>  ESP32 C++ Base Firmware Libraries

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


<br />

# [![CodeFactor](https://www.codefactor.io/repository/github/aeonsolutions/aeonlabs-esp32-c-base-firmware-libraries/badge)](https://www.codefactor.io/repository/github/aeonsolutions/aeonlabs-esp32-c-base-firmware-libraries) ESP32 C++ Base Firmware Libraries

These are simple C++ class libraries i maintain to speed up firmware code developent on all my ESP32 PCB electonics.
This is still an early code develpment. Is memory intensive and lacks many code optimizations.

Currently it uncludes:

- Geolocation over WIFI 
- Onboard sensors 
   - SHT3x Temperature & Humidty sensor
   - AHT2x Temperature & Humidty sensor
   - LSM6DS3 motion sensor
- FFat File managment
- WIFI connectivity managemnt
- NPT Internet Time Syncronization
- $GBRL command style communication
   - over Serial USB-C interface
   - over Bluetooth BLE interface
- Microprocessor Core Frequency Management
- Some simple Math functions
- CSV style Files for settings and configuration
- AT204SHA IC for unique data fingerprint identification on open environments 
- Dataset upload to a dataverse data repository. More info at www.dataverse.org and also on my [C++ API library](https://github.com/aeonSolutions/OpenScience-Dataverse-API-C-library) .

<br>

## ToDo List

  - Calc MD5 hash of the dataset file yo compare with json result on upload 
  - ✓ ~~OTA firmware update~~
  - ✓ ~~Validate Json received on a new dataset upload~~
  - ✓ ~~Load dataset repository metadata~~
  - ✓ ~~Output board startup diagnostics serial stream to a Bluetooth or WIFI data stream~~
  - ✓ ~~accept GBRL like setup and config $ commands.~~
  - SQLite database to store measured experumental data and upload to a dataverse repository
  - Swarm connectivity to other nearby Smart DAQ devices for experimental data redundancy and sharing (IoE - Internet of Everything). 
  - Blockchain like data storage and exchange of experimental data collected
  - Sensor Data multiple Signing with multiple unique Fingerprint ID requests to other Smart DAQ devices
  - ✓ ~~Automatic geolocation of experimental data origins using the wifi network it connects~~
  - ✓ ~~Motion Detection of unauthorized handling of a specimen during an ongoing experiment~~
  - ✓ ~~Smart Device Language Pack Downloader and Language Selection~~
  - ✓ ~~Github Certificate of Authority (CA) Updater~~

<br>

### Smart Devices I prototyped and using this code on their OEM firmware versions

- [Smart Concrete Curing Monitoring Device](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity)

This repo contains intructions on how to assemble a smart PCB able to monitor Fresh Reinforced concrete Hardening to determine concrete maturity and estimate its streght. The smart PCB is a 12bit smart DAQ. 

<p align="center">
<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart%20concrete%20maturity%20inside%20transparent%20enclosure.jpeg" height="200">
</p>

<br>

- [Validation of Experimental Data Origins: A Swarm of DAQ devices able to Deliver Unique Experimental Data using Blockchain-like Fingerprint ID to a Data Repository](https://github.com/aeonSolutions/openScience-Smart-DAQ-to-Upload-Live-Experimental-Data-to-a-Data-Repository)


This Sci. research presents an innovative method for experimental data acquisition and management of collected data in real-time and is compatible with any open environment. The proposed smart DAQ device prototype has the minimum hardware characteristics to handle data measurements collected from sensors locally connected to it, store it on a local CSV or SQLite database file, and finally connect and synchronize data measurements collected with a data repository hosted remotely on a Dataverse.

These Smart DAQ devices are of type "Internet of Everything" (IoE) Smart Devices and are able to connect with each other using swarm intelligence. The main purpose is to increase data integrity and trustworthiness among DAQ devices connected and on all experimental data collected during an experiment or research project.

<p align="center">
  <img height="200" src="https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/blob/main/Design/12bitSmartDAQ2023january_2.jpeg">
</p>

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

### Be supportive of my dedication and work towards technology education and buy me a cup of coffee
The PCB Desgin Files i provide here for anyone to use are free. If you like this Smart Device or use it, please consider buying me a cup of coffee, a slice of pizza or a book to help me study, eat and think new PCB design files.

<p align+"left">
<a href="https://www.buymeacoffee.com/migueltomas">
   <img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" data-canonical-src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" height="35" />
 </a>
<br>
<img src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/Designs/bmc_qr.png" height="130">
</p>



<br />

### Make a donation on Paypal
Make a donation on paypal and get a TAX refund*.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/paypal_small.png)](http://paypal.me/mtpsilva)

<br>

### Support all these open hardware projects and become a patreon  
Liked any of my PCB KiCad Designs? Help and Support my open work to all by becomming a LDAD Patreon.
In return I will give a free PCB design in KiCad to all patreon supporters. To learn more go to patreon.com. Link below.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/patreon_small.png)](https://www.patreon.com/ldad)

<br />
<br />

______________________________________________________________________________________________________________________________
### License

Before proceeding to download any of AeonLabs software solutions for open-source development and/or PCB hardware electronics development make sure you are choosing the right license for your project. See [AeonLabs Solutions for Open Hardware & Source Development](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development) for more information. 



______________________________________________________________________________________________________________________________
