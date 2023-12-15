/*
 Copyright (c) 2023 Miguel Tomas, http://www.aeonlabs.science

License Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
You are free to:
   Share — copy and redistribute the material in any medium or format
   Adapt — remix, transform, and build upon the material

The licensor cannot revoke these freedoms as long as you follow the license terms. Under the following terms:
Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. 
You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

NonCommercial — You may not use the material for commercial purposes.

ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under
 the same license as the original.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others
 from doing anything the license permits.

Notices:
You do not have to comply with the license for elements of the material in the public domain or where your use
 is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. 
For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


Before proceeding to download any of AeonLabs software solutions for open-source development
 and/or PCB hardware electronics development make sure you are choosing the right license for your project. See 
https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development
 for Open Hardware & Source Development for more information.

*/

#include "telegram.h"
#include "Arduino.h"


TELEGRAM_CLASS::TELEGRAM_CLASS() {
    this->lastTimeBotRan = 0;
    this->openOrderRequest = false;

    this->CupOrderSize ="normal";
    this->orderRequestUsername = "xxxxxx";

    this->orderRequestChatID = "xxxxxxxxxx";
    this->orderRequestType = "xxxxxxxx";
    this->orderRequestTime = 0;

    this->totalCoffees = 0;
    this->totalTea = 0;
    this->totalDecaf = 0;
    this->totalCapuccino = 0;
        
    this->OWNER_CHAT_ID = "xxxxxxxxxxx";
    // Initialize Telegram BOT
    this->BOTtoken = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // your Bot Token (Get from Botfather)

}

void TELEGRAM_CLASS::init(){
  
  this->botRequestDelay = 1000;

  this->client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  this->bot = new UniversalTelegramBot(this->BOTtoken, this->client);
  
  this->OWNER_CHAT_ID_LIST[0] = this->OWNER_CHAT_ID;
  this-> OWNER_CHAT_ID_LIST_ID = 1;

}

// ************************************************************
String TELEGRAM_CLASS::runTelegramBot(){
  if (millis() > this->lastTimeBotRan + this->botRequestDelay)  {
    int numNewMessages = this->bot->getUpdates(this->bot->last_message_received + 1);

    if (numNewMessages > 0) {
      return this->handleNewMessages(0);
    }

    this->lastTimeBotRan = millis();
    return "";
  }
}

// ***********************************************************************************************
// Handle what happens when you receive new messages
String TELEGRAM_CLASS::handleNewMessages(int i) {

    String chat_id = String(this->bot->messages[i].chat_id);
    
    // Print the received message
    String text = this->bot->messages[i].text;

    String from_name = this->bot->messages[i].from_name;
    String str = "/cmd "+ this->TELEGRAM_DEVICE_ID +" ";
    if (chat_id == this->OWNER_CHAT_ID && text.indexOf(str)>-1 ){
      return text.substring(str.length(), text.length());
    }

    if (text == "/devices") {
      this->bot->sendMessage(chat_id, "Device name: " + this->TELEGRAM_DEVICE_ID, "");
    }

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "You have available the following commands \n\n";
      welcome += "/add_me to subscribe LIVE experimental data notifications \n";
      welcome += "/remove_me to unsubscribe LIVE experimental data notifications \n";
      welcome += "/devices view current DAQs connected\n";
      welcome += "\n";
      welcome += "/donation to support this open project\n";
      welcome += "/projectPage to view this open project on Github";

      this->bot->sendMessage(chat_id, welcome, "");
      return "";

    }

    if (text == "/remove_me") {
      for(int i=0 ; i < this->OWNER_CHAT_ID_LIST_ID ; i++ ){
        if ( this->OWNER_CHAT_ID_LIST[i] == chat_id ){
          this->OWNER_CHAT_ID_LIST[i] = this->OWNER_CHAT_ID_LIST[ this->OWNER_CHAT_ID_LIST_ID - 1 ];
          this->OWNER_CHAT_ID_LIST[ this->OWNER_CHAT_ID_LIST_ID - 1 ] ="";
          this->OWNER_CHAT_ID_LIST_ID --;
          this->bot->sendMessage( chat_id , "You have been removed from the list.", "");
          return "";
        }
      }
      
      this->bot->sendMessage( chat_id , "You are not part of the LIVE experimental data list.", "");
      return "";
    }

    if (text == "/add_me") {
      if (this->OWNER_CHAT_ID_LIST_ID >= 200 ){
        this->bot->sendMessage( chat_id , "The List is full. No more users allowed. Try again later. ", "");
        return "";
      }

      for(int i=0 ; i < this->OWNER_CHAT_ID_LIST_ID ; i++ ){
        if ( this->OWNER_CHAT_ID_LIST[i] == chat_id ){
          this->bot->sendMessage( chat_id , "You are already receiving LIVE results ", "");
          return "";
        }
      }

      this->OWNER_CHAT_ID_LIST[ this->OWNER_CHAT_ID_LIST_ID ] = chat_id;
      this->OWNER_CHAT_ID_LIST_ID ++;
    }

    if (text == "/projectPage") {
      String msg ="This smart datalogger is an open project available on github here:\n";
      msg += "https://github.com/aeonSolutions/openScience-Smart-DAQ-to-Upload-Live-Experimental-Data-to-a-Data-Repository\n";
      this->bot->sendMessage(chat_id, msg, "");
      return "";
    }
    
    if (text == "/donation") {
      String msg ="To support this open hardware & open source project you can make a donation using:\n";
      msg += "- Paypal: mtpsilva@gmail.com\n";
      msg += "- or at https://www.buymeacoffee.com/migueltomas";
      this->bot->sendMessage(chat_id, msg, "");
      return "";
    }
    

    if (this->openOrderRequest==true){
      if (this->orderRequestChatID  == chat_id){
        this->bot->sendMessage(chat_id, "You already have a pending 🕒 request. Tell Miguel to acccept your offer or wait until he replies. Thank you. 🙏🏼", "");
      }else{
        this->bot->sendMessage(chat_id, "There's already a pending request 🕒 made by someone else. Try again later. Thank you. 🙏🏼", "");
      }      
    }

  return "";
}


// ************************************************************
