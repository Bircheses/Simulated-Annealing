#include "Counter.h"

void Counter::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Counter::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
}

double Counter::getElapsedTime(){
    endTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return elapsedTime;
}