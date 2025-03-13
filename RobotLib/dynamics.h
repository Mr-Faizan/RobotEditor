#ifndef DYNAMICS_H
#define DYNAMICS_H

/*
Implementation Structure.
Dynamics can be for the Robot or for the Joints or for the Tools.
So I will implement the concept of Inheritance for the Dynamics.
As Dynamics is the base class and JointDynamics is the derived class.
And I will also implement the concept of Abrstraction and Polymorphism for the Dynamics.
As Dynamics is the abstract class and it will contain the common functionalities for the Dynamics.
And JointDynamics, ToolDynamics, and RobotDynamics will implement the common functionalities of the Dynamics.
And Also can override the functionalities according to their requirements.

*/

// This class will responsible for handling the functionality that is common in Robot, Joint, and Tool.
class Dynamics
{
public:

    virtual void calculateDynamics() = 0;
    virtual ~Dynamics() = default; 
};

// This class will responsible for calculating the Dynamics of the Joint.
class JointDynamics : public Dynamics
{
private:
    double testPayload;
    double payloadPercentage;
    double reachabilityPercentage;
    double speedPercentage;
    double breakingDistance;
    double breakingTime;

public:
    JointDynamics()
        : testPayload(0),
          payloadPercentage(0),
          reachabilityPercentage(0),
          speedPercentage(0),
          breakingDistance(0),
          breakingTime(0) {}

    JointDynamics(double testPayload, double payloadPercentage, double reachabilityPercentage, double speedPercentage, double breakingDistance, double breakingTime)
        : testPayload(testPayload),
          payloadPercentage(payloadPercentage),
          reachabilityPercentage(reachabilityPercentage),
          speedPercentage(speedPercentage),
          breakingDistance(breakingDistance),
          breakingTime(breakingTime) {}

    void calculateDynamics() override
    {
        // Implementation of dynamics calculation
    }

    // Setters
    void setTestPayload(double testPayload) { this->testPayload = testPayload; }
    void setPayloadPercentage(double payloadPercentage) { this->payloadPercentage = payloadPercentage; }
    void setReachabilityPercentage(double reachabilityPercentage) { this->reachabilityPercentage = reachabilityPercentage; }
    void setSpeedPercentage(double speedPercentage) { this->speedPercentage = speedPercentage; }
    void setBreakingDistance(double breakingDistance) { this->breakingDistance = breakingDistance; }
    void setBreakingTime(double breakingTime) { this->breakingTime = breakingTime; }

    // Getters
    double getTestPayload() const { return testPayload; }
    double getPayloadPercentage() const { return payloadPercentage; }
    double getReachabilityPercentage() const { return reachabilityPercentage; }
    double getSpeedPercentage() const { return speedPercentage; }
    double getBreakingDistance() const { return breakingDistance; }
    double getBreakingTime() const { return breakingTime; }
};

// This class will responsible for calculating the Dynamics of the Robot.
class RobotDynamics : public Dynamics
{
public:
    void calculateDynamics() override
    {
        // Implementation
    }
};

// This class will responsible for calculating the Dynamics of the Tool.
class ToolDynamics : public Dynamics
{
public:
    void calculateDynamics() override
    {
        // Implementation
    }
};

#endif
