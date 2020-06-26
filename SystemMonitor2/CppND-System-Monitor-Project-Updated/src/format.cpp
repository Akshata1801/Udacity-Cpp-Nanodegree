 #include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::string forma;
    int hours,minutes,sec;
    hours = seconds / 3600;
    int rem = seconds % 3600;
    minutes = rem / 60;
    rem = rem % 60;
    sec = rem;
    forma = std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(sec);
    return forma; }