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

    try
    {
        file >> jsonData;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Error: Failed to parse JSON file: " << e.what() << std::endl;
        return false;
    }

    // Check if the main Robot section exists
    if (!jsonData.contains(RobotKeys2::Robot) || !jsonData[RobotKeys2::Robot].is_object())
    {
        std::cerr << "Error: Missing or invalid Robot section in JSON." << std::endl;
        return false;
    }

    // Load robots from JSON
    const auto &robotJson = jsonData[RobotKeys2::Robot];
    Robot robot;
    if (robotJson.contains(RobotKeys2::RobotName) && robotJson[RobotKeys2::RobotName].is_string())
        robot.setName(robotJson[RobotKeys2::RobotName]);
    if (robotJson.contains(RobotKeys2::RobotManufacturer) && robotJson[RobotKeys2::RobotManufacturer].is_string())
        robot.setManufacturer(robotJson[RobotKeys2::RobotManufacturer]);
    if (robotJson.contains(RobotKeys2::RobotPayload) && robotJson[RobotKeys2::RobotPayload].is_number())
        robot.setPayload(robotJson[RobotKeys2::RobotPayload]);
    if (robotJson.contains(RobotKeys2::RobotFootprint) && robotJson[RobotKeys2::RobotFootprint].is_number())
        robot.setFootprint(robotJson[RobotKeys2::RobotFootprint]);
    if (robotJson.contains(RobotKeys2::RobotMaxReach) && robotJson[RobotKeys2::RobotMaxReach].is_number())
        robot.setMaxReach(robotJson[RobotKeys2::RobotMaxReach]);
    if (robotJson.contains(RobotKeys2::RobotRepeatability) && robotJson[RobotKeys2::RobotRepeatability].is_number())
        robot.setRepeatability(robotJson[RobotKeys2::RobotRepeatability]);
    if (robotJson.contains(RobotKeys2::RobotWeight) && robotJson[RobotKeys2::RobotWeight].is_number())
        robot.setWeight(robotJson[RobotKeys2::RobotWeight]);
    if (robotJson.contains(RobotKeys2::DOF) && robotJson[RobotKeys2::DOF].is_number())
        robot.setDof(robotJson[RobotKeys2::DOF]);

    // check if the joint section exists
    if (robotJson.contains(RobotKeys2::Joints) && robotJson[RobotKeys2::Joints].is_object())
    {

        for (const auto &jointJson : robotJson[RobotKeys2::Joints].items())
        {
            const auto &jointData = jointJson.value();
            Joint joint;
            if (jointData.contains(JointKeys2::JointName) && jointData[JointKeys2::JointName].is_string())
                joint.setName(jointData[JointKeys2::JointName]);
            if (jointData.contains(JointKeys2::MotionRangeMax) && jointData[JointKeys2::MotionRangeMax].is_number())
                joint.setMotionRangeMax(jointData[JointKeys2::MotionRangeMax]);
            if (jointData.contains(JointKeys2::MotionRangeMin) && jointData[JointKeys2::MotionRangeMin].is_number())
                joint.setMotionRangeMin(jointData[JointKeys2::MotionRangeMin]);
            if (jointData.contains(JointKeys2::JointSpeedLimit) && jointData[JointKeys2::JointSpeedLimit].is_number())
                joint.setJointSpeedLimit(jointData[JointKeys2::JointSpeedLimit]);
            if (jointData.contains(JointKeys2::FrictionCoefficient) && jointData[JointKeys2::FrictionCoefficient].is_number())
                joint.setFrictionCoefficient(jointData[JointKeys2::FrictionCoefficient]);
            if (jointData.contains(JointKeys2::StiffnessCoefficient) && jointData[JointKeys2::StiffnessCoefficient].is_number())
                joint.setStiffnessCoefficient(jointData[JointKeys2::StiffnessCoefficient]);
            if (jointData.contains(JointKeys2::DampingCoefficient) && jointData[JointKeys2::DampingCoefficient].is_number())
                joint.setDampingCoefficient(jointData[JointKeys2::DampingCoefficient]);
            if (jointData.contains(JointKeys2::Visualization) && jointData[JointKeys2::Visualization].is_string())
                joint.setVisualization(jointData[JointKeys2::Visualization]);

            JointKinematics kinematics;
            JointKinematics::DHParameters dhParameters;

            if (jointData.contains(JointKeys2::JointKinematics) && jointData[JointKeys2::JointKinematics].is_object())
            {
                const auto &dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];

                if (dhParams.contains(DhParametersKeys2::Alpha) && dhParams[DhParametersKeys2::Alpha].is_number())
                    dhParameters.setAlpha(dhParams[DhParametersKeys2::Alpha]);
                if (dhParams.contains(DhParametersKeys2::D) && dhParams[DhParametersKeys2::D].is_number())
                    dhParameters.setD(dhParams[DhParametersKeys2::D]);
                if (dhParams.contains(DhParametersKeys2::Theta) && dhParams[DhParametersKeys2::Theta].is_number())
                    dhParameters.setTheta(dhParams[DhParametersKeys2::Theta]);
                if (dhParams.contains(DhParametersKeys2::A) && dhParams[DhParametersKeys2::A].is_number())
                    dhParameters.setA(dhParams[DhParametersKeys2::A]);

                if (dhParams.contains(DhParametersKeys2::DHType) && dhParams[DhParametersKeys2::DHType].is_string())
                    kinematics.setDhType(dhParams[DhParametersKeys2::DHType]);
                //if (dhParams.contains(DhParametersKeys2::ModifiedDh) && dhParams[DhParametersKeys2::ModifiedDh].is_boolean())
                //    kinematics.setModifiedDh(dhParams[DhParametersKeys2::ModifiedDh]);
                
                kinematics.setDhParameters(dhParameters);

                JointKinematics::RotationalValues rotationalValues;
                const auto &rotValues = jointData[JointKeys2::JointKinematics][KinematicsKeys2::RotationalValues];

                if (rotValues.contains(RotationalValuesKeys2::Ixx) && rotValues[RotationalValuesKeys2::Ixx].is_number())
                    rotationalValues.setIxx(rotValues[RotationalValuesKeys2::Ixx]);
                if (rotValues.contains(RotationalValuesKeys2::Ixy) && rotValues[RotationalValuesKeys2::Ixy].is_number())
                    rotationalValues.setIxy(rotValues[RotationalValuesKeys2::Ixy]);
                if (rotValues.contains(RotationalValuesKeys2::Ixz) && rotValues[RotationalValuesKeys2::Ixz].is_number())
                    rotationalValues.setIxz(rotValues[RotationalValuesKeys2::Ixz]);
                if (rotValues.contains(RotationalValuesKeys2::Iyy) && rotValues[RotationalValuesKeys2::Iyy].is_number())
                    rotationalValues.setIyy(rotValues[RotationalValuesKeys2::Iyy]);
                if (rotValues.contains(RotationalValuesKeys2::Iyz) && rotValues[RotationalValuesKeys2::Iyz].is_number())
                    rotationalValues.setIyz(rotValues[RotationalValuesKeys2::Iyz]);
                if (rotValues.contains(RotationalValuesKeys2::Izz) && rotValues[RotationalValuesKeys2::Izz].is_number())
                    rotationalValues.setIzz(rotValues[RotationalValuesKeys2::Izz]);
                kinematics.setRotationalValues(rotationalValues);

                joint.setKinematics(kinematics);
            }

            if (jointData.contains(JointKeys2::JointDynamics) && jointData[JointKeys2::JointDynamics].is_array())
            {
                for (const auto &dynamicsJson : jointData[JointKeys2::JointDynamics].items())
                {
                    const auto &dynamicsData = dynamicsJson.value();
                    JointDynamics dynamics;
                    if (dynamicsData.contains(DynamicsKeys2::TestPayload) && dynamicsData[DynamicsKeys2::TestPayload].is_number())
                        dynamics.setTestPayload(dynamicsData[DynamicsKeys2::TestPayload]);
                    if (dynamicsData.contains(DynamicsKeys2::PayloadPercentage) && dynamicsData[DynamicsKeys2::PayloadPercentage].is_number())
                        dynamics.setPayloadPercentage(dynamicsData[DynamicsKeys2::PayloadPercentage]);
                    if (dynamicsData.contains(DynamicsKeys2::RepeatabilityPercentage) && dynamicsData[DynamicsKeys2::RepeatabilityPercentage].is_number())
                        dynamics.setReachabilityPercentage(dynamicsData[DynamicsKeys2::RepeatabilityPercentage]);
                    if (dynamicsData.contains(DynamicsKeys2::SpeedPercentage) && dynamicsData[DynamicsKeys2::SpeedPercentage].is_number())
                        dynamics.setSpeedPercentage(dynamicsData[DynamicsKeys2::SpeedPercentage]);
                    if (dynamicsData.contains(DynamicsKeys2::BreakingDistance) && dynamicsData[DynamicsKeys2::BreakingDistance].is_number())
                        dynamics.setBreakingDistance(dynamicsData[DynamicsKeys2::BreakingDistance]);
                    if (dynamicsData.contains(DynamicsKeys2::BreakingTime) && dynamicsData[DynamicsKeys2::BreakingTime].is_number())
                        dynamics.setBreakingTime(dynamicsData[DynamicsKeys2::BreakingTime]);
                    joint.addDynamics(dynamics);
                }
            }
            robot.addJoint(joint);
        }
    }

    addRobot(robot);

    return true;
}

//
bool RobotLib::saveToJson(const std::string &filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    json j;
    for (const auto &robot : robots)
    {
        json robotJson;
        robotJson["name"] = robot.getName();
        robotJson["manufacturer"] = robot.getManufacturer();
        robotJson["payload"] = robot.getPayload();
        robotJson["footprint"] = robot.getFootprint();
        robotJson["maxReach"] = robot.getMaxReach();
        robotJson["repeatability"] = robot.getRepeatability();
        robotJson["weight"] = robot.getWeight();
        robotJson["dof"] = robot.getDof();

        for (const auto &joint : robot.getJoints())
        {
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

            for (const auto &dynamics : joint.getDynamics())
            {
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
