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


using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class dhCalculator
{
public:
    dhCalculator(const std::string &filePath);
    void calculateDHParameters();
    static void processAllFiles(const std::string &robotDataDir);

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
