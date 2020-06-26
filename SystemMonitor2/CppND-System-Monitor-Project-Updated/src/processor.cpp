#include "processor.h"
#include "linux_parser.h"

#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

     long int idle,non_idle,total;
     idle = LinuxParser::ActiveJiffies();
     non_idle = LinuxParser::IdleJiffies();
     total = LinuxParser::Jiffies();

     float actual = total - idle;

     // std::cout<<"idle: "<<idle<<std::endl;
     // std::cout<<"non_idle: "<<non_idle<<std::endl;

     // std::cout<<"total: "<<total<<std::endl;
     // std::cout<<"actual: "<<actual<<std::endl;

    float percent = actual/total;
    return percent; 
    }