/* Example for sending and receiving data through PCIe*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "fpga.h"


#define GETTIME(t) gettimeofday(&t, NULL)
#define GETUSEC(e,s) ((e.tv_sec*1000000)+e.tv_usec) - ((s.tv_sec*1000000)+s.tv_usec)

struct timeval start,end;


int main(){
   unsigned char * send_data;
   unsigned char * recv_data;
   int data_size = 16*1024*1024;
   long usecs;
   int i;

   send_data = (char *)malloc(data_size*sizeof(char));
   recv_data = (char *)malloc(data_size*sizeof(char));
   
   for(i=0; i<data_size; i++)
	send_data[i] = i;

   while(1){

       for(i=0; i<data_size; i++)
           recv_data[i] = 0;

       GETTIME(start);
       //Minimum send size is 64 bytes
       dyract_send_data((unsigned char *)send_data,data_size,0x00000000);
       GETTIME(end);
       usecs = GETUSEC(end,start);
       printf("Send Throughput %f:MB/s\n",(double)(data_size*1000000.0)/(1024*1024*usecs));

       GETTIME(start);

       //Minimum receive size is 64 bytes
       dyract_recv_data((char *)recv_data,data_size,0x00000000);
       GETTIME(end);
       usecs = GETUSEC(end,start);
      
       printf("Receive Throughput %f:MB/s\n",(double)(data_size*1000000.0)/(1024*1024*usecs));

       for(i=0; i<data_size; i++) {
           if(send_data[i] != recv_data[i]){
	        printf("Data mismatch @ %0x\n Expected %0x Received %0x\n", 0x00000000 + i , send_data[i], recv_data[i]);
		break;
            }
       }

       //sleep(1);
   }

   return 0;
}
