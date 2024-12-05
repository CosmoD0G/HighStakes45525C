/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       acnef                                                     */
/*    Created:      9/25/2024, 8:45:38 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::controller Controller = controller(primary);
vex::motor M_6Bar = motor(PORT3,ratio18_1,false);
vex::motor M_Intake = motor(PORT9,ratio36_1,false);
vex::motor M_IntakeS1 = motor(PORT4,ratio18_1,false);
vex::motor M_DriveLF = motor(PORT8,ratio18_1,false);
vex::motor M_DriveLB = motor(PORT7,ratio18_1,false);
vex::motor M_DriveRF = motor(PORT6,ratio18_1,false);
vex::motor M_DriveRB = motor(PORT5,ratio18_1,false);
vex::motor_group MG_DriveLeft = motor_group(M_DriveLF,M_DriveLB);
vex::motor_group MG_DriveRight = motor_group(M_DriveRF,M_DriveRB);
vex::inertial INERTIAL = inertial(PORT2);

vex::digital_out P_MogoA = digital_out(Brain.ThreeWirePort.A);
vex::digital_out P_MogoB = digital_out(Brain.ThreeWirePort.B);
vex::digital_out P_Sweeper = digital_out(Brain.ThreeWirePort.C);
// define your global instances of motors and other devices here

void sixBar() {
    M_6Bar.setStopping(hold);
    if (Controller.ButtonL2.pressing() == Controller.ButtonL1.pressing()) {
        M_6Bar.stop();
    } else if (Controller.ButtonL2.pressing()) {
        M_6Bar.spin(reverse);
    } else {
        M_6Bar.spin(forward);
    }
}

int intake_direction = 0; // variable to determine intake direction -1 = reverse, 0 = stop, 1 = forward
void intakeFWD() {
    if (intake_direction == 1) {
        M_Intake.stop();
        M_IntakeS1.stop();
        intake_direction = 0;
    } else {
        M_Intake.spin(forward);
        M_IntakeS1.spin(forward);
        intake_direction = 1;
    }
}

void intakeREV() {
    if (intake_direction == -1) {
        M_Intake.stop();
        M_IntakeS1.stop();
        intake_direction = 0;
    } else {
        M_Intake.spin(reverse);
        M_IntakeS1.spin(reverse);
        intake_direction = -1;
    }
}

bool mogo_active = false;
void mogoToggle() {
    if (mogo_active) {
        P_MogoA.set(false);
        P_MogoB.set(false);
        mogo_active = false;
    } else {
        P_MogoA.set(true);
        P_MogoB.set(true);
        mogo_active = true;
    }
}

bool sweeper_active = true;
void sweeperToggle() {
    if (sweeper_active) {
        P_Sweeper.set(false);
        sweeper_active = false;
    } else {
        P_Sweeper.set(true);
        sweeper_active = true;
    }
}

int driver_direction = 1;
void driverToggle() {
    if (driver_direction == 1) {
        driver_direction = -1;
    } else {
        driver_direction = 1;
    }
}

void driverControl() {
    while (true) {// driver control loop

        sixBar();

        MG_DriveLeft.setVelocity(driver_direction*(Controller.Axis3.position() + Controller.Axis1.position()), percent);
        MG_DriveRight.setVelocity(driver_direction*(Controller.Axis3.position() - Controller.Axis1.position()), percent);
        MG_DriveLeft.spin(forward);
        MG_DriveRight.spin(forward);
    }

}

// -----AUTONOMOUS-----
void autoMove() {

}




int main() {

    M_Intake.setVelocity(300,rpm);
    M_IntakeS1.setVelocity(200,rpm);
    // define competition driver and auto events
    vex::competition Comp = competition();
    Comp.autonomous(driverControl);
    Comp.drivercontrol(driverControl);


    Brain.Screen.printAt( 10, 50, "Hello V5" );
   
    while(1) {
        
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

    // --CONTROLLER EVENTS--
    Controller.ButtonR2.pressed(intakeFWD);
    Controller.ButtonR1.pressed(intakeREV);
    Controller.ButtonDown.pressed(mogoToggle);
    Controller.ButtonB.pressed(sweeperToggle);
    Controller.ButtonA.pressed(driverToggle);
}
