/**
 * @file importvcmx.cpp
 * @brief Implementation of importvcmx class for importing and processing VCMX robot files.
 *
 * This file contains the implementation of the importvcmx class, which provides methods to
 * extract, convert, and parse VCMX robot files and their resources. It handles zip extraction,
 * image conversion, resource parsing, and DH parameter calculation for robot simulation and modeling.
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

#include "importvcmx.h"

/**
 * @brief Constructor for importvcmx with file path.
 * @param filePath Path to the VCMX file or extracted directory.
 */
importvcmx::importvcmx(const std::string& filePath)
	: filePath(filePath),
	isKinematicsSection(false),
	isJointMapSection(false),
	isDofSection(false),
	isDofFixedSection(false),
	isOffsetSection(false),
	isGeometryMatrixSection(false),
	isGeoFeatureSection(false),
	foundNameInDOFsection(false)
{

	if (fs::is_directory(filePath)) {
		// Already-extracted folder
		outputDir = filePath;
		skipExtraction = true;
	}
	else {
		// VCMX file
		std::string fileNameWithoutExtension = fs::path(filePath).stem().string();
		outputDir = fs::path(filePath).parent_path().string() + "/" + fileNameWithoutExtension;
		skipExtraction = false;
	}

}

/**
 * @brief Main function to import VCMX data and process all steps.
 * @return Status code (0 for success, non-zero for error).
 */
