#ifndef ROBOTJSONKEYS_H
#define ROBOTJSONKEYS_H

#include <string>

using namespace std;

namespace RobotKeys2
{
    const string Robot = "Robot";
    const string RobotName = "Robot Name";
    const string RobotManufacturer = "Robot Manufacturer";
    const string RobotPayload = "Robot Payload (Kg)";
    const string RobotFootprint = "Robot Footprint Ø (mm)";
    const string RobotMaxReach = "Robot Maximum Reach (mm)";
    const string RobotRepeatability = "Robot Repeatability (mm)";
    const string RobotWeight = "Robot Weight (Kg)";
    const string DOF = "Robot DOF";
    const string Joints = "Joints";
    const string Visualization = "Visualization";
}

namespace JointKeys2
{
    const string Joint = "Joint 1";
    const string JointName = "Joint Name";
    const string MotionRangeMax = "Maximum Joint Limit (deg)";
    const string MotionRangeMin = "Minimum Joint Limit (deg)";
    const string JointSpeedLimit = "Joint Speed Limit (deg/s)";
    const string FrictionCoefficient = "Friction Coefficient (N.m)";
    const string StiffnessCoefficient = "Stiffness Coefficient (N/m)";
    const string DampingCoefficient = "Damping Coefficient (Ns/m)";
    const string JointKinematics = "Joint Kinematics";
    const string JointDynamics = "Joint Dynamics";
    const string Visualization = "Joint Visualization";
    const string JointTranslation = "Joint Translation";
    const string JointRotation = "Joint Rotation";
}

namespace KinematicsKeys2
{
    const string DhParameters = "DH Parameters";
    const string RotationalValues = "Rotational Values";
}

namespace DhParametersKeys2
{
    const string Alpha = "Alpha (deg)";
    const string D = "D (m)";
    const string Theta = "Theta (deg)";
    const string A = "A (m)";
    const string DHType = "DH Type";
}

namespace RotationalValuesKeys2
{
    const string Ixx = "Ixx (kg.m²)";
    const string Ixy = "Ixy (kg.m²)";
    const string Ixz = "Ixz (kg.m²)";
    const string Iyy = "Iyy (kg.m²)";
    const string Iyz = "Iyz (kg.m²)";
    const string Izz = "Izz (kg.m²)";
}

namespace DynamicsKeys2
{
    const string Payload = "Payload";
    const string TestPayload = "Test Payload (kg)";
    const string PayloadPercentage = "Payload Percentage";
    const string RepeatabilityPercentage = "Repeatability Percentage";
    const string SpeedPercentage = "Speed Percentage";
    const string BreakingDistance = "Breaking Distance (m)";
    const string BreakingTime = "Breaking Time (s)";
}

namespace VisualizationKeys2
{
    const string PathToObjFile = "Path to OBJ File";
    const string PathToMltFile = "Path to MTL File";

}

#endif // JSONKEYS_H
