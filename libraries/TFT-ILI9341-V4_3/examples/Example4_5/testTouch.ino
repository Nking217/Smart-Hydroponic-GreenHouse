//===============================================
//                  testTouch
//===============================================
byte testTouch(){
byte i;
while(digitalRead(2)==1);
lcdtouch.readxy();
x=lcdtouch.readx();
y=lcdtouch.ready();
for(i=0; i<4 ; i++){
 if(x > 10+80*i && x<70+80*i && y>170 && y<170+60){
    tone(speakerPin,toneArr[i]);
    delay(350);
    noTone(speakerPin);
    return i;
    }
   }//End for
 return 0xff; 
}//End of testTouch function
//===============================================
//         End Of File "testTouch"        
//===============================================  

