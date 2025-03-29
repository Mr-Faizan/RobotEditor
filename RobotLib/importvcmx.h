#ifndef IMPORTVCMX_H
#define IMPORTVCMX_H

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "libraries/miniz/miniz.h"
/*
#include <libraries/assimp/include/assimp/Importer.hpp>
#include "libraries/assimp/include/assimp/Exporter.hpp"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/postprocess.h"
*/
#include <regex>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class importvcmx
{
public:
    importvcmx(const std::string &filePath);

    int importVCMXData();

private:
    string filePath;  // Input file path
    string outputDir; // Output directory derived from filePath
    string robotDataDir; // Directory for robot data
    string resourceFilePath; // Path to the resource file
// Need to set this Path of resource file.

    // RscToJsonParser
    string filename;
    nlohmann::json kinematics;
    nlohmann::json jointMap;
    nlohmann::json jointOffset;
    nlohmann::json geometryMatrix;
    nlohmann::json rSimLinkObject;
    string currentJointName;
    string currentOffsetExpression;
    string currentRSimLinkName;
    string currentGeoFeatureName;
    string currentMatrix;
    bool isKinematicsSection;
    bool isJointMapSection;
    bool isDofSection;
    bool foundNameInDOFsection;
    bool isDofFixedSection;
    bool isOffsetSection;
    bool captureNextOffset;
    bool isGeometryMatrixSection;
    bool isGeoFeatureSection;




    // ZipExtractor Functions
    bool zipExtractor();
    bool extractZipFile(const std::string &zipFilePath, const std::string &destDir);

    // ImageConverter Functions
    bool imageConverter();
   // void convert3DSToOBJ(const std::string &inputFilePath, const std::string &outputFilePath);



   // RscToJsonParser Functions
   nlohmann::json parse(std::string &filePath);
   
   void processResourceFile();

   void processLine(std::string &line, std::ifstream &file);
   void processKinematicsOrJointMapSection(const std::string &line);
   void processOtherSections(const std::string &line, std::ifstream &file);
   void processOffsetSection(const std::string &line, std::ifstream &file);
   void processGeometryMatrixSection(const std::string &line);

};



// DHParameterCalculator class

class DHParameterCalculator
{
public:
    DHParameterCalculator(const std::string &inputFilename, const std::string &outputFilename);
    void calculateDHParameters();
    static void processAllFiles(const std::string &jsonDir, const std::string &outputDir);

private:
    std::string inputFilename;
    std::string outputFilename;

    nlohmann::json computeDHParameters(const nlohmann::json &inputData);
};

#endif // VCMXDATAEXTRACTOR_H
