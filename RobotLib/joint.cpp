/**
 * @file joint.cpp
 * @brief Implementation of Joint class for robot joint properties and dynamics.
 *
 * This file contains the implementation of the Joint class, which manages properties,
 * kinematics, dynamics, and visualization data for a robot joint. It provides methods
 * for setting and getting joint parameters, managing kinematics and dynamics, and
 * handling visualizations and payloads.
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

#include "joint.h"

using namespace std;

/**
 * @brief Default constructor for Joint.
 */
Joint::Joint()
: jointNumber(""),
  name(""),
  motionRangeMin(0),
  motionRangeMax(0),
  jointSpeedLimit(0),
  frictionCoefficient(0),
  stiffnessCoefficient(0),
  dampingCoefficient(0),
  visualizations{},
  payloadCounter(0),
  translation({0, 0, 0}),
  rotation({0, 0, 0})
{
    // Add a default JointDynamics object
    //dynamics.push_back(JointDynamics());
}

/**
 * @brief Constructor for Joint with name.
 * @param name Name of the joint.
 */
Joint::Joint(string &name)
: jointNumber(""),
  name(name),
  motionRangeMin(0),
  motionRangeMax(0),
  jointSpeedLimit(0),
  frictionCoefficient(0),
  stiffnessCoefficient(0),
  dampingCoefficient(0),
  visualizations{},
  payloadCounter(0),
  translation({0, 0, 0}),
  rotation({0, 0, 0})
{
    // Add a default JointDynamics object
    //dynamics.push_back(JointDynamics());
}

/**
 * @brief Constructor for Joint with all main properties and visualization.
 * @param name Name of the joint.
 * @param motionRangeMin Minimum motion range.
 * @param motionRangeMax Maximum motion range.
 * @param jointSpeedLimit Joint speed limit.
 * @param frictionCoefficient Friction coefficient.
 * @param stiffnessCoefficient Stiffness coefficient.
 * @param dampingCoefficient Damping coefficient.
 * @param visualization Visualization file path.
 */
Joint::Joint(string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization)
    : name(name),
      motionRangeMin(motionRangeMin),
      motionRangeMax(motionRangeMax),
      jointSpeedLimit(jointSpeedLimit),
      frictionCoefficient(frictionCoefficient),
      stiffnessCoefficient(stiffnessCoefficient),
      dampingCoefficient(dampingCoefficient),
      visualizations{ {visualization, visualization} }
{
    // Add a default JointDynamics object
    //dynamics.push_back(JointDynamics());
}

/**
 * @brief Add a JointDynamics object to the joint.
 * @param dynamics JointDynamics object to add.
 */
void Joint::addDynamics(JointDynamics &dynamics)
{
    dynamics.setPayloadNumber(DynamicsKeys2::Payload + std::to_string(++payloadCounter));
    this->dynamics.push_back(dynamics);
}

/**
 * @brief Create and add a new JointDynamics object to the joint.
 * @return Reference to the newly added JointDynamics object.
 */
JointDynamics& Joint::createAndAddDynamics()
{
    JointDynamics newPayload;
    dynamics.push_back(newPayload);
    return dynamics.back();
}

/**
 * @brief Add a JointDynamics payload to the joint.
 * @param payload JointDynamics object to add.
 * @return Reference to the newly added JointDynamics object.
 */
JointDynamics& Joint::setDynamics(JointDynamics &payload)
{
    //dynamics.setPayloadNumber(DynamicsKeys2::Payload + std::to_string(++payloadCounter));
    this->dynamics.push_back(payload);
    return dynamics.back();
}

/**
 * @brief Get a JointDynamics object by its payload number.
 * @param payloadNumber Payload number identifier.
 * @return Reference to the matching JointDynamics object.
 * @throws std::runtime_error if not found.
 */
JointDynamics& Joint::getDynamicsByPayloadNumber(const std::string &payloadNumber)
{
    for (auto &dynamics : dynamics)
    {
        if (dynamics.getPayloadNumber() == payloadNumber)
        {
            return dynamics;
        }
    }
    throw std::runtime_error("Payload of Joint Dynamics not found");
}
