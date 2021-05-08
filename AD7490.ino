#include <SPI.h>

//AD7490
#define CS_AD7490_1 2
#define CS_AD7490_2 3
#define PM 3
#define WEAK 0
#define RANGE 0
#define CODING 1
#define SPI_Speed 4000000

volatile uint16_t ADCS[32];

int AD7490Read(byte CS, byte ch)
{
  digitalWrite(CS, LOW);
  SPI.beginTransaction(SPISettings(SPI_Speed, MSBFIRST, SPI_MODE0));
  word Control = ((1<<11)|(ch<<6)|(PM<<4)|(WEAK<<2)|(RANGE<<1)|(CODING))<<4;
  SPI.transfer16(Control);
  digitalWrite(CS, HIGH);
  delayMicroseconds(5);
  digitalWrite(CS, LOW);
  word Data = SPI.transfer16(0);
  SPI.endTransaction();
  digitalWrite(CS, HIGH);
  if((Data>>12)==ch)
    Data &= 0x0FFF;
  else
    Data = 0xF000;
  return(Data);
}

int AD7490ReadAll()
{
  for(int i=0; i<16; i++)
  {
    ADCS[i] = AD7490Read(CS_AD7490_1, i);
    ADCS[i+16] = AD7490Read(CS_AD7490_2, i);
  }
}

void setup() {
  SPI.begin();
  pinMode(CS_AD7490_1, OUTPUT);
  digitalWrite(CS_AD7490_1, HIGH);
  pinMode(CS_AD7490_2, OUTPUT);
  digitalWrite(CS_AD7490_2, HIGH);
}

void loop()
{
  AD7490ReadAll();


}
