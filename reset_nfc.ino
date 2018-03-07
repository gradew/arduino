#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

constexpr uint8_t NR_KNOWN_KEYS = 8;
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 00 00 00 00 00 00
};

MFRC522::MIFARE_Key key;
byte buffer[16]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
bool alreadyAuthed=false;

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  Serial.println(F("Reset MIFARE PICC "));
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

bool testAuth(byte block)
{
  for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
        key.keyByte[i] = knownKeys[k][i];
    }
    if(alreadyAuthed){
      // http://arduino.stackexchange.com/a/14316
      if ( ! mfrc522.PICC_IsNewCardPresent()){
        Serial.println("CARD MISSING");
        return false;
      }
      if ( ! mfrc522.PICC_ReadCardSerial()){
        Serial.println("CARD MISSING");
        return false;
      }
    }
    alreadyAuthed=true;
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      //Serial.print(F("PCD_Authenticate() failed: "));
      //Serial.println(mfrc522.GetStatusCodeName(status));
    }else{
      Serial.print("Authed with key ");
      dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
      Serial.println();
      return true;
    }
  }
  return false;
}

bool authWrite(byte block, byte *buffer, byte len)
{
  MFRC522::StatusCode status;
  if(testAuth(block)==false){
    return false;
  }
  alreadyAuthed=false;
  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  return true;
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  for(int i=3;i<64;i+=4){
    byte sector=i/4;
    Serial.print("READING BLOCK ");
    Serial.print(i);
    Serial.print(" (sector ");
    Serial.print(sector);
    Serial.println(")");
    if(authWrite(i, buffer, 16)){
        Serial.println("==> OK");
      }else{
        Serial.println("==> ERROR");
      }
  }

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

}
