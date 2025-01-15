#include "kinematics.h"

JointKinematics::JointKinematics()
    : modifiedDh(false) {}

JointKinematics::JointKinematics(const DHParameters &dhParameters, const RotationalValues &rotationalValues, const QString &dhType, bool modifiedDh)
    : dhParameters(dhParameters),
      rotationalValues(rotationalValues),
      dhType(dhType),
      modifiedDh(modifiedDh) {}

void JointKinematics::calculateDhParams()
{
    // Implementation of DH parameter calculations
}

void RobotKinematics::calculateDhParams()
{
    // Implemention
}

void JointKinematics::calculateDhParams()
{
    // Implemention
}
