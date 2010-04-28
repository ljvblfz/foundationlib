/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationMsgQ.c
 *  Description: private message queue api from linux/vxworks
 *  Author: chenwangxian
 *  Create:2010-03-09
 *  Modification history:
 *  2010-03-09, created the file,         chenwangxian
 *
 *
 **************************************************************************************/ 

#include "foundationPthread.h"
#include "foundationMsgQ.h"
#include "foundationDbg.h"

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
 * NOTE:    Message in queue ordered by the msg_priority. 
 *          High priority added before low ones,the same priority ordered one after one according to fifo.
 *          Tasks waiting for receiving messages if no available, ordered by fifo.
 * Output:  N/A
 * Return:  MsgQId on success or NULL otherwise.
 *======================================================================
 */
MSG_QUEUE_ID MsgQCreate(const char* mqname, int maxMsgs, int maxMsgLength)
{
#ifdef LINUX_OS
    struct mq_attr mqAttr = {0};
    mqd_t* msgQId = AII_NULL;

    if (mqname == AII_NULL)
    {
        return AII_NULL;
    }
    /* allocate memory in MsgQCreate
     * free memory later in MsgQDelete */
    msgQId = (mqd_t*)malloc(sizeof(mqd_t));
    if (msgQId == AII_NULL)
    {
        return AII_NULL;
    }
    
    mqAttr.mq_maxmsg = maxMsgs;
    mqAttr.mq_msgsize = maxMsgLength;
    *msgQId = Mq_open(mqname, O_CREAT|O_EXCL|O_RDWR, 0600, (struct mq_attr*)&mqAttr);
    if (*msgQId == -1)
    {
        free(msgQId);
        return AII_NULL;
    }
    return msgQId;

#elif VXWORKS_OS
    /* create msgQ with MSG_Q_FIFO, pending tasks in fifo order, 
     * the same with  linux posix queue */
    return msgQCreate(maxMsgs, maxMsgLength, MSG_Q_FIFO);
#endif
}

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
int MsgQDelete(const char* mqname, MSG_QUEUE_ID msgQId)
{
#ifdef LINUX_OS
    if (mqname == AII_NULL || msgQId == AII_NULL)
    {
        return (-1);
    }

    free(msgQId);
    msgQId = AII_NULL;
    return Mq_unlink(mqname);

#elif VXWORKS_OS
    if (msgQId == AII_NULL)
    {
        return (-1);
    }
    return msgQDelete(msgQId);
#endif
}

/*
 * =====================================================================
 * Function:MsgQSend()
 * Description: send a message to a message queue
 * Input:   msgQId      -- msgQ id
 *          msgBuffer   -- message to send
 *          msgLength   -- length of the message to be sent
 *          timeTick    -- wait time (in ticks)
 *              WAIT_FOREVER,   this routine block until queue not full
 *              NO_WAIT,        this routine return immediately with errorno
 *              wait time,      this routine block until queue not full or time up
 *          msgPriority -- MSG_PRI_NORMAL or MSG_PRI_URGENT
 *              MSG_PRI_NORMAL(0) add the message to the tail of the queue
 *              MSG_PRI_URGENT(1) add the message to the head of the queue
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
int MsgQSend(MSG_QUEUE_ID msgQId, const char* msgBuffer, int msgLength, int timeTick, int msgPriority)
{
#ifdef LINUX_OS
    if (msgQId == AII_NULL || msgBuffer == AII_NULL)
    {
        return (-1);
    }
    return Mq_send((mqd_t)(*msgQId), msgBuffer, msgLength, timeTick, msgPriority);

#elif VXWORKS_OS
    if (msgQId == AII_NULL || msgBuffer == AII_NULL)
    {
        return (-1);
    }
    return msgQSend(msgQId, msgBuffer, msgLength, timeTick, msgPriority);
#endif
}

/*
 * =====================================================================
 * Function:MsgQReceive()
 * Description: receive a message from a message queue
 * Input:   msgQId      -- msgQ id
 *          recvBuffer  -- buffer to save message
 *          bufLength   -- length of recvBuffer
 *          timeTick    -- wait time (in ticks)
 *              WAIT_FOREVER,   this routine block until message available 
 *              NO_WAIT,        this routine return immediately with errorno
 *              wait time,      this routine block until message available or time up
 * Output:  N/A
 * Return:  OK on success or ERROR otherwise.
 *======================================================================
 */
int MsgQReceive(MSG_QUEUE_ID msgQId, char* recvBuffer, int bufLength, int timeTick)
{
#ifdef LINUX_OS
    if (msgQId == AII_NULL || recvBuffer == AII_NULL)
    {
        return (-1);
    }
    return Mq_receive((mqd_t)(*msgQId), recvBuffer, bufLength, timeTick, AII_NULL);

#elif VXWORKS_OS
    if (msgQId == AII_NULL || recvBuffer == AII_NULL)
    {
        return (-1);
    }
    return msgQReceive(msgQId, recvBuffer, bufLength, timeTick);
#endif
}

/*
 * =====================================================================
 * Function:MsgQNumMsgs()
 * Description: current amount of messages in message queue
 * Input:   msgQId      -- msgQ id
 * Output:  N/A
 * Return:  message amount on success or ERROR otherwise.
 *======================================================================
 */
int MsgQNumMsgs(MSG_QUEUE_ID msgQId)
{
#ifdef LINUX_OS
    struct mq_attr mqAttr = {0};
    int rval;

    if (msgQId == AII_NULL)
    {
        return (-1);
    }
     
    rval = Mq_getattr((mqd_t)(*msgQId), (struct mq_attr*)&mqAttr);
    if (rval != 0)
    {
        debug_info(DEBUG_LEVEL_4, "MsgQNumMsgs() Mq_getattr error!");
        return (-1);
    }
    return mqAttr.mq_curmsgs;
#elif VXWORKS_OS
    if (msgQId == AII_NULL)
    {
        return (-1);
    }
    return msgQNumMsgs(msgQId);
#endif
}

