#ifndef JSONKEYS_H
#define JSONKEYS_H

#include <QString>


namespace RobotKeys {
const QString Robot = "Robot";
const QString RobotName = "Robot Name";
const QString RobotManufacturer = "Robot Manufacturer";
const QString RobotPayload = "Robot Payload";
const QString RobotFootprint = "Robot Footprint";
const QString RobotMaxReach = "Robot MaxReach";
const QString RobotRepeatability = "Robot Repeatability";
const QString RobotWeight = "Robot Weight";
const QString DOF = "DOF";
const QString Joints = "Joints";
const QString Visualization = "Visualization";
}

namespace JointKeys {
const QString JointName = "Joint Name";
const QString MotionRangeMax = "MotionRangeMax";
const QString MotionRangeMin = "MotionRangeMin";
const QString JointSpeedLimit = "JointSpeedLimit";
const QString FrictionCoefficient = "FrictionCoefficient";
const QString StiffnessCoefficient = "StiffnessCoefficient";
const QString DampingCoefficient = "DampingCoefficient";
const QString JointKinematics = "JointKinematics";
const QString JointDynamics = "JointDynamics";
const QString JointVisualization = "Joint Visualization";
}

namespace KinematicsKeys {
const QString DhParameters = "DhParameters";
const QString RotationalValues = "RotationalValues";
}

namespace DhParametersKeys {
const QString Alpha = "Alpha (deg)";
const QString D = "D (m)";
const QString Theta = "Theta (deg)";
const QString A = "A (m)";
const QString DHType = "DH Type";
}

namespace RotationalValuesKeys {
const QString Ixx = "Ixx (kg.m)";
const QString Ixy = "Ixy (kg.m)";
const QString Ixz = "Ixz (kg.m)";
const QString Iyy = "Iyy (kg.m)";
const QString Iyz = "Iyz (kg.m)";
const QString Izz = "Izz (kg.m)";
}

namespace DynamicsKeys {
const QString TestPayload = "Test Payload (kg)";
const QString PayloadPercentage = "Payload Percentage";
const QString RepeatabilityPercentage = "Repeatability Percentage";
const QString SpeedPercentage = "Speed Percentage";
const QString BreakingDistance = "Breaking Distance";
const QString BreakingTime = "Breaking Time";
}

namespace VisualizationKeys {
const QString PathToObjFile = "path to obj file";
const QString PathToMltFile = "path to mlt file";
}

#endif // JSONKEYS_H
