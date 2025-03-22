#include "robotlib_test.h"
#include "robotlib.h"
#include <iostream>
#include <fstream>

void RobotLibTest::runTests() {
    testAddRobot();
    testRemoveRobot();
    testGetRobot();
    testGetRobotById();
    testLoadFromJson();
    testSaveToJson();
    testUpdateAndSaveRobotData();
}

void RobotLibTest::testAddRobot() {
    RobotLib lib;
    Robot robot("TestRobot");
    lib.addRobot(robot);

    if (lib.getRobots().size() == 1 && lib.getRobots()[0].getName() == "TestRobot") {
        std::cout << "testAddRobot passed." << std::endl;
    } else {
        std::cout << "testAddRobot failed." << std::endl;
    }
}

void RobotLibTest::testRemoveRobot() {
    RobotLib lib;
    Robot robot("TestRobot");
    lib.addRobot(robot);
    lib.removeRobot("TestRobot");

    if (lib.getRobots().empty()) {
        std::cout << "testRemoveRobot passed." << std::endl;
    } else {
        std::cout << "testRemoveRobot failed." << std::endl;
    }
}

void RobotLibTest::testGetRobot() {
    RobotLib lib;
    Robot robot("TestRobot");
    robot.setManufacturer("TestManufacturer");
    robot.setPayload(10.5);
    robot.setFootprint(1.2);
    robot.setMaxReach(2.5);
    robot.setRepeatability(0.01);
    robot.setWeight(50.0);
    robot.setDof(6);
    lib.addRobot(robot);

    try {
        Robot &retrievedRobot = lib.getRobot("TestRobot");
        if (retrievedRobot.getName() == "TestRobot") {
            std::cout << "testGetRobot passed." << std::endl;
        } else {
            std::cout << "testGetRobot failed." << std::endl;
        }

        // Print robot data
        std::cout << "Robot Name: " << retrievedRobot.getName() << std::endl;
        std::cout << "Manufacturer: " << retrievedRobot.getManufacturer() << std::endl;
        std::cout << "Payload: " << retrievedRobot.getPayload() << std::endl;
        std::cout << "Footprint: " << retrievedRobot.getFootprint() << std::endl;
        std::cout << "Max Reach: " << retrievedRobot.getMaxReach() << std::endl;
        std::cout << "Repeatability: " << retrievedRobot.getRepeatability() << std::endl;
        std::cout << "Weight: " << retrievedRobot.getWeight() << std::endl;
        std::cout << "DOF: " << retrievedRobot.getDof() << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << "testGetRobot failed: " << e.what() << std::endl;
    }
}

void RobotLibTest::testGetRobotById() {
    RobotLib lib;
    Robot robot("TestRobot");
    robot.setId(1);
    lib.addRobot(robot);

    try {
        Robot &retrievedRobot = lib.getRobotById(1);
        if (retrievedRobot.getId() == 1) {
            std::cout << "testGetRobotById passed." << std::endl;
        } else {
            std::cout << "testGetRobotById failed." << std::endl;
        }
    } catch (const std::runtime_error &e) {
        std::cout << "testGetRobotById failed: " << e.what() << std::endl;
    }
}

void RobotLibTest::testLoadFromJson() {
    RobotLib lib;
    Robot newRobot = lib.createRobot();
    std::string jsonString = R"({
        "Robot": {
            "RobotName": "TestRobot",
            "RobotManufacturer": "TestManufacturer",
            "RobotPayload": 10.5,
            "RobotFootprint": 1.2,
            "RobotMaxReach": 2.5,
            "RobotRepeatability": 0.01,
            "RobotWeight": 50.0,
            "DOF": 6,
            "Joints": {}
        }
    })";
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);

    Robot robot = lib.loadFromJson(jsonData);

    if (robot.getName() == "TestRobot" &&
        robot.getManufacturer() == "TestManufacturer" &&
        robot.getPayload() == 10.5 &&
        robot.getFootprint() == 1.2 &&
        robot.getMaxReach() == 2.5 &&
        robot.getRepeatability() == 0.01 &&
        robot.getWeight() == 50.0 &&
        robot.getDof() == 6)
    {
        std::cout << "testLoadFromJson passed." << std::endl;
    }
    else
    {
        std::cout << "testLoadFromJson failed." << std::endl;
    }
}

void RobotLibTest::testSaveToJson() {
    RobotLib lib;
    Robot robot("TestRobot");
    robot.setId(1);
    lib.addRobot(robot);

    bool success = lib.saveToJson("test_robot_output.json", robot);

    if (success) {
        std::cout << "testSaveToJson passed." << std::endl;
    } else {
        std::cout << "testSaveToJson failed." << std::endl;
    }

    // Verify the contents of the saved file
    std::ifstream file("test_robot_output.json");
    nlohmann::json jsonData;
    file >> jsonData;

    if (jsonData["Robot"]["RobotName"] == "TestRobot") {
        std::cout << "testSaveToJson verification passed." << std::endl;
    } else {
        std::cout << "testSaveToJson verification failed." << std::endl;
    }
}

void RobotLibTest::testUpdateAndSaveRobotData() {
    RobotLib lib;
    Robot robot("TestRobot");
    robot.setId(1);
    lib.addRobot(robot);

    std::string jsonString = R"({
        "Robot": {
            "RobotName": "UpdatedRobot",
            "RobotManufacturer": "UpdatedManufacturer",
            "RobotPayload": 20.5,
            "RobotFootprint": 2.2,
            "RobotMaxReach": 3.5,
            "RobotRepeatability": 0.02,
            "RobotWeight": 60.0,
            "DOF": 7,
            "Joints": {}
        }
    })";
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);

    bool success = lib.updateAndSaveRobotData("updated_robot_output.json", jsonData);

    if (success) {
        std::cout << "testUpdateAndSaveRobotData passed." << std::endl;
    } else {
        std::cout << "testUpdateAndSaveRobotData failed." << std::endl;
    }

    // Verify the contents of the saved file
    std::ifstream file("updated_robot_output.json");
    nlohmann::json savedJsonData;
    file >> savedJsonData;

    if (savedJsonData["Robot"]["RobotName"] == "UpdatedRobot") {
        std::cout << "testUpdateAndSaveRobotData verification passed." << std::endl;
    } else {
        std::cout << "testUpdateAndSaveRobotData verification failed." << std::endl;
    }
}