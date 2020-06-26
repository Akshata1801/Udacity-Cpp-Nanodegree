#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid)
{
   
}

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float uptime,utime,cstime,cutime,stime,starttime;
    std::vector<std::string> values;
    std::string line,element;
    std::string path = LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename;

    std::ifstream fstream(path);
    if(fstream.is_open())
    {
        while(getline(fstream,line))
        {
            std::istringstream linestream(line);
            while(linestream >> element)
            {
            values.push_back(element);
            }
        }
    }

    uptime = std::stof(values[0]);
    utime = std::stof(values[13]);
    stime = std::stof(values[14]);
    cutime = std::stof(values[15]);
    cstime = std::stof(values[16]);
    starttime = std::stof(values[21]);

    float tot_time = utime + stime + cutime + cstime;
    float secs = LinuxParser::UpTime() - (starttime/(sysconf(_SC_CLK_TCK)));
    //float secs = ;
    float cpu_usage = 100 * ((tot_time/(sysconf(_SC_CLK_TCK)))/secs);
    this->cpu_util = cpu_usage;
     return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    
    return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    std::string user_name = LinuxParser::User(Pid());
    //std::cout<<"user name"<<user_name<<std::endl;
    return user_name; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    
    return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    bool value = cpu_util > a.cpu_util;
    return value; }

