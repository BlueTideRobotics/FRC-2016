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

	Timer autonTimer;

	AnalogGyro gyro;
	PID gyroPID;
	bool gyroPIDrunning;
	double gyroPIDoutput;
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

		autonTimer(),

		gyro(0),
		gyroPID(0,0,0,360,360),
		gyroPIDrunning(false),
		gyroPIDoutput(0.0)
	{

	}

private:
	void RobotInit(){
		myRobot.SetSafetyEnabled(false);
		myRobot.SetExpiration(0.1); // watchdog is the WORST THING EVER

		gyro.Calibrate();

		SmartDashboard::PutNumber("intake power", 0.5);
		SmartDashboard::PutNumber("shot power", 1.0);

		SmartDashboard::PutNumber("gyro P", 1);
		SmartDashboard::PutNumber("gyro I", 0);
		SmartDashboard::PutNumber("gyro D", 0);
	}

	void __goalTracker(float& centerX, float& centerY) {
		/* TODO */
		centerX = 0.0;
		centerY = 0.0;
	}
	float getGoalAngleOffset() {
		/* TODO */
		float x,y;
		__goalTracker(x,y);

		// math your way from the x-val to an offset angle?

		return 0.0;
	}
	float getRequiredShotPower() {
		/* TODO */
		float x,y;
		__goalTracker(x,y);
		return 0.0;
	}
	bool isValidShot() {
		/* TODO */
		// y-range with the __goalTracker
		return true;
	}
	void goToAngleWithPID(float angle) {
		/* TODO at competition */

		// gyro PID
		myRobot.ArcadeDrive(0.0, 0.0);

		if(!gyroPIDrunning)
			gyroPID.ResetPID();

		gyroPID.Enable();
		gyroPID.SetPID(
			SmartDashboard::GetNumber("gyro P", 1),
			SmartDashboard::GetNumber("gyro I", 0),
			SmartDashboard::GetNumber("gyro D", 0)
		);

		gyroPID.SetEpsilon(0);
		gyroPID.SetMaxChangeSetpoint(0.01);

		gyroPID.SetSetpoint(angle);

		// might need to trueMap/constrain this
		gyroPIDoutput=gyroPID.GetOutput(gyro.GetAngle(),NULL,NULL,NULL);
		SmartDashboard::PutNumber("gyroPID Output",gyroPIDoutput);

//		if(gyroPIDrunning)
//			myRobot.ArcadeDrive(0, float(gyroPIDoutput));
//		else
			myRobot.ArcadeDrive(0.0,0.0);
		gyroPIDrunning = true;
	}

	void AutonomousInit()
	{
		autonTimer.Reset();
		autonTimer.Start();

		// TODO: auton chooser?  Like from the sample
	}

	void AutonomousPeriodic()
	{
		if (autonTimer.Get() < 2)
			myRobot.ArcadeDrive(1.0, 0.0);
		else
			myRobot.ArcadeDrive(0.0, 0.0);
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		if(stick.GetRawButton(1))
			goToAngleWithPID(gyro.GetAngle() + getGoalAngleOffset());
		else {
			myRobot.ArcadeDrive(-stick.GetY(), -stick.GetRawAxis(5));

			gyroPIDrunning = false;
			gyroPID.Disable();
		}

		if (stick.GetRawButton(4))
			sageArmSetVal = 0.45;
		else if (stick.GetRawButton(3))
			sageArmSetVal = -1.0;
		else
			sageArmSetVal = 0.0;

		winchSetVal = 0.0;
		whipSetVal = 0.0;
		if (stick.GetRawButton(5) && stick.GetRawButton(1))
			winchSetVal = 1.0;
		else if (stick.GetRawButton(5))
			whipSetVal = 1.0;

		if(stick.GetRawButton(6))
			intakeSetVal = -SmartDashboard::GetNumber("intake power", 0.5);
		else
			intakeSetVal = 0.0;

		if (stick.GetRawButton(2) && !shootingNow) {
			shootingNow = true;
			shooterTimer.Reset();
			shooterTimer.Start();

			shotPower = SmartDashboard::GetNumber("shot power", 1.0);
			// TODO: vision processing and things (maybe)
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
		else {
			shooterSetVal = 0.0;
			intakeSetVal = 0.0;
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
