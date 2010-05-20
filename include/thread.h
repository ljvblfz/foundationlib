/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: thread.h
 *  Description: thread api from linux/vxworks/win32
 *  Author: Ben Shaw
 *  Create:2010-05-17
 *  Modification history:
 *  2010-05-17, created the file,         Ben Shaw
 *
 *
 **************************************************************************************/ 
#include "libtype.h"

#define THREAD_NAME_LEN 33

typedef void* THREAD_HANDLE;

/*
     Linux schedule policy
     
     Scheduling Policies
     The scheduler is the kernel part that decides  which  runnable  process
     will be executed by the CPU next. The Linux scheduler offers three dif-
     ferent scheduling policies, one for normal processes and two for  real-
     time  applications.  A static priority value sched_priority is assigned
     to each process and this value can be changed only  via  system  calls.
     Conceptually,  the scheduler maintains a list of runnable processes for
     each possible sched_priority value, and sched_priority can have a value
     in the range 0 to 99. In order to determine the process that runs next,
     the Linux scheduler looks for  the  non-empty  list  with  the  highest
     static  priority  and  takes  the process at the head of this list. The
     scheduling policy  determines  for  each  process,  where  it  will  be
     inserted  into the list of processes with equal static priority and how
     it will move inside this list.

     SCHED_OTHER is the default universal time-sharing scheduler policy used
     by most processes.  SCHED_BATCH is intended for "batch" style execution
     of processes.  SCHED_FIFO and SCHED_RR are intended for  special  time-
     critical  applications  that need precise control over the way in which
     runnable processes are selected for execution.

     Processes scheduled with SCHED_OTHER or SCHED_BATCH  must  be  assigned
     the  static  priority  0.   Processes  scheduled  under  SCHED_FIFO  or
     SCHED_RR can have a static priority in the range 1 to 99.   The  system
     calls sched_get_priority_min() and sched_get_priority_max() can be used
     to find out the valid priority range  for  a  scheduling  policy  in  a
     portable way on all POSIX.1-2001 conforming systems.

     All  scheduling is preemptive: If a process with a higher static prior-
     ity gets ready to run,  the  current  process  will  be  preempted  and
     returned  into its wait list. The scheduling policy only determines the
     ordering within the list of runnable processes with equal static prior-
     ity.

     SCHED_FIFO: First In-First Out scheduling
     SCHED_FIFO can only be used with static priorities higher than 0, which
     means that when a SCHED_FIFO processes becomes runnable, it will always
     immediately  preempt  any  currently running SCHED_OTHER or SCHED_BATCH
     process.  SCHED_FIFO is a  simple  scheduling  algorithm  without  time
     slicing.  For processes scheduled under the SCHED_FIFO policy, the fol-
     lowing rules are applied: A SCHED_FIFO process that has been  preempted
     by another process of higher priority will stay at the head of the list
     for its priority and will resume execution as soon as all processes  of
     higher  priority  are  blocked again. When a SCHED_FIFO process becomes
     runnable, it will be inserted at the end of the list for its  priority.
     A  call  to  sched_setscheduler()  or  sched_setparam()  will  put  the
     SCHED_FIFO (or SCHED_RR) process identified by pid at the start of  the
     list  if  it  was  runnable.  As a consequence, it may preempt the cur-
     rently running process if it  has  the  same  priority.   (POSIX.1-2001
     specifies  that  the process should go to the end of the list.)  A pro-
     cess calling sched_yield() will be put at the end of the list. No other
     events will move a process scheduled under the SCHED_FIFO policy in the
     wait  list  of  runnable  processes  with  equal  static  priority.   A
     SCHED_FIFO  process  runs until either it is blocked by an I/O request,
     it  is  preempted  by  a  higher  priority   process,   or   it   calls
     sched_yield().

     SCHED_RR: Round Robin scheduling
     SCHED_RR  is  a  simple enhancement of SCHED_FIFO. Everything described
     above for SCHED_FIFO also applies to SCHED_RR, except that each process
     is  only  allowed to run for a maximum time quantum. If a SCHED_RR pro-
     cess has been running for a time period equal to  or  longer  than  the
     time quantum, it will be put at the end of the list for its priority. A
     SCHED_RR process that has been preempted by a higher  priority  process
     and  subsequently  resumes execution as a running process will complete
     the unexpired portion of its round robin time quantum.  The  length  of
     the time quantum can be retrieved using sched_rr_get_interval(2).
     SCHED_OTHER: Default Linux time-sharing scheduling
     SCHED_OTHER  can only be used at static priority 0.  SCHED_OTHER is the
     standard Linux time-sharing scheduler that is  intended  for  all  pro-
     cesses  that  do  not  require special static priority real-time mecha-
     nisms. The process to run is chosen from the  static  priority  0  list
     based  on  a dynamic priority that is determined only inside this list.
     The dynamic priority is based on the nice level (set by nice(2) or set-
     priority(2))  and  increased for each time quantum the process is ready
     to run, but denied to run by the scheduler. This ensures fair  progress
     among all SCHED_OTHER processes.

     SCHED_BATCH: Scheduling batch processes
     (Since  Linux 2.6.16.)  SCHED_BATCH can only be used at static priority
     0.  This policy is similar to SCHED_OTHER, except that this policy will
     cause the scheduler to always assume that the process is CPU-intensive.
     Consequently, the scheduler will apply a small  scheduling  penalty  so
     that  this process is mildly disfavoured in scheduling decisions.  This
     policy is useful for workloads that are  non-interactive,  but  do  not
     want to lower their nice value, and for workloads that want a determin-
     istic scheduling policy without interactivity causing extra preemptions
     (between the workload¡¯s tasks).

     Privileges and resource limits
     In  Linux  kernels  before  2.6.12, only privileged (CAP_SYS_NICE) pro-
     cesses can set a non-zero static priority.  The  only  change  that  an
     unprivileged  process  can  make  is to set the SCHED_OTHER policy, and
     this can only be done if  the  effective  user  ID  of  the  caller  of
     sched_setscheduler()  matches the real or effective user ID of the tar-
     get process (i.e., the process specified by pid) whose policy is  being
     changed.

     Since  Linux 2.6.12, the RLIMIT_RTPRIO resource limit defines a ceiling
     on an unprivileged process¡¯s priority for the SCHED_RR  and  SCHED_FIFO
     policies.  If an unprivileged process has a non-zero RLIMIT_RTPRIO soft
     limit, then it can change its scheduling policy and  priority,  subject
     to  the  restriction  that the priority cannot be set to a value higher
     than the RLIMIT_RTPRIO soft limit.  If the RLIMIT_RTPRIO soft limit  is
     0, then the only permitted change is to lower the priority.  Subject to
     the same rules,  another  unprivileged  process  can  also  make  these
     changes,  as  long  as  the effective user ID of the process making the
     change matches the real or effective user ID  of  the  target  process.
     See  getrlimit(2) for further information on RLIMIT_RTPRIO.  Privileged
     (CAP_SYS_NICE) processes ignore this limit; as with  older  older  ker-
     nels,  they  can make arbitrary changes to scheduling policy and prior-
     ity.
 */

