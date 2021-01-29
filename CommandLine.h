#include <string.h>
#include <stdlib.h>

//this following macro is good for debugging, e.g.  print2("myVar= ", myVar);
#define print2(x,y) (Serial.print(x), Serial.println(y))


#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

const char *delimiters            = ", \n";                    //commands can be separated by return, space or comma

/*************************************************************************************************************
     Add command words here
*************************************************************************************************************/


const char *activateCommandToken       = "activate";                   
const char *deactivateCommandToken     = "deactivate";
const char *toggleCommandToken         = "toggle";
const char *holdCommandToken           = "hold";    
const char *helpCommandToken           = "?";              


/**********************************************/


/*************************************************************************************************************
     Relay pins and state variables. This is only needed for the relay board.
*************************************************************************************************************/

volatile byte relayOneState = LOW;
volatile byte relayTwoState = LOW;
volatile byte relayThreeState = LOW;
volatile byte relayFourState = LOW;
volatile byte relayFiveState = LOW;
volatile byte relaySixState = LOW;
volatile byte relaySevenState = LOW;
volatile byte relayEightState = LOW;
volatile byte relayNineState = LOW;
volatile byte relayTenState = LOW;
volatile byte relayElevenState = LOW;
volatile byte relayTwelveState = LOW;
volatile byte relayThirteenState = LOW;
volatile byte relayFourteenState = LOW;
volatile byte relayFiveteenState = LOW;
volatile byte relaySixteenState = LOW;


bool getCommandLineFromSerialPort(char * commandLine)
{
  static uint8_t charsRead = 0;                      //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        commandLine[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          Serial.println(commandLine);
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          commandLine[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS);  //no idea how this works, found it on the Internet
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;     //just in case
        break;
    }
  }
  return false;
}


/* ************************************************
   readNumber: return a 16bit (for Arduino Uno) signed integer from the command line
   readWord: get a text word from the command line

***************************************************/


int readNumber () {
  char * numTextPtr = strtok(NULL, delimiters);         //K&R string.h  pg. 250
  return atoi(numTextPtr);                              //K&R string.h  pg. 251
}

char * readWord() {
  char * word = strtok(NULL, delimiters);               //K&R string.h  pg. 250
  return word;
}

void nullCommand(char * ptrToCommandName) {
  print2("Command not found: ", ptrToCommandName);      //see above for macro print2
}


/****************************************************
   Add your commands here
****************************************************/
void helpCommand() {
  Serial.println("List of available commands:");
  Serial.println("  activate [Channel] -      Power a given relay channel (ex: 'activate 1' turns on the connection for relay #1)");
  Serial.println("  deactivate [Channel] -    Depower a given relay channel (ex: 'deactivate 1' turns off the connection for relay #1");
  Serial.println("  toggle [Channel] -        Toggle power to a given relay channel (ex: 'toggle 2' will turn off the connection for relay #2 if it is currently on, and will turn it on if it is currently off)");
  Serial.println("  hold [Channel] [Seconds]- Turns the connection to the provided channel on for a given number of seconds (ex: 'hold 1 3' will turn on the connection to relay #1 for 3 seconds and then turn it off)");
  Serial.println("  ? -                       Displays this help list");
}

void activateCommand() {
  int relay = readNumber();
  digitalWrite(relay+21, LOW);                           //Set to low to power since all connections are on Normally Open
  Serial.print("Relay ");
  Serial.print(relay);
  Serial.println(": ON");
}

void deactivateCommand() {                               
  int relay = readNumber();
  if(relay > 0 && relay <=16){
    digitalWrite(relay+21, HIGH);                           //Set to low to power since all connections are on Normally Open
    Serial.print("Relay ");
    Serial.print(relay);
    Serial.println(": OFF");
    }
    if(relay == 0){
      for(int i=1; i<=16; i++){
        digitalWrite(i+21, HIGH);
        Serial.print("Relay ");
        Serial.print(i);
        Serial.println(": OFF");
      } 
    }
}

void holdCommand() {
  int relay = readNumber();
  int duration = readNumber();
  digitalWrite(relay+21, LOW);                           //Set to low to power since all connections are on Normally Open
  Serial.print("Relay ");
  Serial.print(relay);
  Serial.println(": ON");
  delay(duration * 1000);
  digitalWrite(relay+21, LOW);                           //Set to low to power since all connections are on Normally Open
  Serial.print("Relay ");
  Serial.print(relay);
  Serial.println(": OFF");
}

/****************************************************
   Execute the command
****************************************************/


bool ExecuteCommand(char * commandLine) {
  //  print2("\nCommand: ", commandLine);

  char * ptrToCommandName = strtok(commandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName);
  if (strcmp(ptrToCommandName, helpCommandToken) == 0) {                   
      helpCommand();

    } else {
    if (strcmp(ptrToCommandName, activateCommandToken) == 0) {                   
      activateCommand();

    } else {
      if (strcmp(ptrToCommandName, deactivateCommandToken) == 0) {           
       deactivateCommand();

      } else {
        if (strcmp(ptrToCommandName, toggleCommandToken) == 0) {                   
          //toggleCommand();

        } else {
          if (strcmp(ptrToCommandName, holdCommandToken) == 0) {                   
            holdCommand();

          } else {
            nullCommand(ptrToCommandName);
          }
        }
      }   
    }
  }
}
