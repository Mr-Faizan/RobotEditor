#include "joint.h"

#include <string>

using namespace std;

Joint::Joint(const string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient)
    : name(name),
      motionRangeMin(motionRangeMin),
      motionRangeMax(motionRangeMax),
      jointSpeedLimit(jointSpeedLimit),
      frictionCoefficient(frictionCoefficient),
      stiffnessCoefficient(stiffnessCoefficient),
      dampingCoefficient(dampingCoefficient),

      kinematics(nullptr),
      visualization(nullptr)
{
}

Joint::~Joint()
{
    delete kinematics;
    delete visualization;
}
