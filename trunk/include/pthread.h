/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationPthread.h 
 *  Description: this file contains api list of pthread operations 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/
#ifndef _PTHREAD_API_H
#define _PTHREAD_API_H

#include "foundationInclude.h"
#include "foundationDataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NO_WAIT
#define NO_WAIT	        0           /* time method */
#endif

#ifndef WAIT_FOREVER
#define WAIT_FOREVER    (-1)        /* time method */
#endif

#ifndef SEM_EMPTY
#define	SEM_EMPTY		0           /* Sem_init locked */
#endif

#ifndef SEM_FULL
#define	SEM_FULL		1           /* Sem_init unlocked */
#endif

/*==========================================================
 *				POSIX thread library interface
 *=========================================================*/

/*
 * =====================================================================
 * Function:pthreadSpawn()
 * Description: This function create a new pthread.
 * Input:   priority -- priority of new task
 *          stacksize -- stack size
 *          funcptr -- entry of new task
 *          args -- amount of arguments, max to 10
 *          ... -- argument
 * Output:  ptid
 * Return:  0/errno
 *======================================================================
 */
int pthreadSpawn(pthread_t *ptid, int priority, size_t stacksize, void *funcptr, unsigned args, ...);

/*
 * =====================================================================
 * Function:Pthread_self()
 * Description: This function returns the calling thread's ID. 
 * Input:   N/A
 * Output:  N/A
 * Return:  Calling thread's ID. 
 *======================================================================
 */
int Pthread_self(void);

/*
 * =====================================================================
 * Function:Pthread_equal()
 * Description: Tests the equality of the two threads t1 and t2. 
 * Input:   tid1 -- id of pthread 1
 *          tid2 -- id of pthread 2
 * Output:  N/A
 * Return:  Non-zero if t1 and t2 refer to the same thread, otherwise zero.
 *======================================================================
 */
int Pthread_equal(pthread_t tid1, pthread_t tid2);

/*
 * =====================================================================
 * Function:Pthread_cancel()
 * Description: 
   This routine sends a cancellation request to the thread specified by thread. 
   Depending on the settings of that thread, it may ignore the request, 
   terminate immediately or defer termination until it reaches a cancellation point. 

 * IMPLEMENTATION NOTE
   In VxWorks, asynchronous thread cancellation is accomplished using a signal. 
   The signal SIGCNCL has been reserved for this purpose. 
   Applications should take care not to block or handle this signal.
 * Input:   tid -- id of pthread 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code. 
 *          ESRCH  No thread could be found corresponding to that specified by the given thread ID.
 *======================================================================
 */
int Pthread_cancel(pthread_t tid);

/*
 * =====================================================================
 * Function:Pthread_exit()
 * Description: 
   This function terminates the calling thread. 
   All cleanup handlers that have been set for the calling thread with pthread_cleanup_push( )
   are executed in reverse order (the most recently added handler is executed first). 
   Termination functions for thread-specific data are then called for all keys 
   that have non-NULL values associated with them in the calling thread. 
   Finally, execution of the calling thread is stopped. 

    The status argument is the return value of the thread 
    and can be consulted from another thread using pthread_join( ) 
    unless this thread was detached 
    (i.e. a call to pthread_detach( ) had been made for it, or it was created in the detached state). 

    All threads that remain joinable at the time they exit should ensure 
    that pthread_join( ) is called on their behalf by another thread to reclaim the resources that they hold. 
 *
 * Input:   value -- exit status value
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
void Pthread_exit(void *value);

/*
 * =====================================================================
 * Function:Pthread_join()
 * Description: 
       The  pthread_join()  function shall suspend execution of the calling thread until the target thread terminates,
       unless the target thread has already terminated. On return from a successful pthread_join() call  with  a  non-
       NULL  value_ptr  argument, the value passed to pthread_exit() by the terminating thread shall be made available
       in the location referenced by value_ptr. When a pthread_join() returns successfully, the target thread has been
       terminated.  The results of multiple simultaneous calls to pthread_join() specifying the same target thread are
       undefined. If the thread calling pthread_join() is canceled, then the target thread shall not be detached.

       It is unspecified whether a thread that has exited but remains unjoined counts against {PTHREAD_THREADS_MAX}.
 * Input:   thread -- pid of thread
 * Output:  
 *          value -- exit status of thread
 * Return:  On success zero; on failure a non-zero error code.
            EINVAL The  implementation has detected that the value specified by thread does not refer to a joinable thread.
            ESRCH  No thread could be found corresponding to that specified by the given thread ID.
            EDEADLK A deadlock was detected or the value of thread specifies the calling thread.
 *======================================================================
 */
