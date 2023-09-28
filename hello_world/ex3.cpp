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


const char *fragmentShaderSource1 = 
	"#version 330 core\n"
    	"out vec4 FragColor;\n"
    	"void main()\n"
    	"{\n"
    	"   FragColor = vec4(1.0f, 0.8f, 0.6f, 1.0f);\n"
    	"}\0";

const char *fragmentShaderSource2 = 
	"#version 330 core\n"
    	"out vec4 FragColor;\n"
    	"void main()\n"
    	"{\n"
    	"   FragColor = vec4(0.1f, 1.0f, 0.4f, 1.0f);\n"
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
	unsigned int fragmentShader1, fragmentShader2;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);

	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);


	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if(!success){
	    glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader Program
	unsigned int shaderProgram1, shaderProgram2;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	//delete shader objects (no need for them anymore)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);

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
		
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);
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
