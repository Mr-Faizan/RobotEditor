#include "robotlib.h"

RobotLib::RobotLib() {}

RobotLib::~RobotLib() {}

void RobotLib::addRobot(const Robot &robot)
{
    robots.push_back(robot);
}

void RobotLib::removeRobot(const std::string &name)
{
    robots.erase(std::remove_if(robots.begin(), robots.end(), [&](const Robot &robot)
                                { return robot.getName() == name; }),
                 robots.end());
}

Robot *RobotLib::getRobot(const std::string &name)
{
    for (auto &robot : robots)
    {
        if (robot.getName() == name)
        {
            return &robot;
        }
    }
    return nullptr;
}

const std::vector<Robot> &RobotLib::getRobots() const
{
    return robots;
}

// Load data from JSON file
// Things to Do in this function:
// 1. Verify that the file we pick is correct file to read from.
// 2. Verify data type before saving to JSON to avoid runtime errors.

bool RobotLib::loadFromJson(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    json jsonData;
    file >> jsonData;

    // Clear existing robots
    robots.clear();

    // Load robots from JSON
    const auto &robotJson = jsonData[RobotKeys2::Robot];
    Robot robot(robotJson[RobotKeys2::RobotName]);
    robot.setManufacturer(robotJson[RobotKeys2::RobotManufacturer]);
    robot.setPayload(robotJson[RobotKeys2::RobotPayload]);
    robot.setFootprint(robotJson[RobotKeys2::RobotFootprint]);
    robot.setMaxReach(robotJson[RobotKeys2::RobotMaxReach]);
    robot.setRepeatability(robotJson[RobotKeys2::RobotRepeatability]);
    robot.setWeight(robotJson[RobotKeys2::RobotWeight]);
    robot.setDof(robotJson[RobotKeys2::DOF]);

    for (const auto &jointJson : robotJson[RobotKeys2::Joints].items())
    {
        const auto &jointData = jointJson.value();
        Joint joint(jointData[JointKeys2::JointName]);
        joint.setMotionRangeMax(jointData[JointKeys2::MotionRangeMax]);
        joint.setMotionRangeMin(jointData[JointKeys2::MotionRangeMin]);
        joint.setJointSpeedLimit(jointData[JointKeys2::JointSpeedLimit]);
        joint.setFrictionCoefficient(jointData[JointKeys2::FrictionCoefficient]);
        joint.setStiffnessCoefficient(jointData[JointKeys2::StiffnessCoefficient]);
        joint.setDampingCoefficient(jointData[JointKeys2::DampingCoefficient]);
        joint.setVisualization(jointData[JointKeys2::Visualization]);

        JointKinematics kinematics;
        JointKinematics::DHParameters dhParameters;
        const auto &dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];
        dhParameters.setAlpha(dhParams[DhParametersKeys2::Alpha]);
        dhParameters.setD(dhParams[DhParametersKeys2::D]);
        dhParameters.setTheta(dhParams[DhParametersKeys2::Theta]);
        dhParameters.setA(dhParams[DhParametersKeys2::A]);
        kinematics.setDhParameters(dhParameters);


        JointKinematics::RotationalValues rotationalValues;
        const auto &rotValues = jointData[JointKeys2::JointKinematics][KinematicsKeys2::RotationalValues];
        rotationalValues.setIxx(rotValues[RotationalValuesKeys2::Ixx]);
        rotationalValues.setIxy(rotValues[RotationalValuesKeys2::Ixy]);
        rotationalValues.setIxz(rotValues[RotationalValuesKeys2::Ixz]);
        rotationalValues.setIyy(rotValues[RotationalValuesKeys2::Iyy]);
        rotationalValues.setIyz(rotValues[RotationalValuesKeys2::Iyz]);
        rotationalValues.setIzz(rotValues[RotationalValuesKeys2::Izz]);
        kinematics.setRotationalValues(rotationalValues);

        kinematics.setDhType(dhParams[DhParametersKeys2::DHType]);
        //kinematics.setModifiedDh(dhParams[DhParametersKeys2::DHType]);
        joint.setKinematics(kinematics);

        for (const auto &dynamicsJson : jointData[JointKeys2::JointDynamics].items())
        {
            const auto &dynamicsData = dynamicsJson.value();
            JointDynamics dynamics;
            dynamics.setTestPayload(dynamicsData[DynamicsKeys2::TestPayload]);
            dynamics.setPayloadPercentage(dynamicsData[DynamicsKeys2::PayloadPercentage]);
            dynamics.setReachabilityPercentage(dynamicsData[DynamicsKeys2::RepeatabilityPercentage]);
            dynamics.setSpeedPercentage(dynamicsData[DynamicsKeys2::SpeedPercentage]);
            dynamics.setBreakingDistance(dynamicsData[DynamicsKeys2::BreakingDistance]);
            dynamics.setBreakingTime(dynamicsData[DynamicsKeys2::BreakingTime]);
            joint.addDynamics(dynamics);
        }

        robot.addJoint(joint);
    }

    addRobot(robot);

    return true;
}