int Pthread_join(pthread_t thread, void **value);

/* 
 * Pthread_cleanup_push() and Pthread_cleanup_pop() must to be used together in the same pthread
 */
/*
 * =====================================================================
 * Function:Pthread_cleanup_push()
 * Description: pushes a routine onto the cleanup stack (POSIX)
 *              This routine pushes the specified cancellation cleanup handler routine, routine, 
 *              onto the cancellation cleanup stack of the calling thread. 
 *              When a thread exits and its cancellation cleanup stack is not empty, 
 *              the cleanup handlers are invoked with the argument arg in LIFO order from the cancellation cleanup stack. 
 * Input:   routine --  cleanup routine 
 *          args -- argument
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
#define Pthread_cleanup_push(routine, args) pthread_cleanup_push(void (*routine)(void*), void *args) 
/*
 * =====================================================================
 * Function:Pthread_cleanup_pop()
 * Description: pop a cleanup routine off the top of the stack (POSIX)
                This routine removes the cleanup handler routine at the top of the cancellation 
                cleanup stack of the calling thread and executes it if run is non-zero. 
                The routine should have been added using the pthread_cleanup_push( ) function. 

                Once the routine is removed from the stack it will no longer be called when the thread exits. 
 * Input:   execute -- execute handler
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
#define Pthread_cleanup_pop(execute) pthread_cleanup_pop(int execute)


/*
 * =====================================================================
 * Function:pthreadIdVerify()
 * Description: verify the existence of a pthread 
 *              This routine verifies the existence of a specified pthread by validating the pthread ID. 
 *              Note that an exception occurs if the ID parameter points to an address not located in physical memory. 
 * Input:   tid -- id of pthread 
 * Output:  N/A
 * Return:  returns 0 if pthread is valid, errno if invalid
 *======================================================================
 */
int pthreadIdVerify(pthread_t tid);

