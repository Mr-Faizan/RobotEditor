#include "dhcalculator.h"

dhCalculator::dhCalculator(const std::string &filePath)
    : filePath(filePath) {}

void dhCalculator::calculateDHParameters()
{
    std::ifstream inputFile(filePath);
    if (!inputFile)
    {
        throw std::runtime_error("Unable to open input file " + filePath);
    }

    json inputData;
    inputFile >> inputData;
    inputFile.close();

    // Here we got the computed DH parameters.
    json dhParameters = computeDHParameters(inputData);
    
    // So we will save these DH parameters in component.json file.
    // Add the DH parameters to the existing JSON data
    inputData["dhParameters"] = dhParameters;

    std::ofstream outputFile(filePath);
    if (!outputFile)
    {
        throw std::runtime_error("Unable to open output file " + filePath);
    }
    outputFile << inputData.dump(4); // Pretty print with 4 spaces indent
    outputFile.close();

    std::cout << "DH parameters calculation completed successfully and saved to the same file!" << std::endl;
}

json dhCalculator::computeDHParameters(const json &inputData)
{
    json dhParameters;
    const auto &kinematics = inputData["kinematics"];
    const auto &geometryMatrix = inputData["geometryMatrix"];

    for (const auto &joint : inputData["jointOffset"].items())
    {
        std::string jointName = joint.key();
        std::string expression = joint.value();
        json jointData;

        try
        {
            double Rx = 0, Tx = 0, Tz = 0, Rz = 0;

            // Parse the expression
            std::istringstream exprStream(expression);
            std::string token;
            while (std::getline(exprStream, token, '.'))
            {
                std::string key;
                double sign = 1.0;

                if (token.find("Tz(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2); // Remove "Kinematics::"
                    Tz = sign * kinematics.at(key).get<double>();
                }
                else if (token.find("Tx(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2);
                    Tx = sign * kinematics.at(key).get<double>();
                }
                else if (token.find("Ty(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2);
                    // Don't know where I use this but for now just keep as it is.
                }
                else if (token.find("Rz(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2);
                    Rz = sign * kinematics.at(key).get<double>();
                }
                else if (token.find("Rx(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2); // Remove "Kinematics::"
                    Rx = sign * kinematics.at(key).get<double>();
                }
                else if (token.find("Ry(") != std::string::npos)
                {
                    key = token.substr(3, token.size() - 4);
                    if (key[0] == '-')
                    {
                        sign = -1.0;
                        key = key.substr(1);
                    }
                    key = key.substr(key.find("::") + 2); // Remove "Kinematics::"
                    // Also not sure for now, where I will use it.
                }
            }

            // Get the GeometryFile for the current joint
            std::string geometryFile = geometryMatrix.at(jointName).at(0).at("GeometryFile");

            // Now join the pieces of the puzzle.
            jointData["visualization"] = geometryFile;
            jointData["alpha"] = Rx;
            jointData["a"] = Tx;
            jointData["d"] = Tz;
            jointData["theta"] = Rz;
            
            dhParameters[jointName] = jointData;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing joint " << jointName << ": " << e.what() << std::endl;
        }
    }

    return dhParameters;
}

void dhCalculator::processAllFiles(const std::string &robotDataDir)
{
    for (const auto &entry : fs::directory_iterator(robotDataDir))
    {
        if (entry.is_directory())
        {
            std::string subDir = entry.path().string();
            std::string componentFilePath = subDir + "/component.json";
            if (fs::exists(componentFilePath))
            {
                try
                {

                    dhCalculator calculator(componentFilePath);
                    calculator.calculateDHParameters();

                    // std::cout << "Processed: " << componentFilePath << " -> " << outputFilePath << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error processing file " << componentFilePath << ": " << e.what() << std::endl;
                }
            }
        }
    }
}



/************************* DH Parameter Verification Section ********************* */

// Helper function to parse the provided matrix from a string
std::vector<std::vector<double>> dhCalculator::parseMatrix(const std::string& matrixStr) {
    std::vector<std::vector<double>> matrix(4, std::vector<double>(4, 0.0));
    std::istringstream iss(matrixStr);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            iss >> matrix[i][j];
        }
    }
    return matrix;
}

// Compute the transformation matrix from DH parameters
std::vector<std::vector<double>> dhCalculator::computeDHMatrix(double a, double alpha, double d, double theta) {
    std::vector<std::vector<double>> matrix(4, std::vector<double>(4, 0.0));

    // Convert angles to radians
    alpha = alpha * M_PI / 180.0;
    theta = theta * M_PI / 180.0;

    // Fill the DH transformation matrix
    matrix[0][0] = cos(theta);
    matrix[0][1] = -sin(theta) * cos(alpha);
    matrix[0][2] = sin(theta) * sin(alpha);
    matrix[0][3] = a * cos(theta);

    matrix[1][0] = sin(theta);
    matrix[1][1] = cos(theta) * cos(alpha);
    matrix[1][2] = -cos(theta) * sin(alpha);
    matrix[1][3] = a * sin(theta);

    matrix[2][0] = 0;
    matrix[2][1] = sin(alpha);
    matrix[2][2] = cos(alpha);
    matrix[2][3] = d;

    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;

    return matrix;
}

// Compare two matrices with a tolerance
bool dhCalculator::compareMatrices(const std::vector<std::vector<double>>& mat1, const std::vector<std::vector<double>>& mat2, double tolerance) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::abs(mat1[i][j] - mat2[i][j]) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

// Function to validate DH parameters against geometryMatrix
void dhCalculator::validateDHParameters(const std::string& filePath) {
    // Open and parse the JSON file
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    json inputData;
    inputFile >> inputData;
    inputFile.close();

    // Extract dhParameters and geometryMatrix
    const auto& dhParameters = inputData["dhParameters"];
    const auto& geometryMatrix = inputData["geometryMatrix"];

    // Iterate through each joint in dhParameters
    for (const auto& [joint, dhParam] : dhParameters.items()) {
        double a = dhParam["a"];
        double alpha = dhParam["alpha"];
        double d = dhParam["d"];
        double theta = dhParam["theta"];

        // Compute the DH matrix
        auto computedMatrix = computeDHMatrix(a, alpha, d, theta);

        // Find the corresponding matrix in geometryMatrix
        if (geometryMatrix.contains(joint)) {
            const auto& jointData = geometryMatrix[joint];
            if (!jointData.empty() && jointData[0].contains("Matrix")) {
                std::string providedMatrixStr = jointData[0]["Matrix"];
                auto providedMatrix = parseMatrix(providedMatrixStr);

                // Compare the matrices
                if (compareMatrices(computedMatrix, providedMatrix)) {
                    std::cout << "Joint " << joint << ": The DH parameters are correct!" << std::endl;
                } else {
                    std::cout << "Joint " << joint << ": The DH parameters are incorrect!" << std::endl;
                }
            } else {
                std::cerr << "Joint " << joint << ": No matrix found in geometryMatrix!" << std::endl;
            }
        } else {
            std::cerr << "Joint " << joint << ": Not found in geometryMatrix!" << std::endl;
        }
    }
}