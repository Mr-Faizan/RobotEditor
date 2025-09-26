#ifndef DHCALCULATOR_H
#define DHCALCULATOR_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "nlohmann/json.hpp"
#include "jsonkeys2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

/**
 * @file dhcalculator.h
 * @brief Declaration of dhCalculator class for DH parameter calculation and verification.
 *
 * This file contains the declaration of the dhCalculator class, which provides methods to
 * compute Denavit-Hartenberg (DH) parameters, transformation matrices, and verify DH parameters
 * against robot geometry data. The class reads robot configuration from JSON files, processes
 * kinematic and geometric data, and outputs results for robot modeling and validation.
 *
 * Main features:
 * - Parse robot configuration and kinematics from JSON files
 * - Compute DH parameters for each joint
 * - Calculate homogeneous transformation matrices
 * - Extract translation and rotation from transformation matrices
 * - Validate computed DH parameters against provided geometry matrices
 * - Compare transformation matrices with a tolerance
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

/**
 * @class dhCalculator
 * @brief Class for calculating and verifying DH parameters for robot joints.
 *
 * The dhCalculator class provides methods to read robot configuration from a JSON file,
 * compute DH parameters for each joint, calculate transformation matrices, extract translation
 * and rotation, and validate the results against geometry data.
 */
class dhCalculator
{
public:
    /**
     * @brief Constructor for dhCalculator.
     * @param filePath Path to the robot configuration JSON file.
     */
    dhCalculator(const std::string &filePath);

    /**
     * @brief Calculate DH parameters and transformation matrices, and update the JSON file.
     */
    void calculateDHParameters();

    /**
     * @brief Compute transformation matrices for all joints from DH parameters.
     * @param dhParameters JSON object containing DH parameters for each joint.
     * @return JSON object with updated joint data including translation and rotation.
     */
    json computeTransformationMatrix(const json &dhParameters);

    /**
     * @brief Extract translation and rotation (Euler angles) from a transformation matrix.
     * @param matrix 4x4 transformation matrix.
     * @return Tuple containing translation array and rotation array (degrees).
     */
    std::tuple<std::array<double, 3>, std::array<double, 3>> getTranslationAndRotation(const std::vector<std::vector<double>>& matrix);

    /**
     * @brief Parse a 4x4 matrix from a string.
     * @param matrixStr String containing matrix values.
     * @return 4x4 matrix as a vector of vectors.
     */
    std::vector<std::vector<double>> parseMatrix(const std::string& matrixStr);

    /**
     * @brief Compute a 4x4 transformation matrix from DH parameters.
     * @param a Link length.
     * @param alpha Link twist (degrees).
     * @param d Link offset.
     * @param theta Joint angle (degrees).
     * @return 4x4 transformation matrix.
     */
    std::vector<std::vector<double>> computeDHMatrix(double a, double alpha, double d, double theta);

    /**
     * @brief Validate DH parameters against geometryMatrix in the JSON file.
     * @param filePath Path to the robot configuration JSON file.
     */
    void validateDHParameters(const std::string &filePath);

    /**
     * @brief Compare two 4x4 matrices with a given tolerance.
     * @param mat1 First matrix.
     * @param mat2 Second matrix.
     * @param tolerance Allowed difference between elements.
     * @return True if matrices are equal within tolerance, false otherwise.
     */
    bool compareMatrices(const std::vector<std::vector<double>>& mat1, const std::vector<std::vector<double>>& mat2, double tolerance = 1e-6);



private:

    string filePath;      /**< Path to the robot configuration JSON file. */
    string robotDataDir;  /**< Directory for robot data. */


    /**
     * @brief Compute DH parameters for all joints from input JSON data.
     * @param inputData JSON object containing robot configuration and kinematics.
     * @return JSON object with computed DH parameters for each joint.
     */
    nlohmann::json computeDHParameters(const nlohmann::json &inputData);
};

#endif // DHCALCULATOR_H