/*
 * =====================================================================
 * Function:Pthread_attr_set()
 * Description: set thread at priority and stacksize
 * Input:   priority
 *          stacksize
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_set(pthread_attr_t *attr, int priority, size_t stacksize);
/*
 * =====================================================================
 * Function:Pthread_attr_init()
 * Description: initialize thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_init(pthread_attr_t *attr);

/*
 * =====================================================================
 * Function:Pthread_attr_destroy()
 * Description: destroy a thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_destroy(pthread_attr_t *attr);

/*
 * =====================================================================
 * Function:Pthread_attr_getdetachstate()
 * Description: get value of detachstate attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  detachstate -- current detach state
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

/*
 * =====================================================================
 * Function:Pthread_attr_setdetachstate()
 * Description: set value of detachstate attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          detachstate -- new detach state
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);

/*
 * =====================================================================
 * Function:Pthread_attr_getschedpolicy()
 * Description: get schedpolicy attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  policy -- current policy 
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);

/*
 * =====================================================================
 * Function:Pthread_attr_setschedpolicy()
 * Description: set schedpolicy attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          policy -- new policy 
 * POSIX defines the following policies: 
 *          SCHED_RR Realtime, round-robin scheduling. 
 *          SCHED_FIFO Realtime, first-in first-out scheduling. 
 *          SCHED_OTHER Other, non-realtime scheduling. 
 *          VxWorks only supports SCHED_RR and SCHED_FIFO. 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);

/*
 * =====================================================================
 * Function:Pthread_attr_getschedparam()
 * Description: get value of schedparam attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  param -- current policy 
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param * param);

/*
 * =====================================================================
 * Function:Pthread_attr_setschedparam()
 * Description: set value of schedparam attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          param -- new param
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);

/*
 * =====================================================================
 * Function:Pthread_attr_getinheritsched()
 * Description: get current value if inheritsched attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  inheritsched -- current inheritsched
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);

/*
 * =====================================================================
 * Function:Pthread_attr_setinheritsched()
 * Description: set current value if inheritsched attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          inheritsched -- new inheritsched
 * Possible values are: 
 *          PTHREAD_INHERIT_SCHED, Inherit scheduling parameters from parent thread. 
 *          PTHREAD_EXPLICIT_SCHED, Use explicitly provided scheduling parameters 
 *                                  (i.e. those specified in the thread attributes object). 
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);

/*
 * =====================================================================
 * Function:Pthread_attr_getschedpriority()
 * Description: get current value if priority attribute from thread attributes object
 * Input:   attr -- thread attributes
 * Output:  priority -- current priority
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getschedpriority(const pthread_attr_t *attr, int *priority);

/*
 * =====================================================================
 * Function:Pthread_attr_setschedpriority()
 * Description: set current value if priority attribute in thread attributes object
 * Input:   attr -- thread attributes
 *          priority -- new priority
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setschedpriority(pthread_attr_t *attr, int priority);

/*
 * =====================================================================
 * Function:Pthread_attr_setscope()
 * Description: get contention scope from thread attributes (POSIX)
 * Input:   attr -- thread attributes
 * Output:  scope -- current contention scope
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getscope(const pthread_attr_t *attr, int *scope);

/*
 * =====================================================================
 * Function:Pthread_attr_setscope()
 * Description: set contention scope in thread attributes (POSIX)
 * Input:   attr -- thread attributes
 *          scope -- current contention scope
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setscope(pthread_attr_t *attr, int scope);

/*
 * =====================================================================
 * Function:Pthread_attr_getstacksize()
 * Description: get stack value of stacksize attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  stacksize -- current stack size
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

/*
 * =====================================================================
 * Function:Pthread_attr_setstacksize()
 * Description: set stack value of stacksize attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          stacksize -- new stack size
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/*
 * =====================================================================
 * Function:Pthread_attr_getstackaddr()
 * Description: get value of stackaddr attribute from thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 * Output:  stackaddr -- current stack address 
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);

/*
 * =====================================================================
 * Function:Pthread_attr_setstackaddr()
 * Description: set value of stackaddr attribute in thread attributes object (POSIX)
 * Input:   attr -- thread attributes
 *          stackaddr -- new stack address 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);

/*
 * =====================================================================
 * Function:Pthread_attr_getguardsize()
 * Description: get value of guardsize attribute from thread attributes object (POSIX)
 *              Only support by Linux!
 * Input:   attr -- thread attributes
 * Output:  guardsize -- current guardsize  
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);

/*
 * =====================================================================
 * Function:Pthread_attr_getguardsize()
 * Description: set value of guardsize attribute from thread attributes object (POSIX)
 *              Only support by Linux!
 * Input:   attr -- thread attributes
 *          guardsize -- new guardsize  
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);


/*==========================================================
 *				POSIX scheduling library interface
 *=========================================================*/

/*
 * =====================================================================
 * Function:Sched_get_priority_min()
 * Description: get the minimum priority (POSIX)
 * Input:   policy -- scheduling policy
 * Output:  N/A
 * Return:  Minimum priority value, or -1 (ERROR) on error. 
 *======================================================================
 */
