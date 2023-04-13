# Philosophers-bonus

There are 3 main aspects which we will learn about during this project:
- Concurrent programming
- Threads
- Mutexes

## What is concurrent programming?

We are used to programs whose flow is lineal (sequential programming), they start, do some tasks and end; this is not the way concurrent programming works.

In this case, our program will be doing several tasks at the same time, instead of having to wait for the result of one task to start another one.

There are 3 ways to implement this:
- Processes
- Threads
- Multiplexing

We will talk about processes since it its the way we will implement it.

## Processes
A process is an instance of a program that is being executed by 1 or more threads.
Each process has its own address space and (usually) one thread of control.

Processes are organizde hierarchically, each one has a parent process which created it. The process created by a parent is called the child process. This child will inherit many of its attributes from its parent process.

A process is identified by its process ID number, a value of type `pid_t`. We can access to this ID by `getpid` function

The `waitpid` function is udsed to request status information from a child process whose process ID is `pid`. The calling process will be suspended until the child process terminates.

Any thread can read and write to the same virtual memory.

## Creating a process

In order to create a process we will use the ***fork*** function (explained later).

## Managing process execution

In order to lock the execution of a process we will use semaphores (explained later).

## What is a semaphore?

A semaphore will have an amount of resources, it will allow to take them until it run out of them, once this happens it will create a queue so new requesters will wait for the resources to be available.

## Declaring a semaphore

To declare a semaphore we can do it like this: `sem_t sem`. But, in order to use it, we must initialize it using ***sem_open*** function (explained later).

## Locking and unlocking a semaphore

To lock or unlock a semaphore we will use two diferent functions, one for each task: ***sem_wait*** and ***sem_post***.

To lock it we will use ***sem_wait***, which will decrement the semaphore. If the vale is greater than zero, the decrement proceeds and the function returns. If the value is currently zero the call blocks until the decrement can be done or the call is interrupted by a signal.

To unlock it we will use ***sem_post*** which increments the semaphore. If the value become greater than zero, another process or thread blocked in a sem_wait will be woken up to lock the semaphore.

## Destroying a semaphore

Lastly, when our semaphore is no longer needed, we must destroy it so we do not have any kind of leak.

In order to do this we will use the ***sem_close*** (explained later).

This function will allow the system to free any allocated resources for this semaphore.

## Common functions

### memset

    void *memset(void *str, int c, size_t n)
This function copies the character ***c*** to the first n characters of the string pointed by str.

### usleep

    int usleep(useconds_t usec)
This function suspends execution of the calling thread for (at least) ***usec*** microseconds. This time could be increased by the system performance.

### gettimeofday

    int gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz)
This function can get the time as well as a timezone.
- ***tv*** is a struct timeval (from <sys/time.h>) which gives the number of seconds and microseconds since the [Epoch](https://en.wikipedia.org/wiki/Epoch_(computing)) in two separated variables, ***tv_sec*** and ***tv_usec***.
- ***tz*** is a struct timezone, it seems to be obsolete so we will specify it as NULL.

This function returns 0 for success or -1 for error.

## Thread functions

### pthread_create

    int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *retrict arg)
This function starts a new thread in the calling process. This new thread will start its execution by calling the function ***start_routine()*** with the argument ***args*** given (it can be only 1 argument).

The ***attr*** argument points to a structure which contents are used at thread creation to determine its attributes (since we want default threads we will set this to NULL).

Before returning, a successful creation will store the ID of the new thread in ***thread***. This ID will be used to call this thread.

The function itself will return 0 for a success.

### pthread_join 

    int pthread_join(pthread_t thread void **retval)
This function suspends processing of the calling thread until the target ***thread*** completes. ***thread*** must not be a detached thread (we will talk about it later).

If more than one thread is waiting for the same thread, all will suspend until it completes. After that one of them will return successfully and the others will return with an error.

A successful call will return a non-null ***retval***, but the function itself will return 0

### pthread_detach

    int pthread_deatch(pthread_t thread)
This function marks ***thread*** as detached. When a detached thread completes its resources are released back to the system. Attempting to detach an already detached thread results in unspecified behavior.

This function returns 0 for success or an error number for error.

## Process functions

### fork

  pid_t fork(void)
This function creates a new process by duplicating the calling process. The new process will be the child process, whereas the calling process will be the parent process.

Both processes will run in separate memory spaces. At the time of `fork` both memory spaces have the same content. Both processes memory are isolated so they dont affect each other.

The child process is an exact duplicate except for:

- The child has its own unique process ID and this PID does not match the ID of any existing process group or session.
- The child's parent process ID is the same as the parent's process ID.
- The child does not inherit its parent's memory locks.
- Process resource utilization and CPU time counters are reset to 0 in the child.
- The child's set of pending signals is initially empty.
- The child does not inherit semaphore adjustments from its parent.
- The child does not inherit process-associaed record locks from its parent.
- The child does not inherit timers from its parent.
- The child odes not inherit outstanding asynchronous operations nor context from its parent.

The child process is created with a single thread, the one called ***fork***. The child will inherit a copy of every file descriptor of the parent's ones.

This function returns the PID of the child in the parent and 0 in the child on success. On failure, -1 is returned in the parent and no child process is created.

### kill

  int kill (pid_t pid, int sig)
