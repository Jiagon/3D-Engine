#include "Model.h"



Model::Model()
{
	vertCount = 0;
	vertArr = 0;
}


Model::~Model()
{
}

bool Model::buffer(std::string objFile) {
	// Declare vectors
	std::vector<glm::vec3> locs;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;
	std::vector<VertInd> vertInds;

	// Open file
	std::fstream modelFile;
	modelFile.open(objFile);

	if (!(modelFile.is_open()))
	{
		return false;
	}

	// Open the file -> obj models aren't binary
	std::fstream inFile(objFile);
	if (inFile.is_open()) {

		std::string line;
		std::string linelabel;
		// Read from a single line in the file until every iteration is empty
		while (std::getline(inFile, line)) {
			std::istringstream ss(line);
			// Read a string (line label) from the line
			ss >> linelabel;
			// If the label is "v" read 3 floats from the line / add new vec3 location
			// If the label is "vt" read 2 floats from the line / add new vec2 uv
			if (linelabel == "vt") {
				float a;
				float b;

				ss >> a >> b;
				uvs.push_back(glm::vec2(a, b));
			}
			// If the label is "vn" read 3 floats from the line / add new vec3 norm
			else if (linelabel == "vn") {
				float a;
				float b;
				float c;

				ss >> a >> b >> c;
				norms.push_back(glm::vec3(a, b, c));
			}
			else if (linelabel == "v") {
				float a;
				float b;
				float c;

				ss >> a >> b >> c;
				locs.push_back(glm::vec3(a, b, c));
			}
			else if (linelabel == "f") {
				unsigned int a;
				unsigned int b;
				unsigned int c;
				char chr;

				for (int i = 0; i < 3; i++) {
					// Read three unsigned ints separated by char slashes (/)
					ss >> a >> chr >> b >> chr >> c;
					// Decrement each integer by 1
					a--;
					b--;
					c--;

					// Put the indices into a VertInd and add it to the vertInds collections
					VertInd v;
					v.locInd = a;
					v.uvInd = b;
					v.normInd = c;

					vertInds.push_back(v);
				}
			}
			else {
				std::cout << "Model file loaded linelabel " << linelabel << ", could not be deciphered." << std::endl;
			}
		}
		inFile.close();
	}
	else {
		std::cout << "Model file " << objFile << " could not be opened." << std::endl;
		return false;
	}



	// Get vertBuffData
	vertCount = vertInds.size();
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locs[vertInds[i].locInd], uvs[vertInds[i].uvInd], norms[vertInds[i].normInd] };
	}

	// Set up a buffer
	GLuint vertBuf;
	glGenVertexArrays(1, &vertArr);
	glGenVertexArrays(1, &vertBuf);

	// Bind the vertex array
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &vertBufData[0], GL_STATIC_DRAW);

	// LOCS
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// UVS
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	// FILTHY NORMS
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + (sizeof(glm::vec2))));

	glBindVertexArray(0);


	glClearColor(.392f, .584f, .929f, 1.0f);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}