int Sched_get_priority_min(int policy);

/*
 * =====================================================================
 * Function:Sched_get_priority_max()
 * Description: get the maximum priority (POSIX)
 * Input:   policy -- scheduling policy
 * Output:  N/A
 * Return:  Maximum priority value, or -1 (ERROR) on error. 
 *======================================================================
 */
int Sched_get_priority_max(int policy);

/*
 * =====================================================================
 * Function:Sched_getscheduler()
 * Description: get the current scheduling policy (POSIX)
 * Input:   pid -- pthread id
 * Output:  N/A
 * Return:  This routine returns the currents scheduling policy (i.e., SCHED_FIFO or SCHED_RR). 
 *======================================================================
 */
int Sched_getscheduler(pid_t pid);

/*
 * =====================================================================
 * Function:Sched_setscheduler()
 * Description: set scheduling policy and scheduling parameters (POSIX)
 *              This routine sets the scheduling policy and scheduling parameters for a specified task, tid. 
 *              If tid is 0, it sets the scheduling policy and scheduling parameters for the calling task. 
 * Input:   pid -- pthread id
 *          policy -- scheduling policy requested
 *          param -- scheduling parameters requested
 * NOTE:    Because VxWorks does not set scheduling policies (e.g., round-robin scheduling) on a task-by-task basis, 
 *          setting a scheduling policy that conflicts with the current system policy simply fails and errno is set to EINVAL. 
 *          If the requested scheduling policy is the same as the current system policy, then this routine acts just like sched_setparam( ). 
 * Output:  N/A
 * Return:  This routine returns the currents scheduling policy (i.e., SCHED_FIFO or SCHED_RR). 
 *======================================================================
 */
int Sched_setscheduler(pid_t pid, int policy, struct sched_param *param);

/*
 * =====================================================================
 * Function:Sched_getparam()
 * Description: get the scheduling parameters for a specified task (POSIX)
 * Input:   pid -- pthread id
 * Output:  param -- current scheduling param
 * Return:  0 (OK) if successful, or -1 (ERROR) on error. 
 *======================================================================
 */
int Sched_getparam(pid_t pid, struct sched_param *param);

/*
 * =====================================================================
 * Function:Sched_setparam()
 * Description: set the scheduling parameters for a specified task (POSIX)
 * This routine sets the priority of a specified task, tid. If tid is 0, it sets the priority of the calling task.
 * Input:   pid -- pthread id
 *          param -- current scheduling param
 * Output:  N/A
 * Return:  0 (OK) if successful, or -1 (ERROR) on error. 
 *======================================================================
 */
int Sched_setparam(pid_t pid, struct sched_param *param);

/*
 * =====================================================================
 * Function:Sched_rr_get_interval()
 * Description: get the current time slice (POSIX)
 *              This routine get current time slice period if round-robin scheduling is currently enabled.
 * Input:   pid -- pthread id
 * Output:  tp -- current time slice
 * Return:  0 (OK) if successful, or -1 (ERROR) on error. 
 *======================================================================
 */
int Sched_rr_get_interval(pid_t pid, struct timespec *tp);


/*==========================================================
 *					POSIX thread synchronism
 *=========================================================*/

