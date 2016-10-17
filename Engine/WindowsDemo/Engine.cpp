#include "Engine.h"

//----------------------------------------------------------------------------------------------------------->>CONSTRUCTOR / DESTRUCTOR<<
Engine::Engine()
{
	sm = ShaderManager();
	obj = new Object[numObjects];
	textures = new GLuint[numObjects];

	// CAMERA SETUP
	camera.fov = 3.14159f * .4f / 1.f;
	camera.aspect = 800.f / 600.f;
	camera.zNear = .01f;
	camera.zFar = 1000.f;

	camera.transform.loc = vec3(0, 0, 0);
	camera.transform.rot = vec3(0, 0, 0);
	camera.transform.size = vec3(1, 1, 1);

	camera.rBody.force = vec3(0, 0, 0);
	camera.rBody.mass = 1;
	camera.rBody.velocity = vec3(0, 0, 0);

	camera.cameraMat = glm::mat4{ 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

	quadModel = Model();
	sphereModel = Model();
	boxModel = Model();
	//barrelModel = Model();

}

Engine::~Engine()
{
	delete[] obj;
	delete[] textures;
}

//----------------------------------------------------------------------------------------------------------->>INITIALIZE GLFW / GLEW<<
//----------------------------------------------------------------------------------------------------------->>MAPS KEY / MOUSE INPUT<<

bool Engine::init()
{

	// Initialize the window library
	// Initializes GLFW
	// Returns GL_True, which is #define'd as 1 if it works
	// Returns GL_FALSE, which is #define'd as 0 is if fails
	if (glfwInit() == GL_FALSE)
		return false;


	// Create a windowed mode window
	// Creates a window w/parameters given
	// Returns address of GLFWwindow struct
	// Returns nullptr if it fails to create the window
	GLFWwindowPtr = glfwCreateWindow(800, 600, "Sam Levey DSA1 Engine", NULL, NULL);

	// Make it the currently active window or quit
	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else
	{
		glfwTerminate();
		return false;
	}

	// Initialize GLEW or quit
	// Returns GLEW_OK, which is #define'd as 0
	// Returns something else if it fails
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}


	// Sets click function when loading game
	glfwSetMouseButtonCallback(GLFWwindowPtr, InputManager::mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, InputManager::keyCallback);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

//----------------------------------------------------------------------------------------------------------->>BUFFERS MODELS / CREATES VERTICES<<

bool Engine::bufferModels()
{

	if (!quadModel.buffer("models/models/quad.obj"))
		return false;

	if (!sphereModel.buffer("models/models/sphere.obj"))
		return false;

	if (!boxModel.buffer("models/models/box.obj"))
		return false;

	/*if (!barrelModel.buffer("models/models/Barrel.obj"))
		return false;
		*/

		//---------------------->>CALLS LOAD TEXTURES TO LOAD ALL TEXTURES<<

	if (!loadTextures("images/Bird.png", 0) || (!loadTextures("images/Ground.png", 1)))
		return false;

	//---------------------->>CREATES THE OBJECTS<<
	vec3 loc = { -.75, 0, 0 };
	vec3 rot = { 0, 0, 0 };
	vec3 size = { 0.1, 0.1, 0.1 };
	vec3 vel = { 0, 0, 0 };
	vec3 force = { 0, 0, 0 };
	float mass = 3;

	// OBJECT 0
	loadObjects(0, "images/Bird.png", loc, rot, size, vel, force, mass, SPHERE);
	startLocation = loc;

	// OBJECT 1
	loc = { -.75, .75, 0 };
	size = { 4, 0.25, 0.01 };
	rot = { 0, 0, glm::radians(180.0f) };
	mass = 1;
	loadObjects(1, "images/Ground.png", loc, rot, size, vel, force, mass, AABB);

	loc = { 0, 0, 0 };
	size = { 0.2, 0.2, 0.2 };
	rot = { 0, 0, 0 };
	loadObjects(2, "images/Bird.png", loc, rot, size, vel, force, mass, AABB);


	// Cornflower blue = for you XNA fans
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

	return true;
}

//----------------------------------------------------------------------------------------------------------->>GAME LOOP<<

bool Engine::gameLoop()
{

	glUseProgram(sm.getProgram());
	float myData = .5;
	glUniform1f(3, myData);

	GLfloat g1 = 0;
	GLfloat g2 = 1;

	vec2 myData2(g1, g2);
	glUniform2f(4, 1, g1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::mat4 translate;

	glm::vec3 lightPos = glm::vec3(3, 0, 1.5);

	// Initialize a value for game time using GLFW

	currTime = glfwGetTime();

	// Loop until the user closes the window -- the Game Loop!
	// Returns true if the window was told to close during the last frame
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{

		// Remake the variable every time to initialize it to 0
		map<string, int> keyboard;		// Map to keep track of which character key was pressed and how many times - FOR CAMERA OBJECT


		// Update time values
		prevTime = currTime;
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;

		// Upload lighting values to shaders
		glUniform3f(3, lightPos.x, lightPos.y, lightPos.z);

		// Update input
		// Esc key - exits screen
		if (InputManager::keyIsDown[GLFW_KEY_ESCAPE] && !InputManager::keyWasDown[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		// Mouse button - does nothing right now
		if (InputManager::keyIsDown[GLFW_MOUSE_BUTTON_1] && !InputManager::keyWasDown[GLFW_MOUSE_BUTTON_1]) {
			keyboard["key"]++;
		}
		// Left arrow key - moves to the left - cannot be used with RAK
		if (InputManager::keyIsDown[GLFW_KEY_LEFT]) {
			keyboard["left"]++;
		}
		// Right arrow key - moves to the right - cannot be used with LAK
		else if (InputManager::keyIsDown[GLFW_KEY_RIGHT]) {
			keyboard["right"]++;
		}
		// Space bar - moves up (jumps)
		if (InputManager::keyIsDown[GLFW_KEY_UP] && !InputManager::keyWasDown[GLFW_KEY_UP]) {
			keyboard["up"]++;
		}
		if (InputManager::keyIsDown[GLFW_KEY_S]) {
			keyboard["s"]++;
		}
		else if (InputManager::keyIsDown[GLFW_KEY_W]) {
			keyboard["w"]++;
		}
		if (InputManager::keyIsDown[GLFW_KEY_A]) {
			keyboard["a"]++;
		}
		else if (InputManager::keyIsDown[GLFW_KEY_D]) {
			keyboard["d"]++;
		}
		if (InputManager::keyIsDown[GLFW_KEY_R]) {
			camera.transform.loc = glm::vec3(0, 0, 0);
		}
		InputManager::keyWasDown = InputManager::keyIsDown;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate the value of the transformation matrix for each object using glm\gtx\transform and glm\gtx\euler-angles and the object's transform data
		for (int i = 0; i < numObjects; i++) {

			// RESET THE FORCES ON EVERYTHING
			obj[i].rBody.force = glm::vec3(0, 0, 0);

			// Approximate Newtonian mechanics to:
			// Apply gravitational force (projectile objects) (F = m * a)
			// GRAVITATIONAL FORCE AND CLICKS WILL ONLY BE A(CLICK)ABLE FOR THE BIRB -- (aclickable geddit lol I'm lonely)
			if (obj[i].filename == "images/Bird.png") {

				// GRAVITY - ONLY FOR THE BOID
				//obj[i].rBody.force.y += obj[i].rBody.mass * GRAVITY;
				camera.pov.rBody.force.y += camera.pov.rBody.mass;

				for (int j = 0; j < keyboard["left"]; j++) {
					//obj[i].rBody.force.x = -5;
					camera.pov.rBody.force.x = -5;
				}
				for (int j = 0; j < keyboard["right"]; j++) {
					//obj[i].rBody.force.x = 5;
					camera.pov.rBody.force.x = 5;
				}
				for (int j = 0; j < keyboard["up"]; j++) {
					//obj[i].rBody.force.y = 95;
					//obj[i].rBody.velocity.y = 0;
					camera.pov.rBody.force.y = 95;
					camera.pov.rBody.velocity.y = 0;
				}

				// COLLIDER - ONLY FOR THE BOID
				for (int j = 0; j < numObjects - 1; j++) {
					if (i != j)
					{
						if (collidesWith(obj[i], obj[j])) {
							obj[i].transform.loc = startLocation;
							obj[i].rBody.force = vec3(0, 0, 0);
							obj[i].rBody.velocity = vec3(0, 0, 0);
						}
					}
				}
			}

			// Derive change in velocity from total force, mass, change in time
			obj[i].rBody.force += obj[i].rBody.velocity * LIMITER;
			obj[i].rBody.velocity += obj[i].rBody.force * (deltaTime / obj[i].rBody.mass);


			// Derive change in location from velocity, change in time
			obj[i].transform.loc += obj[i].rBody.velocity * (float)deltaTime;


			/*for (map<string, int>::iterator it = keyboard.begin(); it != keyboard.end(); it++) {
				it = 0;
			}*/



			// TRANSLATE MATRIX
			translate = { 1, 0, 0, obj[i].transform.loc.x,
				0, 1, 0, obj[i].transform.loc.y,
				0, 0, 1, obj[i].transform.loc.z,
				0, 0, 0, 1 };

			translate = glm::transpose(translate);

			// OTHER TRANSLATE MATRIX
			//glm::translate(obj[i].transform.loc);		<-- DOESN'T WORK		// JUST KIDDING MAYBE DOES WORK BUT I'M NOT GOING TO REIMPLEMENT IT?

			obj[i].transform.objWorld = translate *
				glm::yawPitchRoll(obj[i].transform.rot.y, obj[i].transform.rot.x, obj[i].transform.rot.z) *
				glm::scale(obj[i].transform.size);

		}



		// Derive change in velocity from total force, mass, change in time
		camera.pov.rBody.force += camera.pov.rBody.velocity * LIMITER;
		camera.pov.rBody.velocity += camera.pov.rBody.force * (deltaTime / camera.pov.rBody.mass);


		// Derive change in location from velocity, change in time
		camera.pov.transform.loc += camera.pov.rBody.velocity * (float)deltaTime;


		// TRANSLATE MATRIX
		translate = { 1, 0, 0, camera.pov.transform.loc.x,
			0, 1, 0, camera.pov.transform.loc.y,
			0, 0, 1, camera.pov.transform.loc.z,
			0, 0, 0, 1 };

		translate = glm::transpose(translate);

		// OTHER TRANSLATE MATRIX
		//glm::translate(obj[i].transform.loc);		<-- DOESN'T WORK		// JUST KIDDING MAYBE DOES WORK BUT I'M NOT GOING TO REIMPLEMENT IT?

		camera.pov.transform.objWorld = translate *
			glm::yawPitchRoll(camera.pov.transform.rot.y, camera.pov.transform.rot.x, camera.pov.transform.rot.z) *
			glm::scale(camera.pov.transform.size);

		updateCam(keyboard["s"] - keyboard["w"], keyboard["a"] - keyboard["d"]);



		// Draw buffered models
		// Process input/window events



		// RENDER CAMERA
		GLint uniformVar = glGetUniformLocation(sm.getProgram(), "cameraMat");

		glProgramUniformMatrix4fv(sm.getProgram(), uniformVar, 1, GL_FALSE, &camera.cameraMat[0][0]);
		//glUniformMatrix4fv(uniformVar, 1, GL_FALSE, &camMat[0][0]);


		// RENDER OBJECTS
		uniformVar = glGetUniformLocation(sm.getProgram(), "myMatrix");

		//DOESN'T WORK
		//glUniform2fv("shaders/vertex.glsl", 1, obj[i].transform.objWorld);

		render(sm.getProgram(), uniformVar, 1, GL_FALSE, &obj[0].transform.objWorld[0][0], GL_TEXTURE_2D, textures[0], sphereModel);

		render(sm.getProgram(), uniformVar, 1, GL_FALSE, &obj[1].transform.objWorld[0][0], GL_TEXTURE_2D, textures[1], boxModel);

		render(sm.getProgram(), uniformVar, 1, GL_FALSE, &obj[2].transform.objWorld[0][0], GL_TEXTURE_2D, textures[0], boxModel);

		glfwSwapBuffers(GLFWwindowPtr);



		// Process quewed window, mouse, & keyboard callback events
		// Events queue up during a frame
		// Draging screen around
		// Pressing a key
		// Once per frame, the events must be processed (will be done at a later time)


		glfwPollEvents();

	}
	// Must be called before engine closes or it will probably leak memory or worse
	glfwTerminate();
	return true;
}

//----------------------------------------------------------------------------------------------------------->>LOADS SHADERS<<

bool Engine::useShader()
{
	return sm.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl");
}

//----------------------------------------------------------------------------------------------------------->>LOADS TEXTURES<<

bool Engine::loadTextures(char * filePath, int index)
{
	/* Use FreeImage to load texture / check if it worked */
	if (filePath == NULL)
		return false;

	image = FreeImage_ConvertTo32Bits(FreeImage_Load(FreeImage_GetFileType(filePath, 0), filePath));

	texture = 0;

	glGenTextures(1, &texture);

	textures[index] = texture;

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(image);
	return true;
}

//----------------------------------------------------------------------------------------------------------->>SETS OBJECTS<<

void Engine::loadObjects(int whichObj, char* filePath, vec3 loc, vec3 rot, vec3 size, vec3 vel, vec3 force, float mass, collider coll)
{
	obj[whichObj].filename = filePath;
	obj[whichObj].transform.loc = loc;
	obj[whichObj].transform.rot = rot;
	obj[whichObj].transform.size = size;
	obj[whichObj].rBody.velocity = vel;
	obj[whichObj].rBody.force = force;
	obj[whichObj].rBody.mass = mass;
	obj[whichObj].collider = coll;
}

//----------------------------------------------------------------------------------------------------------->>COLLISION CHECKER<<
bool Engine::collidesWith(const Object &a, const Object &b)
{
	// If either collider is NULL
	if (a.collider == NULL || b.collider == NULL || a.collider == COLLIDERLESS || b.collider == COLLIDERLESS)
		return false;
	// If both colliders are spheres
	if (a.collider == SPHERE && b.collider == SPHERE) {
		float distSquared;
		distSquared = (b.transform.loc.x - a.transform.loc.x) * (b.transform.loc.x - a.transform.loc.x);					// (x2 - x1)^2
		distSquared += (b.transform.loc.y - a.transform.loc.y) * (b.transform.loc.y - a.transform.loc.y);					// (y2 - y1)^2
		if (!(distSquared < ((b.transform.size.x + a.transform.size.x) * (b.transform.size.x + a.transform.size.x))))		// If the distSquared is NOT less than the sum of the 2 radii squared
			return false;
	}
	// If both colliders are AABB
	else if (a.collider == AABB && b.collider == AABB) {
		// If the separation on the axis w is greater than the sum of the size.w, return false
		// OK here's how you're gonna do this:
		// Find out which object is more left than the other one. Let's call them lefty and righty
		// Here's a diagram because I know you're stupid:
		//  (-1, 1)_________________(1, 1)
		//		   |_______|_______|
		// (-1, -1)|_______|_______|(1, -1)

		Object *leftTop = new Object;
		Object *rightBottom = new Object;
		if (a.transform.loc.x < b.transform.loc.x) {
			*leftTop = a;
			*rightBottom = b;
		}
		else {
			*leftTop = b;
			*rightBottom = a;
		}
		// Take lefty's left-most x position (location.x - size.x/2) and righty's right-most x position (location.x + size.x/2) and add them. We'll call it 'dist' or whatever
		float distance = ((*leftTop).transform.loc.x - ((*leftTop).transform.size.x / 2)) + ((*rightBottom).transform.loc.x + ((*rightBottom).transform.size.x / 2));
		// Now add lefty's and righty's size.x's together. Let's call it 'sizeSum'
		float sizeSum = (*leftTop).transform.size.x + (*rightBottom).transform.size.x;
		// If dist > sizeSum, then the object's obviously aren't colliding on this axis.
		if (distance > sizeSum) {
			delete leftTop;
			delete rightBottom;
			return false;
		}
		// If they are colliding, repeat the processes for 'y' and 'z' axes as well
		// Y-AXIS
		if (a.transform.loc.y > b.transform.loc.y) {
			*leftTop = a;
			*rightBottom = b;
		}
		else {
			*leftTop = b;
			*rightBottom = a;
		}
		distance = ((*leftTop).transform.loc.y - ((*leftTop).transform.size.y / 2)) + ((*rightBottom).transform.loc.y + ((*rightBottom).transform.size.y / 2));
		sizeSum = (*leftTop).transform.size.y + (*rightBottom).transform.size.y;
		if (distance > sizeSum) {
			delete leftTop;
			delete rightBottom;
			return false;
		}

		// Z-AXIS
		if (a.transform.loc.z > b.transform.loc.z) {
			*leftTop = a;
			*rightBottom = b;
		}
		else {
			*leftTop = b;
			*rightBottom = a;
		}
		distance = ((*leftTop).transform.loc.z - ((*leftTop).transform.size.z / 2)) + ((*rightBottom).transform.loc.z + ((*rightBottom).transform.size.z / 2));
		sizeSum = (*leftTop).transform.size.z + (*rightBottom).transform.size.z;
		if (distance > sizeSum) {
			delete leftTop;
			delete rightBottom;
			return false;
		}

		delete leftTop;
		delete rightBottom;
		// Congratulations! You've solved global warming! Well, not quite. Let's not get too ahead of ourselves here.
	}
	// If you have a mix of colliders
	else if (a.collider == SPHERE && b.collider == AABB) {
		// OK Jesus where do we begin
		// Here's the diagram again because you have no idea wtf 'object permanence' is:
		//  (-1, 1)_________________(1, 1)
		//		   |_______|_______|
		// (-1, -1)|_______|_______|(1, -1)

		// First of all, we're dividing this between the 3 axes, so start at x
		// Figure out which bounding box is which
		float distSquared = 0.0;
		// Next, we're going to compute the squares of the distances
		// If the sphere is to the left of the box use minX; if to the right use maxX, if in between there distance^2 += 0
		// If the sphere's location + sphere's size is greater than the box's location + box's size it's to the right
		// Distance plus-equals the center of the sphere minus the right-most box side
		// If the sphere's location - sphere's size is less than the box's location - box's size it's to the left
		// Distance plus-equals 

		float min = b.transform.loc.x - b.transform.size.x;
		float max = b.transform.loc.x + b.transform.size.x;
		float circle = a.transform.loc.x;

		// X-AXIS
		if (circle < min)
			distSquared += (min - circle) * (min - circle);
		else if (circle > max)
			distSquared += (circle - max) * (circle - max);

		// Y-AXIS
		min = b.transform.loc.y - b.transform.size.y;
		max = b.transform.loc.y + b.transform.size.y;
		circle = a.transform.loc.y;

		if (circle < min)
			distSquared += (min - circle) * (min - circle);
		else if (circle > max)
			distSquared += (circle - max) * (circle - max);

		// Z-AXIS
		min = b.transform.loc.z - b.transform.size.z;
		max = b.transform.loc.z + b.transform.size.z;
		circle = a.transform.loc.z;

		if (circle < min)
			distSquared += (min - circle) * (min - circle);
		else if (circle > max)
			distSquared += (circle - max) * (circle - max);

		return (distSquared < (a.transform.size.x * a.transform.size.x));
	}
	else
		return collidesWith(b, a);

	return true;
}

//Engine *Engine::engine = nullptr;

void Engine::updateCam(int upDown, int leftRight)
{
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------>>WORK ON THIS PART<<
	// MOVING CAMERA WITH WASD KEYBOARD INPUT
	if (upDown < 0) {
		for (int i = upDown; i < 0; i++) {
			camera.rBody.force.y -= 5;
		}
	}
	else if (upDown > 0) {
		for (int i = 0; i < upDown; i++) {
			camera.rBody.force.y += 5;
		}
	}

	if (leftRight < 0) {
		for (int i = leftRight; i < 0; i++) {
			camera.rBody.force.x -= 5;
		}
	}
	else if (leftRight > 0) {
		for (int i = 0; i < leftRight; i++) {
			camera.rBody.force.x += 5;
		}
	}
	const float speed = 1.f;

	if (upDown != 0 || leftRight != 0) {
		camera.rBody.velocity = glm::normalize(camera.rBody.force) * deltaTime;
		// Derive change in location from velocity, change in time
		camera.transform.loc += camera.rBody.velocity;
		camera.rBody.force = vec3();
	}

	//camera.transform.loc += camera.rBody.velocity;
	/**/
	// MOVING THE CAMERA WITH WASD KEYBOARD INPUT - METHOD 2 (doesn't work yet)
	/*const float speed = 1.f;
	vec3 vec;
	glm::mat3 render = (glm::mat3)glm::yawPitchRoll(camera.pov->transform.rot.y, camera.pov->transform.rot.x, camera.pov->transform.rot.z);

	if (InputManager::keyIsDown[GLFW_KEY_W]) vec += render * vec3(0, 0, -1);
	if (InputManager::keyIsDown[GLFW_KEY_A]) vec += render * vec3(-1, 0, 0);
	if (InputManager::keyIsDown[GLFW_KEY_S]) vec += render * vec3(0, 0, 1);
	if (InputManager::keyIsDown[GLFW_KEY_D]) vec += render * vec3(1, 0, 0);

	if (vec == vec3()) camera.pov->rBody.velocity = vec3();
	else camera.pov->rBody.velocity = glm::normalize(vec) * speed;*/
	// MOVING CAMERA WITH MOUSE POSITION
	// The goal of this part is to rotate the screen based on the mouse's movement. This one is pretty simple; let's break it down:
	// First, we get the cursor's x and y position
	double xPos, yPos;
	glfwGetCursorPos(GLFWwindowPtr, &xPos, &yPos);

	// Next, get the size of the window. This is important since all windows have different resolutions and such
	int width, height;
	glfwGetWindowSize(GLFWwindowPtr, &width, &height);

	// This is the center of the screen. This code basically locks the mouse to the middle of the screen, ensuring that you ain't going nowhere
	int x0 = width / 2;
	int y0 = height / 2;
	glfwSetCursorPos(GLFWwindowPtr, x0, y0);

	// Now THIS is different than the previous stuff - THIS code limits the camera's velocity...yeah, I had it pick up acceleration, so limiting velocity is NECESSARY
	camera.rBody.force += camera.rBody.velocity * LIMITER;
	glm::vec3 vector = camera.rBody.force;
	vector *= deltaTime / camera.rBody.mass;
	camera.rBody.velocity = vector;

	// Derive change in location from velocity, change in time
	camera.transform.loc.x += camera.rBody.velocity.x * deltaTime;
	camera.transform.loc.y += camera.rBody.velocity.y * deltaTime;

	// The next step for moving a camera with the mouse position is rotating it
	// ROTATE MATRIX
	const float sens = .0005f;
	camera.transform.rot.y += sens * ((float)x0 - (float)xPos);		// Yaw
	camera.transform.rot.x += sens * ((float)y0 - (float)yPos);		// Pitch*/
	// MOVING CAMERA BASED ON TARGET OBJECT LOCATION
	/*const float speed = 1.f;

	if (InputManager::keyIsDown[GLFW_KEY_W]) camera.rBody.force += vec3(0, 0, -1);
	if (InputManager::keyIsDown[GLFW_KEY_A]) camera.rBody.force += vec3(-1, 0, 0);
	if (InputManager::keyIsDown[GLFW_KEY_S]) camera.rBody.force += vec3(0, 0, 1);
	if (InputManager::keyIsDown[GLFW_KEY_D]) camera.rBody.force += vec3(1, 0, 0);

	if (camera.rBody.force == vec3()) camera.rBody.velocity = vec3();
	else camera.rBody.velocity = glm::normalize(camera.rBody.force) * speed;/**/

	// This is translating the matrix based on location
	// TRANSLATE MATRIX

	glm::mat4 translate = { 1, 0, 0, camera.transform.loc.x,
		0, 1, 0, camera.transform.loc.y,
		0, 0, 1, camera.transform.loc.z,
		0, 0, 0, 1 };

	translate = glm::transpose(translate);

	// Lastly, we change it's transformation within the world by multiplying the translation value, rotation, and scale
	camera.transform.objWorld = translate *
		glm::yawPitchRoll(camera.transform.rot.y, camera.transform.rot.x, camera.transform.rot.z) *
		glm::scale(camera.transform.size);

	// Just kidding; THIS is the last part. HERE we change the mat4 position based on the aforementioned step. Voila! Success.
	camera.cameraMat = camera.transform.objWorld;
}

void Engine::render(GLuint shader, GLint uniform, GLsizei i, GLboolean glBool, const GLfloat * value, GLenum target, GLuint texture, Model model)
{
	glProgramUniformMatrix4fv(shader, uniform, i, glBool, value);
	glBindTexture(target, texture);
	model.render();
}
