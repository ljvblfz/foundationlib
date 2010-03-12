/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationMsgQ.h 
 *  Description: this file contains api list of message queue operations 
 *  Author: chenwangxian
 *  Create:2010-03-09
 *  Modification history:
 *  2010-03-09, created the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _MSGQ_API_H
#define _MSGQ_API_H


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS 
    #define MSG_QUEUE_ID MSG_Q_ID
#elif VXWORKS_OS
    #define MSG_QUEUE_ID mqd_t
#endif

/*==========================================================
 *				Private task library interface
 * MsgQCreate()     -- create a message queue
 * MsgQDelete()     -- delete a message queue
 * MsgQSend()       -- send a message into msgQ
 * MsgQReceive()    -- receive a message from msgQ
 * MsgQNumMsgs()    -- return current amount in msgQ
 *=========================================================*/


/*
 * =====================================================================
 * Function:MsgQCreate()
 * Description: This routine creates a message queue capable of holding up to maxMsgs messages, 
 *              each up to maxMsgLength bytes long. 
 *              The routine returns a message queue ID used to identify the created message queue 
 *              in all subsequent calls to routines in this library.
 * Input:   mqname --   name of new message queue, just for Linux
 *                      In Linux, the queue is identified by name.
 *          maxMsgs -- max message amount in queue
 *          maxMsgLength -- max size(bytes) of each message  
 * NOTE:    
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
MSG_QUEUE_ID MsgQCreate(const char* mqname, int maxMsgs, int maxMsgLength);

/*
 * =====================================================================
 * Function:MsgQDelete()
 * Description: delete a message queue
 * Input:   mqname -- name of new message queue, just for Linux
 *                    In Linux, the queue is identified by name.
 *          msgQId -- msgQ id
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
int MsgQDelete(const char* mqname, MSG_QUEUE_ID msgQId);

/*
 * =====================================================================
 * Function:MsgQSend()
 * Description: send a message to a message queue
 * Input:   msgQId      -- msgQ id
 *          msgBuffer   -- message to send
 *          msgLength   -- length of the message to be sent
 *          timeTick    -- wait time (in ticks)
 *          msgPriority -- MSG_PRI_NORMAL or MSG_PRI_URGENT
 *              MSG_PRI_NORMAL add the message to the tail of the queue
 *              MSG_PRI_URGENT add the message to the head of the queue
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
int MsgQSend(MSG_QUEUE_ID msgQId, const char* msgBuffer, int msgLength, int timeTick, int msgPriority);

/*
 * =====================================================================
 * Function:MsgQReceive()
 * Description: receive a message from a message queue
 * Input:   msgQId      -- msgQ id
 *          recvBuffer  -- buffer to save message
 *          bufLength   -- length of recvBuffer
 *          timeTick    -- wait time (in ticks)
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
int MsgQReceive(MSG_QUEUE_ID msgQId, char* recvBuffer, int bufLength, int timeTick);

/*
 * =====================================================================
 * Function:MsgQNumMsgs()
 * Description: current amount of messages in message queue
 * Input:   msgQId      -- msgQ id
 * Output:  N/A
 * Return:  message amount on success or ERROR otherwise.
 *======================================================================
 */
int MsgQNumMsgs(MSG_QUEUE_ID msgQId);


#ifdef __cplusplus
}
#endif

#endif

