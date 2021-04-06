//===============================================
//               funCnt3
//===============================================
void funCnt3(){
     printCnt(240,counter3,0);
   do{
       sel=testTouch();
//============== Backwards ======================      
     if(sel==0)
       menu--;      
//================ Up ===========================   
    if(sel==2)
       if(counter3==255)
           counter3=255;
       else
          counter3++;
 //============= Down ===========================
    if(sel==1)
      if(counter3==0)
       counter3=0;       
      else
       counter3--;
       
       if(sel==1 ||sel==2){
          printCnt(240,counter3,0);
       } 
    }while( menu==2);      
 }//End of funCnt3 function 
//===============================================
//         End Of File " funCnt3"        
//===============================================

