#include "robotlib.h"

using json = nlohmann::json;

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
        robot.setFootprint(robotJson["footprint"]);
        robot.setMaxReach(robotJson["maxReach"]);
        robot.setRepeatability(robotJson["repeatability"]);
        robot.setWeight(robotJson["weight"]);
        robot.setDOF(robotJson["dof"]);

        for (const auto &jointJson : robotJson["joints"]) {
            Joint joint(jointJson["name"]);
            joint.setMotionRangeMax(jointJson["motionRangeMax"]);
            joint.setMotionRangeMin(jointJson["motionRangeMin"]);
            joint.setSpeedLimit(jointJson["speedLimit"]);
            joint.setFrictionCoefficient(jointJson["frictionCoefficient"]);
            joint.setStiffnessCoefficient(jointJson["stiffnessCoefficient"]);
            joint.setDampingCoefficient(jointJson["dampingCoefficient"]);

            Kinematics kinematics;
            kinematics.setAlpha(jointJson["kinematics"]["alpha"]);
            kinematics.setD(jointJson["kinematics"]["d"]);
            kinematics.setTheta(jointJson["kinematics"]["theta"]);
            kinematics.setA(jointJson["kinematics"]["a"]);
            kinematics.setDHType(jointJson["kinematics"]["dhType"]);
            joint.setKinematics(kinematics);

            Dynamics dynamics;
            dynamics.setTestPayload(jointJson["dynamics"]["testPayload"]);
            dynamics.setPayloadPercentage(jointJson["dynamics"]["payloadPercentage"]);
            dynamics.setRepeatabilityPercentage(jointJson["dynamics"]["repeatabilityPercentage"]);
            dynamics.setSpeedPercentage(jointJson["dynamics"]["speedPercentage"]);
            dynamics.setBreakingDistance(jointJson["dynamics"]["breakingDistance"]);
            dynamics.setBreakingTime(jointJson["dynamics"]["breakingTime"]);
            joint.setDynamics(dynamics);

            Visualization visualization;
            visualization.setFilePath(jointJson["visualization"]["filePath"]);
            joint.setVisualization(visualization);

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
        robotJson["footprint"] = robot.getFootprint();
        robotJson["maxReach"] = robot.getMaxReach();
        robotJson["repeatability"] = robot.getRepeatability();
        robotJson["weight"] = robot.getWeight();
        robotJson["dof"] = robot.getDOF();

        for (const auto &joint : robot.getJoints()) {
            json jointJson;
            jointJson["name"] = joint.getName();
            jointJson["motionRangeMax"] = joint.getMotionRangeMax();
            jointJson["motionRangeMin"] = joint.getMotionRangeMin();
            jointJson["speedLimit"] = joint.getSpeedLimit();
            jointJson["frictionCoefficient"] = joint.getFrictionCoefficient();
            jointJson["stiffnessCoefficient"] = joint.getStiffnessCoefficient();
            jointJson["dampingCoefficient"] = joint.getDampingCoefficient();

            jointJson["kinematics"]["alpha"] = joint.getKinematics().getAlpha();
            jointJson["kinematics"]["d"] = joint.getKinematics().getD();
            jointJson["kinematics"]["theta"] = joint.getKinematics().getTheta();
            jointJson["kinematics"]["a"] = joint.getKinematics().getA();
            jointJson["kinematics"]["dhType"] = joint.getKinematics().getDHType();

            jointJson["dynamics"]["testPayload"] = joint.getDynamics().getTestPayload();
            jointJson["dynamics"]["payloadPercentage"] = joint.getDynamics().getPayloadPercentage();
            jointJson["dynamics"]["repeatabilityPercentage"] = joint.getDynamics().getRepeatabilityPercentage();
            jointJson["dynamics"]["speedPercentage"] = joint.getDynamics().getSpeedPercentage();
            jointJson["dynamics"]["breakingDistance"] = joint.getDynamics().getBreakingDistance();
            jointJson["dynamics"]["breakingTime"] = joint.getDynamics().getBreakingTime();

            jointJson["visualization"]["filePath"] = joint.getVisualization().getFilePath();

            robotJson["joints"].push_back(jointJson);
        }

        j["robots"].push_back(robotJson);
    }

    file << j.dump(4);
    return true;
}
