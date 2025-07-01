#include "importvcmx.h"

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


/***************** Zip Extraction Funtions ******************** */

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
std::regex KinematicsRegex(R"(Functionality\s*\"(rKinArticulated2|rKinParallellogram)\")");

// This function will parse the .rsc file and return the json object
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

// This function will process the line and extract the data
void importvcmx::processLine(string& line, ifstream& file)
{
	try {
		line.erase(0, line.find_first_not_of(" \t")); // Trim leading whitespace

		//Capture Robot Basic Values
		processRobotValues(line);


		if (line.find("Name \"Kinematics\"") != std::string::npos)
		{
			processKinematicsSection(file);
			return;
		}

		if (line.find("JointMap") != std::string::npos)
		{
			processJointMapSection(file);
			return;
		}

/*
		if (std::regex_search(line, KinematicsRegex))
		{
			// if (line.find("Functionality \"rKinArticulated2\"") != string::npos) {
			isKinematicsSection = true;
			isJointMapSection = false;
			return;
		}

		if (line.find("JointMap") != string::npos)
		{
			isJointMapSection = true;
			isKinematicsSection = false;
			return;
		}
		
		if (std::regex_search(line, dofRegex))
		{
			// if (line.find("Dof  \"Rotational\"") != string::npos) {
			isDofSection = true;
			foundNameInDOFsection = false;
			return;
		}
		/*
				if (line.find("Dof \"Fixed\"") != string::npos) {
					isDofFixedSection = true;
					return;
				}
		*/
		/*
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

// Helper function to get the values of Robot Name , Manufacturer and category.
bool importvcmx::processRobotValues(const std::string& line) 
{

	if (foundAllRobotValues)
		return true;

	bool updated = false;
	auto extractValue = [](const std::string& line) -> std::string {
		auto pos = line.find("\"");
		if (pos != std::string::npos) {
			return line.substr(pos + 1, line.rfind("\"") - pos - 1);
		}
		return {};
		};

	if (line.find("BOMname") != std::string::npos) {
		robotData["BOMname"] = extractValue(line);
		updated = true;
	}
	else if (line.find("BOMdescription") != std::string::npos) {
		robotData["BOMdescription"] = extractValue(line);
		updated = true;
	}
	else if (line.find("Category") != std::string::npos) {
		robotData["Category"] = extractValue(line);
		updated = true;
	}

	// If all three fields are present, set the static flag
	if (robotData.contains("BOMname") &&
		robotData.contains("BOMdescription") &&
		robotData.contains("Category")) {
		foundAllRobotValues = true;
	}

	return updated;
}

// This function will handle the Kinematics section.
void importvcmx::processKinematicsSection(std::ifstream& file)
{
	std::string sectionLine;
	while (std::getline(file, sectionLine))
	{
		sectionLine.erase(0, sectionLine.find_first_not_of(" \t")); // Trim leading whitespace
		if (sectionLine.find("}") != std::string::npos)
		{
			// End of Kinematics section
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

// This function Handles the JointMap section
void importvcmx::processJointMapSection(std::ifstream& file)
{
	std::string sectionLine;
	while (std::getline(file, sectionLine))
	{
		sectionLine.erase(0, sectionLine.find_first_not_of(" \t"));
		if (sectionLine.find("}") != std::string::npos)
		{
			// End of JointMap section
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

// This function will process the Kinematics and JointMap section
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

// This function will process the other sections like Dof, Offset and GeometryMatrix
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

// This function will process the Offset section
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

// This function will process the GeometryMatrix section
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

