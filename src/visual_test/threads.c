Public void Thread_Sleep(u32 milliseconds) {
    if (milliseconds > 0) {
        Sleep((DWORD)milliseconds);
    }
}

typedef struct {
    HANDLE handle;
    u32 ID;
} Thread_Info;

Private Global Thread_Info threadPool[64];
Private Global u32 threadPoolCount;
Private Global Job_Queue globalQueue;
Private Global HANDLE globalSemaphore;

Intern b8 GetNextJob(Job_Queue *queue, Job_Entry *jobEntry) {
    u32 currentJobIndex = queue->index;
    u32 nextJobIndex = currentJobIndex + 1;

    if (queue->index < queue->size) {
        u32 index = (u32)InterlockedCompareExchange(&queue->index, nextJobIndex, currentJobIndex);
        if (index == currentJobIndex) {
            *jobEntry = queue->Q[index];
            return 1;
        }
    }

    return 0;
}

Intern void ThreadPoolAddJob(Job_Queue *queue, HANDLE semaphore, Fn_Ptr_Job func, void *args) {
    Job_Entry *job = &queue->Q[queue->size];

    job->func = func;
    job->args = args;

    (void)Atomic_Inc_U32(&queue->size);

    Win32_Check( ReleaseSemaphore(semaphore, 1, 0), > 0 );
}

Export Fn_Prot_Thread_Function(OS_Thread_Home) {
    u32 reloadIndex = 0;
    u32 semaphoreIndex = 1;
    u32 msgIndex = 2;

    // TODO(JENH) This shouldn't be here.
    while ( semaphoreOS == 0 );

    u32 sleepTime = INFINITE;

    const HANDLE handles[] = { reloadSemaphore, semaphoreOS };

    while ( INFINITE_LOOP ) {
        u32 handleIndex;
        Win32_Check( (handleIndex = (u32)MsgWaitForMultipleObjects(ArrayCount(handles), handles, FALSE, sleepTime, QS_ALLINPUT)), != WAIT_FAILED );

        if ( handleIndex == WAIT_TIMEOUT ) {
            sleepTime = INFINITE;

            Input_Index inputIndex = II_Ms_Movement;
            b8 isPress = JENH_FALSE;
            f32 timeStamp = Time_OS_Seconds_Elapsed(Time_OS_Last_Counter(), Time_OS_Counter());

            Input_Update_Actions(inputIndex, isPress, timeStamp);
            continue;
        }

        if ( handleIndex == reloadIndex ) { return; }

        if ( handleIndex == semaphoreIndex ) {
            Job_Entry job;
            GetNextJob(&queueOS, &job);
            job.func(job.args);
        } else if ( handleIndex == msgIndex ) {
            MSG event;

            while (PeekMessageA(&event, 0, 0, 0, PM_REMOVE)) {
                WIN32_ProcessPendingEvents(&event, &sleepTime);
            }
        }
    }
}

typedef struct {
    String str;
} Thread_String_Data;

Intern Fn_Job(PrintThreadString) {
    String *msg = (String *)args;
    LogDebug("thread: %s", msg->str);
}

Export Fn_Prot_Thread_Function(Job_Home) {
    // TODO(JENH) This shouldn't be here.
    while ( globalSemaphore == 0 );

    HANDLE handles[] = { reloadSemaphore, globalSemaphore };

    while ( INFINITE_LOOP ) {
        Job_Entry job = {0};

        if ( GetNextJob(&globalQueue, &job) ) {
            job.func(job.args);
        } else {
            DWORD handleIndex = WaitForMultipleObjects(ArrayCount(handles), handles, FALSE, INFINITE);

            if ( handleIndex == 0 ) { return; }

            //Win32_Check( ret != WAIT_FAILED );
            //Assert( ret == WAIT_OBJECT_0 );
        }
    }
}

Intern inline void SetMouseRelative() {
    ThreadPoolAddJob(&queueOS, semaphoreOS, TSetMouseRelative, 0);
}

Intern inline void SetMouseAbsolute() {
    ThreadPoolAddJob(&queueOS, semaphoreOS, TSetMouseAbsolute, 0);
}

