#include <iostream>
#include <cmath>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};

struct Vertex
{
	vec3 Position;
	vec3 COlor;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, VAO, BasiceprogramId, shaderProgram;
DrawingMode Current_DrawingMode = DrawingMode::Lines;
float theta = 0;
int flag = 0;

//Task 1
void CreateCircle()
{
	GLfloat TriangleVertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.707f, 0.707f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.707f, 0.707f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.707f, -0.707f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.707f, -0.707f, 0.0f, 1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	GLuint VBO;

	// Generate and bind buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

	// Set up position attribute (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up color attribute (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


//TASK 2
void CreateSierpinskiTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, int depth)
{
	if (depth == 0)
	{
		// Draw a single triangle using the given vertices, all in cyan
		GLfloat TriVertices[] = {
			x1, y1, 0.0f, 0.0f, 0.0f, 1.0f,  // Vertex 1 (Cyan)
			x2, y2, 0.0f, 0.0f, 0.0f, 1.0f,  // Vertex 2 (Cyan)
			x3, y3, 0.0f, 0.0f, 0.0f, 1.0f   // Vertex 3 (Cyan)
		};

		GLuint VBO;

		// Generate and bind buffer
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriVertices), TriVertices, GL_STATIC_DRAW);

		// Set up position attribute (x, y, z)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// Set up color attribute (r, g, b)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Render the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Clean up the VBO
		glDeleteBuffers(1, &VBO);
		return;
	}

	// Calculate midpoints of the triangle's edges
	GLfloat mid1_x = (x1 + x2) / 2.0f;
	GLfloat mid1_y = (y1 + y2) / 2.0f;

	GLfloat mid2_x = (x2 + x3) / 2.0f;
	GLfloat mid2_y = (y2 + y3) / 2.0f;

	GLfloat mid3_x = (x3 + x1) / 2.0f;
	GLfloat mid3_y = (y3 + y1) / 2.0f;

	// Recursively draw smaller triangles
	CreateSierpinskiTriangle(x1, y1, mid1_x, mid1_y, mid3_x, mid3_y, depth - 1);
	CreateSierpinskiTriangle(mid1_x, mid1_y, x2, y2, mid2_x, mid2_y, depth - 1);
	CreateSierpinskiTriangle(mid3_x, mid3_y, mid2_x, mid2_y, x3, y3, depth - 1);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//Task 3
void CreateQuad(GLuint& VAO, GLuint& VBO) {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 
		 1.0f, -1.0f, 0.0f, 
		 1.0f,  1.0f, 0.0f, 
		-1.0f,  1.0f, 0.0f  
	};

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}



//Task 4
//Each line is a vector of points
vector<vector<sf::Vector2f>> lines;
// State to track if the mouse is pressed
bool isDrawing = false;
void LineDrawer() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// Add a new point to the line
void AddPoint(float x, float y) {
	if (!lines.empty()) {
		lines.back().push_back(sf::Vector2f(x, y));

		// Update the VBO with all points from all lines
		vector<sf::Vector2f> allPoints;
		for (const auto& line : lines) {
			allPoints.insert(allPoints.end(), line.begin(), line.end());
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(sf::Vector2f), allPoints.data(), GL_DYNAMIC_DRAW);
	}
}

// Start a new line
void StartNewLine(float x, float y) {
	vector<sf::Vector2f> newLine;
	newLine.push_back(sf::Vector2f(x, y));
	lines.push_back(newLine);
}

