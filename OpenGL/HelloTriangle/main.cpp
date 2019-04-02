#include "iostream"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include "Shader.h"

const char* vertexShaderSource = "#version 330 core\n"
					"layout(location = 0) in vec3 aPos;"
					"layout(location = 1) in vec3 aColor;"
					"out vec3 ourColor;"
					"void main() {"
					"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
					"	ourColor = aColor;"
					"}";

const char* fragmentShaderSource = "#version 330 core\n"
					"out vec4 FragColor;"
					"in vec3 ourColor;"
					"void main()"
					"{"
						//"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
						"FragColor = vec4(ourColor, 1.0f);\n"
					"}\n\0";

void processInput(GLFWwindow* window);
void DrawRectangle(GLFWwindow* window, unsigned int shaderProgram);
void DrawTriangle(GLFWwindow* window, unsigned int shaderProgram);
void DrawByMoreVertexProperty(GLFWwindow* window, unsigned int shaderProgram);

//#define SHADER_USE_FILE
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Faild to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	unsigned shaderProgram;

#ifdef SHADER_USE_FILE
	Shader s = Shader("vertex.glsl", "fragment.glsl");
	s.use();
	shaderProgram = s.ID;
#else
	//vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPIATION_FAILD\n" << infoLog << std::endl;
	}
	else {
		std::cout << "vertex success" << std::endl;
	}

	//fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();

	//link vertex and fragment shader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//get link error log
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILD\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

#endif // BY_FILE
	while (true)
	{
		//DrawTriangle(window, shaderProgram);
		//DrawRectangle(window, shaderProgram);
		DrawByMoreVertexProperty(window, shaderProgram);
		break;
	}
	return 0;
}

//draw triangle
void DrawTriangle(GLFWwindow* window, unsigned int shaderProgram)
{
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


void DrawRectangle(GLFWwindow* window, unsigned int shaderProgram)
{
	float vertices[] = {
		0.5f, 0.5, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};

	unsigned int indices[] = {
		/*0, 1, 3,
		1, 2, 3*/
		0, 1, 2,
		0, 2, 3
	};

	// 1. 绑定顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. 设定顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void DrawByMoreVertexProperty(GLFWwindow* window, unsigned int shaderProgram)
{
	float vertices[] = 
	{
		0.5f, -0.5f, 0.0f,   1, 0, 0,
		-0.5f, -0.5f, 0.0f,	 0, 1, 0,
		0.0f, 0.5f, 0.0f,	 0, 0, 1,
	};

	unsigned int VBO;
	unsigned int VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glDrawBuffer(VBO);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}