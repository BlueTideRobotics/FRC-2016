#include <cstdint>

#include "WPILib.h"

#define FREQ 0.1

class Robot: public IterativeRobot
{
private:
	Joystick stick;

	CANTalon frontLeft, frontRight;
	CANJaguar rearLeft, rearRight;
	RobotDrive myRobot;

	DigitalOutput rave1, rave2; //, rave3;
	bool raveSet1, raveSet2, raveSet3;

	Timer raveTime;
public:
	Robot():
		stick(0),

		frontLeft(12),
		frontRight(16),
		rearLeft(10),
		rearRight(11),

		myRobot(frontLeft, rearLeft, frontRight, rearRight),

		rave1(0),
		rave2(1)
		//rave3(2)
	{
	}


	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{
		raveTime.Start();

		srand(raveTime.GetFPGATimestamp());
	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(-stick.GetY(), stick.GetRawAxis(5));

		SmartDashboard::PutNumber("stickY", stick.GetY());
		SmartDashboard::PutNumber("stickTwist", stick.GetRawAxis(5));

		SmartDashboard::PutNumber("frontLeft", frontLeft.Get());
		SmartDashboard::PutNumber("rearLeft",rearLeft.Get());
		SmartDashboard::PutNumber("frontRight", frontRight.Get());
		SmartDashboard::PutNumber("rearRight", rearRight.Get());

		if (raveTime.Get() > FREQ) {
			raveSet1 = rand() % 2 == 0;
			raveSet2 = rand() % 2 == 0;
			raveSet3 = rand() % 2 == 0;
			raveTime.Reset();
		}

		rave1.Set(raveSet1);
		rave2.Set(raveSet2);
		//rave3.Set(raveSet3);

		SmartDashboard::PutBoolean("rave1", raveSet1);
		SmartDashboard::PutBoolean("rave2", raveSet2);
		SmartDashboard::PutBoolean("rave3", raveSet3);

	}

	void TestPeriodic()
	{
	}
};

START_ROBOT_CLASS(Robot)