/*
 * =====================================================================
 * Function:Pthread_mutex_init()
 * Description: initialize mutex from attributes object (POSIX)
 *              This routine called 
 *              int pthread_mutex_init (
 *                                      pthread_mutex_t *           pMutex, 
 *                                      const pthread_mutexattr_t * pAttr  
 *                                      )
 *              pAttr is NULL, default attributes are used as defined in the POSIX specification. 
 * Input:   mutex
 * Output:  mutex
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_mutex_init(pthread_mutex_t *mutex);

/*
 * =====================================================================
 * Function:Pthread_mutex_destroy()
 * Description: destroy a mutex (POSIX)
 * Input:   mutex
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_mutex_destroy(pthread_mutex_t *mutex);

/*
 * =====================================================================
 * Function:Pthread_mutex_lock()
 * Description: lock a mutex (POSIX)
 * Input:   mutex -- pmutex
 *          wait_ms -- wait time
 * This argument has three selections:
 *          WAIT_FOREVER,   lock mutex if it is unlocked,
 *                          wait forever util mutex unlock by any thread if it is locked.
 *          NO_WAIT,        lock mutex if it is unlocked, 
 *                          return immediately with error code if it is locked.
 *          wait time,      lock mutex if it is unlocked,
 *                          wait util time out if it is locked.
 *                          This is just support in Linux!
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_mutex_lock(pthread_mutex_t *mutex, int wait_ms);

/*
 * =====================================================================
 * Function:Pthread_mutex_unlock()
 * Description: unlock a mutex (POSIX)
 * Input:   mutex
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_mutex_unlock(pthread_mutex_t *mutex);

/*
 * =====================================================================
 * Function:Sem_init()
 * Description: initialize an unnamed semaphore (POSIX)
 *              This routine called 
 *              int sem_init (
 *                            sem_t *      sem,          semaphore to be initialized 
 *                            int          pshared,      process sharing 
 *                            unsigned int value         semaphore initialization value 
 *                            )
 *              In Linux, pshared is 0, sem can be shared between pthreads.
 *              In Vxworks, pshared invalid now.
 * Input:   sem -- semaphore to be initialized 
 *          value -- semaphore initialization value
 * This argument has two selections:
 *          SEM_EMPTY(0),   init state is locked		
 *          SEM_FULL(1)	,   init state is unlocked
 * Output:  N/A
 * Return:  0 (OK), or -1 (ERROR) if unsuccessful. 
 *======================================================================
 */
int Sem_init(sem_t *sem, unsigned value);

/*
 * =====================================================================
 * Function:Sem_destroy()
 * Description: destroy an unnamed semaphore (POSIX)
 * Input:   sem -- semaphore descriptor
 * Output:  N/A
 * Return:  0 (OK), or -1 (ERROR) if unsuccessful.
 *======================================================================
 */
int Sem_destroy(sem_t *sem);

/*
 * =====================================================================
 * Function:Sem_post()
 * Description: unlock (give) a semaphore (POSIX)
 * Input:   sem -- semaphore descriptor
 * Output:  N/A
 * Return:  0 (OK), or -1 (ERROR) if unsuccessful.
 *======================================================================
 */
int Sem_post(sem_t *sem);

/*
 * =====================================================================
 * Function:Sem_wait()
 * Description: lock (take) a semaphore, blocking if not available (POSIX)
 *              If the semaphore’s value is greater than zero, then the decrement proceeds, 
 *              and the function returns, immediately.
 *              If the semaphore currently has  the  value zero, 
 *              then the call blocks until either it becomes possible to perform the decrement 
 *              (i.e., the semaphore value rises above zero), or a signal handler interrupts the call.
 * Input:   sem -- psem
 *          wait_ms -- wait time
 * This argument has three selections:
 *          WAIT_FOREVER,   lock sem if it is unlocked,
 *                          wait forever util sem unlock by any thread if it is locked.
 *          NO_WAIT,        lock sem if it is unlocked, 
 *                          return immediately with error code if it is locked.
 *          wait time,      lock sem if it is unlocked,
 *                          wait util time out if it is locked.
 *                          This is just support in Linux!
 * Output:  N/A
 * Return:  0 (OK), or -1 (ERROR) if unsuccessful. 
 *======================================================================
 */
int Sem_wait(sem_t *sem, int wait_ms);

