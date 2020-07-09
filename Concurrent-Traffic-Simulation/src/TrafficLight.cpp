#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include <algorithm>

#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

        std::unique_lock<std::mutex> uLock(_mutexMsg);
        _conditionMsg.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = std::move(_queue.back());
        _queue.pop_back();

        return msg;
    

}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutexMsg);

    // add vector to queue
    //std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
    _queue.push_back(std::move(msg));
    _conditionMsg.notify_one();
}


/* Implementation of class "TrafficLight" */
 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}




void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    std::chrono::time_point<std::chrono::system_clock> start, end; 

    start = std::chrono::system_clock::now();
    while(true)
    {
       std::cout<<"wait for green"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        end = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(diff >= 1)
        {
            if(_queueTra.receive() ==  TrafficLightPhase::green)
            {
                std::cout<<"Traffic light granted green"<<std::endl;
                return;
            }
        }
    }
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a 
    //thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 

    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    std::random_device dev;  // https://stackoverflow.com/a/13445752
    std::mt19937 rng(dev()); // Random number generator
    std::uniform_int_distribution<std::mt19937::result_type> distribution(4000, 6000); // dist betweeen 4000ms and 6000ms
    int lastCycleDuration = distribution(rng); // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> start, end;

    // init stop watch
    start = std::chrono::system_clock::now();

    while(true)
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        end = std::chrono::system_clock::now();

        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        // std::cout << "diff is: "<<diff<<std::endl;
        // std::cout << "lastcycleUpdate : "<<lastCycleDuration<<std::endl;

        if(diff >= lastCycleDuration)
        {
            std::cout<<"Entered in if statement"<<std::endl;
            // if(_currentPhase == TrafficLightPhase::red)
            //     _currentPhase = TrafficLightPhase::green;
            // else (_currentPhase == TrafficLightPhase::green)
            //     _currentPhase = TrafficLightPhase::red;

            _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;

            std::cout<<"current pahse "<<_currentPhase<<std::endl;
            
            _queueTra.send(std::move(_currentPhase));

            start = std::chrono::system_clock::now();


            lastCycleDuration = distribution(rng);

        }
    }
}

