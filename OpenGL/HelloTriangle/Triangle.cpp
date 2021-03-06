#include "IBase.h"


class Triangle : public BaseRef
{
public:
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

	
	unsigned shaderProgram;
	unsigned int VAO;
	unsigned int VBO;

	void Init()
	{
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
		else {
			std::cout << "fragment success" << std::endl;
		}

		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		float vertices[] =
		{
			0.5f, -0.5f, 0.0f,   1, 0, 0,
			-0.5f, -0.5f, 0.0f,	 0, 1, 0,
			0.0f, 0.5f, 0.0f,	 0, 0, 1,
		};

		//上下颠倒的三角形
		/*float vertices[] =
		{
			-0.5f, 0.5f, 0.0f,   1, 0, 0,
			0.5f, 0.5f, 0.0f,	 0, 1, 0,
			0.0f, -0.5f, 0.0f,	 0, 0, 1,
		};*/

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glUseProgram(shaderProgram);
	}

	void MainLoop(GLFWwindow* windows)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glDrawBuffer(VBO);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
};