//Task 5
//Initialize vertices
void InitializeVertices() {
	GLfloat vertices[50*6] = {};
	float test = sizeof(vertices) / (sizeof(GLfloat) * 6);
	for (int i = 0; i < test * 6; i += 6) {
		vertices[i] = ((float) i / ((test - 1) * 3)) - 1;
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute (first 3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute (next 3 floats)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& programId) {
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glClearColor(0, 0.5, 0.5, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	switch (Current_DrawingMode)
	{
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}
	 
	if (flag == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
	}

	else if (flag == 2) {
		GLfloat x1 = -0.8f, y1 = -0.8f;
		GLfloat x2 = 0.8f, y2 = -0.8f;
		GLfloat x3 = 0.0f, y3 = 0.8f;

		int depth = 5;

		CreateSierpinskiTriangle(x1, y1, x2, y2, x3, y3, depth);
	}

	else if (flag == 3) {
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		glUniform2f(glGetUniformLocation(shaderProgram, "center"), 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, "scale"), 2.5f);
		glUniform1i(glGetUniformLocation(shaderProgram, "iter"), 500);         

		glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), WIDTH, HEIGHT);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	else if (flag == 4) {
		if (!lines.empty()) {
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// Configure vertex attributes
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(sf::Vector2f), (void*)0);
			glEnableVertexAttribArray(0);

			// Draw each line
			size_t offset = 0;
			for (const auto& line : lines) {
				if (line.size() > 1) {
					glDrawArrays(GL_LINE_STRIP, offset, line.size());
				}
				offset += line.size();
			}
		}
	}

	if (flag == 5) {
		glUseProgram(shaderProgram);
		// Update the theta uniform
		GLuint thetaLocation = glGetUniformLocation(shaderProgram, "theta");
		glUniform1f(thetaLocation, theta);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 50);
		glBindVertexArray(0);
	}
}

void Update()
{
	theta += 0.0001f;
	GLuint Theta_Location;
	if (flag == 5 || flag == 3) {
		Theta_Location = glGetUniformLocation(shaderProgram, "theta");
	}
	else {
		Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
	}
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				if (event.key.code == sf::Keyboard::Num4)
				{
					flag = 1;
					CreateCircle();
					CompileShader("VSRCW.glsl", "FS.glsl", BasiceprogramId);
				}
				if (event.key.code == sf::Keyboard::Num5)
				{
					flag = 2;
					// Initial vertices of the large triangle
					GLfloat x1 = -0.8f, y1 = -0.8f;
					GLfloat x2 = 0.8f, y2 = -0.8f;
					GLfloat x3 = 0.0f, y3 = 0.8f;

					// Depth of recursion
					int depth = 5; // Adjust this for more or fewer levels of detail

					// Call the recursive function
					CreateSierpinskiTriangle(x1, y1, x2, y2, x3, y3, depth);					
					CompileShader("VSSierLineToo.glsl", "FS.glsl", BasiceprogramId);
				}
				if (event.key.code == sf::Keyboard::Num6)
				{
					flag = 3;
					CreateQuad(VAO, VBO);
					CompileShader("VS.glsl", "FSM.glsl", shaderProgram);
				}
				if (event.key.code == sf::Keyboard::Num7)
				{
					flag = 4;
					LineDrawer();
					CompileShader("VSSierLineToo.glsl", "FS.glsl", BasiceprogramId);
					case sf::Event::MouseButtonPressed:
						if (event.mouseButton.button == sf::Mouse::Left) {
							// Start a new line from the mouse click position
							isDrawing = true;
							sf::Vector2i mousePos = sf::Mouse::getPosition(window);
							float normalizedX = 2.0f * mousePos.x / WIDTH - 1.0f;
							float normalizedY = 1.0f - 2.0f * mousePos.y / HEIGHT;
							StartNewLine(normalizedX, normalizedY);
						}
						break;

					case sf::Event::MouseButtonReleased:
						if (event.mouseButton.button == sf::Mouse::Left) {
							// Stop drawing the current line
							isDrawing = false;
						}
						break;
				}
				if (event.key.code == sf::Keyboard::Num8)
				{
					flag = 5;
					CompileShader("VSTest.glsl", "FS.glsl", shaderProgram);
					InitializeVertices();
				}
				break;
			}
			}
				
		}
		// Add points to the current line while the mouse is being dragged
		if (isDrawing && flag == 4) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			float normalizedX = 2.0f * mousePos.x / WIDTH - 1.0f;
			float normalizedY = 1.0f - 2.0f * mousePos.y / HEIGHT;

			// Add new point to the current line
			if (!lines.empty() && (lines.back().empty() || lines.back().back() != sf::Vector2f(normalizedX, normalizedY))) {
				AddPoint(normalizedX, normalizedY);
			}
		}
		Update();
		Render();
		window.display();
	}
	return 0;
	}