# Miniproject - TTK4147 Real-Time Systems
This folder contains source code, external libraries - as well as a Makefile for compiling.

## Project structure
* `./Makefile` - The recipe for how to build the code into an executable
* `./src/...` - The code we have written ourselves
* `./lib/...` - The external libraries we were supplied

Note that in order for the Makefile to work, the location of the executable `avr32-linux-gcc` must be in your `PATH` environment variable.

## Design decisions
### Threads
We decided to use two pthreads, in addition to using the main function itself. The two threads never interract directly with each other.
(Only indirectly)

* `communication_receiver_thread`
  - Pick up `GET_ACK` messages (receiving the y-values).
  - Pick up and reply to `SIGNAL`-messages with `SIGNAL_ACK`.
* `communication_requester_thread`
  - Periodically send `GET` messages to the server.
  
The main function loop then simply waits for the next y-value, 
supplies the reference error to the controller before sending the actuation u back to the server.

### TODO:
* How I decided on the period, as well as other paramters?
What kind of controller?

## Plot Analysis
### TODO: 
 * Add plots!
 * Analyze plots
 * Try different periods

