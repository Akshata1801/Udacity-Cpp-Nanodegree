#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

//template<typename t, typename s>
bool CompareProcess(Process& a, Process& b)
{
    //std::cout<<"a.cpu: "<<a.CpuUtilization()<<std::endl;
   // std::cout<<"b.cpu: "<<b.CpuUtilization()<<std::endl;
   bool value = a < b;
   
    return value;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pids= LinuxParser::Pids();
    for (int pid : pids)
    {
        processes_.push_back(Process(pid));
    }
    std::sort(processes_.begin(),processes_.end(),CompareProcess);
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    std::string ker = LinuxParser::Kernel();
    return ker; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    float memused = LinuxParser::MemoryUtilization();
    //std::cout<<"Memefree: "<<memfree<<std::endl;
    return memused; 
    }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    std::string operating = LinuxParser::OperatingSystem();
    return operating; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    int run_procs = LinuxParser::RunningProcesses();
    return run_procs; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    int tot_procs = LinuxParser::TotalProcesses();
    return tot_procs; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    float time = LinuxParser::UpTime();
    //std::cout<<"time: "<<time<<std::endl;
    return time; }