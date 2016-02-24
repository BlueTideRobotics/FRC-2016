#include "WPILib.h"
#include "PID.h"
#include "SageFunctions.h"

/*
 * NAME IDEAS
 *
 * Thor
 * Goldfinger
 * White Knight
 * The GOP
 * Midas
 * Meme Machine
 */

class Robot: public IterativeRobot
{
private:
	Joystick stick;

	CANTalon frontLeft, frontRight, rearLeft, rearRight;
	RobotDrive myRobot;

	CANTalon sageArm;
	float sageArmSetVal;

	CANTalon winch1, winch2;
	float winchSetVal;

	CANTalon whip;
	float whipSetVal;

	float testMotorSetVal;
	SendableChooser *chooser;
	const std::string motorNameSage = "Sage arm";
	const std::string motorNameWinch = "Winch";
	const std::string motorNameWhip = "Whip";
	std::string testMotorSelected;

	AnalogGyro gyro;
public:
	Robot():
		stick(0),

		frontLeft(17),
		frontRight(16),
		rearLeft(13),
		rearRight(14),

		myRobot(frontLeft, rearLeft, frontRight, rearRight),

		sageArm(20),
		sageArmSetVal(0.0),

		winch1(18),
		winch2(19),
		winchSetVal(0.0),

		whip(15),
		whipSetVal(0.0),

		testMotorSetVal(0.0),

		gyro(0)
	{

	}

private:
	void RobotInit(){
		myRobot.SetSafetyEnabled(false);
		myRobot.SetExpiration(0.1); // watchdog is the WORST THING EVER

		gyro.Calibrate();

		chooser = new SendableChooser();
		chooser->AddDefault(motorNameSage, (void*)&motorNameSage);
		chooser->AddObject(motorNameWinch, (void*)&motorNameWinch);
		chooser->AddObject(motorNameWhip, (void*)&motorNameWhip);
		SmartDashboard::PutData("Test-motor mechanism:", chooser);
	}
	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{
		//rearLeft.Set( frontLeft.Get() * 11.0 / 13);
		//rearRight.Set( frontRight.Get() * 11.0 / 13);
		// separate RobotDrive or something? This may be an issue.
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(-stick.GetY(), -stick.GetRawAxis(5));
		///////////////////////////////////////////////
		//THE FRONT IS THE SIDE THAT ISNT OPEN LOL XD//
		///////////////////////////////////////////////

		// COPY THIS FOR OTHER MOTORS AFTER TESTING
		// Sage arm (test for vals)
		/*if (stick.GetRawButton(4))
			sageArmSetVal = 0.45;
		else if (stick.GetRawButton(3))
			sageArmSetVal = -1.0;
		else
			sageArmSetVal = 0.0;*/

		SmartDashboard::PutNumber("Impending test-motor val", stick.GetRawAxis(2));
		SmartDashboard::PutBoolean("Test-motor activated", stick.GetRawButton(7));
		if (stick.GetRawButton(7))
			testMotorSetVal=-stick.GetRawAxis(2);
		else
			testMotorSetVal = 0.0;
		testMotorSelected = *((std::string*)chooser->GetSelected());
		if (testMotorSelected == motorNameSage)
			sageArmSetVal = testMotorSetVal;
		else if (testMotorSelected == motorNameWinch)
			winchSetVal = testMotorSetVal;
		else if (testMotorSelected == motorNameWhip)
			whipSetVal = testMotorSetVal;

		sageArm.Set(sageArmSetVal);
		winch1.Set(winchSetVal);
		winch2.Set(winchSetVal);
		whip.Set(whipSetVal);

		SmartDashboard::PutNumber("Gyro", gyro.GetAngle());
	}

	void TestPeriodic()
	{
	}
};

START_ROBOT_CLASS(Robot)
