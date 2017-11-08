#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <string>
#include <vector>
#include <iterator>
#include <memory>
#include <iostream>

#include "GameEngine.h"

using glm::vec3;
using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;

GLFWwindow* window;
const GLushort WIDTH = 1280;
const GLushort HEIGHT = 720;
int lastMouseState = GLFW_RELEASE;

void window_size_callback(GLFWwindow* window, int width, int height);
static void mouseHandler(GLFWwindow* window, int button, int action, int mods);

class DemoApp : public GameEngine
{
public:
	DemoApp();
	~DemoApp();
	void update(double t);

	vector<shared_ptr<SceneNode>> nodes;
	vector<shared_ptr<GameObject>> gameObjects;
};

DemoApp::DemoApp() : GameEngine(*OpenGLRenderer::getInstance(), WIDTH, HEIGHT)
{
	ShaderManagerLoadStatus status;
	renderer = OpenGLRenderer::getInstance();
	shaderManager = ShaderProgramManager::getInstance();

	camera->setCameraPosition(vec3(0, 0, 3), false);
	camera->setSceneCenter(vec3(0, 0, 0), false);
	camera->setCameraUp(vec3(0, 1, 0), true);

	nodes.push_back(std::make_shared<SceneNode>());
	nodes[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));
	nodes[0]->setScale(vec3(1.0f, 1.0f, 1.0f));

	try
	{
		/*modelManager->loadPolyModel("Assets/goblet/kelch12_lowpolyn2.lwo", "goblet");
		gameObjects.push_back(std::make_shared<GameObject>("goblet"));
		nodes[0]->addRenderable(*gameObjects[0]);
		scene->appendChildNode(nodes[0]); */
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
}

void
DemoApp::update(double t)
{
}

DemoApp::~DemoApp()
{
}

DemoApp *engine;

void init()
{
	engine = new DemoApp();
	engine->onStart();
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Maze", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. \n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Clearing error queue, because the above causes GL_INVALID_ENUM even when
	// there is no problem.
	while (glGetError() != GL_NO_ERROR) {};

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetMouseButtonCallback(window, mouseHandler);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	init();
	engine->getRenderer()->setWindowSize(WIDTH, HEIGHT, engine->getCamera());

	do {
		//ren->render();
		double deltaTime = Clock::getMilliseconds() - engine->numMilliseconds;
		engine->preRender(deltaTime);
		engine->numMilliseconds = Clock::getMilliseconds();

		engine->update(deltaTime);
		engine->render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		engine->postRender();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW, end engine.
	engine->onEnd();
	glfwTerminate();

	return 0;
}

void
window_size_callback(GLFWwindow* window, int width, int height)
{
	engine->getRenderer()->setWindowSize(width, height, engine->getCamera());
}

void
mouseHandler(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS
		&& lastMouseState == GLFW_RELEASE)
	{
		Ray r = RayQuery::getRayFromMouseClick((int)xpos, (int)ypos);

		vector<shared_ptr<SceneNode>> intersections =
			RayQuery::Raycast(r, true);

		if (!intersections.empty())
		{
			GameObject* go = dynamic_cast<GameObject*>((*intersections[0]->getRenderables())[0]);

			if (go)
			{
				std::cout << go->getMaterialName() << std::endl;
				//engine->getSceneGraph()->removeChildNode(intersections[0]);
			}
		}

		lastMouseState = GLFW_PRESS;
	}

	if (action == GLFW_RELEASE)
		lastMouseState = GLFW_RELEASE;
}