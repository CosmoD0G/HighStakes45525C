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
vex::motor M_DriveLF = motor(PORT8,ratio18_1,true);
vex::motor M_DriveLB = motor(PORT7,ratio18_1,true);
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
    M_6Bar.setStopping(coast);
    M_6Bar.setVelocity(20,rpm);
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
    MG_DriveLeft.setStopping(coast);
    MG_DriveRight.setStopping(coast);
    while (true) {// driver control loop

        sixBar();

        MG_DriveLeft.setVelocity(driver_direction*(.0001)*pow((Controller.Axis3.position() + Controller.Axis1.position()),3), percent);
        MG_DriveRight.setVelocity(driver_direction*(.0001)*pow((Controller.Axis3.position() - Controller.Axis1.position()),3), percent);
        MG_DriveLeft.spin(forward);
        MG_DriveRight.spin(forward);
        Brain.Screen.clearScreen();
        Brain.Screen.print((Controller.Axis3.position() + Controller.Axis1.position()));
    }

}

// -----AUTONOMOUS-----
const float integral_margin_error = .1;
const float integral_MAX_VALUE = 50;
void autoMove(float dist) { //dist is in inches
    MG_DriveLeft.setVelocity(100,percent);
    MG_DriveRight.setVelocity(100,percent);
    Brain.Screen.print("go cowboys");

    float KP = 3;
    float KI = 1;
    float KD = .25;

    float initial_position = (M_DriveLB.position(turns));
    float target = initial_position+((dist/12.56));

    float error = (target-initial_position);
    float initial_error = error;
    float percent_error = 0;
    float PID = 0;

    float integral = 0;

    float derivative = 0;

    float prev_error = 0;
    Brain.Screen.print((dist/12.56));
    Brain.Screen.print(M_DriveLB.position(turns));
    Brain.Screen.print(error);

    while (abs(error) >= 0.1 || abs(M_DriveLB.velocity(percent)) >= 50) {
        //----PROPORTIONAL----
        error = target-(M_DriveLB.position(turns));

        percent_error = initial_error/error;

        //----INTEGRAL----
        integral += error;

        if (percent_error >= integral_margin_error || error <= 0) {
            integral = 0;
        } else if (integral > integral_MAX_VALUE) {
            integral = integral_MAX_VALUE;
        }

        //----DERIVATIVE----
        derivative = error-prev_error;
        prev_error=error;
        Brain.Screen.print(PID);
        PID = (error*KP)+(integral*KI)+(derivative*KD);
        MG_DriveLeft.spin(forward,PID,volt);
        MG_DriveRight.spin(forward,PID,volt);
    }
    MG_DriveLeft.stop();
    MG_DriveRight.stop();

}

void AUTO() {
    MG_DriveLeft.setStopping(brake);
    MG_DriveRight.setStopping(brake);
    autoMove(24);
}


int main() {

    M_Intake.setVelocity(75,rpm);
    M_IntakeS1.setVelocity(200,rpm);
    // define competition driver and auto events
    vex::competition Comp = competition();
    Comp.autonomous(AUTO);
    Comp.drivercontrol(driverControl);


    Brain.Screen.printAt( 10, 50, "Hello V5" );
   
    
    // --CONTROLLER EVENTS--
    Controller.ButtonR2.pressed(intakeFWD);
    Controller.ButtonR1.pressed(intakeREV);
    Controller.ButtonDown.pressed(mogoToggle);
    Controller.ButtonB.pressed(sweeperToggle);
    Controller.ButtonA.pressed(driverToggle);

    

    while(1) {
        
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

}
