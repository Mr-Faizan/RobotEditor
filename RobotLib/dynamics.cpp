#include "dynamics.h"

JointDynamics::JointDynamics(double testPayload, double payloadPercentage, double reachabilityPercentage, double speedPercentage, double breakingDistance, double breakingTime)
    : testPayload(testPayload),
      payloadPercentage(payloadPercentage),
      reachabilityPercentage(reachabilityPercentage),
      speedPercentage(speedPercentage),
      breakingDistance(breakingDistance),
      breakingTime(breakingTime) {}

void JointDynamics::calculateDynamics()
{
    // Implementation of dynamics calculation
}

void RobotDynamics::calculateDynamics()
{
    // Implementation
}

void ToolDynamics::calculateDynamics()
{
    // Implementation
}
