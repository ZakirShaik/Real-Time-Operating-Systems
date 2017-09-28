-I have created three tasks according to highest, medium and low priorities.
- Task 1 (highest priority) monitors a parking lot in an area. It generates random coordinates (x,y) of a car and uses the Euclidean distance and period to calculate the speed of the car (output). If the speed is above 10 (to simulate 10 mph), a warning is generated.
- Task 2 (medium priority) simulates an occupancy monitoring system for a class room. Each time (simulates one hour) a random value between 20 and 50, representing the number of students in B190, is generated. Task 2 calculates the accumulated sum, average, maximum and minimum number of students as output, and clears the statistics after every 24 readings (i.e., one day).  
- Task 3 (lowest priority) records the power and energy consumption of an industry. A power reading (random number between 10KW and 200KW) is sent to Task 3 every minute (simulated). Task 3 estimates the energy consumption by calculating Power*Time. The total energy consumption in a month and spikes of power use (i.e., instant power reading >= 5 * average power use) are outputted.  
- Also, I have calculated Average response time of each task.
- Also, I have generated random values for task 1 and also I have accessed random values from a file for task 1.
- I have generated graphs for both average response times of tasks and arrival times of tasks. 


















