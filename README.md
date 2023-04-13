# Philosophers

In computer science the [dining philosphers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) is an example problem used in concurrent algorithm design to illustrate synchronization issues and techniques for resolving them.

## Problem statement

An undefined ammount of philosophers dine together at the same table. Each philosopher has their own place at the table and his own fork, but in order to eat they need two forks at the same time. Once they can take two forks they will eat, after eating they will start sleeping and when they wake up they will think until they can eat again.

The problem here is that there is only one fork for philosopher so they must alternate to take them while an adjacent philosopher is sleeping. We must avoid starving if it is posible. Another problem is that they cannot comunicate or tell other philosophers when they are eating or sleep.

## Problems to solve

The main problem we can find is a [dealock](https://en.wikipedia.org/wiki/Deadlock) which is a system state in which no progress is posible. An example of deadlock would be if every philosopher takes their fork, at that point they all will wait for another fork to be free and will starve.

Another problem would be what is called [resource starvation](https://en.wikipedia.org/wiki/Starvation_(computer_science)), which means that one of our process is denied necessary resources to work. An example would be: let us say we have 5 philosophers with their own fork, philosopher 2 takes forks 1 and 2, and philosopher 4 takes forks 3 and 4; now philosophers 1, 3 and 5 will wait. When they finish eating philosopher 1 takes forks 1 and 5, and philosopher 3 takes forks 2 and 3. Now philosopher 2, 4 and 5 are waiting. When they finish eating philosophers 2 and 4 take the same forks again and start eating. As you can see, philosopher 5 is never allowed to eat since the rest of them do not know if he needs to eat so they just take the fork as soon as they can, denying philospher 5 the possibility to eat.

The next problem we can find is [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion), which occurs when two or more threads try to access to the same shared memory at the same time. An example of this would be if two adjacent philosophers try to take the fork at the same time, who would take it. This would create a data inconsistency so we cannot let this happen. In order to avoid this problem we must block access to a shared resource while one thread has accessed to it.

The last problem which we will talk about is [livelock](https://www.dit.upm.es/~pepe/libros/concurrency/index.html#!1033) which happens when a thread A is working for a thread B, which is working at the same time for the thread A, creating an infinite dependency. It is similar to a deadlock but they do not end or "die" so that is why it is called livelock.

## Solutions

For all this problems we will use two different ways to fix them:
- philo: we will use threads and mutexes to make the program work.
- philo_bonus: we will uses processes and semaphores to make the program work.

You can check their respective folders to check an extensive explaination about how they work.

In _norme folders you will find the same code but without any comments so it follows the [norme](https://github.com/MagicHatJo/-42-Norm/blob/master/norme.en.pdf)
