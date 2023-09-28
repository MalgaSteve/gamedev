#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
const char *vertexShaderSource = 
	"#version 330 core\n"
    	"layout (location = 0) in vec3 aPos;\n"
    	"void main()\n"
    	"{\n"
    	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    	"}\0";


const char *fragmentShaderSource = 
	"#version 330 core\n"
    	"out vec4 FragColor;\n"
    	"void main()\n"
    	"{\n"
    	"   FragColor = vec4(1.0f, 0.8f, 0.6f, 1.0f);\n"
    	"}\0";

int main() {

	// Initialization -----------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation -----------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World!", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
        	glfwTerminate();
        	return -1;
	}

	glfwMakeContextCurrent(window);
    	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glad : load all OpenGL function pointers ----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	//vertex shader 
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success){
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//delete shader objects (no need for them anymore)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vertices of triangle
	float vertices1[] = {
	    -1.0f, -0.5f, 0.0f, 
	    -0.5f,  0.5f, 0.0f,
	     0.0f, -0.5f, 0.0f,
	};

	float vertices2[] = {
	     0.0f, -0.5f, 0.0f,
	     0.5f,  0.5f, 0.0f,
	     1.0f, -0.5f, 0.0f
	};

	//vertex array object
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs); 
	glBindVertexArray(VAOs[0]);

	//buffer object where data is stored in gpu
	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//link input with vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//render loop ---------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		//input 
		processInput(window);

		//render
		glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//viewport change when window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
