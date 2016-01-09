# Registrar-Simulation
Program using my own queue and linked list classes to simulate a registrar's office. Calculates metrics on student wait times and window idle times given a specific traffic flow of students.

@author Adrienne Bergh

# Program Components
- Uses self-built generic queue data structure, implemented with my own generic doubly-linked list data structure.
- Takes as command line argument the name of a test file with information from the registrar's office. The text file defines at what times students arrive, and has the following format: first line - number of windows open; second line - time (or clock tick) at which the next students arrive; third line - number of students that arrive at that time. The following lines will be the amount of time each student needs at a window. This is followed by the next clock tick, number of students, etc.
- The simulation begins at time 0, and runs until all student requests have been addressed, meaning the queue is empty and no more students are going to arrive.
- At the end of the simulation, program displays the following metrics:

- The mean student wait time.
- The median student wait time.
- The longest student wait time.
- The number of students waiting over 10 minutes
- The mean window idle time
- The longest window idle time
- Number of windows idle for over 5 minutes.