//读写锁(注意:pthread_rwlock_t 必须加上编译选择选项-D_GNU_SOURCE!!!)
/*
 * =====================================================================
 * Function:Pthread_rwlock_init()
 * Description: initialize rwlock from attributes object (POSIX)
 * This routine called int pthread_rwlock_init(
 *                          pthread_rwlock_t *rwlock,
 *                          const pthread_rwlockattr_t *pAttr);
 * pAttr is NULL, default attributes are used as defined in the POSIX specification. 
 * Input:   rwlock
 * Output:  rwlock
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_rwlock_init(pthread_rwlock_t *rwlock);

/*
 * =====================================================================
 * Function:Pthread_rwlock_destroy()
 * Description: destroy the rwlock (POSIX)
 * Input:   rwlock
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

/*
 * =====================================================================
 * Function:Pthread_rwlock_rdlock()
 * Description: apply a read lock to the read-write lock(POSIX)
 *              The calling thread acquires the read lock if a writer does not hold the lock. 
 *              It acquires the read lock even the read-lock has been locked.
 *              This is just supported in Linux!
 * Input:   rwlock -- prwlock
 *          wait_ms -- wait time
 * This argument has three selections:
 *          WAIT_FOREVER,   lock read-lock if its write-lock is unlocked,
 *                          wait forever util write-lock unlock if its write-lock is locked.
 *          NO_WAIT,        lock read-lock if its write-lock is unlocked, 
 *                          return immediately with error code if its write-lock is locked.
 *          wait time,      lock read-lock if its write-lock is unlocked,
 *                          wait util time out if its write-lock is locked.
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock, int wait_ms);

/*
 * =====================================================================
 * Function:Pthread_rwlock_wrlock()
 * Description: 
 *              apply a write lock to the read-write lock referenced by rwlock. 
 *              The calling thread acquires the write lock if no other thread (reader or writer) 
 *              holds the read-write lock  rwlock.  Otherwise, the thread shall block until it can 
 *              acquire the lock. The calling thread may deadlock if at the time the call is made 
 *              it holds the read-write lock (whether a read or write lock).
 * Input:   rwlock -- prwlock
 *          wait_ms -- wait time
 * This argument has three selections:
 *          WAIT_FOREVER,   lock write-lock if all the two lock is unlocked,
 *                          wait forever util all the two lock unlock if any of read-lock or write-lock is locked.
 *          NO_WAIT,        lock write-lock if all the two lock is unlocked, 
 *                          return immediately with error code if any of read-lock or write-lock is locked.
 *          wait time,      lock write-lock if all the two lock is unlocked,
 *                          wait util time out if any of read-lock or write-lock is locked.
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock, int wait_ms);

/*
 * =====================================================================
 * Function:Pthread_rwlock_unlock()
 * Description: unlock a read-write lock object
                This rountine release  a lock held on the read-write lock object referenced by rwlock.
                It unlocks write-lock once but unlocks read-lock as decrease the counter.  
                If this function is called to release a read lock from the read-write lock object  and  
                there  are  other  read locks  currently  held  on  this  read-write lock object, the 
                read-write lock object remains in the read locked state. If this function releases 
                the last read lock for this read-write lock object, the read-write lock object shall 
                be put in the unlocked state with no owners.
 * Input:   rwlock
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

/*
 * =====================================================================
 * Function:Pthread_cond_init()
 * Description: initialize condition variable (POSIX)
 *              This routine called 
 *              int pthread_cond_init (
 *                                      pthread_cond_t *     pCond,  condition variable 
 *                                      pthread_condattr_t * pAttr   condition variable attributes 
 *                                     )
 *              pAttr is NULL, default attributes are used as defined in the POSIX specification. 
 * Input:   cond 
 * Output:  cond
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_init(pthread_cond_t *cond);

/*
 * =====================================================================
 * Function:Pthread_cond_destroy()
 * Description: destroy a condition variable (POSIX)
 *              If there are threads waiting on the condition variable, 
 *              then pthread_cond_destroy( ) returns EBUSY. 
 * Input:   cond 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_destroy(pthread_cond_t *cond);

/*
 * =====================================================================
 * Function:Pthread_cond_wait()
 * Description: wait on a condition
 *              These functions atomically release mutex and cause the calling thread 
 *              to block on the condition variable  cond.Firstly, it realese mutex;
 *              Then, wait for the condition until the condition actived; Before return,
 *              It lock the mutex.
 * Input:   cond -- condition variable
 *          mutex -- POSIX mutex
 * NOTE:    The mutex must be locked by the calling thread when pthread_cond_wait( ) is called; 
 *          if it is not then this function returns an error (EINVAL).
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/*
 * =====================================================================
 * Function:Pthread_cond_timedwait()
 * Description: wait for a condition variable with a timeout (POSIX)
 *              These functions atomically release mutex and cause the calling thread 
 *              to block on the condition variable  cond.Firstly, it realese mutex;
 *              Then, wait for the condition until the condition actived or time out; 
 *              Before return, It lock the mutex.
 * Input:   cond -- condition variable
 *          mutex -- POSIX mutex
 *          wait_ms -- timeout time
 * This argument has two selections:
 *          NO_WAIT,        pthread_cond_timedwait() return at once without waiting condition
 *          wait time,      pthread_cond_timedwait() wait for condition active for a while(wait_ms)
 * NOTE:    The mutex must be locked by the calling thread when pthread_cond_wait( ) is called; 
 *          if it is not then this function returns an error (EINVAL).
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, int wait_ms);

/*
 * =====================================================================
 * Function:Pthread_cond_signal()
 * Description: unblock a condition, active a waiting thread (POSIX)
 *              If no threads are waiting on the condition variable then this routine does nothing; 
 *              if more than one thread is waiting, then one will be released, but it is not specified which one.
 * Input:   cond 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_signal(pthread_cond_t *cond);

/*
 * =====================================================================
 * Function:Pthread_cond_broadcast()
 * Description: unblock a condition, active all waiting threads (POSIX)
 *              If no threads are waiting on the condition variable then this routine does nothing; 
 * Input:   cond 
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Pthread_cond_broadcast(pthread_cond_t *cond);


/*==========================================================
 *				POSIX message queue interface
 *=========================================================*/

