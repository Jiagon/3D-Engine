#pragma once
#include "ShaderManager.h"
#include "InputManager.h"
#include "Model.h"

#include <string>
#include <ctime>
#include <map>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


using glm::vec3;
using glm::vec2;
using std::map;
using std::string;

enum collider {
	COLLIDERLESS,
	AABB,
	SPHERE
};

//---------------------------------------------------------------------------------------------------------------------------------------------->>STRUCTS<<

struct Transform {
	vec3 loc;
	vec3 rot;
	vec3 size;
	glm::mat4 objWorld;
};

struct RigidBody {
	vec3 velocity;
	vec3 force;
	float mass;
};

struct Object {
	Transform transform;
	RigidBody rBody;
	std::string filename;
	collider collider;
};

struct Camera {
	Object pov;
	float fov;
	float aspect;
	float zNear;
	float zFar;
	Transform transform;
	RigidBody rBody;
	glm::mat4 cameraMat;
};

//---------------------------------------------------------------------------------------------------------------------------------------------->>ENGINE<<


class Engine
{
private:
	//------------------------------------------------------->>GLFWWINDOW, VERTICE VARIABLES, SHADER MANAGER<<
	GLFWwindow* GLFWwindowPtr;
	ShaderManager sm;

	//------------------------------------------------------->>TEXTURES<<
	FIBITMAP* image;
	GLuint texture;
	GLuint* textures;

	//------------------------------------------------------->>COLLECTION OF OBJECTS<<
	Object* obj;

	//------------------------------------------------------->>CAMERA
	Camera camera;

	//------------------------------------------------------->>MODEL(S)
	Model quadModel;
	Model sphereModel;
	Model boxModel;
	Model barrelModel;

	//------------------------------------------------------->>TIME VARIABLES<<
	float currTime;
	float prevTime;
	float deltaTime;

	//------------------------------------------------------->>MISCELLANEOUS VARIABLES<<
	vec3 startLocation;				// Starting location of the boid

	const float numObjects = 3;		// Number of objects that should exist - change this number to change the object numbers
	const float GRAVITY = -0.98;	// Gravity constant
	const float LIMITER = -7;		// Deceleration constant


public:
	Engine();
	~Engine();

	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShader();

	bool loadTextures(char* filePath, int index);

	void loadObjects(int whichObj, char* filePath, vec3 loc, vec3 rot, vec3 size, vec3 vel, vec3 force, float mass, collider coll);

	bool collidesWith(const Object &a, const Object &b);

	void updateCam(int upDown, int leftRight);

	void render(GLuint shader, GLint uniform, GLsizei i, GLboolean glBool, const GLfloat *value, GLenum target, GLuint texture, Model model);
};