int importvcmx::importVCMXData()
{
	try
	{

		// Step 1: Run ZipExtractor
		if (!skipExtraction) {
			if (!zipExtractor())
			{
				cerr << "Error: Failed to Extract Zip Files." << endl;
				return 1; // Return error code
			}
			cout << "Step 1: Zip extraction completed successfully!" << endl;
		}
		// Step 2: Run ImageConverter for all 3DS files

		if (!imageConverter())
		{
			cerr << "Error: Failed to process new files." << endl;
			return 2; // Return error code
		}

		cout << "Step 2: 3DS to OBJ conversion completed successfully!" << endl;


		// Step 3: Get important data from component.rsc files
		if (!processResourceFile())
		{
			cerr << "Error: Failed to process resource file." << endl;
			return 3; // Return error code
		}
		cout << "Step 3: RSC to JSON conversion completed successfully!" << endl;


		// Step 4: Run DHParameterCalculator
		if (!runDHParameterCalculator()) {
			cerr << "Error: Failed to calculate and validate DH parameters." << endl;
			return 4; // Return error code for Step 4
		}
		cout << "Step 4: DH parameters calculation completed successfully!" << endl;


		/*
				string jsonFilePath = outputDir + "/component.json";

				dhCalculator calculator(jsonFilePath);

				// Calculate DH parameters
				calculator.calculateDHParameters();
		*/

	}
	catch (const exception& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	return 0; // success
}


/***************** Zip Extraction Functions ******************** */

/**
 * @brief Extract the VCMX zip archive.
 * @return True if extraction succeeded, false otherwise.
 */
bool importvcmx::zipExtractor()
{
	try
	{
		// Check if the file exists and has a ".vcmx" extension
		if (!fs::exists(filePath) || fs::path(filePath).extension() != ".vcmx")
		{
			cerr << "Error: File does not exist or is not a .vcmx file: " << filePath << endl;
			return false;
		}

		// Rename the file to .zip
		string zipFilePath = filePath.substr(0, filePath.find_last_of('.')) + ".zip";
		fs::copy(filePath, zipFilePath, fs::copy_options::overwrite_existing);

		// Create the output directory
		fs::create_directories(outputDir);

		// Extract the zip file into the output directory
		if (!extractZipFile(zipFilePath, outputDir))
		{
			cerr << "Error: Failed to extract zip file." << endl;
			return false;
		}

		// Remove the zip file after extraction
		fs::permissions(zipFilePath, fs::perms::owner_write, fs::perm_options::add);
		fs::remove(zipFilePath);
		return true;
	}
	catch (const std::exception& e)
	{
		cerr << "Error in processNewFiles: " << e.what() << endl;
		return false;
	}
}

/**
 * @brief Extract files from a zip archive to a destination directory.
 * @param zipFilePath Path to the zip file.
 * @param destDir Destination directory.
 * @return True if extraction succeeded, false otherwise.
 */
bool importvcmx::extractZipFile(const std::string& zipFilePath, const std::string& destDir)
{
	mz_zip_archive zip;
	memset(&zip, 0, sizeof(zip));

	try
	{
		// Open the zip file
		if (!mz_zip_reader_init_file(&zip, zipFilePath.c_str(), 0))
		{
			cerr << "Error opening zip file: " << zipFilePath << endl;
			return false;
		}

		// Create the destination directory
		fs::create_directories(destDir);

		// Get the number of files in the archive
		int numFiles = (int)mz_zip_reader_get_num_files(&zip);

		// Extract each file
		for (int i = 0; i < numFiles; ++i)
		{
			mz_zip_archive_file_stat fileStat;
			if (!mz_zip_reader_file_stat(&zip, i, &fileStat))
			{
				cerr << "Failed to get file info for file index: " << i << endl;
				continue;
			}

			std::string fileName = fileStat.m_filename;
			std::string outputPath = destDir + "/" + fileName;

			// Create directories if needed
			fs::create_directories(fs::path(outputPath).parent_path());

			// Extract the file
			if (!mz_zip_reader_is_file_a_directory(&zip, i))
			{
				if (!mz_zip_reader_extract_to_file(&zip, i, outputPath.c_str(), 0))
				{
					cerr << "Failed to extract file: " << fileName << endl;
				}
			}
		}

		// Close the zip archive
		mz_zip_reader_end(&zip);
		return true;
	}
	catch (const exception& e)
	{
		cerr << "Error in extractZipFile: " << e.what() << endl;
		mz_zip_reader_end(&zip); // Ensure the zip archive is closed
		return false;
	}
}


/******************** Image Converter Functions ******************** */

/**
 * @brief Convert 3DS files to OBJ format in the output directory.
 * @return True if conversion succeeded, false otherwise.
 */
bool importvcmx::imageConverter()
{
	if (!outputDir.empty())
	{
		std::set<std::string> skipExts = { ".dat", ".rsc", ".tga", ".xml", ".mtl", ".obj", ".json" };

		robotDataDir = outputDir + "/RobotData";
		if (!fs::exists(robotDataDir))
			fs::create_directories(robotDataDir);

		for (const auto& entry : fs::recursive_directory_iterator(outputDir))
		{
			if (entry.is_regular_file())
			{
				std::string filePath = entry.path().string();
				std::string extension = entry.path().extension().string();

				// Skip unwanted extensions
				if (skipExts.find(extension) != skipExts.end())
				{
					if (extension == ".rsc")
						resourceFilePath = filePath;
					continue;
				}

				std::string newFilePath = filePath;
				// Only add .3ds if not already .3ds
				if (extension != ".3ds")
				{
					newFilePath = filePath + ".3ds";
					fs::rename(filePath, newFilePath);
				}

				// Use the original file name (without any extension) for output OBJ
				std::string baseName = fs::path(filePath).stem().string();
				// If the original file had multiple extensions, get the first part
				if (extension != ".3ds" && fs::path(filePath).stem().extension() != "")
					baseName = fs::path(filePath).stem().stem().string();

				std::string outputFileName = baseName + ".obj";
				std::string outputFilePath = robotDataDir + "/" + outputFileName;

				convert3DSToOBJ(newFilePath, outputFilePath);
			}
		}
		return true;
	}
	return false;
}

/**
 * @brief Convert a single 3DS file to OBJ format.
 * @param inputFilePath Path to the input 3DS file.
 * @param outputFilePath Path to the output OBJ file.
 */
void importvcmx::convert3DSToOBJ(const string& inputFilePath, const string& outputFilePath)
{
	try
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(inputFilePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		if (!scene)
		{
			std::cerr << "Error importing 3DS file: " << importer.GetErrorString() << std::endl;
			return;
		}

		Assimp::Exporter exporter;
		if (exporter.Export(scene, "obj", outputFilePath) != AI_SUCCESS)
		{
			std::cerr << "Error exporting OBJ file: " << exporter.GetErrorString() << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception in convert3DSToOBJ: " << e.what() << std::endl;
	}
}


/********************************************************************************************* */

// In these functions i will extract the most important data from the .rsc file and convert it to a json file.
// This data will be used for the robot simulation in QT 3D Studio.

// DOF regex to match the DOF section
std::regex dofRegex(R"(Dof  \"(Rotational|Custom|RotationalFollower|Translational)\")");


/**
 * @brief Parse a resource file (.rsc) and return its data as JSON.
 * @param filePath Path to the resource file.
 * @return Parsed JSON object.
 */
json importvcmx::parse(string& filePath)
{
	try {

		if (!fs::exists(filePath))
		{
			throw runtime_error("File does not exist: " + filePath);
		}

		ifstream file(filePath);
		if (!file)
		{
			throw runtime_error("Unable to open file " + filePath);
		}

		// This root object will contain all the data extracted.
		json root;
		string line;
		while (getline(file, line))
		{
			// Process line by line
			processLine(line, file);
		}

		// Its time to collect all the pieces of puzzle.
		root["robotData"] = robotData;
		root["robotVariables"] = robotVariables;
		root["kinematics"] = kinematics;
		root["jointMap"] = jointMap;
		root["jointOffset"] = jointOffset;
		root["geometryMatrix"] = geometryMatrix;
		return root;

	}
	catch (const std::exception& e) {
		std::cerr << "[parse] Exception: " << e.what() << std::endl;
		throw;
	}
}


/**
 * @brief Process a line from the resource file.
 * @param line The line to process.
 * @param file Input file stream.
 */
void importvcmx::processLine(string& line, ifstream& file)
{
	try {
		line.erase(0, line.find_first_not_of(" \t")); // Trim leading whitespace


		if (!finishKinematicsSection && line.find("Name \"Kinematics\"") != std::string::npos)
		{
			//Extract the Kinematics Section.
			processKinematicsSection(file);
			return;
		}
		else if (!finishJointMapSection && line.find("JointMap") != std::string::npos)
		{
			// Extract the JointMap Section
			processJointMapSection(file);
			return;
		}
		else if (!foundRobotCategory && line.find("BOMname") != std::string::npos) {
			processRobotValues(line, file);
			return;
		}
		else if (foundRobotCategory && !finishVariableSpaceSection && line.find("VariableSpace") != std::string::npos)
		{
			// Here we are searching Robot variables from variableSpace until we find rSimLink section.
			processVariableSpace(file);
			return;
		}
		else if (foundGeometrySection && !foundDofSection)
		{
			// Here we are searching Joint name, Geometry link with joint and matrix until we find Dof section.
			processGeometrySection(file);
			foundGeometrySection = false;
			return;
		}
		else if (foundDofSection && !foundGeometrySection)
		{
			// Here we are searching Joint name, Geometry link with joint and matrix until we find Dof section.
			processDofSection(file);
			foundDofSection = false;
			return;
		}





		/*
		
		if (std::regex_search(line, dofRegex))
		{
			// if (line.find("Dof  \"Rotational\"") != string::npos) {
			isDofSection = true;
			foundNameInDOFsection = false;
			return;
		}

		
		if (isDofSection && line.find("}") != string::npos)
		{
			isDofSection = false;
			captureNextOffset = true;
			return;
		}

		if (captureNextOffset && line.find("Offset") != string::npos)
		{
			isOffsetSection = true;
			captureNextOffset = false;

			// cout << "Offset section Case 1" << endl;
			return;
		}

		if (line.find("Node \"rSimLink\"") != string::npos)
		{
			isGeometryMatrixSection = true;
			return;
		}

		if (isKinematicsSection || isJointMapSection)
		{
			// cout << "isKinematicsSection Section Case 0" << endl;
			processKinematicsOrJointMapSection(line);
		}
		else if (isGeometryMatrixSection)
		{
			// cout << "isGeometryMatrixSection Section Case 0" << endl;
			processGeometryMatrixSection(line);
		}
		else
		{
			// cout << "process other section Case 1" << endl;
			processOtherSections(line, file);
		}
 */

	}
	catch (const std::exception& e) {
		std::cerr << "[processLine] Exception: " << e.what() << std::endl;
		throw;
	}
}


/**
 * @brief Process robot values (name, manufacturer, category) from a line.
 * @param line The line to process.
 * @param file Input file stream.
 * @return True if robot category found, false otherwise.
 */
bool importvcmx::processRobotValues(const std::string& line, std::ifstream& file)
{

	auto extractValue = [](const std::string& line) -> std::string {
		auto pos = line.find("\"");
		if (pos != std::string::npos) {
			return line.substr(pos + 1, line.rfind("\"") - pos - 1);
		}
		return {};
		};

	if (line.find("BOMname") != std::string::npos) {
		robotData["BOMname"] = extractValue(line);
	}

	// Read next lines for BOMdescription and Category
	std::string nextLine;
	while (std::getline(file, nextLine)) {
		nextLine.erase(0, nextLine.find_first_not_of(" \t"));
		if (nextLine.find("BOMdescription") != std::string::npos) {
			robotData["BOMdescription"] = extractValue(nextLine);
		}
		else if (nextLine.find("Category") != std::string::npos) {
			robotData["Category"] = extractValue(nextLine);
			foundRobotCategory = true; // Set the flag to true when all values are found
			break; // Stop after Category
		}
	}

	return foundRobotCategory;
}

/**
 * @brief Process the VariableSpace section.
 * @param file Input file stream.
 */
void importvcmx::processVariableSpace(std::ifstream& file) {
	std::string line;
	bool inVariableSpace = true;
	while (inVariableSpace && std::getline(file, line)) {
		line.erase(0, line.find_first_not_of(" \t")); // Trim leading whitespace

		// Stop if we reach Node "rSimLink"
		if (line.find("Node \"rSimLink\"") != std::string::npos) {
			finishVariableSpaceSection = true;
			foundGeometrySection = true; // Set the flag to true when rSimLink section is found
			break;
		}

		// Look for Variable section
		if (line.find("Variable") == 0) {
			std::string varName, varValue;
			bool foundName = false, foundValue = false;
			// Enter Variable block
			while (std::getline(file, line)) {
				line.erase(0, line.find_first_not_of(" \t"));
				if (line.find("Name") == 0) {
					auto pos = line.find(" ");
					if (pos != std::string::npos) {
						varName = line.substr(pos + 1);
						if (!varName.empty() && varName.front() == '"' && varName.back() == '"')
							varName = varName.substr(1, varName.size() - 2);
						foundName = true;
					}
				}
				else if (line.find("Value") == 0) {
					auto pos = line.find(" ");
					if (pos != std::string::npos) {
						varValue = line.substr(pos + 1);
						// Remove quotes if present
						if (!varValue.empty() && varValue.front() == '"' && varValue.back() == '"')
							varValue = varValue.substr(1, varValue.size() - 2);
						foundValue = true;
					}
				}
				else if (line.find("Group") == 0 || line.find("}") == 0) {
					// End of this variable block
					break;
				}
			}
			if (foundName && foundValue) {
				// Check if varValue is numeric (integer or floating point)
				bool isNumeric = !varValue.empty() && (varValue.find_first_not_of("0123456789.-") == std::string::npos);
				if (isNumeric) {
					try {
						if (varValue.find('.') != std::string::npos) {
							robotVariables[varName] = std::stod(varValue);
						}
						else {
							robotVariables[varName] = std::stoi(varValue);
						}
					}
					catch (...) {
						// If conversion fails, skip this value
					}
				}
				// If not numeric, skip storing this variable
			}
		}
	}
}


/**
 * @brief Process the Geometry section.
 * @param file Input file stream.
 */
void importvcmx::processGeometrySection(ifstream& file) {
	string line;
	string currentJointName;
	bool foundJointName = false;

	try {
		while (getline(file, line)) {
			line.erase(0, line.find_first_not_of(" \t"));

			// Stop if we reach a Dof section
			if (line.find("Dof ") == 0) {
				// Let processLine handle the Dof section
				foundDofSection = true;
				break;
			}

			// Detect start of a new joint
			if (!foundJointName && line.find("Name ") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					currentJointName = line.substr(pos + 1);
					if (!currentJointName.empty() && currentJointName.front() == '"' && currentJointName.back() == '"')
						currentJointName = currentJointName.substr(1, currentJointName.size() - 2);
					foundJointName = true;
				}
				continue;
			}

			// Only process geometry features if we are inside a joint
			if (foundJointName && line.find("Feature \"rGeoFeature\"") == 0) {
				try {
					json geoObj;
					string geometryName, uri, matrix;
					bool foundGeometryUri = false, foundGeometryName = false;

					// Parse the rGeoFeature block
					while (getline(file, line)) {
						line.erase(0, line.find_first_not_of(" \t"));

						// End of rGeoFeature block is when we find Uri (always last in block)
						if (!foundGeometryName && line.find("Name ") == 0) {
							auto pos = line.find(" ");
							if (pos != string::npos) {
								geometryName = line.substr(pos + 1);
								if (!geometryName.empty() && geometryName.front() == '"' && geometryName.back() == '"')
									geometryName = geometryName.substr(1, geometryName.size() - 2);
								foundGeometryName = true;
							}
						}

						else if (line.find("Matrix") == 0) {
							matrix = line.substr(line.find("Matrix") + 7);
						}

						else if (line.find("Uri ") == 0) {
							auto pos = line.find(" ");
							if (pos != string::npos) {
								uri = line.substr(pos + 1);
								if (!uri.empty() && uri.front() == '"' && uri.back() == '"')
									uri = uri.substr(1, uri.size() - 2);
								foundGeometryUri = true;
							}
						}
						
						else if (line.find("Dof ") == 0) {
							// Dof found inside geometry block (should not happen, but break for safety)
							foundDofSection = true;
							break;
						}
						// Ignore stray braces and other lines
						if (foundGeometryUri) break;
					}
					if (foundGeometryName) geoObj["GeometryFile"] = geometryName;
					if (!uri.empty()) geoObj["Uri"] = uri;
					if (!matrix.empty()) geoObj["Matrix"] = matrix;
					if (!currentJointName.empty() && foundGeometryName && foundGeometryUri)
						geometryMatrix[currentJointName].push_back(geoObj);
				}
				catch (const exception& e) {
					cerr << "[processGeometrySection] Exception in rGeoFeature: " << e.what() << endl;
					
				}
			}
		}
	}
	catch (const exception& e) {
		cerr << "[processGeometrySection] Exception: " << e.what() << endl;
		
	}
}

/**
 * @brief Process the Dof (Degree of Freedom) section.
 * @param file Input file stream.
 */
void importvcmx::processDofSection(ifstream& file) {
	string line;
	string jointName;
	json jointData;
	bool inDof = true;
	bool inProperties = false;
	bool inVariable = false;
	string currentVarName;
	bool inValueBlock = false;
	bool inMinLimit = false, inMaxLimit = false, inOffset = false;

	try {
		while (getline(file, line)) {
			line.erase(0, line.find_first_not_of(" \t"));

			// Break if we reach the next rSimLink node
			if (line.find("Node \"rSimLink\"") != string::npos) {
				foundDofSection = false;
				foundGeometrySection = true;
				break;
			}
			// End of Dof section
			if (line.find("Dof ") == 0 && !jointName.empty()) {
				// If another Dof starts, stop (let processLine handle it)
				foundDofSection = true;
				foundGeometrySection = false;
				break;
			}

			// Joint name
			if (line.find("Name ") == 0 && jointName.empty()) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					jointName = line.substr(pos + 1);
					if (!jointName.empty() && jointName.front() == '"' && jointName.back() == '"')
						jointName = jointName.substr(1, jointName.size() - 2);
				}
				continue;
			}



			/*
			// Enter Properties block
			if (line.find("Properties") == 0) {
				inProperties = true;
				continue;
			}
			if (inProperties && line.find("}") == 0) {
				inProperties = false;
				continue;
			}

			// Inside Properties: look for Variable blocks
			if (inProperties && line.find("Variable") == 0) {
				inVariable = true;
				currentVarName.clear();
				continue;
			}
			if (inVariable && line.find("}") == 0) {
				inVariable = false;
				currentVarName.clear();
				continue;
			}

			// Inside Variable: get Name
			if (inVariable && line.find("Name ") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					currentVarName = line.substr(pos + 1);
					if (!currentVarName.empty() && currentVarName.front() == '"' && currentVarName.back() == '"')
						currentVarName = currentVarName.substr(1, currentVarName.size() - 2);
				}
				continue;
			}

			// MaxSpeed
			if (inVariable && !currentVarName.empty() && currentVarName == "MaxSpeed" && line.find("Value") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					string value = line.substr(pos + 1);
					jointData["MaxSpeed"] = value;
				}
				continue;
			}

			// MinLimit/MaxLimit (SoftMinLimit/SoftMaxLimit)
			if (inVariable && (currentVarName == "SoftMinLimit" || currentVarName == "SoftMaxLimit")) {
				if (line.find("Value") == 0) {
					inValueBlock = true;
					continue;
				}
				if (inValueBlock && line.find("Expression") == 0) {
					auto pos = line.find(" ");
					if (pos != string::npos) {
						string expr = line.substr(pos + 1);
						if (!expr.empty() && expr.front() == '"' && expr.back() == '"')
							expr = expr.substr(1, expr.size() - 2);
						if (currentVarName == "SoftMinLimit")
							jointData["MinLimit"] = expr;
						else if (currentVarName == "SoftMaxLimit")
							jointData["MaxLimit"] = expr;
					}
					inValueBlock = false;
					continue;
				}
			}

			*/
			// MinLimit/MaxLimit outside Properties
			if (line.find("MinLimit") == 0) {
				inMinLimit = true;
				continue;
			}
			if (inMinLimit && line.find("Expression") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					string expr = line.substr(pos + 1);
					if (!expr.empty() && expr.front() == '"' && expr.back() == '"')
						expr = expr.substr(1, expr.size() - 2);
					jointData["MinLimit"] = expr;
				}
				inMinLimit = false;
				continue;
			}
			if (line.find("MaxLimit") == 0) {
				inMaxLimit = true;
				continue;
			}
			if (inMaxLimit && line.find("Expression") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					string expr = line.substr(pos + 1);
					if (!expr.empty() && expr.front() == '"' && expr.back() == '"')
						expr = expr.substr(1, expr.size() - 2);
					jointData["MaxLimit"] = expr;
				}
				inMaxLimit = false;
				continue;
			}

			// AxisType
			if (line.find("AxisType") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					string axisType = line.substr(pos + 1);
					jointData["AxisType"] = axisType;
				}
				continue;
			}

			// Offset block
			if (line.find("Offset") == 0) {
				inOffset = true;
				continue;
			}
			if (inOffset && line.find("Expression") == 0) {
				auto pos = line.find(" ");
				if (pos != string::npos) {
					string expr = line.substr(pos + 1);

					// Handle multi-line expressions ending with '\'
					while (!expr.empty() && expr.back() == '\\') {
						expr.pop_back(); // Remove the backslash
						string nextLine;
						if (getline(file, nextLine)) {
							nextLine.erase(0, nextLine.find_first_not_of(" \t"));
							expr += nextLine;
						}
						else {
							break;
						}
					}

					// Remove surrounding quotes if present
					if (expr.size() > 1 && expr.front() == '"' && expr.back() == '"')
						expr = expr.substr(1, expr.size() - 2);

					jointData["Offset"] = expr;
				}
				inOffset = false;
				continue;
			}
		}
		// Store the joint data if jointName is found
		if (!jointName.empty() && !jointData.empty()) {
			jointOffset[jointName] = jointData;
		}
	}
	catch (const exception& e) {
		cerr << "[processDofSection] Exception: " << e.what() << endl;
	}
}