/*
 * =====================================================================
 * Function:Mq_open()
 * Description: Creates a new POSIX message queue or opens an existing queue.  
 *              The queue is identified by name.
 * Input:   name -- name of queue to open/create
 *          oflag -- open flags, O_RDWR|O_CREAT_OEXCL as default.
 * Here are three flag bits, MUST only one of them can be set:
 *             O_RDONLY 
 *                 Open the message queue for receiving messages. 
 *                 The task can use the returned message queue descriptor with mq_receive( ), but not mq_send( ). 
 *             O_WRONLY 
 *                 Open the message queue for sending messages. 
 *                 The task can use the returned message queue descriptor with mq_send( ), but not mq_receive( ). 
 *             O_RDWR 
 *                 Open the queue for both receiving and sending messages. 
 *                 The task can use any of the functions allowed for O_RDONLY and O_WRONLY. 
 * Here are three combination flag bits:
 *             O_NONBLOCK
 *                Open  the  queue in non-blocking mode.  In circumstances where mq_receive() and mq_send() would normally
 *                block, these functions instead fail with the error EAGAIN.
 *             O_CREAT
 *                Create the message queue if it does not exist.  The owner (user ID) of the message queue is set  to  the
 *                effective  user ID of the calling process.  The group ownership (group ID) is set to the effective group
 *                ID of the calling process.
 *             O_EXCL 
 *                If O_CREAT was specified in oflag, and a queue with the given name already exists, then  fail  with  the
 *                error EEXIST.
 *          mode -- file permission mode, 0600 as default,
 *                  only usable while creating.
 *          attr -- attribution 
 * Output:  N/A
 * Return:  A message queue descriptor, otherwise -1 (ERROR).
 *======================================================================
 */
mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

