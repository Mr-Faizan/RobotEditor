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

    robotDataDir = fs::path(filePath).parent_path().string();

    json inputData;
    inputFile >> inputData;
    inputFile.close();

    // Here we got the computed DH parameters.
    json dhParameters = computeDHParameters(inputData);

    // from these DH parameters we will now compute the transformation matrix.

    // Compute transformation matrices and update the DH parameters
    json updatedJoints = computeTransformationMatrix(dhParameters);

    // Save the updated joints back to the JSON file
    inputData[RobotKeys2::Joints] = updatedJoints;

 
    // So we will save these DH parameters in component.json file.
    // Add the DH parameters to the existing JSON data
    //inputData[RobotKeys2::Joints] = dhParameters;

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

    // verify input data exist otherwise return
    if (inputData.is_null() || !inputData.is_object() || inputData.empty()) {

        cerr << "Input data for computeDHParameters function is invalid or empty" << endl;
        return dhParameters;
    }

    // Get important values from Json File
    const auto &kinematics = inputData.value("kinematics",json::object());
    const auto &geometryMatrix = inputData.value("geometryMatrix", json::object());
    const auto &jointMap = inputData.value("jointMap", json::object());
    const auto &jointOffset = inputData.value("jointOffset", json::object());
    const auto &robotVariables = inputData.value("robotVariables", json::object());


    for (const auto &joint : jointOffset.items())
    {
        std::string jointName = joint.key();

        // 3. Check if joint exists in jointMap
        bool jointInMap = false;
        for (const auto& item : jointMap.items()) {
            if (item.value() == jointName) {
                jointInMap = true;
                break;
            }
        }
        if (!jointInMap) {
            cerr << "Joint " << jointName << " not found in jointMap values. Skipping." << endl;
            continue;
        }

        std::string expression;
        if (joint.value().contains("Offset") && !joint.value().at("Offset").get<std::string>().empty()) {
            expression = joint.value().at("Offset").get<std::string>();
        }
        else {
            
            std::cerr << "Warning: Joint " << jointName << " has no Offset expression." << std::endl;
            continue;
        }

        json jointData;
        std::string geometryFile = "";

        try
        {
            double  Tx = 0,Ty= 0, Tz = 0, Rx = 0, Ry = 0, Rz = 0;

            // Parse the expression
            std::istringstream exprStream(expression);
            std::string token;
            while (std::getline(exprStream, token, '.'))
            {


                // Find operator and value
                size_t open = token.find('(');
                size_t close = token.find(')');
                if (open == std::string::npos || close == std::string::npos || close <= open + 1)
                    continue;

                std::string op = token.substr(0, open);
                std::string val = token.substr(open + 1, close - open - 1);

                double sign = 1.0;
                if (!val.empty() && val[0] == '-') {
                    sign = -1.0;
                    val = val.substr(1);
                }

                double parsedVal = 0;
                bool found = false;

                // Direct numeric value
                try {
                    size_t idx;
                    parsedVal = std::stod(val, &idx);
                    if (idx == val.size()) {
                        found = true;
                        std::cout << "Value for " << op << " from direct numeric: " << parsedVal << std::endl;
                    }
                }
                catch (...) {}

                // Kinematics
                if (!found && val.find("Kinematics::") == 0) {
                    std::string kinKey = val.substr(std::string("Kinematics::").size());
                    if (kinematics.contains(kinKey)) {
                        parsedVal = kinematics.at(kinKey).get<float>();
                        found = true;
                        std::cout << "Value for " << op << " from kinematics: " << parsedVal << std::endl;
                    }
                    else {
                        std::cerr << "Kinematics key " << kinKey << " not found for joint " << jointName << std::endl;
                    }
                }

                // RobotVariables
                if (!found && robotVariables.contains(val)) {
                    parsedVal = robotVariables.at(val).get<float>();
                    found = true;
                    std::cout << "Value for " << op << " from robotVariables: " << parsedVal << std::endl;
                }

                if (!found) {
                    std::cerr << "Value for " << op << " (" << val << ") not found for joint " << jointName << std::endl;
                    continue;
                }

                parsedVal *= sign;
                if (op == "Tx") Tx = parsedVal;
                else if (op == "Ty") Ty = parsedVal;
                else if (op == "Tz") Tz = parsedVal;
                else if (op == "Rx") Rx = parsedVal;
                else if (op == "Ry") Ry = parsedVal;
                else if (op == "Rz") Rz = parsedVal;

            }

            // Get the GeometryFile for the current joint
            /*
            vector<string> geometryUris;
            if (geometryMatrix.contains(jointName)) {
                const auto& arr = geometryMatrix.at(jointName);
                for (const auto& entry : arr) {

                    if (entry.contains("Uri")) {
                        geometryUris.push_back(entry.at("Uri").get<std::string>());
                    }
                }
            }
            */

            vector<json> geometryVisualizations;
            if (geometryMatrix.contains(jointName)) {
                const auto& arr = geometryMatrix.at(jointName);
                for (const auto& entry : arr) {
                    if (entry.contains("Uri")) {
                        string userDefinedName = "[User Defined Name]";
                        string filename = entry.at("Uri").get<string>();
                       
                        geometryVisualizations.push_back({ {"filename", userDefinedName}, {"filepath", filename} });
                    }
                }
            }


            // Now join the pieces of the puzzle.
            jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters][DhParametersKeys2::Alpha] = Rx;
            jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters][DhParametersKeys2::A] = Tx;
            jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters][DhParametersKeys2::D] = Tz;
            jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters][DhParametersKeys2::Theta] = Rz;
            jointData[JointKeys2::Visualization] = geometryVisualizations;
            
            dhParameters[jointName] = jointData;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing joint " << jointName << ": " << e.what() << std::endl;
        }
    }

    return dhParameters;
}




