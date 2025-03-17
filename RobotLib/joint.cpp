#include "joint.h"

using namespace std;

Joint::Joint(const string &jointNumber, const string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization)
    : jointNumber(jointNumber),
      name(name),
      motionRangeMin(motionRangeMin),
      motionRangeMax(motionRangeMax),
      jointSpeedLimit(jointSpeedLimit),
      frictionCoefficient(frictionCoefficient),
      stiffnessCoefficient(stiffnessCoefficient),
      dampingCoefficient(dampingCoefficient),
      visualization(visualization)
{
  // Add a default JointDynamics object
  dynamics.push_back(JointDynamics());
}
