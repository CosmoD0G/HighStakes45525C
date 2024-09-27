/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       acnef                                                     */
/*    Created:      9/25/2024, 8:45:38 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include <string>

using namespace vex;
using namespace std;
// definition of all devices
brain Brain;
controller Controller1;

// define your global instances of motors and other devices here

enum PrintType {
    COLOR,
    SIDE,
    PROGRAM
};

bool AUTON_TESTING_MODE = false;

// prints the auton interface buttons to the screen
void paintRectangles(PrintType mode) {
    switch (mode) {
        case COLOR:
            Brain.Screen.setFillColor(red);
            Brain.Screen.drawRectangle(3,50,200,75);
            Brain.Screen.setFillColor(blue);
            Brain.Screen.drawRectangle(240,50,200,75);
            Brain.Screen.setFillColor(white);
            Brain.Screen.drawRectangle(3,165,200,75);
            break;
        case SIDE:
            Brain.Screen.setFillColor(green);
            Brain.Screen.drawRectangle(3,50,200,75);
            Brain.Screen.setFillColor(yellow);
            Brain.Screen.drawRectangle(240,50,200,75); 
            break; 
        case PROGRAM:
            Brain.Screen.setFillColor(orange);
            Brain.Screen.drawRectangle(3,50,200,75);
            Brain.Screen.setFillColor(purple);
            Brain.Screen.drawRectangle(240,50,200,75);
            Brain.Screen.setFillColor(white);
            Brain.Screen.drawRectangle(3,165,200,75);
            Brain.Screen.setFillColor(cyan);
            Brain.Screen.drawRectangle(240,165,200,75);
            break;
    }
}

bool isTouchWithinRange() {
    return false;
}







int main() {

    paintRectangles(COLOR);

    Brain.Screen.clearLine(0);
    Brain.Screen.setFillColor(transparent);
    Brain.Screen.print("select a color");






    while(1) {
        
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
