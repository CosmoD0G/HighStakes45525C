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



// should be true if creating a new route
bool AUTON_TESTING_MODE = false;
int route = 0;

// enum to determine what colors to print on screen
enum SelectorType {
    COLOR,
    SIDE,
    PROGRAM
};

// prints the auton interface buttons to the screen
void paintRectangles(SelectorType mode) {
    Brain.Screen.clearScreen();
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

// find the index of the button that was clicked
// if no button was clicked, return 4
int getClickRegion() {
    if ((Brain.Screen.xPosition() >= 3 && Brain.Screen.xPosition() <= 203) && 
    (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 125)) {
        return 0;
    } else if ((Brain.Screen.xPosition() >= 240 && Brain.Screen.xPosition() <= 440) && 
    (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 125)) {
        return 1;
    } else if ((Brain.Screen.xPosition() >= 3 && Brain.Screen.xPosition() <= 203) && 
    (Brain.Screen.yPosition() >= 165 && Brain.Screen.yPosition() <= 240)) {
        return 2;
    } else if ((Brain.Screen.xPosition() >= 240 && Brain.Screen.xPosition() <= 440) && 
    (Brain.Screen.yPosition() >= 165 && Brain.Screen.yPosition() <= 240)) {
        return 3;
    } else {
        return 4;
    }

}

// determine if a button exists for the region of the click
bool isRegionValid(SelectorType mode, int region) {
    int num_regions = 0;
    switch (mode) {
        case COLOR: num_regions = 2;
        case SIDE: num_regions = 1;
        case PROGRAM: num_regions = 3;
    }
    return region <= num_regions;
}

int selectAutonRoute() {
    int current_click_region = 4;
    int color = 4;
    int side = 4;
    int program = 4;

    paintRectangles(COLOR);
    while (true) {
        current_click_region = getClickRegion();
        if (isRegionValid(COLOR,current_click_region)) {
            color = current_click_region;
            break;
        }
    }
    current_click_region = 4;
    paintRectangles(SIDE);
    while (true) {
        current_click_region = getClickRegion();
        if (isRegionValid(SIDE,current_click_region)) {
            side = current_click_region;
            break;
        }
    }
    current_click_region = 4;
    paintRectangles(PROGRAM);
    while (true) {
        current_click_region = getClickRegion();
        if (isRegionValid(PROGRAM,current_click_region)) {
            program = current_click_region;
            break;
        }
    }

    switch (color) {
        case 0: //red
            switch (side) {
                case 0: //left
                    switch (program) {
                        case 0:
                            return 0;
                            break;
                        case 1:
                            return 1;
                            break;
                    }
                    break;
                case 1: //right
                    switch (program) {
                        case 0:
                            return 2;
                            break;
                        case 1:
                            return 3;
                            break;
                    }
                    break;
            }
            break;
        case 1: //blue
            switch (side) {
                case 0: //left
                    switch (program) {
                        case 0:
                            return 4;
                            break;
                        case 1:
                            return 5;
                            break;
                    }
                    break;
                case 1: //right
                    switch (program) {
                        case 0:
                            return 6;
                            break;
                        case 1:
                            return 7;
                            break;
                    }
                    break;
            }
            break;
        case 3: //skills
            break;
        } 
        
    }

void auton0() {
    
}

void auton1() {

}

void auton2() {
    
}

void auton3() {
    
}

void auton4() {
    
}

void auton5() {
    
}

void auton6() {
    
}

void auton7() {
    
}

void AUTO() {
    switch (route) {
        case 0: auton0(); break;
        case 1: auton1(); break;
        case 2: auton2(); break;
        case 3: auton3(); break;
        case 4: auton4(); break;
        case 5: auton5(); break;
        case 6: auton6(); break;
        case 7: auton7(); break;
    }
}

int main() {
    route = selectAutonRoute();

    while(1) {
        
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}