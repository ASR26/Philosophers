# Philosophers

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

We will talk about threads since it its the way we will implement it.

## Threads
A thread is a sequence of instructions inside a process that is managed by the kernel. Each thread has its owns properties:
- ID
- Stack
- Instruction pointer
- Processor register
It is faster for a processor to create a thread than a program, and so it is to switch between them.

Threads do not have a parent-child hierarchy that process have. They form a group of peers, which means that within the same process any thread can wait for any other to complete or kill any other thread.

Any thread can read and write to the same virtual memory.

## Creating a thread

In order to create a thread we will use the ***pthread_create*** function (explained later).

## Joining and detaching threads

In order to lock the execution of a thread until another thread finishes we will use ***pthread_join*** function (explained later).

In some cases it will be preferable or even mandatory not to wait for the end of certain threads. For these cases we will use ***pthread_detach*** function, which tells the OS to reclaim its resources once the thread has finished.

Once a thread is detached it cannot be killed or joined by other threads.

## What is a mutex?

Mutex means _mutual exclusion_, in other words, it will allow us to manage data access and prevent shared resources from being used at the same time.

## Declaring a mutex

To declare a mutex we can do it like this: `pthread_mutext_t mutex`. But, in order to use it we must initialize it using ***pthread_mutex_init*** function (explained later).

## Locking and unlocking a mutex

To lock or unlock a mutex we will use two diferent functions, one for each task: ***pthread_mutex_lock*** and ***pthread_mutex_unlock***.

To lock it we will use ***pthread_mutex_lock*** (obviously) so the calling thread will become its owner, but this thread which will be locked must be unlocked. If it is already locked by another thread this function will lock the calling thread until the called thread is available.

## Destroying a mutex

Lastly, when our mutex is no longer needed, we must destroy it so we do not have any kind of leak.

In order to do this we will use the ***pthread_mutex_destroy*** (explained later).

This function will destroy the mutex (must be unlocked or we will get an undefined behaviour) , freeing whatever it holds.

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

## Mutex functions

### pthread_mutex_init

    int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
This function initializes a mutex pointed by ***mutex***, ***attr*** will be NULL since we want a default mutex.

Mutex is initialized in an unlocked state.

### pthread_mutex_destroy

    int pthread_mutex_destroy(pthread_mutex_t *mutex)
This function destroys the mutex referenced by ***mutex***. We should destroy unlocked mutex, since destroying locked mutex results in an undefined behavior.

### pthread_mutex_lock

    int pthread_mutex_lock(pthread_mutex_t *mutex)
This function will lock the mutex element pointed by ***mutex***. If the mutex is already blocked the calling thread will block until the mutex becomes available.

This function returns 0 for success or an error number for error.

### pthread_mutex_unlock

    int pthread_mutex_unlock(pthread_mutex_t *mutex)
This function will release the mutex element pointed by ***mutex***.

This function returns 0 for success or an error number for error.

## Program workflow

- The program receives 4 or 5 arguments being: `number of philosophers` `time to die` `time to eat` `time to sleep` `[number of meals]`. Any other amount of arguments results in an error message.
- Having these values, the program initializes everythin necessary. In `rules` structure we will hold the global settings for the program: `number of philosophers` `time to die` `time to eat` `time to sleep` `number of meals` `dead value` `all have eaten enough value` `first timestamp` `meal check mutex` `forks mutex array` `writing mutex` `philosopher mutex array`. In case of any value is not valid it will return an error message.
- After the values are properly set in `rules` the program will initialize the mutex for each fork, writing and meal_check.
- Once the mutex are initialized it will create each philosopher and will initialize their values: `id` `number of meals` `left fork id` `right fork id` `time of last meal` `rules`.
- At this point everything is initialized, but in case we have had an error the program will show an error message.
- If everything worked properly, the program will start the main process which is the function `starter`.
- Firstly, the program will take a timestamp which will be used for following time calculation.
- After that the program will create a thread for each philosopher. All of these threads will execute the function `p_thread` and every philosopher will get their own timestamp so they know the time they have been alive.
- `p_thread` function will manage the philosophers behaviour. In case their `ID` is odd they will wait for 10 ms so adjacent philosophers will not try to get a fork at the same time. Once they start, they will eat.
- For the eating process the program will execute the `philo_eating` function. This function will check if there is only 1 philosopher, since he only has 1 fork he will not be able to eat so he will take it and wait until he dies. In case there is more than 1 philosopher the eating philosopher will take his left and right forks (locking their mutexes) will lock the `meal_check` mutex, change his last meal time to current time, unlock the `meal_check` mutex, wait for the time he needs to eat, increase his number of meals and free both forks.
- After eating the philosopher will check if he has eaten enough, if so, his thread will stop working. If not he will start sleeping (waiting for time to sleep), and then start thinking (waiting for free forks to start eating again).
- Every philosopher will do this until one of them die or they have eaten enough.
- Once every thread has finished the program will start the `death_checker` function which will check if they have finished eating or died. It will not start if there is only 1 philosopher since he will die no matter what happens.
- For every philosopher, we will lock `meal_check` mutex, then we will check if the time since his last meal is higher than the time to die; in case it is, we set `dead` to 1. After this check we unlock the `meal_check` mutex, go to the next philosopher, and wait for 0,1 ms.
- Once we have checked every philosopher if dead is set to 1 we break the loop and the function returns. In case they are all alive we will check all of them again, to be sure they have eaten enough. If all of them have eaten enough we will set `all_eaten` to 1 and he loop will stop, so the function will return.
- At this point our program has finished, because a philosopher died or because all of them have eaten enough. Anyways the program will execute the `exit_starter` function.
- This function will execute a `pthread_join` for each philosopher thread, then it will destroy every fork mutex and finally it will destroy the writing mutex, being the end of the program.
