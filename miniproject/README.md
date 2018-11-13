# Miniproject - TTK4147 Real-Time Systems
This folder contains source code, external libraries - as well as a Makefile for compiling.

## Project structure
* `./Makefile` - The recipe for how to build the code into an executable
* `./src/...` - The code we have written ourselves
* `./lib/...` - The external libraries we were supplied

Note that in order for the Makefile to work, the location of the executable `avr32-linux-gcc` must be in your `PATH` environment variable.

## Design decisions
### Threads
We decided to use two pthreads, in addition to the parent thread. The two threads never interract directly with each other.
(Only indirectly)

* `communication_receiver_thread`
  - Pick up `GET_ACK` messages (receiving the y-values).
  - Pick up and reply to `SIGNAL`-messages with `SIGNAL_ACK`.
* `communication_requester_thread`
  - Periodically send `GET` messages to the server.
  
The main function loop then simply waits for the next y-value, 
supplies the reference error to the controller before sending the actuation u back to the server.

### Controller
A PID controller was chosen, with the suggested controller parameters. (Kp = 20, Ki = 1000, Kd = 0.01). You can find these constants (among with other constants) in `./src/config.h`. 

A constant period was assumed by the controller, in spite of the period not actually being constant in practice. This might cause the system to be a bit harder to stabilize.

### Choice of period
Some different periods were tested, all from 1 ms to 10 ms. At first, this was entirely experimental - to see how fast/efficient the system really were. But based on the results, it seemed that 2 ms provided the best results among the periods that were tested.
* How I decided on the period, as well as other paramters?
What kind of controller?

### Discussion of results

## Results
#### Period: 1 ms
![img](results/plot5_1ms.png)
#### Period: 2 ms
![img](results/plot5_2ms.png)
#### Period: 3 ms
![img](results/plot5_3ms.png)
#### Period: 5 ms
![img](results/plot5_5ms.png)
#### Period: 10 ms
![img](results/plot5_10ms.png)