/************************* Homogeneous Transformation matrix calculation Section ********************* */

json dhCalculator::computeTransformationMatrix(const json &dhParameters) {
    json updatedJoints;

    if(dhParameters.empty()) {
        std::cerr << "No DH parameters provided!" << std::endl;
        return updatedJoints;
    }

    // Iterate through each joint in the DH parameters
    for (const auto& [jointName, jointData] : dhParameters.items()) {
        if (jointData.contains(JointKeys2::JointKinematics) &&
            jointData[JointKeys2::JointKinematics].contains(KinematicsKeys2::DhParameters)) {
            const auto& dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];

            // Extract DH parameters
            double a = dhParams[DhParametersKeys2::A];
            double alpha = dhParams[DhParametersKeys2::Alpha];
            double d = dhParams[DhParametersKeys2::D];
            double theta = dhParams[DhParametersKeys2::Theta];

            // Compute the DH matrix
            auto matrix = computeDHMatrix(a, alpha, d, theta);

            // Extract translation and rotation
            auto [translation, rotation] = getTranslationAndRotation(matrix);

            // Create a copy of the joint data and add translation and rotation
            json updatedJointData = jointData;
            updatedJointData[JointKeys2::JointTranslation] = {translation[0], translation[1], translation[2]};
            updatedJointData[JointKeys2::JointRotation] = {rotation[0], rotation[1], rotation[2]};

            // Add the updated joint data to the result
            updatedJoints[jointName] = updatedJointData;
        }
    }

    std::cout << "Transformation matrices computed successfully!" << std::endl;
    return updatedJoints;
}

// Compute the transformation matrix from DH parameters
std::vector<std::vector<double>> dhCalculator::computeDHMatrix(double a, double alpha, double d, double theta) {
    std::vector<std::vector<double>> matrix(4, std::vector<double>(4, 0.0));

    // Convert angles to radians
    alpha = alpha * M_PI / 180.0;
    theta = theta * M_PI / 180.0;

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

// Function to extract translation and rotation from the matrix
std::tuple<std::array<double, 3>, std::array<double, 3>> dhCalculator::getTranslationAndRotation(const std::vector<std::vector<double>>& matrix) {
    // Extract translation
    std::array<double, 3> translation = {matrix[0][3], matrix[1][3], matrix[2][3]};

    // Extract rotation (Euler angles in degrees)
    double thetaX = atan2(matrix[2][1], matrix[2][2]) * 180.0 / M_PI;
    double thetaY = atan2(-matrix[2][0], sqrt(matrix[2][1] * matrix[2][1] + matrix[2][2] * matrix[2][2])) * 180.0 / M_PI;
    double thetaZ = atan2(matrix[1][0], matrix[0][0]) * 180.0 / M_PI;
    std::array<double, 3> rotation = {thetaX, thetaY, thetaZ};

    return {translation, rotation};
}


/************************* DH Parameter Verification Section ********************* */


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
    const auto& joints = inputData[RobotKeys2::Joints];
    const auto& geometryMatrix = inputData["geometryMatrix"];

    // Iterate through each joint in dhParameters
    for (const auto& [jointName, jointData] : joints.items()) {

        const auto& dhParams = jointData[JointKeys2::JointKinematics][KinematicsKeys2::DhParameters];

        double a = dhParams[DhParametersKeys2::A];
        double alpha = dhParams[DhParametersKeys2::Alpha];
        double d = dhParams[DhParametersKeys2::D];
        double theta = dhParams[DhParametersKeys2::Theta];

        // Compute the DH matrix
        auto computedMatrix = computeDHMatrix(a, alpha, d, theta);

        // Find the corresponding matrix in geometryMatrix
        if (geometryMatrix.contains(jointName)) {
            const auto& jointData = geometryMatrix[jointName];
            if (!jointData.empty() && jointData[0].contains("Matrix")) {
                std::string providedMatrixStr = jointData[0]["Matrix"];
                auto providedMatrix = parseMatrix(providedMatrixStr);

                // Compare the matrices
                if (compareMatrices(computedMatrix, providedMatrix)) {
                    std::cout << "Joint " << jointName << ": The DH parameters are correct!" << std::endl;
                } else {
                    std::cout << "Joint " << jointName << ": The DH parameters are incorrect!" << std::endl;
                }
            } else {
                std::cerr << "Joint " << jointName << ": No matrix found in geometryMatrix!" << std::endl;
            }
        } else {
            std::cerr << "Joint " << jointName << ": Not found in geometryMatrix!" << std::endl;
        }
    }
}


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