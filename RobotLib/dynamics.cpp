#include "dynamics.h"


JointDynamics::JointDynamics()
    : payloadNumber(""),
      testPayload(0),
      payloadPercentage(0),
      reachabilityPercentage(0),
      speedPercentage(0),
      breakingDistance(0),
      breakingTime(0)
{
}

JointDynamics::JointDynamics(std::string payloadNumber, double testPayload, double payloadPercentage, double reachabilityPercentage, double speedPercentage, double breakingDistance, double breakingTime)
    : payloadNumber(""),
      testPayload(testPayload),
      payloadPercentage(payloadPercentage),
      reachabilityPercentage(reachabilityPercentage),
      speedPercentage(speedPercentage),
      breakingDistance(breakingDistance),
      breakingTime(breakingTime)
{
}

