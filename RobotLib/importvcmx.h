#ifndef IMPORTVCMX_H
#define IMPORTVCMX_H

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "jsonkeys2.h"
#include <set>

#include <miniz/miniz.h>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <regex>
#include "nlohmann/json.hpp"
#include "dhcalculator.h"


using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class importvcmx
{
public:
    importvcmx() = default;
    importvcmx(const std::string &filePath);
    int importVCMXData();
       // RscToJsonParser Functions
   nlohmann::json parse(std::string &filePath);

   // function to get the robotDataDir path
    std::string getRobotDataDir() const { return robotDataDir; }

private:
    string filePath;  // Input file path
    string outputDir; // Output directory derived from filePath
    string robotDataDir; // Directory for robot data
    string resourceFilePath; // Path to the resource file
    string jsonFilePath; // Path to the component.json file
// Need to set this Path of resource file.

    // RscToJsonParser
    string filename;
    json robotData;
	json robotVariables;
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
    bool skipExtraction = false; // Temporary

    bool foundRobotCategory = false; // Flag to indicate the Robot values like name , manufacturer , category is picked up.
	bool foundGeometrysection = false; // Flag to indicate the rSimLink section is found or not.





    // ZipExtractor Functions
    bool zipExtractor();
    bool extractZipFile(const std::string &zipFilePath, const std::string &destDir);

    // ImageConverter Functions
    bool imageConverter();
    void convert3DSToOBJ(const std::string &inputFilePath, const std::string &outputFilePath);

    bool runDHParameterCalculator();


   
   bool processResourceFile();

   void processLine(string &line, ifstream &file);
   bool processRobotValues(const string& line, ifstream& file);
   void processVariableSpace(ifstream& file);
   void processKinematicsSection(ifstream& file);
   void processJointMapSection(ifstream& file);

   void processKinematicsOrJointMapSection(const std::string &line);
   void processOtherSections(const std::string &line, std::ifstream &file);
   void processOffsetSection(const std::string &line, std::ifstream &file);
   void processGeometryMatrixSection(const std::string &line);


};


#endif // VCMXDATAEXTRACTOR_H