/**
 * @brief Process the Kinematics section.
 * @param file Input file stream.
 */
void importvcmx::processKinematicsSection(std::ifstream& file)
{
	std::string sectionLine;
	while (std::getline(file, sectionLine))
	{
		sectionLine.erase(0, sectionLine.find_first_not_of(" \t")); // Trim leading whitespace
		if (sectionLine.find("}") != std::string::npos)
		{
			// End of Kinematics section
			finishKinematicsSection = true;
			break;
		}
		// Parse key-value pairs
		auto pos = sectionLine.find(" ");
		if (pos != std::string::npos)
		{
			std::string key = sectionLine.substr(0, pos);
			std::string value = sectionLine.substr(pos + 1);

			// Remove quotes from string values
			if (!value.empty() && value.front() == '\"' && value.back() == '\"')
			{
				value = value.substr(1, value.size() - 2);
			}

			// Convert numeric values to appropriate types
			if (!value.empty() && value.find_first_not_of("0123456789.-") == std::string::npos)
			{
				try
				{
					if (value.find('.') != std::string::npos)
						kinematics[key] = std::stod(value);
					else
						kinematics[key] = std::stoi(value);
				}
				catch (...)
				{
					kinematics[key] = value; // fallback to string
				}
			}
			else
			{
				kinematics[key] = value;
			}
		}
	}
}

