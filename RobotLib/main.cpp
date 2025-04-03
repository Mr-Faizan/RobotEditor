#include "robotlib_test.h"



#include "importvcmx.h"
#include <filesystem>

#include "nlohmann/json.hpp"

namespace fs = std::filesystem;

int main() {
    try {
        // Define the input and output file paths
        
        std::string inputFilePath = "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/test/robot1/component.rsc";
        std::string outputFilePath = "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/test/robot1/component.json";
        std::string dhParameterFile = "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/test/robot1/dhParameters.json";

        // Check if the input file exists
        if (!fs::exists(inputFilePath)) {
            std::cerr << "Error: Input file does not exist: " << inputFilePath << std::endl;
            return 1;
        }

        // Create an instance of importvcmx and parse the file
        importvcmx parser(inputFilePath);
        nlohmann::json parsedData = parser.parse(inputFilePath);

        // Save the parsed data to the output file
        std::ofstream outputFile(outputFilePath);
        if (!outputFile) {
            std::cerr << "Error: Unable to create output file: " << outputFilePath << std::endl;
            return 1;
        }
        outputFile << parsedData.dump(4); // Pretty print with 4 spaces indent
        outputFile.close();

        
        DHParameterCalculator calculator(outputFilePath, dhParameterFile);
        calculator.calculateDHParameters();

        cout << "Step 4: DH parameters calculation completed successfully!" << endl;

        std::cout << "File processed successfully!" << std::endl;
        std::cout << "Output is saved to: " << outputFilePath << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}




/*

int main() {
    RobotLibTest test;
    test.runTests();
    return 0;
}

*/