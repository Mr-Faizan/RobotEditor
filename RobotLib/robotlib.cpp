#include "robotlib.h"


RobotLib::RobotLib() {}

void RobotLib::addRobot(const Robot &robot) {
    robots.push_back(robot);
}

void RobotLib::removeRobot(const std::string &name) {
    robots.erase(std::remove_if(robots.begin(), robots.end(), [&](const Robot &robot) {
        return robot.getName() == name;
    }), robots.end());
}

Robot* RobotLib::getRobot(const std::string &name) {
    for (auto &robot : robots) {
        if (robot.getName() == name) {
            return &robot;
        }
    }
    return nullptr;
}

const std::vector<Robot>& RobotLib::getRobots() const {
    return robots;
}

bool RobotLib::loadFromJson(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    json j;
    file >> j;

    // Clear existing robots
    robots.clear();

    // Load robots from JSON
    for (const auto &robotJson : j["robots"]) {
        Robot robot(robotJson["name"]);
        robot.setManufacturer(robotJson["manufacturer"]);
        robot.setPayload(robotJson["payload"]);
        robot.setFootPrint(robotJson["footprint"]);
        robot.setMaxReach(robotJson["maxReach"]);
        robot.setRepeatability(robotJson["repeatability"]);
        robot.setWeight(robotJson["weight"]);
        robot.setDof(robotJson["dof"]);

        for (const auto &jointJson : robotJson["joints"]) {
            Joint joint(jointJson["name"]);
            joint.setMotionRangeMax(jointJson["motionRangeMax"]);
            joint.setMotionRangeMin(jointJson["motionRangeMin"]);
            joint.setJointSpeedLimit(jointJson["speedLimit"]);
            joint.setFrictionCoefficient(jointJson["frictionCoefficient"]);
            joint.setStiffnessCoefficient(jointJson["stiffnessCoefficient"]);
            joint.setDampingCoefficient(jointJson["dampingCoefficient"]);
            joint.setVisualization(jointJson["visualization"]);

            JointKinematics kinematics;
            JointKinematics::DHParameters dhParameters;
            dhParameters.setAlpha(jointJson["kinematics"]["dhParameters"]["alpha"]);
            dhParameters.setD(jointJson["kinematics"]["dhParameters"]["d"]);
            dhParameters.setTheta(jointJson["kinematics"]["dhParameters"]["theta"]);
            dhParameters.setA(jointJson["kinematics"]["dhParameters"]["a"]);
            kinematics.setDhParameters(dhParameters);

            JointKinematics::RotationalValues rotationalValues;
            rotationalValues.setIxx(jointJson["kinematics"]["rotationalValues"]["ixx"]);
            rotationalValues.setIxy(jointJson["kinematics"]["rotationalValues"]["ixy"]);
            rotationalValues.setIxz(jointJson["kinematics"]["rotationalValues"]["ixz"]);
            rotationalValues.setIyy(jointJson["kinematics"]["rotationalValues"]["iyy"]);
            rotationalValues.setIyz(jointJson["kinematics"]["rotationalValues"]["iyz"]);
            rotationalValues.setIzz(jointJson["kinematics"]["rotationalValues"]["izz"]);
            kinematics.setRotationalValues(rotationalValues);

            kinematics.setDhType(jointJson["kinematics"]["dhType"]);
            kinematics.setModifiedDh(jointJson["kinematics"]["modifiedDh"]);
            joint.setKinematics(kinematics);

            JointDynamics dynamics;
            dynamics.setTestPayload(jointJson["dynamics"]["testPayload"]);
            dynamics.setPayloadPercentage(jointJson["dynamics"]["payloadPercentage"]);
            dynamics.setReachabilityPercentage(jointJson["dynamics"]["reachabilityPercentage"]);
            dynamics.setSpeedPercentage(jointJson["dynamics"]["speedPercentage"]);
            dynamics.setBreakingDistance(jointJson["dynamics"]["breakingDistance"]);
            dynamics.setBreakingTime(jointJson["dynamics"]["breakingTime"]);
            joint.setDynamics(dynamics);



            robot.addJoint(joint);
        }

        addRobot(robot);
    }

    return true;
}

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
        robotJson["footprint"] = robot.getFootPrint();
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
