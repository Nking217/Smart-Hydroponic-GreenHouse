//===============================================
//               funCnt2
//===============================================
void funCnt2(){
     printCnt(135,counter2,0);
   do{
        sel=testTouch();
 //============= Backwards ======================      
     if(sel==0)
       menu--;   
//=============== Up ===========================   
    if(sel==2)
       if(counter2==255)
           counter2=255;
       else
          counter2++;
 //============= Down ===========================
    if(sel==1)
      if(counter2==0)
       counter2=0;       
      else
       counter2--;
//============== Forward ========================             
    if(sel==3)
       menu++;
       if(sel==1 ||sel==2)
           printCnt(135,counter2,0);
   }while( menu==1);      
 }//End of funCnt2 function 
//===============================================
//         End Of File " funCnt2"        
//===============================================


