# POSIX_THREADS

Pthreads

# Part 1

In this assignment I will write two pthreads programs that evaluate some aspects
of the pthreads system. The first part and simpler of the two, lets threads write to the same file
using mutexes to avoid race conditions. The main thread creates N (N is an optional command
line argument by dafault 100) threads and all of them immediately block on condition
variable start_line. After all threads are created, the main thread wakes them up
and then waits until they all terminate with join. The threads after waking up lock the
mutex filemut and write their PID followed by newline to file pthread_stats and
then release the mutex. After they do this M times each (the second optional command
line argument that defaults to 1000) they join the main thread.

# Part 2

In the second pthread program of the assignment the main program creates N threads
designated as red and then another N threads designated as green. All of them immediately
block on condition variable start_line. After all threads are created, the main
thread wakes them up and then waits until the all terminate with join. The threads after
waking up lock the mutex filemut and write their PID followed by the word red or
green followed by newline to file pthread_stats. The difference in this second version
is that all the red threads write first their PID M times releasing and re-acquiring the
mutex, then all the green ones another M times, then the red threads again, and so on,
until they have written L (another optional command line argument that defaults to 10)
times each.
