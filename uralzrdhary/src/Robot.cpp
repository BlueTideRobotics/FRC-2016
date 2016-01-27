#include "WPILib.h"


class Robot: public IterativeRobot
{
private:
	Joystick stick;
	LiveWindow *lw = LiveWindow::GetInstance();

	CANJaguar frontLeft, frontRight, rearLeft, rearRight;
	RobotDrive myRobot;
public:
	Robot():
		stick(0),
		lw(NULL),

		frontLeft(11),
		frontRight(12),
		rearLeft(10),
		rearRight(13),

		myRobot(frontLeft, rearLeft, frontRight, rearRight)
	{
		lw = LiveWindow::GetInstance();
	}

private:
	void RobotInit(){
		myRobot.SetSafetyEnabled(false);
		myRobot.SetExpiration(0.1); // watchdog is the WORST THING EVER

		SmartDashboard::PutNumber("twist reduction factor", 1);
		SmartDashboard::PutNumber("Y reduction factor", 1);
	}
	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutNumber("stickY", stick.GetY());
		SmartDashboard::PutNumber("stickTwist", stick.GetRawAxis(5));

		myRobot.ArcadeDrive(
				-stick.GetY() ,//* SmartDashboard::GetNumber("Y reduction factor", 1),
				-stick.GetRawAxis(5) //* SmartDashboard::GetNumber("twist reduction factor", 1)
		);
	}

	void TestPeriodic()
	{
	}
};

START_ROBOT_CLASS(Robot)