This function can be used to send a signal to any process or process group.

If ***pid*** is positive, the signal is sent to the process with that ID.

If ***pid*** is 0, the signal is sent to every process in the process group of the calling process.

If ***pid*** is -1, the signal is sent to every process for whicih the calling process has permission to send signals, except for process 1.

If ***pid*** is less than -1, the signal is sent to every process in the process group whose ID is ***-pid***.

This function returns 0 on succes or -1 on error.

## Semaphore functions

### sem_open

  sem_t *sem_open(const char *name, int oflag)
  sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value)
This function creates a new semaphore or opens an existing one. The semaphore is identified by name. The oflag specifies the operation of the call.

O_CREAT: the semaphore is created if it does not already exist. The owner will be the calling process ID.

If O_CREAT is specified, we will add 2 additional arguments. 

The mode argument specifies the [permissions](https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html) for the new semaphore. For this argument we will use S_IRWXU which gives reading, writing, and execution permission to the user. The value argument specifies the initial value for the new semaphore.

### sem_close

  int sem_close(sem_t *sem)
This function closes the semaphore ***sem*** allowing the system to free any resources allocated for it.

Returns 0 on success or -1 on error.

### sem_post

  int sem_post(sem_t *sem)
This function increments the semaphore ***sem***, if the value is greater than 0 any process or thread blocked bt sem_wait will be woken up to lock the semaphore.

Returns 0 on success or -1 on error (the semaphore value will not change on error).

### sem_wait

  int sem_wait(sem_t *sem)
This function decrements the semaphore ***sem***, if the semaphore value is greater than 0 it will decrement it, if it is 0 the call blocks until the decrement is posible or it is interrupted by a signal.

Returns 0 on succes or -1 on error (the semaphore value will not change on error).

### sem_unlink

  int sem_unlink(sem_t *sem)
This function removes the semaphore ***sem***. The semaphore is destroyed once all other processes that have it open close it.

Returns 0 on succes or -1 on error.

## Program workflow

- The program receives 4 or 5 arguments being: `number of philosophers` `time to die` `time to eat` `time to sleep` `[number of meals]`. Any other amount of arguments results in an error message.
- Having these values, the program initializes everythin necessary. In `rules` structure we will hold the global settings for the program: `number of philosophers` `time to die` `time to eat` `time to sleep` `number of meals` `dead value` `all have eaten enough value` `first timestamp` `meal check semaphore` `forks semaphore` `writing semaphore` `philosopher mutex array`. In case of any value is not valid it will return an error message.
- After the values are properly set in `rules` the program will initialize the semaphores for forks, writing and meal_check.
- Once the semaphore are initialized it will create each philosopher and will initialize their values: `id` `number of meals` `time of last meal` `rules`.
- At this point everything is initialized, but in case we have had an error the program will show an error message.
- If everything worked properly, the program will start the main process which is the function `starter`.
- Firstly, the program will take a timestamp which will be used for following time calculation.
- After that the program will create a child process (using fork function) for each philosopher. All of these processes will go on from that point. If the fork failed the function will end, otherwise, if we are in the child process we will call to p_process giving it a philosopher memory location as argument.
- `p_process` function will manage the philosophers behaviour. Firstly we will create a thread that will execute `death_checker` function. In case their `ID` is odd they will wait for 10 ms so adjacent philosophers will not try to get a fork at the same time (in this case that they are adjacent or not is not important, but we will keep this as a way to be sure that only half of them try to take forks at the same time, so we avoid deadlocks). Once they start, they will eat.
- For the eating process the program will execute the `philo_eating` function. This function will check if there is only 1 philosopher, since he only has 1 fork he will not be able to eat so he will take it and wait until he dies. In case there is more than 1 philosopher the eating philosopher will take two forks (locking the semaphore) will lock the `meal_check` semaphore, change his last meal time to current time, unlock the `meal_check` semaphore, wait for the time he needs to eat, increase his number of meals and free both forks.
- After eating the philosopher will check if he has eaten enough, if so, the loop will break, the death checker thread will stop and the process will exit. If not he will start sleeping (waiting for time to sleep), and then start thinking (waiting for free forks to start eating again).
- Every philosopher will do this until one of them die or they have eaten enough.
- During the whole execution of eating we will have our threads executing `death_checker` function which will check if they have finished eating or died. It will not start if there is only 1 philosopher since he will die no matter what happens.
- During `death_checker` execution we will lock `meal_check` semaphore, then we will check if the time since his last meal is higher than the time to die; in case it is, we set `dead` to 1, print a death message and lock the writing semaphore so nobody else can write anymore. If our philosopher is alive we unlock the `meal_check` semaphore and check if any of the others philosophers has died, if not we will check if they have eaten enough.
- At this point our program has finished, because a philosopher died or because all of them have eaten enough. Anyways we will stop our `death_checker` thread, and we will exit our process (reminder that we are in the child process), returning a 1 value if one of them died or 0 if not.
- At this point our program is done, but in order to finish it properly we will call to the `exit_starter` function.
- This function will go through every philosopher making them wait for their child processes. If their state change (meaning they have finished) we will send a `SIGTERM` signal to its process id so we terminate that process. After this we will close our 3 semaphores (forks, meal_check and writing) and we will unlink them so they will not exist for future executions.