/**
 * @brief Process the JointMap section.
 * @param file Input file stream.
 */
void importvcmx::processJointMapSection(std::ifstream& file)
{
	std::string sectionLine;
	while (std::getline(file, sectionLine))
	{
		sectionLine.erase(0, sectionLine.find_first_not_of(" \t"));
		if (sectionLine.find("}") != std::string::npos)
		{
			// End of JointMap section
			finishJointMapSection = true;
			break;
		}
		// Find the last space (separates key and value)
		auto lastSpace = sectionLine.rfind(' ');
		if (lastSpace != std::string::npos)
		{
			std::string key = sectionLine.substr(0, lastSpace);
			std::string value = sectionLine.substr(lastSpace + 1);
			// Remove quotes from value if present
			if (!value.empty() && value.front() == '"' && value.back() == '"')
			{
				value = value.substr(1, value.size() - 2);
			}
			jointMap[key] = value;
		}
	}
}

/**
 * @brief Process a line in the Kinematics or JointMap section.
 * @param line The line to process.
 */
void importvcmx::processKinematicsOrJointMapSection(const string& line)
{
	try {

		if (line.find("}") != string::npos)
		{
			if (isKinematicsSection)
			{
				isKinematicsSection = false;
			}
			if (isJointMapSection)
			{
				isJointMapSection = false;
			}
			return;
		}

		auto pos = line.find(" ");
		if (isJointMapSection)
		{
			// Find the second space
			if (pos != string::npos)
			{
				auto secondPos = line.find(" ", pos + 1); // Find space after the first one
				if (secondPos != string::npos)
				{
					pos = secondPos;
				}
			}
		}

		if (pos != string::npos)
		{
			string key = line.substr(0, pos);
			string value = line.substr(pos + 1);

			// Remove quotes from string values
			if (!value.empty() && value.front() == '\"' && value.back() == '\"')
			{
				value = value.substr(1, value.size() - 2);
			}

			// Convert numeric values to appropriate types
			if (!value.empty() && value.find_first_not_of("0123456789.-") == string::npos)
			{
				try
				{
					if (value.find('.') != string::npos)
					{
						if (isKinematicsSection)
						{
							kinematics[key] = stod(value);
						}
						else if (isJointMapSection)
						{
							jointMap[key] = stod(value);
						}
					}
					else
					{
						if (isKinematicsSection)
						{
							kinematics[key] = stoi(value);
						}
						else if (isJointMapSection)
						{
							jointMap[key] = stoi(value);
						}
					}
				}
				catch (const std::invalid_argument& e)
				{
					cerr << "Invalid argument for stoi: " << value << endl;
				}
				catch (const std::out_of_range& e)
				{
					cerr << "Out of range for stoi: " << value << endl;
				}
			}
			else
			{

				if (isKinematicsSection)
				{
					kinematics[key] = value;
				}
				else if (isJointMapSection)
				{
					jointMap[key] = value;
				}
			}
		}

	}
	catch (const std::exception& e) {
		std::cerr << "[processKinematicsOrJointMapSection] Exception: " << e.what() << std::endl;
		throw; // rethrow to propagate up if needed
	}
}

