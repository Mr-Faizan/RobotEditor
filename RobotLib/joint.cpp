#include "joint.h"

using namespace std;


Joint::Joint()
: jointNumber(""),
  name(""),
  motionRangeMin(0),
  motionRangeMax(0),
  jointSpeedLimit(0),
  frictionCoefficient(0),
  stiffnessCoefficient(0),
  dampingCoefficient(0),
  visualization(""),
  payloadCounter(0),
  translation({0, 0, 0}),
  rotation({0, 0, 0})
{
// Add a default JointDynamics object
//dynamics.push_back(JointDynamics());
}


Joint::Joint(string &name)
: jointNumber(""),
  name(name),
  motionRangeMin(0),
  motionRangeMax(0),
  jointSpeedLimit(0),
  frictionCoefficient(0),
  stiffnessCoefficient(0),
  dampingCoefficient(0),
  visualization(""),
  payloadCounter(0),
  translation({0, 0, 0}),
  rotation({0, 0, 0})
{
// Add a default JointDynamics object
//dynamics.push_back(JointDynamics());
}

Joint::Joint(string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization)
    : name(name),
      motionRangeMin(motionRangeMin),
      motionRangeMax(motionRangeMax),
      jointSpeedLimit(jointSpeedLimit),
      frictionCoefficient(frictionCoefficient),
      stiffnessCoefficient(stiffnessCoefficient),
      dampingCoefficient(dampingCoefficient),
      visualization(visualization)
{
  // Add a default JointDynamics object
  //dynamics.push_back(JointDynamics());
}

void Joint::addDynamics(JointDynamics &dynamics)
{
    dynamics.setPayloadNumber(DynamicsKeys2::Payload + std::to_string(++payloadCounter));
    this->dynamics.push_back(dynamics);
}
    

JointDynamics& Joint::createAndAddDynamics()
{
    JointDynamics newPayload;
    dynamics.push_back(newPayload);

    return dynamics.back();
}


JointDynamics& Joint::setDynamics(JointDynamics &payload)
{
    //dynamics.setPayloadNumber(DynamicsKeys2::Payload + std::to_string(++payloadCounter));
    this->dynamics.push_back(payload);
    return dynamics.back();
}

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
