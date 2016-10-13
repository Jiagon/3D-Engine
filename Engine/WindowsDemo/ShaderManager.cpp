#include "ShaderManager.h"


//----------------------------------------------------------------------------------------------------------->>CONSTRUCTOR / DESTRUCTOR<<
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

//----------------------------------------------------------------------------------------------------------->>PROGRAM GETTER<<

GLuint ShaderManager::getProgram() const
{
	return program;
}

//----------------------------------------------------------------------------------------------------------->>LOADS VERT / FRAG SHADERS<<

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	// Call loadShader() for both files and pass in the appropriate GLenum shader types
		// If either loads failed (result is 0) then also return a 0
	GLuint vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexShader == 0)
		return 0;

	GLuint fragShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragShader == 0)
		return 0;

	// Create a shader program with glCreateProgram()
		// Save the result in the member data program
	program = glCreateProgram();

	// Use glAttachShader() twice to attach both shaders loaded
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);

	// Link the program with glLinkProgram()
	glLinkProgram(program);

	// Determine if the program was linked correctly with glGetProgramiv()
		// If linked, return true
	GLint returnData;
	glGetProgramiv(program, GL_LINK_STATUS, &returnData);
	if (returnData == GL_TRUE)
	{
		return true;
	}

	// If it didn't link, use glGetProgramiv() and glGetProgramInfoLog() - similar to shader versions

	// Create a GLint to hold log length then call glGetProgramiv() again to get length
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	// Create an array to hold the log
	GLchar* logData = new GLchar[logLength];
	// Call glGetShaderInfoLog() to get and print out the data
	glGetProgramInfoLog(program, logLength, 0, logData);

	// Delete everything and return false
	glDeleteProgram(program);
	delete[] logData;

	return false;
}

//----------------------------------------------------------------------------------------------------------->>READS VERT / FRAG SHADERS<<

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	GLchar* arr;
	int track = 0;

	// Open the specified file
	// Reading Output File.txt using a binary ifstream
	std::ifstream inFileBinary(file, std::ios::binary);
	if (inFileBinary.is_open())
	{
		// Get length
		inFileBinary.seekg(0, std::ios::end);
		unsigned int length = inFileBinary.tellg();
		inFileBinary.seekg(0, std::ios::beg);

		// Load into a null-terminated array of characters
		// Read file into char array
		arr = new char[length + 1];
		inFileBinary.read(arr, length);
		arr[length] = '\0';

		inFileBinary.close();
	}
	// If there's a problem opening the file, print an error message and return 0
	else
	{
		std::cout << "There was a problem loading the file " << file << "." << std::endl;
		arr = new char[1];
		delete[] arr;
		return 0;
	}

	//Call glCreateShader(), glShaderSource(), and glCompileShader()
		//Delete[] char array from last step
	GLuint glShader = glCreateShader(shaderType);

	glShaderSource(glShader, 1, &arr, 0);

	glCompileShader(glShader);

	delete[] arr;

	// Determine if the shader compiled by calling glGetShaderiv()
		// Declare GLint var to hold results
	GLint shaderCompile;
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &shaderCompile);
	// If compilation succeeded, return shader's index
	if (shaderCompile != 0)
	{
		return glShader;
	}

	// If compilation didn't succeed you'll need the info log
		// Create a GLint to hold log length then call glGetShaderiv() again to get length
	GLint logLength;
	glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &logLength);

	// Create an array to hold the log
	GLchar* logData = new GLchar[logLength];
	// Call glGetShaderInfoLog() to get and print out the data
	glGetShaderInfoLog(glShader, logLength, 0, logData);

	std::cout << "Log Length: " << logLength << std::endl;
	std::cout << "Log Data: " << logData << std::endl;

	// Delete everything and return 0
	glDeleteShader(glShader);
	delete[] logData;

	return 0;
}
