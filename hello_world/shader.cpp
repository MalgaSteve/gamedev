#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ProgramShader {
	private:
		const char *vertexShaderSource;
		const char *fragmentShaderSource;
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
	public:
		ProgramShader (char *vertexSrc, char *fragmentSrc) {
			vertexShaderSource = vertexSrc;
			fragmentShaderSource = fragmentSrc;
		}

		void createShaders() {
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);
		}

		void createShaderProgram () {
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

};
