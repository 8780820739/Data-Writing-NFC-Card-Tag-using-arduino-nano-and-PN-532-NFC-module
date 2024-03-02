#include "Wire.h"
#include "nfc.h"

/** define a nfc class */
NFC_Module nfc;
u8 sectornum = 0;
u8 counter = 0;
u8 newblock[] = "RWU is a university of applied sciences in one of the 
strongest economic regions in Europe. "
"Courses are offered in fields ranging from engineering, IT, and business to 
social studies and healthcare."
"The degree programs are characterized by a high degree of practical, realworld orientation and close cooperation between all involved."
"On RWU's friendly campus, students can test and expand their potential and 
shape the future through interdisciplinary dialogue."
"Around 3,800 young people from all over the world study at RWU. Partner 
universities on every continent offer valuable experience abroad."
"Studying at RWU is more than just acquiring knowledge:This is where talents 
are discovered and fostered, "
"where personalities bloom, and where life-long friendships start";
//char* temparr = newblock;
void setup(void)
{
Serial.begin(9600);
nfc.begin();
Serial.println("MF1S50 Reader Demo From Elechouse!");
uint32_t versiondata = nfc.get_version();
if (! versiondata) {
Serial.print("Didn't find PN53x board");
while (1); // halt
}
// Got ok data, print it out!
Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, 
HEX);
Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
/** Set normal mode, and disable SAM */
nfc.SAMConfiguration();
}
void loop(void)
{
u8 buf[32],sta;
/** Polling the mifar card, buf[0] is the length of the UID */
sta = nfc.InListPassiveTarget(buf);

/** check state and UID length */
if(sta && buf[0] == 4){
/** the card may be Mifare Classic card, try to read the block */
Serial.print("UUID length:");
Serial.print(buf[0], DEC);
Serial.println();
Serial.print("UUID:");
nfc.puthex(buf+1, buf[0]);
Serial.println();
/** factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF */
u8 key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
u8 wblocknum = 0;
if(sectornum <16){
u8 authblock = sectornum * 4;
/** Authentication bloks */
sta = nfc.MifareAuthentication(0, authblock, buf+1, buf[0], key);
if(sta){
Serial.println("Authentication success... ");
if(sectornum == 0){
/** write block data in sector 0*/
for(wblocknum = 1; wblocknum < 3; wblocknum++)
{
/** write block*/
sta = nfc.MifareWriteBlock(wblocknum, newblock + (counter*16));
if(sta){
Serial.println("Write block successfully..");
}
counter = counter + 1;
}
}
else{
/** write block data in sector 1 to 15*/
for(wblocknum = authblock; wblocknum < (authblock + 3); 
wblocknum++){
/** write block*/
sta = nfc.MifareWriteBlock(wblocknum, newblock + 
(counter*16));
if(sta){
Serial.println("Write block successfully..");
}
counter = counter + 1;
}
}
}

Serial.print("sector written done: ");
Serial.print(sectornum, DEC);
Serial.println();
Serial.println();
/* handle sectors */
sectornum = sectornum + 1;
}
else{
Serial.println("****************All sectors are 
written*********************");
Serial.println();
sectornum = 0;
counter = 0;
}
}
}
