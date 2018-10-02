
boolean tagCheckSum(String tagString) {
  boolean res = false;
  unsigned int b1,b2,b3,b4,b5,checksum;
  char charBuf[13];
  tagString.toCharArray(charBuf, 13); 
  sscanf(charBuf , "%2x%2x%2x%2x%2x%2x", &b1, &b2, &b3, &b4, &b5, &checksum);
  if ( (b1 ^ b2 ^ b3 ^ b4 ^ b5) == checksum ) {
      return true;
      }else{
      return false;
      } 
}//tagCheckSum()

 
///// 
String getTagString(){
  String tagString = "", tempTagString = "";
  char c;
   delay(MENU_PAUSE);
    while(RFID.available()){
        c=RFID.read(); 
        tempTagString = tempTagString + c;
      if (tempTagString.length() == 16 ){ 
        if ((tempTagString[0]==2) && (tempTagString[15]==3)) {
          tempTagString = tempTagString.substring(1,13); 
          if (tagCheckSum(tempTagString) == true ){ 
              tagString = tempTagString;
          
          }
        }
        tempTagString="";
      }
    }//while()
  
  //reset the reader  
  digitalWrite(RFID_RESET_PIN, LOW);
  delay(150); 
  digitalWrite(RFID_RESET_PIN, HIGH);
  delay(150);
  //

if (tagString != lastTagString){
  // if (tagString != ""){ 
     lastTagString = tagString;
   //   }
    return (tagString);
    
    }else{
    return ("");
   }
}//getTagString()


//////
void saveTagToEeprom(String tag, int startAddress){
for (int i = 0; i < 12; i++){
    EEPROM.write(startAddress + i, tag[i]);
    } 
}//saveTagToEeprom()



//////
String readTagFromEeprom(int startAddress){
String tagString = "";
char c;
  for (int i = 0; i < 12; i++){
      c = EEPROM.read(startAddress + i);  
      tagString = tagString + c;
      } 
return (tagString);      
}//readTagFromEeprom()