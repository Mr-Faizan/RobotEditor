/**
 * @file importvcmx.h
 * @brief Declaration of importvcmx class for importing and processing VCMX robot files.
 *
 * This file defines the importvcmx class, which provides methods to extract, convert, and parse
 * VCMX robot files and their resources. It handles zip extraction, image conversion, resource parsing,
 * and DH parameter calculation for robot simulation and modeling.
 *
 * Main features:
 * - Extract VCMX files and convert 3DS models to OBJ format
 * - Parse resource files (.rsc) and convert to JSON
 * - Process kinematics, joint maps, offsets, geometry matrices, and robot variables
 * - Run DH parameter calculator and validate results
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

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

/**
 * @class importvcmx
 * @brief Class for importing and processing VCMX robot files and resources.
 *
 * The importvcmx class provides methods to extract VCMX archives, convert 3DS files to OBJ,
 * parse resource files, and process robot configuration data for simulation and modeling.
 */
class importvcmx
{
public:
    /**
     * @brief Default constructor for importvcmx.
     */
    importvcmx() = default;

    /**
     * @brief Constructor with file path for importvcmx.
     * @param filePath Path to the VCMX file or extracted directory.
     */
    importvcmx(const std::string &filePath);

    /**
     * @brief Main function to import VCMX data and process all steps.
     * @return Status code (0 for success, non-zero for error).
     */
    int importVCMXData();

    /**
     * @brief Parse a resource file (.rsc) and return its data as JSON.
     * @param filePath Path to the resource file.
     * @return Parsed JSON object.
     */
    nlohmann::json parse(std::string &filePath);

    /**
     * @brief Get the robot data directory path.
     * @return Path to the robot data directory.
     */
    std::string getRobotDataDir() const { return robotDataDir; }

private:
    // --- Member variables ---
    string filePath;           /**< Input file path. */
    string outputDir;          /**< Output directory derived from filePath. */
    string robotDataDir;       /**< Directory for robot data. */
    string resourceFilePath;   /**< Path to the resource file. */
    string jsonFilePath;       /**< Path to the component.json file. */
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
    bool skipExtraction = false; /**< Temporary flag for skipping extraction. */
    bool finishKinematicsSection = false;
    bool finishJointMapSection = false;
    bool foundRobotCategory = false; /**< Flag to indicate the Robot values like name, manufacturer, category are picked up. */
    bool finishVariableSpaceSection = false; /**< Flag to indicate that we already picked up the Variable space. */
    bool foundGeometrySection = false; /**< Flag to indicate the rSimLink section is found or not. */
    bool foundDofSection = false; /**< Flag to indicate the Dof (Degree of Freedom) section is found or not. */

    // --- Zip extraction functions ---
    /**
     * @brief Extract the VCMX zip archive.
     * @return True if extraction succeeded, false otherwise.
     */
    bool zipExtractor();

    /**
     * @brief Extract files from a zip archive to a destination directory.
     * @param zipFilePath Path to the zip file.
     * @param destDir Destination directory.
     * @return True if extraction succeeded, false otherwise.
     */
    bool extractZipFile(const std::string &zipFilePath, const std::string &destDir);

    // --- Image conversion functions ---
    /**
     * @brief Convert 3DS files to OBJ format in the output directory.
     * @return True if conversion succeeded, false otherwise.
     */
    bool imageConverter();

    /**
     * @brief Convert a single 3DS file to OBJ format.
     * @param inputFilePath Path to the input 3DS file.
     * @param outputFilePath Path to the output OBJ file.
     */
    void convert3DSToOBJ(const std::string &inputFilePath, const std::string &outputFilePath);

    /**
     * @brief Run the DH parameter calculator and validate results.
     * @return True if successful, false otherwise.
     */
    bool runDHParameterCalculator();

    /**
     * @brief Process the resource file and convert to JSON.
     * @return True if successful, false otherwise.
     */
    bool processResourceFile();

    // --- Resource parsing functions ---
    /**
     * @brief Process a line from the resource file.
     * @param line The line to process.
     * @param file Input file stream.
     */
    void processLine(string &line, ifstream &file);

    /**
     * @brief Process robot values (name, manufacturer, category) from a line.
     * @param line The line to process.
     * @param file Input file stream.
     * @return True if robot category found, false otherwise.
     */
    bool processRobotValues(const string& line, ifstream& file);

    /**
     * @brief Process the VariableSpace section.
     * @param file Input file stream.
     */
    void processVariableSpace(ifstream& file);

    /**
     * @brief Process the Geometry section.
     * @param file Input file stream.
     */
    void processGeometrySection(ifstream& file);

    /**
     * @brief Process the Dof (Degree of Freedom) section.
     * @param file Input file stream.
     */
    void processDofSection(ifstream& file);

    /**
     * @brief Process the Kinematics section.
     * @param file Input file stream.
     */
    void processKinematicsSection(ifstream& file);

    /**
     * @brief Process the JointMap section.
     * @param file Input file stream.
     */
    void processJointMapSection(ifstream& file);

    /**
     * @brief Process a line in the Kinematics or JointMap section.
     * @param line The line to process.
     */
    void processKinematicsOrJointMapSection(const std::string &line);

    /**
     * @brief Process other sections such as Dof, Offset, and GeometryMatrix.
     * @param line The line to process.
     * @param file Input file stream.
     */
    void processOtherSections(const std::string &line, std::ifstream &file);

    /**
     * @brief Process the Offset section.
     * @param line The line to process.
     * @param file Input file stream.
     */
    void processOffsetSection(const std::string &line, std::ifstream &file);

    /**
     * @brief Process the GeometryMatrix section.
     * @param line The line to process.
     */
    void processGeometryMatrixSection(const std::string &line);
};

#endif // VCMXDATAEXTRACTOR_H
