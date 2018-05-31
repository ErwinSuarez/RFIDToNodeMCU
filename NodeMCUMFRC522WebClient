/* This uses RFID_RC522 connected to NodeMCU with connection below
 * This uses the UID of scannedRFID tag and sends the UID as a request to server
 * THERE IS ADDITIONAL STEPS NEEDED TO SERVER TO ACCEPT THIS REQUEST
   ------------------------------------------------------
 -- Pin Connections between NodeMCU 1.0 and RFID-RC522 Board
 -- 
 -- NodeMCU            RFID-RC522
 --    D5                  SCK
 --    D6                  MISO
 --    D7                  MOSI
 --    D4                  SDA
 --                        IRQ
 --    D3                  RST
 --    D2                  LED2
 --    D1                  LED1
 --    3.3V                3.3V
 --    GND                 GND
   --------------------------------------------------------*/
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <MFRC522.h>

#define uchar unsigned char
#define uint  unsigned int
#define MAX_LEN 16


const char* host = "HOST_IP_HERE";  //This is IP of server
const int port = 80;              
const int httpPort = 443;
const char* ssid = "NETWORK_HERE"; //network
const char* password = "NETWORK_KEY_HERE";          //network key
const char* rmID = "S201";          //this is the initial roomID to enter

String myNum = "";
String page = "";
String url = "";

/*---------------------
  PIN Settings
  ---------------------*/
#define LED1 D1
#define LED2 D2
#define RST_PIN D3
#define SDA_PIN D4

/*-------------------------
  MF_RC522 Command Words
  -------------------------*/ 
#define PCD_IDLE      0x00      // No Action
#define PCD_AUTHENT   0x0E      // Authentication Key
#define PCD_RX        0x08      // Receive Data
#define PCD_TX        0x04      // Transmit Data
#define PCD_TR        0x0C      // Transmit & Receive Data
#define PCD_RESET_PHI 0x0F      // Reset
#define PCD_CALC_CRC  0x03      // CRC Calculate

/*------------------------------- 
  MIFARE_One Card Command Word
  -------------------------------*/ 
 #define PICC_REQ_IDL   0x26    // Stop Antenna Idle
 #define PICC_REQ_ALL   0x52    // Find all Card Antenna area
 #define PICC_ANTI_COLL 0x93    // Multi card detection
 #define PICC_SEL_TAG   0x93    // Select Tag???
 #define PICC_AUTH_KA   0x60    // authentication key A
 #define PICC_AUTH_KB   0x61    // authentication key B
 #define PICC_READ      0x30    // Read Block
 #define PICC_WRITE     0xA0    // Write Block
 #define PICC_DECR      0xC0    // Debit
 #define PICC_INCR      0xC1    // Credit
 #define PICC_RESTR     0xC2    // Transfer block data to buffer
 #define PICC_TXFR      0xB0    // Save the data in the buffer
 #define PICC_HALT      0x50    // Sleep

 /*-----------------------------
    MF_RC522 Return Codes 
    -----------------------------*/
 #define MI_OK          0
 #define MI_NOTAGERR    1
 #define MI_ERROR       2

  /*-----------------------------
    MF_RC522 Register 
    -----------------------------*/
/* --      Page 0:Status       --*/
#define Reserved00    0x00
#define CommandReg    0x01
#define CommIEnReg    0x02
#define DivlEnReg     0x03
#define CommIrqReg    0x04
#define DivIrqReg     0x05
#define ErrorReg      0x06
#define Status1Reg    0x07
#define Status2Reg    0x08
#define FIFODataReg   0x09
#define FIFOLevelReg  0x0A
#define WaterLevelReg 0x0B
#define ControlReg    0x0C
#define BitFramingReg 0x0D
#define CollReg       0x0E
#define Reserved01    0x0F

/* --    Page 1: Command       --*/
#define Reserved10    0x10
#define ModeReg       0x11
#define TxModeReg     0x12
#define RxModeReg     0x13
#define TxControlReg  0x14
#define TxAutoReg     0x15
#define TxSelReg      0x16
#define RxSelReg      0x17
#define RxThreshldReg 0x18
#define DemodReg      0x19
#define Reserved11    0x1A
#define Reserved12    0x1B
#define MifareReg     0x1C
#define Reserved13    0x1D
#define Reserved14    0x1E
#define SerlSpedReg   0x1F

