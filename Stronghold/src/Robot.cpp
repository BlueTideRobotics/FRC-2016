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

	CANJaguar intake, shooter; // CHANGE TO CANTalon at competition
	float intakeSetVal, shooterSetVal;

	Timer shooterTimer;
	bool shootingNow;

	Servo shootServo;
	DigitalInput ballInfra;

	float shotPower;

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

		intake(11),
		shooter(10),
		intakeSetVal(0.0),
		shooterSetVal(0.0),

		shooterTimer(),
		shootingNow(false),

		shootServo(0),
		ballInfra(9),

		shotPower(0.0),

		gyro(0)
	{

	}

private:
	void RobotInit(){
		myRobot.SetSafetyEnabled(false);
		myRobot.SetExpiration(0.1); // watchdog is the WORST THING EVER

		gyro.Calibrate();

		SmartDashboard::PutNumber("intake power", 0.5);
		SmartDashboard::PutNumber("shot power", 1.0);
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

		sageArmSetVal = 0.0;
		winchSetVal = 0.0;
		winchSetVal = 0.0;
		whipSetVal = 0.0;
		shooterSetVal = 0.0;
		intakeSetVal = 0.0;

		if (stick.GetRawButton(4))
			sageArmSetVal = 0.45;
		else if (stick.GetRawButton(3))
			sageArmSetVal = -1.0;
		else
			sageArmSetVal = 0.0;

		shotPower = SmartDashboard::GetNumber("shot power", 1.0);
		// TODO: vision processing and things (maybe)

		if(stick.GetRawButton(6))
			intakeSetVal = -SmartDashboard::GetNumber("intake power", 0.5);
		else
			intakeSetVal = 0.0;

		if (stick.GetRawButton(2) && !shootingNow) {
			shootingNow = true;
			shooterTimer.Reset();
			shooterTimer.Start();
		}
		if (shootingNow) {
			if (shooterTimer.Get() < 1.5) {
				shooterSetVal = shotPower;
				intakeSetVal = -shotPower;

				if (shooterTimer.Get() > 1)
					shootServo.Set(1.0);
			}
			else {
				shootServo.Set(0.5);

				shooterTimer.Stop();
				shootingNow = false;
			}
		}

		SmartDashboard::PutBoolean("Ball loaded", ballInfra.Get());

		sageArm.Set(sageArmSetVal);
		winch1.Set(winchSetVal);
		winch2.Set(winchSetVal);
		whip.Set(whipSetVal);
		shooter.Set(shooterSetVal);
		intake.Set(intakeSetVal);

		SmartDashboard::PutNumber("Gyro", gyro.GetAngle());
	}

	void TestPeriodic()
	{
	}
};

START_ROBOT_CLASS(Robot)
