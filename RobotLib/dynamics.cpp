/**
 * @file dynamics.cpp
 * @brief Implementation of Dynamics classes for robot, joint, and tool dynamics calculations.
 *
 * This file contains the implementation of the JointDynamics, RobotDynamics, and ToolDynamics classes,
 * which provide methods for calculating dynamics properties for robots, joints, and tools. The JointDynamics
 * class includes constructors and member functions for managing joint-specific dynamics parameters.
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

#include "dynamics.h"

/**
 * @brief Default constructor for JointDynamics.
 */
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

/**
 * @brief Parameterized constructor for JointDynamics.
 * @param payloadNumber Identifier for the payload.
 * @param testPayload Test payload value.
 * @param payloadPercentage Payload percentage.
 * @param reachabilityPercentage Reachability percentage.
 * @param speedPercentage Speed percentage.
 * @param breakingDistance Breaking distance.
 * @param breakingTime Breaking time.
 */
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

