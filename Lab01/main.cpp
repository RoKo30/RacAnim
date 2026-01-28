// Local Headers
#include "Mesh.h"
#include "Renderer.h"
#include "Renderable.h"
#include "InputManager.h"
#include "MeshLoader.h"
#include "BSpline.h"
#include "Lines.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
#include <fstream>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Standard Headers
#include <iostream>
#include <cstdlib>

Shader* loadShader(char* path, char* nazivVert, char* nazivGeom, char* nazivFrag) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;
	std::string pathGeom; // Geometry shader path

	// Extract base shader path
	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	pathGeom.append(path, sPath.find_last_of("\\/") + 1); // << added geometry path setup

	// Append shader directory depending on slash type
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathFrag.append("shaders/");
		pathGeom.append("shaders/"); // << added
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathFrag.append("shaders\\");
		pathGeom.append("shaders\\"); // << added
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	// Append file names
	pathVert.append(nazivVert);
	pathVert.append(".vert");

	pathFrag.append(nazivFrag);
	pathFrag.append(".frag");

	pathGeom.append(nazivGeom);
	pathGeom.append(".geom");

	// Check if geometry shader file exists
	std::ifstream geomFile(pathGeom);
	bool hasGeometry = geomFile.good();
	geomFile.close();

	// Return new Shader, passing geometry path if it exists
	if (hasGeometry) {
		return new Shader(pathVert.c_str(), pathFrag.c_str(), pathGeom.c_str()); // << modified
	}
	else {
		return new Shader(pathVert.c_str(), pathFrag.c_str(), nullptr); // << fallback if no .geom
	}
}

const int mWidth = 1280;
const int mHeight = 800;


int main(int argc, char* argv[]) {
	//postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
	GLFWwindow* window;

	glfwInit();
	gladLoadGL();


	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);


	window = glfwCreateWindow(mWidth, mHeight, "Zadatak 5a", nullptr, nullptr);
	// provjeri je li se uspio napraviti prozor
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// dohvati sve dostupne OpenGL funkcije
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}


	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


	glEnable(GL_DEPTH_TEST); //ukljuci z spremnik (prikazuju se oni fragmenti koji su najblizi promatracu)
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE); //ukljuci uklanjanje straznjih poligona -- za ovaj primjer je iskljuceno
	//glCullFace(GL_BACK); 

	glClearColor(0.15, 0.5, 0.5, 1); //boja brisanja platna izmedu iscrtavanja dva okvira

	glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)
	Assimp::Importer importer;
	auto scene = MeshLoader::loadMesh(importer, std::string(argv[0]), "frog.obj");
	Assimp::Importer importer1;
	auto scene1 = MeshLoader::loadMesh(importer1, std::string(argv[0]), "axis6.obj");
	Assimp::Importer importer2;
	auto scene2 = MeshLoader::loadMesh(importer2, std::string(argv[0]), "bspline_points_spiral.obj");

	if (!scene) {
		std::cout << "ERR" << "\n";
		return -1;
	}

	glfwSetKeyCallback(window, im::key_callback);
	glfwSetCursorPosCallback(window, im::mouse_callback);

	if (scene != nullptr && scene->HasMeshes()) {

		aiMesh* mesh = scene->mMeshes[0];
		aiMesh* axises = scene1->mMeshes[0];

		Bspline bSpline(scene2);
		Mesh m(mesh);
		Mesh m1(axises);

		Shader* shader = loadShader(argv[0], "shader3", "alg1vv", "shader");

		Renderer renderer(glm::vec2(mWidth, mHeight), 30);

		renderer.createObject(std::make_shared<Mesh>(m1), shader);

		renderer.getObjects()[0].drawLines();

		renderer.createObject(std::make_shared<Mesh>(m), shader);

		im::camera = renderer.getCamera();
		im::renderer = &renderer;
		im::shader = shader;

		renderer.getCamera()->setPosition(glm::vec3(0, 1, 0));
		//renderer.getCamera().setOrientation(glm::vec3(0, 0, 1));

		renderer.createObject(std::make_shared<Mesh>(m), shader);
		renderer.getObjects()[1].setPosition(glm::vec3(3, 0, 0));
		renderer.getObjects()[1].rotate(45, glm::vec3(0, 1, 0));

		renderer.getObjects()[2].setPosition(glm::vec3(-3, 0, 0));
		renderer.getObjects()[2].rotate(45, glm::vec3(0, 0.5, 0));

		renderer.createObject(std::make_shared<Bspline>(scene2, 0.05f), shader);
		for (int i = 0; i < bSpline.getCurvePoints().size(); i += 5) {
			std::vector<glm::vec3> points;
			float l = .5f;
			glm::vec3 t = bSpline.getTangents().at(i);
			points.emplace_back(bSpline.getCurvePoints().at(i));
			points.emplace_back(bSpline.getCurvePoints().at(i) + (l * t));
			renderer.createObject(std::make_shared<Lines>(points), shader);
		}

		im::setControlledObject(&renderer.getObjects()[2]);
		im::setSpline(&bSpline);
		while (glfwWindowShouldClose(window) == false) {
			//pobrisi platno
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderer.render();
			renderer.getCamera()->updateMovement();
			for (Object &obj : renderer.getObjects()) {
				obj.updateMovement();
			}
			//glUseProgram(shader->ID);
			//draw();
			glfwSwapBuffers(window);
			glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}
	}
	
	glfwTerminate();
	return EXIT_SUCCESS;
}