typedef enum 
{
    FCFS_SCHED      = 0,
    OTHER_SCHED     = 1,
    RR_SCHED        = 2,
}SCHEDULE_POLICY_T;

typedef struct
{
    CHAR_T              thread_name[THREAD_NAME_LEN];             
    INT32_T             stack_size;
    INT32_T             priority;
    SCHEDULE_POLICY_T   schedule_policy;
    void*               route_ptr;
    void*               args;
    /*
     *	Reserve for vxWorks option flag.
     */
    UINT32_T            vxworks_option_flag;
    /*
     *
	 *	For windows, Options implemented as dwCreationFlags.
	 *	[in] Flags that control the creation of the thread. 
	 *	If the CREATE_SUSPENDED flag is specified, 
	 *	the thread is created in a suspended state, 
	 *	and will not run until the ResumeThread function 
	 *	is called. If this value is zero, the thread runs 
	 *	immediately after creation. If the STACK_SIZE_PARAM_IS_A_RESERVATION 
	 *	flag is specified, the dwStackSize parameter 
	 *	specifies the initial reserve size of the stack. 
	 *	Otherwise, dwStackSize specifies the commit size.
     */
    UINT32_T            win_create_flag;
}THREAD_PARAM,*THREAD_PARAM_PTR;

typedef struct
{
    BOOL                is_initialized;
    BOOL                is_created;
    THREAD_PARAM_PTR    thread_params;
    UINT32_T            thread_id;
    void*               thread_attr;
}THREAD_PROPERTY,*THREAD_PROPERTY_PTR;

