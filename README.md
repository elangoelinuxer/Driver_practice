semaphore concept with following APIs implemented

sema_init -  initialize semaphore with initial value 1

_1- unlocked
 0-locked


DECLARE_MUTEX 
DECLARE_MUTEX_LOCKED  

init_MUTEX
init_MUTEX_LOCKED



Down function:

down
down_interruptible
down_trylock



up function:

up


TODO:

insmod ./semaphore.ko

cat /dev/Char_dev  _to start the threads

note:

change the location of your file to be created 




