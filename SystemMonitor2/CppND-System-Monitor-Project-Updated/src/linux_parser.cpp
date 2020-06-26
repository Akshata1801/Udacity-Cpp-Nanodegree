#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string line;
  string key;
  float value;
  string kB;
  float memfree;
  float memtot,memused;
float memavail;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
      // line.erase(std::remove(line.begin(), line.end(), 'k'), line.end());
      // line.erase(std::remove(line.begin(), line.end(), 'B'), line.end());
      // line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
      //std::replace(line.begin(), line.end(), ' ', '_');
      // std::replace(line.begin(), line.end(), ':', '_');
      // std::replace(line.begin(), line.end(), 'k', '\0');
      // std::replace(line.begin(), line.end(), 'B', '\0');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          //std::cout<<"MemTotal : "<<std::endl;
          //std::replace(value.begin(), value.end(), '_', '\0');
          //std::cout<<value<<std::endl;
          memtot = (value);
        }
        if(key == "MemFree:"){ 
          //std::cout<<"MemFree : "<<value<<std::endl;
          //std::replace(value.begin(), value.end(), '_', '\0');
          //std::cout<<value<<std::endl;
          memfree = (value);
        }
      }
    }
  }
  memavail = memtot - memfree;
  memused = memavail/memtot;
  return memused;
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string os, kernel;
  string line;
  float time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time;
  }
  //std::cout<<"time : "<<time<<std::endl;
  return time; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return (ActiveJiffies()+IdleJiffies()); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  std::string line,value;
  std::string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,guest_nice;
  //long int x1,x2,x3,x4,x5,x6,x7,x8,x9,x10;
  // vector<std::string> values;
  float act_jiffies = 0;

  std::ifstream fstream(kProcDirectory + kStatFilename);
  if(fstream.is_open())
  {
    (getline(fstream,line));
    std::istringstream linestream(line);
    (linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice);
  }

  act_jiffies = std::stof(nice) + std::stof(system) + std::stof(irq) + std::stof(softirq) + std::stof(steal) + std::stof(guest) + std::stof(guest_nice);

  return (long int)(act_jiffies); 

  //return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  std::string line,value;
  std::string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,guest_nice;
  //vector<std::string> values_;
  float idle_jiffies = 0;

   std::ifstream fstream(kProcDirectory + kStatFilename);
  if(fstream.is_open())
  {
    //std::cout <<"Hello"<<std::endl;
    std::getline(fstream,line);
    std::istringstream linestream(line);
    (linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice);
  }


   //std::cout <<"iowait"<<(iowait)<<std::endl;

   idle_jiffies = std::stof(iowait) + std::stof(idle);

   return (long int)(idle_jiffies);
  
   //return 0;

  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  
  return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string key,line;
  int value;
  float tot_proc;
  if(filestream.is_open())
  {
    while(getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "processes")
        tot_proc = value;


      }
    }
  }

  //std::cout<<"total procs : "<<tot_proc<<std::endl;
  return tot_proc; 
  //return 0;
  
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string key,line;
  int value;
  float run_proc;
  if(filestream.is_open())
  {
    while(getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "procs_running")
        run_proc = value;
      }

      }
    }
  
  return run_proc;
//return 0;  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string path,line,command;

  path = kProcDirectory + std::to_string(pid) + kCmdlineFilename;

  std::ifstream fstream(path);
  if(fstream.is_open())
  {
   (std::getline(fstream,line));
    
  }

  
  return line;
  //return string();
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, key;
  float value,mem;

  std::ifstream fstream(kProcDirectory+std::to_string(pid)+"/status");
  if(fstream.is_open())
  {
      while(getline(fstream,line))
      {
        std::istringstream linestream(line);
        while(linestream >> key >>value)
        {
          if(key == "VmSize:")
            mem = (value);
        }
        
      }
  }

  float ram = mem/1024;
  int tem = int(ram);
  
  return std::to_string(tem); 
  //return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line,key,user_id;
  std::string value;

  std::ifstream fstream(kProcDirectory+std::to_string(pid)+"/status");
  if(fstream.is_open())
  {
      while(getline(fstream,line))
      {
        std::istringstream linestream(line);
        while(linestream >> key >>value)
        {
          if(key == "Uid:")
            user_id = (value);
        }
        
      }
  }
  return user_id;
  //return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string line,user,key,temp,x1,x2,x;
  //int x2;
  
  std::string user_pid = LinuxParser::Uid(pid);
  //int user_id = std::stoi(user_pid);
  std::ifstream fstream(kPasswordPath);
  if (fstream.is_open())
  {
    while(getline(fstream,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> x >> x1)
      {
        if(x1 == user_pid)
         user = key;
      }
    }
  }
  return user;

  //return string();

  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime;
    std::vector<std::string> values;
    std::string line,element;
    std::string path = LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename;

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

    uptime = std::stol(values[21]);
  
  return uptime; 
  //return 0;
  }