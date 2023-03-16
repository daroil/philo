**Philosophers Project**


This project is a multithreaded program that simulates the dining philosophers problem. The challenge in this problem is to prevent deadlocks and starvation, where philosophers may be left hungry and unable to eat. To overcome this challenge, the program uses mutexes and semaphores to control the access to shared resources such as chopsticks the philosophers use to eat.


**Table of Contents**

Installation

Usage

Implementation Details


**Installation**

To install and run this program, follow these steps:

Clone this repository to your local machine.
Compile the program by running make.
Run the program with the desired number of philosophers and times they will eat.


**Usage**

To run the program, use the following command:

./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [<number_of_times_each_philosopher_must_eat>]

number_of_philosophers: The number of philosophers sitting at the table.

time_to_die: The time in milliseconds after which a philosopher dies if they haven't eaten.

time_to_eat: The time in milliseconds it takes for a philosopher to eat.

time_to_sleep: The time in milliseconds a philosopher spends sleeping after they finish eating.

number_of_times_each_philosopher_must_eat (optional): The number of times each philosopher must eat before the simulation ends. If not specified, the simulation will run until a philosopher dies or the program is interrupted.


**Implementation Details**

In this program, I have used a combination of mutexes and semaphores to control the access to shared resources such as choptsticks. I have also used threads or forks to simulate the philosophers.

