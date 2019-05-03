# CSC 415 - Project 5 - Producer Consumer - Part 2

## Student Name : Cory Lewis

## Student ID : 917359162

## Build Instructions

### Open Linux terminal and navigate to project folder. cd into directory part2. Ex| cd part2
### Type `make` to build the project

## Run Instructions

### Type `./pandc 7 7 9 15 0 0` to run a sample.

- Arguments ordered left to right and what they do

(1st) N -> Number of buffers to maintain
(2nd) P -> Number of producer threads
(3rd) C -> Number of consumer threads
(4th) X -> Number of items each producer thread will produce
(5th) Ptime -> (in seconds) how long each producer thread will sleep after producing a single item
(6th) Ctime -> (in seconds) how long each consumer thread will sleep after consuming a single item

Summary: ./pandc N P C X Ptime Ctime
