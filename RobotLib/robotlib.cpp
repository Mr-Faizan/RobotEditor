#include "robotlib.h"

RobotLib::RobotLib()
{
}

RobotLib::~RobotLib() {}

Robot RobotLib::initializeNewRobot()
{
    Robot newRobot;
    Joint &newJoint = newRobot.createAndAddJoint();
    JointDynamics &newDynamics = newJoint.createAndAddDynamics();
    return newRobot;
}

// Creating a new Robot here because i want to only expose this class to the outside world.
Robot RobotLib::createRobot()
{
    Robot robot;
    return robot;
}

Joint RobotLib::createJoint()
{
    Joint joint;

    return joint;
}

JointDynamics RobotLib::createDynamics()
{
    JointDynamics dynamics;
    return dynamics;
}

void RobotLib::addRobot(const Robot &robot)
{
    robotCollection.push_back(robot);
}

void RobotLib::removeRobot(const std::string &name)
{
    robotCollection.erase(std::remove_if(robotCollection.begin(), robotCollection.end(), [&](const Robot &robot)
                                         { return robot.getName() == name; }),
                          robotCollection.end());
}

Robot &RobotLib::getRobot(const std::string &name)
{
    for (auto &robot : robotCollection)
    {
        if (robot.getName() == name)
        {
            return robot;
        }
    }
    throw std::runtime_error("Robot not found");
}

Robot &RobotLib::getRobotById(int robotId)
{
    for (auto &robot : robotCollection)
    {
        if (robot.getId() == robotId)
        {
            return robot;
        }
    }
    throw std::runtime_error("Robot not found");
}

const Robot *RobotLib::getRobotById2(int robotId) const
{
    auto it = std::find_if(robotCollection.begin(), robotCollection.end(), [robotId](const Robot &robot)
                           { return robot.getId() == robotId; });

    if (it != robotCollection.end())
    {
        return &(*it);
    }

    return nullptr;
}

const std::vector<Robot> &RobotLib::getRobots() const
{
    return robotCollection;
}

