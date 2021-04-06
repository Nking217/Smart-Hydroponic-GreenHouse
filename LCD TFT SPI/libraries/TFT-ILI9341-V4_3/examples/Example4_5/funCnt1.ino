//===============================================
//               funCnt1
//===============================================
void funCnt1(){
    printCnt(30,counter1,0);
do{
   sel=testTouch();
//============= Down ===========================
   if(sel==1)
     if(counter1==0)
        counter1=0;       
      else
          counter1--;
//=============== Up ===========================   
    if(sel==2)
       if(counter1==255)
           counter1=255;
       else
          counter1++;

//============= Forward ========================             
    if(sel==3)
       menu++;

   if(sel==1 ||sel==2)
         printCnt(30,counter1,0);
    }while( menu==0);      

  }//End of funCnt1 function 
//===============================================
//         End Of File "funCnt1"        
//===============================================

