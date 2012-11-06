/**
 *	@file main.h
 *  @brief Test
 *  @date 05/18/2012
 *  @author maxime.maitre@parrot.com
**/

#include <stdio.h> //sup
#include <stdlib.h> 
#include <libSAL/print.h>
#include <libSAL/thread.h>

#include <string.h>

//#include <libSAL/mutex.h>
#include <libNetWork/common.h>//!! modif
#include <libNetWork/inOutBuffer.h>
#include <libNetWork/sender.h>
#include <libNetWork/receiver.h>
#include <libNetWork/netWork.h>
#include <libSAL/socket.h>

#include <unistd.h>

#include <netWorkDef.h>

int main(int argc, char *argv[])
{
	
	netWork_t* pNetWork1= NULL;
	
	int cmdType = -1;
	char chData = 0;
	
	int intData = 0;
	
	netWork_inOutBuffer_t* pInOutTemp = NULL;
	
	netWork_paramNewInOutBuffer_t paramNetWork1[4];
	
	//--- netWork 1 ---
	
	
	
	// input ID_CHAR_DATA int
	paramNetWork1[0].id = ID_CHAR_DATA;
	paramNetWork1[0].dataType = CMD_TYPE_DATA;
	paramNetWork1[0].sendingWaitTime = 3;
	paramNetWork1[0].ackTimeoutMs = 10;//not used
	paramNetWork1[0].nbOfRetry = 5;//not used
	paramNetWork1[0].buffSize = 1;
	paramNetWork1[0].buffCellSize = sizeof(char);
	paramNetWork1[0].overwriting = 1;
	
	// input ID_INT_DATA_WITH_ACK char
	paramNetWork1[1].id = ID_INT_DATA_WITH_ACK;
	paramNetWork1[1].dataType = CMD_TYPE_DATA_WITH_ACK;
	paramNetWork1[1].sendingWaitTime = 2;
	paramNetWork1[1].ackTimeoutMs = 10;
	paramNetWork1[1].nbOfRetry = 5;
	paramNetWork1[1].buffSize = 5;
	paramNetWork1[1].buffCellSize = sizeof(int);
	paramNetWork1[1].overwriting = 0;
	
	// input ID_KEEP_ALIVE char
	paramNetWork1[2].id = ID_KEEP_ALIVE;
	paramNetWork1[2].dataType = CMD_TYPE_KEEP_ALIVE;
	paramNetWork1[2].sendingWaitTime = 100;
	paramNetWork1[2].ackTimeoutMs = 10;//not used
	paramNetWork1[2].nbOfRetry = 5;//not used
	paramNetWork1[2].buffSize = 1;
	paramNetWork1[2].buffCellSize = sizeof(int);
	paramNetWork1[2].overwriting = 1;
	
	// output ID_INT_DATA int
	paramNetWork1[3].id = ID_INT_DATA;
	paramNetWork1[3].dataType = CMD_TYPE_DATA;
	paramNetWork1[3].sendingWaitTime = 3;//not used
	paramNetWork1[3].ackTimeoutMs = 10;//not used
	paramNetWork1[3].nbOfRetry = 5;//not used
	paramNetWork1[3].buffSize = 10;
	paramNetWork1[3].buffCellSize = sizeof(int);
	paramNetWork1[3].overwriting = 1;		
					
	pNetWork1 = newNetWork( 256, 256, 1, 3,
							paramNetWork1[3],
							paramNetWork1[0], paramNetWork1[1], paramNetWork1[2]);
												
	sal_print( PRINT_WARNING," ------pNetWork1->pSender connect error: %d \n", 
								senderConnection(pNetWork1->pSender,"127.0.0.1", 5551) );
								
	sal_print( PRINT_WARNING," ------pNetWork1->pReceiver Bind  error: %d \n", 
								receiverBind(pNetWork1->pReceiver, 5552, 10) );

	sal_thread_t thread_send1;
	sal_thread_t thread_recv1;
	
	
	sal_print(PRINT_WARNING,"main start \n");

	sal_thread_create(&(thread_recv1), (sal_thread_routine) runReceivingThread, pNetWork1->pReceiver);

	usleep(50000);
	
	sal_thread_create(&thread_send1, (sal_thread_routine) runSendingThread, pNetWork1->pSender);
	
	
	
	while(cmdType != 0)
	{
		
		printf("	1 - send char \n \
	2 - send int ack \n \
	0 - quit \n");
	
		scanf("%d",&cmdType);
		
		while ( ((chData = getchar()) != '\n') && chData != EOF)
		{
		};
		
		switch(cmdType)
		{
			case 0 :
				cmdType = 0; 
			break;
			
			case 1 :
			
				printf("char data ? ");
				scanf("%c", &chData);
				printf("\n ");
				
				pInOutTemp = inOutBufferWithId(	pNetWork1->ppTabInput, pNetWork1->numOfInput,
												ID_CHAR_DATA);
				ringBuffPushBack(pInOutTemp->pBuffer, &chData);
				
			break;
			
			case 2 :
				printf("int data ?");
				scanf("%d", &intData);
				printf("\n ");
				
				pInOutTemp = inOutBufferWithId(	pNetWork1->ppTabInput,
										pNetWork1->numOfInput, ID_INT_DATA_WITH_ACK);
										
				ringBuffPushBack(pInOutTemp->pBuffer, &intData);
			break;
				
			default:
				cmdType = -1;
			break;
		}
	}
	
	sal_print(PRINT_WARNING," #### stop thread:\n");
	
	//stop all therad
	stopSender(pNetWork1->pSender);
	stopReceiver(pNetWork1->pReceiver);
	
	//kill all thread
	
	sal_thread_join(&(thread_send1), NULL);
	sal_thread_join(&(thread_recv1), NULL);

	//delete
	deleteNetWork( &pNetWork1 );
	
	sal_print(PRINT_WARNING,"fin\n");

}