// Function responsible for updating and saving the robot data
bool RobotLib::updateAndSaveRobotData(const std::string &filePath, const json &json)
{
    try
    {
        // load from json

        Robot robot = loadFromJson(json);

        // Save the updated robot data to the file
        if (!saveToJson(filePath, robot))
        {
            std::cerr << "Failed to save robot data to file." << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

// Load data from JSON file
Robot RobotLib::loadFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open file: " << filePath << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    json jsonData;

    try
    {
        file >> jsonData;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Error: Failed to parse JSON file: " << e.what() << std::endl;
        throw std::runtime_error("Failed to parse JSON file");
    }

    // Now pass this JSON data to loadFromJson
    Robot robot = loadFromJson(jsonData);

    return robot;
}

// Load data from JSON file
// Things to Do in this function:
// 1. Verify that the file we pick is correct file to read from.
// 2. Verify data type before saving to JSON to avoid runtime errors.
/*
Robot RobotLib::loadFromJson(const json jsonData)
{
    // Check if the JSON data is an object and jsonData is valid json
    if (!jsonData.is_object())
    {
        std::cerr << "Error: Invalid JSON data." << std::endl;
        throw std::runtime_error("Invalid JSON data.");
    }

    // Check if the main Robot section exists
    if (!jsonData.contains(RobotKeys2::Robot) || !jsonData[RobotKeys2::Robot].is_object())
    {
        std::cerr << "Error: Missing or invalid Robot section in JSON." << std::endl;
        throw std::runtime_error("Missing or invalid Robot section in JSON.");
    }

    // Create a new Robot object
    Robot robot;
    // Load robots from JSON
    const auto &robotJson = jsonData[RobotKeys2::Robot];
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
            const std::string jointKey = jointJson.key();
            const auto &jointData = jointJson.value();

            try
            {
                // Get Joint from getJointByJointNumber function based on jointKey
               // Joint &joint = robot.getJointByJointNumber(jointKey);

                Joint joint = robot.createAndAddJoint();

                // jointkey has value then set the joint number
                if (!jointKey.empty())
                {
                    joint.setJointNumber(jointKey);
                }

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
                    // if (dhParams.contains(DhParametersKeys2::ModifiedDh) && dhParams[DhParametersKeys2::ModifiedDh].is_boolean())
                    //     kinematics.setModifiedDh(dhParams[DhParametersKeys2::ModifiedDh]);

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
                        const std::string payloadKey = dynamicsJson.key();
                        const auto &dynamicsData = dynamicsJson.value();

                        //JointDynamics &dynamics = joint.getDynamicsByPayloadNumber(payloadKey);

                        JointDynamics dynamics = joint.createAndAddDynamics();

                        if (!payloadKey.empty())
                        {
                            dynamics.setPayloadNumber(payloadKey);
                        }

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
                        //joint.setDynamics(dynamics);
                    }
                }
                //robot.addJoint(joint);
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                continue;
            }
        }
    }

    return robot;
}
*/

double RobotLib::getNumberFromJson(const json &value)
{
    if (value.is_number())
    {
        return value.get<double>();
    }
    else if (value.is_string())
    {
        return std::stod(value.get<std::string>());
    }
    else
    {
        throw std::runtime_error("Invalid value type for number conversion");
    }
}

Robot RobotLib::loadFromJson(const json jsonData)
{
    // Check if the JSON data is an object and jsonData is valid json
    if (!jsonData.is_object())
    {
        std::cerr << "Error: Invalid JSON data." << std::endl;
        throw std::runtime_error("Invalid JSON data.");
    }

    // Check if the main Robot section exists
    if (!jsonData.contains(RobotKeys2::Robot) || !jsonData[RobotKeys2::Robot].is_object())
    {
        std::cerr << "Error: Missing or invalid Robot section in JSON." << std::endl;
        throw std::runtime_error("Missing or invalid Robot section in JSON.");
    }

    // Create a new Robot object
    Robot robot;
    // add robot to the collection
    addRobot(robot);

    // Load robots from JSON
    const auto &robotJson = jsonData[RobotKeys2::Robot];
    if (robotJson.contains(RobotKeys2::RobotName) && robotJson[RobotKeys2::RobotName].is_string())
        robot.setName(robotJson[RobotKeys2::RobotName]);
    if (robotJson.contains(RobotKeys2::RobotManufacturer) && robotJson[RobotKeys2::RobotManufacturer].is_string())
        robot.setManufacturer(robotJson[RobotKeys2::RobotManufacturer]);
    if (robotJson.contains(RobotKeys2::RobotPayload))
        robot.setPayload(getNumberFromJson(robotJson[RobotKeys2::RobotPayload]));
    if (robotJson.contains(RobotKeys2::RobotFootprint))
        robot.setFootprint(getNumberFromJson(robotJson[RobotKeys2::RobotFootprint]));
    if (robotJson.contains(RobotKeys2::RobotMaxReach))
        robot.setMaxReach(getNumberFromJson(robotJson[RobotKeys2::RobotMaxReach]));
    if (robotJson.contains(RobotKeys2::RobotRepeatability))
        robot.setRepeatability(getNumberFromJson(robotJson[RobotKeys2::RobotRepeatability]));
    if (robotJson.contains(RobotKeys2::RobotWeight))
        robot.setWeight(getNumberFromJson(robotJson[RobotKeys2::RobotWeight]));
    if (robotJson.contains(RobotKeys2::DOF))
        robot.setDof(static_cast<int>(getNumberFromJson(robotJson[RobotKeys2::DOF])));

    // Check if the joint section exists
    if (robotJson.contains(RobotKeys2::Joints) && robotJson[RobotKeys2::Joints].is_object())
    {
        for (const auto &jointJson : robotJson[RobotKeys2::Joints].items())
        {
            const std::string jointKey = jointJson.key();
            const auto &jointData = jointJson.value();

            try
            {
                Joint &joint = robot.createAndAddJoint();

                // jointkey has value then set the joint number
                if (!jointKey.empty())
                {
                    joint.setJointNumber(jointKey);
                }

                if (jointData.contains(JointKeys2::JointName) && jointData[JointKeys2::JointName].is_string())
                    joint.setName(jointData[JointKeys2::JointName]);
                if (jointData.contains(JointKeys2::MotionRangeMax))
                    joint.setMotionRangeMax(getNumberFromJson(jointData[JointKeys2::MotionRangeMax]));
                if (jointData.contains(JointKeys2::MotionRangeMin))
                    joint.setMotionRangeMin(getNumberFromJson(jointData[JointKeys2::MotionRangeMin]));
                if (jointData.contains(JointKeys2::JointSpeedLimit))
                    joint.setJointSpeedLimit(getNumberFromJson(jointData[JointKeys2::JointSpeedLimit]));
                if (jointData.contains(JointKeys2::FrictionCoefficient))
                    joint.setFrictionCoefficient(getNumberFromJson(jointData[JointKeys2::FrictionCoefficient]));
                if (jointData.contains(JointKeys2::StiffnessCoefficient))
                    joint.setStiffnessCoefficient(getNumberFromJson(jointData[JointKeys2::StiffnessCoefficient]));
                if (jointData.contains(JointKeys2::DampingCoefficient))
                    joint.setDampingCoefficient(getNumberFromJson(jointData[JointKeys2::DampingCoefficient]));
                if (jointData.contains(JointKeys2::Visualization) && jointData[JointKeys2::Visualization].is_array()) {
                    vector<pair<string, string>> visualizations;
                    for (const auto& vis : jointData[JointKeys2::Visualization]) {
                        if (vis.contains("filename") && vis.contains("filepath")) {
                            visualizations.emplace_back(
                                vis["filename"].get<string>(),
                                vis["filepath"].get<string>()
                            );
                        }
                    }
                    joint.setVisualizations(visualizations);
                }

                JointKinematics kinematics;
                JointKinematics::DHParameters dhParameters;

                if (jointData.contains(JointKeys2::JointKinematics) && jointData[JointKeys2::JointKinematics].is_object())
                {
                    const auto &dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];

                    if (dhParams.contains(DhParametersKeys2::Alpha))
                        dhParameters.setAlpha(getNumberFromJson(dhParams[DhParametersKeys2::Alpha]));
                    if (dhParams.contains(DhParametersKeys2::D))
                        dhParameters.setD(getNumberFromJson(dhParams[DhParametersKeys2::D]));
                    if (dhParams.contains(DhParametersKeys2::Theta))
                        dhParameters.setTheta(getNumberFromJson(dhParams[DhParametersKeys2::Theta]));
                    if (dhParams.contains(DhParametersKeys2::A))
                        dhParameters.setA(getNumberFromJson(dhParams[DhParametersKeys2::A]));

                    if (dhParams.contains(DhParametersKeys2::DHType) && dhParams[DhParametersKeys2::DHType].is_string())
                        kinematics.setDhType(dhParams[DhParametersKeys2::DHType]);

                    kinematics.setDhParameters(dhParameters);

                    JointKinematics::RotationalValues rotationalValues;
                    const auto &rotValues = jointData[JointKeys2::JointKinematics][KinematicsKeys2::RotationalValues];

                    if (rotValues.contains(RotationalValuesKeys2::Ixx))
                        rotationalValues.setIxx(getNumberFromJson(rotValues[RotationalValuesKeys2::Ixx]));
                    if (rotValues.contains(RotationalValuesKeys2::Ixy))
                        rotationalValues.setIxy(getNumberFromJson(rotValues[RotationalValuesKeys2::Ixy]));
                    if (rotValues.contains(RotationalValuesKeys2::Ixz))
                        rotationalValues.setIxz(getNumberFromJson(rotValues[RotationalValuesKeys2::Ixz]));
                    if (rotValues.contains(RotationalValuesKeys2::Iyy))
                        rotationalValues.setIyy(getNumberFromJson(rotValues[RotationalValuesKeys2::Iyy]));
                    if (rotValues.contains(RotationalValuesKeys2::Iyz))
                        rotationalValues.setIyz(getNumberFromJson(rotValues[RotationalValuesKeys2::Iyz]));
                    if (rotValues.contains(RotationalValuesKeys2::Izz))
                        rotationalValues.setIzz(getNumberFromJson(rotValues[RotationalValuesKeys2::Izz]));
                    kinematics.setRotationalValues(rotationalValues);

                    joint.setKinematics(kinematics);
                }

                if (jointData.contains(JointKeys2::JointDynamics) && jointData[JointKeys2::JointDynamics].is_object())
                {
                    for (const auto &dynamicsJson : jointData[JointKeys2::JointDynamics].items())
                    {
                        const std::string payloadKey = dynamicsJson.key();
                        const auto &dynamicsData = dynamicsJson.value();

                        JointDynamics &dynamics = joint.createAndAddDynamics();

                        if (!payloadKey.empty())
                        {
                            dynamics.setPayloadNumber(payloadKey);
                        }

                        if (dynamicsData.contains(DynamicsKeys2::TestPayload))
                            dynamics.setTestPayload(getNumberFromJson(dynamicsData[DynamicsKeys2::TestPayload]));
                        if (dynamicsData.contains(DynamicsKeys2::PayloadPercentage))
                            dynamics.setPayloadPercentage(getNumberFromJson(dynamicsData[DynamicsKeys2::PayloadPercentage]));
                        if (dynamicsData.contains(DynamicsKeys2::RepeatabilityPercentage))
                            dynamics.setReachabilityPercentage(getNumberFromJson(dynamicsData[DynamicsKeys2::RepeatabilityPercentage]));
                        if (dynamicsData.contains(DynamicsKeys2::SpeedPercentage))
                            dynamics.setSpeedPercentage(getNumberFromJson(dynamicsData[DynamicsKeys2::SpeedPercentage]));
                        if (dynamicsData.contains(DynamicsKeys2::BreakingDistance))
                            dynamics.setBreakingDistance(getNumberFromJson(dynamicsData[DynamicsKeys2::BreakingDistance]));
                        if (dynamicsData.contains(DynamicsKeys2::BreakingTime))
                            dynamics.setBreakingTime(getNumberFromJson(dynamicsData[DynamicsKeys2::BreakingTime]));
                    }
                }
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                continue;
            }
        }
    }

    return robot;
}

//
bool RobotLib::saveToJson(const std::string &filePath, Robot &robot) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    json j;

    // if robot exit and not empty then save the data to the file.

    if (robot.getId() != 0)
    {
        json robotJson;
        robotJson[RobotKeys2::RobotName] = robot.getName();
        robotJson[RobotKeys2::RobotManufacturer] = robot.getManufacturer();
        robotJson[RobotKeys2::RobotPayload] = robot.getPayload();
        robotJson[RobotKeys2::RobotFootprint] = robot.getFootprint();
        robotJson[RobotKeys2::RobotMaxReach] = robot.getMaxReach();
        robotJson[RobotKeys2::RobotRepeatability] = robot.getRepeatability();
        robotJson[RobotKeys2::RobotWeight] = robot.getWeight();
        robotJson[RobotKeys2::DOF] = robot.getDof();

        json jointsJson;
        for (const auto &joint : robot.getJoints())
        {
            json jointJson;
            jointJson[JointKeys2::JointName] = joint.getName();
            jointJson[JointKeys2::MotionRangeMax] = joint.getMotionRangeMax();
            jointJson[JointKeys2::MotionRangeMin] = joint.getMotionRangeMin();
            jointJson[JointKeys2::JointSpeedLimit] = joint.getJointSpeedLimit();
            jointJson[JointKeys2::FrictionCoefficient] = joint.getFrictionCoefficient();
            jointJson[JointKeys2::StiffnessCoefficient] = joint.getStiffnessCoefficient();
            jointJson[JointKeys2::DampingCoefficient] = joint.getDampingCoefficient();

            json visualizationsJson = json::array();
            for (const auto& vis : joint.getVisualizations()) {
                visualizationsJson.push_back({ {"filename", vis.first}, {"filepath", vis.second} });
            }
            jointJson[JointKeys2::Visualization] = visualizationsJson;


            json kinematicsJson;
            json dhParamsJson;
            dhParamsJson[DhParametersKeys2::Alpha] = joint.getKinematics().getDhParameters().getAlpha();
            dhParamsJson[DhParametersKeys2::D] = joint.getKinematics().getDhParameters().getD();
            dhParamsJson[DhParametersKeys2::Theta] = joint.getKinematics().getDhParameters().getTheta();
            dhParamsJson[DhParametersKeys2::A] = joint.getKinematics().getDhParameters().getA();
            dhParamsJson[DhParametersKeys2::DHType] = joint.getKinematics().getDhParameters().getDhType();
            kinematicsJson[KinematicsKeys2::DhParameters] = dhParamsJson;

            json rotValuesJson;
            rotValuesJson[RotationalValuesKeys2::Ixx] = joint.getKinematics().getRotationalValues().getIxx();
            rotValuesJson[RotationalValuesKeys2::Ixy] = joint.getKinematics().getRotationalValues().getIxy();
            rotValuesJson[RotationalValuesKeys2::Ixz] = joint.getKinematics().getRotationalValues().getIxz();
            rotValuesJson[RotationalValuesKeys2::Iyy] = joint.getKinematics().getRotationalValues().getIyy();
            rotValuesJson[RotationalValuesKeys2::Iyz] = joint.getKinematics().getRotationalValues().getIyz();
            rotValuesJson[RotationalValuesKeys2::Izz] = joint.getKinematics().getRotationalValues().getIzz();
            kinematicsJson[KinematicsKeys2::RotationalValues] = rotValuesJson;

            jointJson[JointKeys2::JointKinematics] = kinematicsJson;

            json dynamicsJson;
            for (const auto &dynamics : joint.getDynamics())
            {
                json singleDynamicsJson;
                singleDynamicsJson[DynamicsKeys2::TestPayload] = dynamics.getTestPayload();
                singleDynamicsJson[DynamicsKeys2::PayloadPercentage] = dynamics.getPayloadPercentage();
                singleDynamicsJson[DynamicsKeys2::RepeatabilityPercentage] = dynamics.getReachabilityPercentage();
                singleDynamicsJson[DynamicsKeys2::SpeedPercentage] = dynamics.getSpeedPercentage();
                singleDynamicsJson[DynamicsKeys2::BreakingDistance] = dynamics.getBreakingDistance();
                singleDynamicsJson[DynamicsKeys2::BreakingTime] = dynamics.getBreakingTime();

                dynamicsJson[dynamics.getPayloadNumber()] = singleDynamicsJson;
            }

            jointJson[JointKeys2::JointDynamics] = dynamicsJson;
            jointsJson[joint.getJointNumber()] = jointJson;
        }

        robotJson[RobotKeys2::Joints] = jointsJson;
        j[RobotKeys2::Robot] = robotJson;
    }

    // print JSON values here

    file << j.dump(4);
    return true;
}

