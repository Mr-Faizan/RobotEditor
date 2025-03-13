#include "robotlib_test.h"

void RobotLibTest::runTests() {
    testAddRobot();
    testRemoveRobot();
    testGetRobot();
    testLoadFromJson();
    testSaveToJson();
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

    Robot *retrievedRobot = lib.getRobot("TestRobot");
    if (retrievedRobot && retrievedRobot->getName() == "TestRobot") {
        std::cout << "testGetRobot passed." << std::endl;
    } else {
        std::cout << "testGetRobot failed." << std::endl;
    }

    // Print robot data
    if (retrievedRobot) {
        std::cout << "Robot Name: " << retrievedRobot->getName() << std::endl;
        std::cout << "Manufacturer: " << retrievedRobot->getManufacturer() << std::endl;
        std::cout << "Payload: " << retrievedRobot->getPayload() << std::endl;
        std::cout << "Footprint: " << retrievedRobot->getFootprint() << std::endl;
        std::cout << "Max Reach: " << retrievedRobot->getMaxReach() << std::endl;
        std::cout << "Repeatability: " << retrievedRobot->getRepeatability() << std::endl;
        std::cout << "Weight: " << retrievedRobot->getWeight() << std::endl;
        std::cout << "DOF: " << retrievedRobot->getDof() << std::endl;
    }
}

void RobotLibTest::testLoadFromJson() {
    RobotLib lib;
    Robot newRobot = lib.createRobot();
    bool success = lib.loadFromJson("test_robot.json", newRobot);

    if (success && !lib.getRobots().empty() && lib.getRobots()[0].getName() == "TestRobot") {
        std::cout << "testLoadFromJson passed." << std::endl;
    } else {
        std::cout << "testLoadFromJson failed." << std::endl;
    }
}

void RobotLibTest::testSaveToJson() {
    RobotLib lib;
    Robot robot("TestRobot");
    lib.addRobot(robot);

    bool success = lib.saveToJson("test_robot_output.json", 0);

    if (success) {
        std::cout << "testSaveToJson passed." << std::endl;
    } else {
        std::cout << "testSaveToJson failed." << std::endl;
    }
}