/**
 * @brief Process other sections such as Dof, Offset, and GeometryMatrix.
 * @param line The line to process.
 * @param file Input file stream.
 */
void importvcmx::processOtherSections(const string& line, ifstream& file)
{
	try {

		if (isDofSection)
		{

			if (!foundNameInDOFsection && line.find("Name") != string::npos)
			{
				auto pos = line.find(" ");
				if (pos != string::npos)
				{
					currentJointName = line.substr(pos + 1);
					// cout << "Offset section Case 2 Name found" << endl;
					if (currentJointName.size() > 1 && currentJointName.front() == '\"' && currentJointName.back() == '\"')
					{
						currentJointName = currentJointName.substr(1, currentJointName.size() - 2);
					}
					foundNameInDOFsection = true;
				}
			}
		}

		if (isOffsetSection)
		{
			processOffsetSection(line, file);
		}

	}
	catch (const std::exception& e) {
		std::cerr << "[processOtherSections] Exception: " << e.what() << std::endl;
		throw; // rethrow to propagate up if needed
	}
}

/**
 * @brief Process the Offset section.
 * @param line The line to process.
 * @param file Input file stream.
 */
void importvcmx::processOffsetSection(const string& line, ifstream& file)
{
	try {

		string offsetLine = line;
		// cout << "Offset section Case 3 processoffsetSection" << endl;
		offsetLine.erase(0, offsetLine.find_first_not_of(" \t")); // Trim leading whitespace
		if (offsetLine.find("Expression") != string::npos)
		{
			auto pos = offsetLine.find(" ");
			if (pos != string::npos)
			{
				currentOffsetExpression = offsetLine.substr(pos + 1);

				// I need to handle a case where the expression is multiline.
				while (!currentOffsetExpression.empty() && currentOffsetExpression.back() == '\\')
				{
					currentOffsetExpression.pop_back(); // Remove the backslash
					string nextLine;
					if (getline(file, nextLine))
					{
						nextLine.erase(0, nextLine.find_first_not_of(" \t")); // Trim leading whitespace
						currentOffsetExpression += nextLine;
					}
				}

				if (currentOffsetExpression.size() > 1 && currentOffsetExpression.front() == '\"' && currentOffsetExpression.back() == '\"')
				{
					currentOffsetExpression = currentOffsetExpression.substr(1, currentOffsetExpression.size() - 2);
				}
				//  cout << "Offset section Case 4 currentoffsetexpression = " + currentOffsetExpression << endl;
			}
		}
		if (offsetLine.find("}") != string::npos)
		{
			if (!currentJointName.empty() && !currentOffsetExpression.empty())
			{
				jointOffset[currentJointName] = currentOffsetExpression;
				// cout << "Offset section Case 5 currentJointName = " + currentJointName << endl;
			}
			currentJointName.clear();
			currentOffsetExpression.clear();
			isOffsetSection = false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "[processOffsetSection] Exception: " << e.what() << std::endl;
		throw; // rethrow to propagate up if needed
	}
}

/**
 * @brief Process the GeometryMatrix section.
 * @param line The line to process.
 */
void importvcmx::processGeometryMatrixSection(const string& line)
{
	try {

		// As I need three things from this section that is Joint Name, Geometry File and Matrix.
		// Finishing at VariableSpace is bad approach but I dont want to process long lines that i dont need.
		if (line.find("VariableSpace") != string::npos)
		{
			if (!currentRSimLinkName.empty() && !currentGeoFeatureName.empty())
			{
				rSimLinkObject["GeometryFile"] = currentGeoFeatureName;
				if (!currentMatrix.empty())
				{
					rSimLinkObject["Matrix"] = currentMatrix;
				}
				geometryMatrix[currentRSimLinkName].push_back(rSimLinkObject);
			}
			isGeometryMatrixSection = false;
			currentRSimLinkName.clear();
			currentGeoFeatureName.clear();
			currentMatrix.clear();
			return;
		}

		// only get first name and ignore the rest
		if (currentRSimLinkName.empty() && line.find("Name") != string::npos)
		{
			auto pos = line.find(" ");
			if (pos != string::npos)
			{
				currentRSimLinkName = line.substr(pos + 1);
				if (currentRSimLinkName.size() > 1 && currentRSimLinkName.front() == '\"' && currentRSimLinkName.back() == '\"')
				{
					currentRSimLinkName = currentRSimLinkName.substr(1, currentRSimLinkName.size() - 2);
				}
			}
		}

		if (line.find("Feature \"rGeoFeature\"") != string::npos)
		{
			isGeoFeatureSection = true;
			return;
		}

		if (isGeoFeatureSection && line.find("VariableSpace") != string::npos)
		{
			isGeoFeatureSection = false;
			return;
		}

		// Getting the Geometry File name.
		if (isGeoFeatureSection)
		{
			if (line.find("Name") != string::npos)
			{
				auto pos = line.find(" ");
				if (pos != string::npos)
				{
					currentGeoFeatureName = line.substr(pos + 1);
					if (currentGeoFeatureName.size() > 1 && currentGeoFeatureName.front() == '\"' && currentGeoFeatureName.back() == '\"')
					{
						currentGeoFeatureName = currentGeoFeatureName.substr(1, currentGeoFeatureName.size() - 2);
					}
				}
			}

			// Getting the Matrix with the hope that i will use it in 3D modeling.
			if (line.find("Matrix") != string::npos)
			{
				currentMatrix = line.substr(line.find("Matrix") + 7);
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "[processGeometryMatrixSection] Exception: " << e.what() << std::endl;
		throw; // rethrow to propagate up if needed
	}
}

/**
 * @brief Process the resource file and convert to JSON.
 * @return True if successful, false otherwise.
 */
bool importvcmx::processResourceFile()
{

	if ((!resourceFilePath.empty()) && (fs::exists(resourceFilePath)))
	{
		try
		{
			json rscJson = parse(resourceFilePath);

			jsonFilePath = robotDataDir + "/component.json";
			ofstream outputFile(jsonFilePath);
			outputFile << rscJson.dump(4); // Pretty print with 4 spaces indent
			outputFile.close();

			cout << "Processed: " << resourceFilePath << " -> " << jsonFilePath << endl;
			return true;
		}
		catch (const exception& e)
		{
			cerr << "Error processing file " << resourceFilePath << ": " << e.what() << endl;
			return false;
		}
	}
	return false;
}

/********************************************************************************************* */

/**
 * @brief Run the DH parameter calculator and validate results.
 * @return True if successful, false otherwise.
 */
bool importvcmx::runDHParameterCalculator() {
	try {

		if ((!jsonFilePath.empty()) && (fs::exists(jsonFilePath)))
		{

			dhCalculator calculator(jsonFilePath);

			// Calculate DH parameters
			calculator.calculateDHParameters();

			// Validate DH parameters
			calculator.validateDHParameters(jsonFilePath);

			return true; // Success
		}
		else {
			cerr << "Error: component.json file does not exist." << endl;
			return false; // Failure
		}
	}
	catch (const std::exception& e) {
		cerr << "Error in DHParameterCalculator: " << e.what() << endl;
		return false; // Failure
	}
}