void RobotLib::printData() const
{
    for (const auto &robot : robotCollection)
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

            cout << "  Visualizations:" << endl;
            for (const auto& vis : joint.getVisualizations()) {
                cout << "    Filename: " << vis.first << ", Filepath: " << vis.second << endl;
            }

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

Robot RobotLib::importRobotFromVCMX(const string &filePath)
{
    Robot newRobot;
    try
    {
        
        importvcmx importer(filePath);
        int status = importer.importVCMXData();

        if (status == 0)
        {
            newRobot = parseRobotFromVCMX(importer.getRobotDataDir());

            std::cout << "VCMX data imported successfully!" << std::endl;
        }
        else
        {
            std::cerr << "Failed to import VCMX data. Status code: " << status << std::endl;
        }

    }
    catch (const std::exception &e)
    {
        cerr << "Error while importing robot from VCMX: " << e.what() << endl;
    }

    return newRobot;
}



// Function to parse the robot data from a JSON file and populate a Robot object
Robot RobotLib::parseRobotFromVCMX(const string &robotDataFolderPath)
{

    // Locate the component.json file in the given folder
    string filePath = robotDataFolderPath + "/component.json";
    if (!fs::exists(filePath))
    {
        throw runtime_error("component.json file not found in folder: " + robotDataFolderPath);
    }

    // Open and parse the JSON file
    ifstream inputFile(filePath);
    if (!inputFile)
    {
        throw runtime_error("Unable to open file: " + filePath);
    }

    json inputData;
    try {
        inputFile >> inputData;
    } catch (const json::parse_error &e) {
        throw std::runtime_error("Error parsing JSON file: " + std::string(e.what()));
    }
    inputFile.close();

    Robot robot;

    // Populate robot details
    if (inputData.contains("robotData")) {
        const auto &robotData = inputData["robotData"];
        if (robotData.contains("Category") && robotData["Category"].is_string() && robotData["Category"] == "Robots") {
            if (robotData.contains("BOMname") && robotData["BOMname"].is_string()) {
                robot.setName(robotData["BOMname"]);
            }
            if (robotData.contains("BOMdescription") && robotData["BOMdescription"].is_string()) {
                robot.setManufacturer(robotData["BOMdescription"]);
            }
        }
    }

    // Populate DH parameters
    if (inputData.contains(RobotKeys2::Joints)) {
        const auto &jointsJson = inputData[RobotKeys2::Joints];
        for (const auto &[jointName, jointData] : jointsJson.items()) {
            Joint &joint = robot.createAndAddJoint();
            joint.setName(jointName);

            if (jointData.contains(JointKeys2::JointKinematics) && jointData[JointKeys2::JointKinematics].contains(KinematicsKeys2::DhParameters)) {
                const auto &dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];

                JointKinematics kinematics;
                JointKinematics::DHParameters dhParameters;

                if (dhParams.contains(DhParametersKeys2::A) && dhParams[DhParametersKeys2::A].is_number()) {
                    dhParameters.setA(dhParams[DhParametersKeys2::A]);
                }
                if (dhParams.contains(DhParametersKeys2::Alpha) && dhParams[DhParametersKeys2::Alpha].is_number()) {
                    dhParameters.setAlpha(dhParams[DhParametersKeys2::Alpha]);
                }
                if (dhParams.contains(DhParametersKeys2::D) && dhParams[DhParametersKeys2::D].is_number()) {
                    dhParameters.setD(dhParams[DhParametersKeys2::D]);
                }
                if (dhParams.contains(DhParametersKeys2::Theta) && dhParams[DhParametersKeys2::Theta].is_number()) {
                    dhParameters.setTheta(dhParams[DhParametersKeys2::Theta]);
                }

                kinematics.setDhParameters(dhParameters);
                joint.setKinematics(kinematics);
            }

            // Extract Visualization 
            if (jointData.contains(JointKeys2::Visualization) && jointData[JointKeys2::Visualization].is_array()) {
                vector<pair<string, string>> visualizations;
                for (const auto& vis : jointData[JointKeys2::Visualization]) {
                    if (vis.contains("filename") && vis.contains("filepath")) {
                        string filename = vis["filename"].get<string>();
                        string filepath = vis["filepath"].get<string>();
                        
                        // Check for file existence with and without .obj extension
                        if (fs::exists(filepath)) {
                            visualizations.emplace_back(filename, filepath);
                        }
                        else {
                            // Try with .obj extension if not present
                            string filepathWithObj = filepath;
                            if (filepathWithObj.find(".obj") == string::npos) {
                                filepathWithObj += ".obj";
                            }
                            if (fs::exists(filepathWithObj)) {
                                visualizations.emplace_back(filename, filepathWithObj);
                            }
                            else {
                                std::cerr << "Warning: Visualization file not found: " << filepath << " or " << filepathWithObj << std::endl;
                            }
                        }

                    }
                }
                joint.setVisualizations(visualizations);
            }

            if (jointData.contains(JointKeys2::JointTranslation) && jointData[JointKeys2::JointTranslation].is_array()) {
                
                joint.setTranslation(jointData[JointKeys2::JointTranslation].get<std::array<double, 3>>());
            }
            
            if (jointData.contains(JointKeys2::JointRotation) && jointData[JointKeys2::JointRotation].is_array()) {
                
                joint.setRotation(jointData[JointKeys2::JointRotation].get<std::array<double, 3>>());
            }

        }
    }


    return robot;
}



