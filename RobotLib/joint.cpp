#include "joint.h"

using namespace std;

Joint::Joint()
    : name(""),
      motionRangeMin(0),
      motionRangeMax(0),
      jointSpeedLimit(0),
      frictionCoefficient(0),
      stiffnessCoefficient(0),
      dampingCoefficient(0),
      visualization("")
{
}

Joint::Joint(const string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization)
    : name(name),
      motionRangeMin(motionRangeMin),
      motionRangeMax(motionRangeMax),
      jointSpeedLimit(jointSpeedLimit),
      frictionCoefficient(frictionCoefficient),
      stiffnessCoefficient(stiffnessCoefficient),
      dampingCoefficient(dampingCoefficient),
      visualization(visualization)
{
}