//
bool RobotLib::saveToJson(const std::string &filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    json j;
    for (const auto &robot : robots) {
        json robotJson;
        robotJson["name"] = robot.getName();
        robotJson["manufacturer"] = robot.getManufacturer();
        robotJson["payload"] = robot.getPayload();
        robotJson["footprint"] = robot.getFootprint();
        robotJson["maxReach"] = robot.getMaxReach();
        robotJson["repeatability"] = robot.getRepeatability();
        robotJson["weight"] = robot.getWeight();
        robotJson["dof"] = robot.getDof();

        for (const auto &joint : robot.getJoints()) {
            json jointJson;
            jointJson["name"] = joint.getName();
            jointJson["motionRangeMax"] = joint.getMotionRangeMax();
            jointJson["motionRangeMin"] = joint.getMotionRangeMin();
            jointJson["speedLimit"] = joint.getJointSpeedLimit();
            jointJson["frictionCoefficient"] = joint.getFrictionCoefficient();
            jointJson["stiffnessCoefficient"] = joint.getStiffnessCoefficient();
            jointJson["dampingCoefficient"] = joint.getDampingCoefficient();

            jointJson["kinematics"]["dhParameters"]["alpha"] = joint.getKinematics().getDhParameters().getAlpha();
            jointJson["kinematics"]["dhParameters"]["d"] = joint.getKinematics().getDhParameters().getD();
            jointJson["kinematics"]["dhParameters"]["theta"] = joint.getKinematics().getDhParameters().getTheta();
            jointJson["kinematics"]["dhParameters"]["a"] = joint.getKinematics().getDhParameters().getA();
            jointJson["kinematics"]["dhParameters"]["dhType"] = joint.getKinematics().getDhParameters().getDhType();
            jointJson["kinematics"]["dhParameters"]["modifiedDh"] = joint.getKinematics().getDhParameters().isModifiedDh();

            jointJson["kinematics"]["rotationalValues"]["ixx"] = joint.getKinematics().getRotationalValues().getIxx();
            jointJson["kinematics"]["rotationalValues"]["ixy"] = joint.getKinematics().getRotationalValues().getIxy();
            jointJson["kinematics"]["rotationalValues"]["ixz"] = joint.getKinematics().getRotationalValues().getIxz();
            jointJson["kinematics"]["rotationalValues"]["iyy"] = joint.getKinematics().getRotationalValues().getIyy();
            jointJson["kinematics"]["rotationalValues"]["iyz"] = joint.getKinematics().getRotationalValues().getIyz();
            jointJson["kinematics"]["rotationalValues"]["izz"] = joint.getKinematics().getRotationalValues().getIzz();

            for (const auto &dynamics : joint.getDynamics()) {
                json dynamicsJson;
                dynamicsJson["testPayload"] = dynamics.getTestPayload();
                dynamicsJson["payloadPercentage"] = dynamics.getPayloadPercentage();
                dynamicsJson["repeatabilityPercentage"] = dynamics.getReachabilityPercentage();
                dynamicsJson["speedPercentage"] = dynamics.getSpeedPercentage();
                dynamicsJson["breakingDistance"] = dynamics.getBreakingDistance();
                dynamicsJson["breakingTime"] = dynamics.getBreakingTime();
                jointJson["dynamics"].push_back(dynamicsJson);
            }

            robotJson["joints"].push_back(jointJson);
        }

        j["robots"].push_back(robotJson);
    }

    file << j.dump(4);
    return true;
}



