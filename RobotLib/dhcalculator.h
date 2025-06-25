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

class dhCalculator
{
public:
    dhCalculator(const std::string &filePath);
    void calculateDHParameters();
    json computeTransformationMatrix(const json &dhParameters);
    std::tuple<std::array<double, 3>, std::array<double, 3>> getTranslationAndRotation(const std::vector<std::vector<double>>& matrix);

    // Validate DH parameters against geometryMatrix
    std::vector<std::vector<double>> parseMatrix(const std::string& matrixStr);
    std::vector<std::vector<double>> computeDHMatrix(double a, double alpha, double d, double theta);
    void validateDHParameters(const std::string &filePath);
    bool compareMatrices(const std::vector<std::vector<double>>& mat1, const std::vector<std::vector<double>>& mat2, double tolerance = 1e-6);
    


private:
    std::string filePath;

    nlohmann::json computeDHParameters(const nlohmann::json &inputData);
};

#endif // DHCALCULATOR_H
