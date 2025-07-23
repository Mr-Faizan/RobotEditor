#ifndef JSONKEYS_H
#define JSONKEYS_H

#include <QString>


namespace RobotKeys {
const QString Robot = "Robot";
const QString RobotName = "Robot Name";
const QString RobotManufacturer = "Robot Manufacturer";
const QString RobotPayload = "Robot Payload (Kg)";
const QString RobotFootprint = "Robot Footprint Ø (mm)";
const QString RobotMaxReach = "Robot Maximum Reach (mm)";
const QString RobotRepeatability = "Robot Repeatability (mm)";
const QString RobotWeight = "Robot Weight (Kg)";
const QString DOF = "Robot DOF";
const QString Joints = "Joints";
const QString Visualization = "Visualization";
}

namespace JointKeys {
const QString Joint = "Joint 1";
const QString JointName = "Joint Name";
const QString MotionRangeMax = "Maximum Joint Limit (deg)";
const QString MotionRangeMin = "Minimum Joint Limit (deg)";
const QString JointSpeedLimit = "Joint Speed Limit (deg/s)";
const QString FrictionCoefficient = "Friction Coefficient (N.m)";
const QString StiffnessCoefficient = "Stiffness Coefficient (N/m)";
const QString DampingCoefficient = "Damping Coefficient (Ns/m)";
const QString JointKinematics = "Joint Kinematics";
const QString JointDynamics = "Joint Dynamics";
const QString Visualization = "Joint Visualization";
const string JointTranslation = "Joint Translation";
const string JointRotation = "Joint Rotation";
}

namespace KinematicsKeys {
const QString DhParameters = "DH Parameters";
const QString RotationalValues = "Rotational Values";
}

namespace DhParametersKeys {
const QString Alpha = "Alpha (deg)";
const QString D = "D (m)";
const QString Theta = "Theta (deg)";
const QString A = "A (m)";
const QString DHType = "DH Type";
}

namespace RotationalValuesKeys {
const QString Ixx = "Ixx (kg.m²)";
const QString Ixy = "Ixy (kg.m²)";
const QString Ixz = "Ixz (kg.m²)";
const QString Iyy = "Iyy (kg.m²)";
const QString Iyz = "Iyz (kg.m²)";
const QString Izz = "Izz (kg.m²)";
}

namespace DynamicsKeys {
const QString Payload = "Payload";
const QString TestPayload = "Test Payload (kg)";
const QString PayloadPercentage = "Payload Percentage";
const QString RepeatabilityPercentage = "Repeatability Percentage";
const QString SpeedPercentage = "Speed Percentage";
const QString BreakingDistance = "Breaking Distance (m)";
const QString BreakingTime = "Breaking Time (s)";
}

namespace VisualizationKeys {
const QString PathToObjFile = "Path to OBJ File";
const QString PathToMltFile = "Path to MTL File";

}

#endif // JSONKEYS_H