void RobotLib::printData() const
{
    for (const auto &robot : robots)
    {
        cout << "Robot Name: " << robot.getName() << endl;
        cout << "Manufacturer: " << robot.getManufacturer() << endl;
        cout << "Payload: " << robot.getPayload() << endl;
        cout << "Footprint: " << robot.getFootprint() << endl;
        cout << "Max Reach: " << robot.getMaxReach() << endl;
        cout << "Repeatability: " << robot.getRepeatability() << endl;
        cout << "Weight: " << robot.getWeight() << endl;
        cout << "DOF: " << robot.getDof() << endl;

        for (const auto &joint : robot.getJoints())
        {
            cout << "  Joint Name: " << joint.getName() << endl;
            cout << "  Motion Range Min: " << joint.getMotionRangeMin() << endl;
            cout << "  Motion Range Max: " << joint.getMotionRangeMax() << endl;
            cout << "  Speed Limit: " << joint.getJointSpeedLimit() << endl;
            cout << "  Friction Coefficient: " << joint.getFrictionCoefficient() << endl;
            cout << "  Stiffness Coefficient: " << joint.getStiffnessCoefficient() << endl;
            cout << "  Damping Coefficient: " << joint.getDampingCoefficient() << endl;
            cout << "  Visualization: " << joint.getVisualization() << endl;

            const auto &kinematics = joint.getKinematics();
            const auto &dhParameters = kinematics.getDhParameters();
            cout << "    DH Parameters:" << endl;
            cout << "      Alpha: " << dhParameters.getAlpha() << endl;
            cout << "      D: " << dhParameters.getD() << endl;
            cout << "      Theta: " << dhParameters.getTheta() << endl;
            cout << "      A: " << dhParameters.getA() << endl;
            cout << "      DH Type: " << kinematics.getDhType() << endl;
            cout << "      Modified DH: " << kinematics.isModifiedDh() << endl;

            const auto &rotationalValues = kinematics.getRotationalValues();
            cout << "    Rotational Values:" << endl;
            cout << "      Ixx: " << rotationalValues.getIxx() << endl;
            cout << "      Ixy: " << rotationalValues.getIxy() << endl;
            cout << "      Ixz: " << rotationalValues.getIxz() << endl;
            cout << "      Iyy: " << rotationalValues.getIyy() << endl;
            cout << "      Iyz: " << rotationalValues.getIyz() << endl;
            cout << "      Izz: " << rotationalValues.getIzz() << endl;

            for (const auto &dynamics : joint.getDynamics())
            {
                cout << "    Dynamics:" << endl;
                cout << "      Test Payload: " << dynamics.getTestPayload() << endl;
                cout << "      Payload Percentage: " << dynamics.getPayloadPercentage() << endl;
                cout << "      Repeatability Percentage: " << dynamics.getReachabilityPercentage() << endl;
                cout << "      Speed Percentage: " << dynamics.getSpeedPercentage() << endl;
                cout << "      Breaking Distance: " << dynamics.getBreakingDistance() << endl;
                cout << "      Breaking Time: " << dynamics.getBreakingTime() << endl;
            }
        }
    }
}
