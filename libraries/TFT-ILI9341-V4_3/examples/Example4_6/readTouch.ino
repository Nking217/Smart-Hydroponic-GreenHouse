//===============================================
//                  readTouch
//===============================================
void readTouch(){
while (digitalRead(2)==1);
    lcdtouch.readxy();
    x = lcdtouch.readx();
    y = lcdtouch.ready();

}//End of readTouch function 
//===============================================
//         End Of File "readTouch"        
//===============================================
 