/*
 * =====================================================================
 * Function:Mq_send()
 * Description: send a message to a message queue (POSIX)
 * Input:   mqdes -- message queue descriptor
 *          msg_ptr -- message to send
 *          meg_len -- size of message, in bytes
 *          wait_ms
 * This argument has three selections:
 *              WAIT_FOREVER,   wait forever if the queue is full
 *              NO_WAIT,        return at once if the queue if full
 *              wait time,      return after time expire if the queue is full
 *          msg_prio -- priority of message, 0(low)-31(high) in Posix.
 *              high priority message will insert before low ones into queue,
 *              the same priority will insert after the same priority ones.
 * Output:  N/A
 * Return:  On success zero; on failure a non-zero error code.
 *======================================================================
 */
int Mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, int wait_ms, unsigned msg_prio);

/*
 * =====================================================================
 * Function:Mq_receive()
 * Description: receive a message from a message queue (POSIX)
 * Input:   mqdes -- message queue descriptor
 *          msg_ptr -- receive buffer to save message
 *          meg_len -- size of buffer in bytes
 *          wait_ms
 * This argument has three selections:
 *              WAIT_FOREVER,   wait forever if the queue is full
 *              NO_WAIT,        return at once if the queue if full
 *              wait time,      return after time expire if the queue is full
 *          *msg_prio -- restore priority of received message
 * NOTE:    while tasks blocking at waiting for message, it according to fifo,
 *          it means who calls mq_receive first, who get message first.
 * Output:  N/A
 * Return:  The length of the selected message in bytes, otherwise -1 (ERROR).
 *======================================================================
 */
ssize_t Mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, int wait_ms, unsigned *msg_prio);

/*
 * =====================================================================
 * Function:Mq_getattr()
 * Description: get message queue attributes (POSIX)
 * Input:   mqdes -- message queue descriptor
 *          mqstat -- buffer in which to return attributes
             struct mq_attr 
             {
                 long mq_flags;        Flags: 0 or O_NONBLOCK 
                 long mq_maxmsg;       Max. # of messages on queue 
                 long mq_msgsize;      Max. message size (bytes) 
                 long mq_curmsgs;      # of messages currently in queue 
             }
 * Output:  N/A
 * Return:  0 (OK) if message attributes can be determined, otherwise -1 (ERROR).
 *======================================================================
 */
int Mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);

/*
 * =====================================================================
 * Function:Mq_setattr()
 * Description: set message queue attributes (POSIX),Only mq_flags can be set!
 * Input:   mqdes -- message queue descriptor
 *          mqstat -- buffer in which to return attributes
             struct mq_attr 
             {
                 long mq_flags;        Flags: 0 or O_NONBLOCK 
                 long mq_maxmsg;       Max. # of messages on queue 
                 long mq_msgsize;      Max. message size (bytes) 
                 long mq_curmsgs;      # of messages currently in queue 
             }
            The only attribute that can be modified is the setting of the O_NONBLOCK flag in mq_flags.  
            The other fields  in newattr are ignored. 
 * Output:  N/A
 * Return:  0 (OK) if message attributes can be determined, otherwise -1 (ERROR).
 *======================================================================
 */
int Mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat);

/*
 * =====================================================================
 * Function:Mq_unlink()
 * Description: remove a message queue (POSIX)
 * Input:   name -- name of message queue
 * Output:  N/A
 * Return:  0 (OK) if the message queue is unlinked successfully, otherwise -1 (ERROR). 
 *======================================================================
 */
int Mq_unlink(const char *name);

/*
 * =====================================================================
 * Function:Mq_close()
 * Description: close a message queue (POSIX)
 * Input:   mqdes -- message queue descriptor
 * Output:  N/A
 * Return:  0 (OK) if the message queue is closed successfully, otherwise -1 (ERROR).
 *======================================================================
 */
int Mq_close(mqd_t mqdes);

#ifdef __cplusplus
}
#endif

#endif


