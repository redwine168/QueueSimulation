// Jonathan Redwine
// QueueSimulation
// Copyright 2018


// #include "pch.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// CUSTOMER CLASS
class Customer {
public:
    Customer(int currTime, int newDuration, int custNumer, int timeRem);
    int serviceDuration; // time to serve this customer
    int timeRemaining; // wait time until service on this customer begins
    int number; // identifying number assigned to customer (first customer 0, second customer 1, etc.)
    int entryMinute; // minute at which this customer entered the queue
    Customer* next;
    Customer* prev;
};

// Constructor for Customer class
Customer::Customer(int currTime, int newDuration, int custNumber, int timeRem) {
    serviceDuration = newDuration;
    number = custNumber;
    entryMinute = currTime;
    timeRemaining = timeRem;
}

// QUEUE CLASS
class Queue {
public:
    Queue();
    void enterCustomer(int currTime, int maxTime, int custNumber); // add a customer to the back of queue
    void exitCustomer(); // remove a customer from the front of the queue
    void displayQueue(int minute); // display customers in queue
    Customer* front;
};

// Constructor for Queue class
Queue::Queue() {
    front = NULL;
}

// Creates a new Customer objects and adds it to the Queue
void Queue::enterCustomer(int currTime, int maxTime, int custNumber) {
    int newServiceDuration = (rand() % maxTime) + 1; // generate random service duration for new customer
    int timeRem = 0;
    Customer* currCust = front;
    while (currCust != NULL) { // calculate wait time based on service durations of customers already in queue
        timeRem += currCust->serviceDuration;
        currCust = currCust->next;
    }
    Customer* newCustomer; // create new customer object
    newCustomer = new Customer(currTime, newServiceDuration, custNumber, timeRem);
    if (front == NULL) { // if queue is empty, put new customer at front
        front = newCustomer;
    }
    else { // if queue is not empty, put new customer at end of queue
        Customer* curr = front;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newCustomer;
    }
    newCustomer->next = NULL;
}

// Removes the front Customer object from the Queue
void Queue::exitCustomer() {
    Customer* removed = front;
    front = front->next; // make front of the queue the next customer
    delete removed; // return the former front customer to memory
}

// Displays the information of the Customer objects currently in the Queue
void Queue::displayQueue(int minute) {
    cout << minute;
    if (front == NULL) { // report if the queue is empty
        cout << "    The queue is empty!" << endl;
    }
    else {
        Customer* curr = front;
        while (curr != NULL) {
            (minute < 10) ? cout << " " : cout << "";
            (curr->timeRemaining > 0) ? cout << " " : cout << ""; // I include these conditional statements in an attempt to align the table (had some OCD with the numbers not aligning)
            ((minute >= 10) && (curr->timeRemaining > 0)) ? cout << " " : cout << "";
            cout << "            " << curr->number << "          " << curr->entryMinute << "            " << curr->serviceDuration << "             " << curr->timeRemaining << "              " << (curr->serviceDuration) + (curr->timeRemaining) << endl;
            curr = curr->next;
        }
    }
}


int main(int argc, const char * argv[]) {
    int runTime = 0;
    int timeBetweenEntering = 0; // define variables for main function
    int maxServiceTime = 0;
    cout << "Welcome to the Supermarket Queue program!" << endl;
    cout << "Please enter the following information.  Each number inputted should be an integer." << endl;
    cout << "Number of minutes for the simulation to run: ";
    cin >> runTime;
    cout << "Maximum number of minutes between people entering the queue (greater than 0): ";
    cin >> timeBetweenEntering;
    cout << "Maximum number of minutes to serve a customer: ";
    cin >> maxServiceTime;
    if (!cin.eof() && cin.good()) {
        Queue queue;
        int numCustomers = 0;
        queue.enterCustomer(0, maxServiceTime, numCustomers); // put a first customer into the queue to start
        numCustomers += 1;
        int timeUntilNextEnter = (rand() % timeBetweenEntering) + 1; // randomly determine time until next customer enter
        cout << "Minute    Customer    Entry     Service Time   Wait Time   Total Minutes Remaining" << endl;
        cout << "Number     Number     Minute      Remaining    Remaining   Until Service is Completed" << endl;    // display the top of the table
        for (int i = 0; i < runTime; i++) {
            queue.displayQueue(i); // first, display the current queue at minute i
            if (queue.front != NULL) { // then, if queue is not empty, beging updating service and wait times
                queue.front->serviceDuration -= 1; // subtract 1 minute from front customer's service duration
                Customer* curr = queue.front->next;
                while (curr != NULL) { // then, iterate through the rest of the queue and update wait times
                    curr->timeRemaining -= 1; // subtract 1 minute from the wait time
                    curr = curr->next;
                }
                if (queue.front->serviceDuration == 0) { // customer exits queue if service time hits 0
                    queue.exitCustomer();
                }
            }
            if (timeUntilNextEnter == 0) { // enter a new customer when the randomly generated time hits 0
                queue.enterCustomer(i+1, maxServiceTime, numCustomers);
                numCustomers += 1;
                timeUntilNextEnter = (rand() % timeBetweenEntering) + 1; // randomly determine time until next customer enter
            }
            else if (timeUntilNextEnter != 0) { // if it's not 0, subtract by 1
                timeUntilNextEnter -= 1;
            }
        }
        cout << endl << "    Thank you for using the program.  Terminating..." << endl;
    }
    else if (!cin.good()) {
        cout << endl << "Invalid input entered.  Terminating..." << endl;
    }
    else if (cin.eof()) {
        cout << endl << "End of file entered.  Terminating..." << endl;
    }
}