/* --   Page 2:Configuration   --*/
#define Reserved20    0x20
#define CRCResultRegM 0x21
#define CRCResultRegL 0x22
#define Reserved21    0x23
#define ModWidthReg   0x24
#define Reserved22    0x25
#define RFCfgReg      0x26
#define GsNReg        0x27
#define CWGsPReg      0x28
#define ModGsPReg     0x29
#define TModeReg      0x2A
#define TPresclrReg   0x2B
#define TReloadRegH   0x2C
#define TReloadRegL   0x2D
#define TCntrValRegH  0x2E
#define TCntrValRegL  0x2F

/* --    Page 3:Test Register  --*/
#define Reserved30    0x30
#define TestSel1Reg   0x31
#define TestSel2Reg   0x32
#define TestPinEnReg  0x33
#define TstPinValReg  0x34
#define TestBusReg    0x35
#define AutoTestReg   0x36
#define VersionReg    0x37
#define AnalogTstReg  0x38
#define TestDAC1Reg   0x39
#define TestDAC2Reg   0x3A
#define TestADCReg    0x3B
#define Reserved31    0x3C
#define Reserved32    0x3D
#define Reserved33    0x3E
#define Reserved34    0x3F

/*-- End Definitions  --*/

uchar serNum[5];
uchar writeData[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100};
uchar moneyConsum = 18;
uchar moneyAdd    = 10;
uchar sectorKey[16][16] = { {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
                            {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
                            //{0x19,0x84,0x07,0x15,0x76,0x14},
                            {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
};
uchar sectorNewKeyA[16][16] = { {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
                                {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0x19,0x84,0x07,0x15,0x76,0x14},
                                {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0x19,0x33,0x07,0x15,0x34,0x14},
};


//##########################################################################################################
MFRC522 MIFAREReader(SDA_PIN, RST_PIN);
ESP8266WebServer server(port);

//##########################################################################################################
void setup() 
{
  // put your setup code here, to run once:
  pinMode(LED1,OUTPUT);
  digitalWrite(LED1,LOW);
  pinMode(LED2,OUTPUT);
  digitalWrite(LED2,LOW);
  pinMode(SDA_PIN,OUTPUT);
  digitalWrite(SDA_PIN,LOW);
  pinMode(RST_PIN,OUTPUT);
  digitalWrite(RST_PIN,HIGH);
  
  Serial.begin(115200);
  Serial.println("Loading setup");
  Serial.print("connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    
  SPI.begin();  

  server.on("/", [](){
    page = "<h1>Node MCU Web Client to Server MRFC522</h1>";
//    <h3>Card UID:</h3> <h4>"+String(myNum)+"</h4>";
    server.send(200, "text/html", page);
  });
  
  Serial.println("Entering Init");
  MFRC522_Init();
}

//##########################################################################################################
void loop() 
{
  WiFiClient client;
  
  if (!client.connect(host, port)) {
    Serial.println("connection failed INSIDE LOOP");
    return;
  }
  
  uchar i, tmp, checksum1;
  uchar status1;                  //uchar status in doc but status seems to be a reserved word as it 'colours'
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;                // Selection operation block address 0 to 63
  myNum = "";

/*  THIS WILL BE THE PART TO PASS THE VALUE TO index.php in server
  page = "\
      <html>\
        <body>\
          <form action="index.php" method="post">\
            Card UID: <input type="text" name="uid" value= myNum />\
            Room ID: <input type="text" name="roomID" value =rmID />\
          </form>\
        </body>\
      </html> ";
*/

/*  THIS WILL BE THE PART TO GET  AUTHORIZED FROM SERVER
  url = "?uid=";
  url += myNum;
  url += "&roomID=";
  url += rmID;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
*/


  status1 = MFRC522_Request(PICC_REQ_IDL, str);
  if(status1 == MI_OK)
    Serial.println("Card detected");
    
  status1 = MFRC522_Anticoll(str);
  memcpy(serNum, str, 5);
  
  if(status1 == MI_OK)
  {
    checksum1 = serNum[0] ^ serNum[1] ^ serNum[2] ^ serNum[3] ^ serNum[4];
    myNum += serNum[0];
    myNum += ",";
    myNum += serNum[1];
    myNum += ",";
    myNum += serNum[2];
    myNum += ",";
    myNum += serNum[3];
    myNum += ",";
    myNum += serNum[4];
  
    Serial.print("The card's UID is : ");
    Serial.println(myNum);  

    url = "?uid=";
    url += myNum;
    url += "&roomID=";
    url += rmID;

    //check if connection is still OK
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
           
    Serial.print("Sending requesting URL: ");

    //This is a temporary fix to pass values to server
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
    
    digitalWrite(LED1,HIGH);
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    // Read all the lines of the reply from server and print them to Serial port
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    delay(5000);
    digitalWrite(LED1,LOW);
  }

  // Indicate LED as error
  else if (status1 == MI_ERROR) {
    digitalWrite(LED2,HIGH);
    delay(2000);
    digitalWrite(LED2,LOW);
  }
  
  MFRC522_Halt();
  delay(500);


}
//##########################################################################################################
void MFRC522_Init(void)
{
  digitalWrite(RST_PIN,HIGH);
  MFRC522_Reset();
  /* Timer: TPrescaler*TreloadVal/6.78 MHz = 24 ms   */
  Write_MFRC522(TModeReg, 0x8D);        // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  Write_MFRC522(TPresclrReg, 0x3E);     // TModeReg[3..0] + TPrescalerReg
  Write_MFRC522(TReloadRegL, 30);
  Write_MFRC522(TReloadRegH, 0);        // 100%ASK
  Write_MFRC522(TxAutoReg, 0x40);
  Write_MFRC522(ModeReg, 0x3D);         // CRC Initial value 0x6363   ???
                                        //ClearBitMask(Status2Reg, 0x08);   //MFCrypto1On=0
                                        //Write_MFRC522(RxSelReg, 0x86);    //RxWait = RxSelReg[5..0] 
                                        //Write_MFRC522(RFCfgReg, 0x7F);    //RxGain = 48dB
  AntennaOn();                          //Open the antenna
}
void MFRC522_Reset(void)
{
  Write_MFRC522(CommandReg, PCD_RESET_PHI);
}

void Write_MFRC522(uchar addr, uchar val)
{
  digitalWrite(SDA_PIN, LOW);
  SPI.transfer((addr<<1)&0x7E);
  SPI.transfer(val);
  digitalWrite(SDA_PIN, HIGH);
}

uchar Read_MFRC522(uchar addr)
{
  uchar val;
  digitalWrite(SDA_PIN, LOW);
  SPI.transfer(((addr<<1)&0x7E) | 0x80);
  val = SPI.transfer(0x00);
  digitalWrite(SDA_PIN, HIGH);
  return val;
}
void AntennaOn(void)
{
  uchar temp;
  temp = Read_MFRC522(TxControlReg);
  if(!(temp & 0x03))
  {
    SetBitMask(TxControlReg, 0x03);
  }
}

void SetBitMask(uchar reg, uchar mask)
{
  uchar tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg,tmp | mask);
}

uchar MFRC522_Request(uchar reqMode, uchar *TagType)
{
  uchar status1;
  uint backBits;
  Write_MFRC522(BitFramingReg, 0x07);
  TagType[0] = reqMode;
  status1 = MFRC522_ToCard(PCD_TR, TagType, 1, TagType, &backBits);
  if((status1 != MI_OK) || (backBits != 0x10))
  {
    status1 = MI_ERROR;
  }
  return status1;
}

uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
{
  uchar status1 = MI_ERROR;
  uchar irqEn = 0x00;
  uchar waitIRq = 0x00;
  uchar lastBits;
  uchar n;
  uint i;

  switch(command)
  {
    case PCD_AUTHENT:
    {
      irqEn = 0x12;
      waitIRq = 0x10;
      break;
    }
    case PCD_TR:
    {
      irqEn = 0x77;
      waitIRq = 0x30;
      break;
    }
    default:
      break;
  }
  Write_MFRC522(CommIEnReg, irqEn|0x80);        // Interrupt request
  ClearBitMask(CommIrqReg, 0x80);              // Clear all interrupt bits
  SetBitMask(FIFOLevelReg, 0x80);
  Write_MFRC522(CommandReg, PCD_IDLE);
  for(i=0; i < sendLen; i++)
  {
    Write_MFRC522(FIFODataReg, sendData[i]);
  }
  Write_MFRC522(CommandReg, command);
  if(command == PCD_TR)
  {
    SetBitMask(BitFramingReg, 0x80);
  }
  i = 2000;
  do
  {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = Read_MFRC522(CommIrqReg);
    i--;
  }
  while ((i != 0) && !(n&0x01) && !(n&waitIRq));
  ClearBitMask(BitFramingReg, 0x80);
  if(i != 0)
  {
    if(!Read_MFRC522(ErrorReg) & 0x1B)
    {
      status1 = MI_OK;
      if(n & irqEn & 0x01)
      {
        status1 = MI_NOTAGERR;
      }
      if(command == PCD_TR)
      {
        n = Read_MFRC522(FIFOLevelReg);
        lastBits = Read_MFRC522(ControlReg) & 0x07;
        if(lastBits)
        {
          *backLen = (n-1)*8 + lastBits;
        }
        else
        {
          *backLen = n*8;
        }
        if(n == 0)
        {
          n = 1;
        }
        if(n > MAX_LEN)
        {
          n = MAX_LEN;
        }
        for(i = 0; i < n; i++)
        {
          backData[i] = Read_MFRC522(FIFODataReg);
        }
      }
    }
    else
    {
      status1 = MI_ERROR;
    }
  }
  return status1;
}

void ClearBitMask(uchar reg, uchar mask)
{
  uchar tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, tmp & (~mask)); //clear bit mask
}

uchar MFRC522_Anticoll(uchar *serNum)
{
  uchar status1;
  uchar i;
  uchar serNumCheck = 0;
  uint unLen;

  Write_MFRC522(BitFramingReg, 0x00);
  serNum[0] = PICC_ANTI_COLL;
  serNum[1] = 0x20;
  status1 = MFRC522_ToCard(PCD_TR, serNum, 2, serNum, &unLen);
  if(status1 == MI_OK)
  {
    for(i = 0; i < 4; i++)
    {
      serNumCheck ^= serNum[i];
    }
    if(serNumCheck != serNum[i])
    {
      status1 = MI_ERROR;
    }
  }
  //SetBitMask(CollReg, 0x80);
  return status1;
}

void MFRC522_Halt(void)
{
  uchar status1;
  uint unLen;
  uchar buff[4];
  buff[0] = PICC_HALT;
  buff[1] = 0;
  calculateCRC(buff, 2, &buff[2]);
  status1 = MFRC522_ToCard(PCD_TR, buff, 4, buff, &unLen);
}

void calculateCRC(uchar *pIndata, uchar len, uchar *pOutData)
{
  uchar i, n;
  ClearBitMask(DivIrqReg, 0x04);
  SetBitMask(FIFOLevelReg, 0x80);
  //Write_MFRC522(CommandReg, PCD_IDLE);
  for(i = 0; i < len; i++)                        //Writing data to the FIFO
  {
    Write_MFRC522(FIFODataReg, *(pIndata + i));
  }
  Write_MFRC522(CommandReg, PCD_CALC_CRC);
  i = 0xFF;                                       // Wait CRC calculation is complete
  do
  {
    n = Read_MFRC522(DivIrqReg);
    i--;
  }
  while((i != 0) && !(n & 0x04));                 // CRCIrq = 1
  pOutData[0] = Read_MFRC522(CRCResultRegL);      // Read CRC calculation result
  pOutData[1] = Read_MFRC522(CRCResultRegM);
}