string RobotLib::zipRobotPackage(const string& folderPath)
{
    namespace fs = std::filesystem;
    string zipFilePath = folderPath + ".zip";
    string reFilePath = folderPath + ".re";

    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_writer_init_file(&zip, zipFilePath.c_str(), 0))
        return "";

    // Recursively add files
    auto addDirToZip = [&](const fs::path& dir, const std::string& base, auto&& addDirToZipRef) -> void {
        for (const auto& entry : fs::directory_iterator(dir)) {
            std::string relPath = base.empty() ? entry.path().filename().string() : base + "/" + entry.path().filename().string();
            if (fs::is_directory(entry.status())) {
                addDirToZipRef(entry.path(), relPath, addDirToZipRef);
            }
            else if (fs::is_regular_file(entry.status())) {
                mz_zip_writer_add_file(&zip, relPath.c_str(), entry.path().string().c_str(), nullptr, 0, MZ_BEST_COMPRESSION);
            }
        }
        };
    addDirToZip(fs::path(folderPath), "", addDirToZip);

    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);

    // Rename .zip to .re
    std::error_code ec;
    fs::remove(reFilePath, ec); // Remove if exists, ignore error
    fs::rename(zipFilePath, reFilePath, ec);
    if (ec) return "";

    return reFilePath;
}

bool RobotLib::extractRobotPackage(const std::string& reFilePath, const std::string& destDir)
{
    namespace fs = std::filesystem;

    // Clean up destination directory if it exists
    std::error_code ec;
    fs::remove_all(destDir, ec);
    fs::create_directories(destDir, ec);

    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    // Open the .re (zip) file
    if (!mz_zip_reader_init_file(&zip, reFilePath.c_str(), 0)) {
        return false;
    }

    mz_uint numFiles = mz_zip_reader_get_num_files(&zip);
    for (mz_uint i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
            continue;

        std::string outPath = destDir + "/" + file_stat.m_filename;

        // Create directories if needed
        fs::create_directories(fs::path(outPath).parent_path(), ec);

        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            fs::create_directories(outPath, ec);
            continue;
        }

        if (!mz_zip_reader_extract_to_file(&zip, i, outPath.c_str(), 0)) {
            mz_zip_reader_end(&zip);
            return false;
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}