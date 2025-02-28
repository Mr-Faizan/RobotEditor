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
    lib.addRobot(robot);

    Robot *retrievedRobot = lib.getRobot("TestRobot");
    if (retrievedRobot && retrievedRobot->getName() == "TestRobot") {
        std::cout << "testGetRobot passed." << std::endl;
    } else {
        std::cout << "testGetRobot failed." << std::endl;
    }
}

void RobotLibTest::testLoadFromJson() {
    RobotLib lib;
    bool success = lib.loadFromJson("test_robot.json");

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

    bool success = lib.saveToJson("test_robot_output.json");

    if (success) {
        std::cout << "testSaveToJson passed." << std::endl;
    } else {
        std::cout << "testSaveToJson failed." << std::endl;
    }
}
