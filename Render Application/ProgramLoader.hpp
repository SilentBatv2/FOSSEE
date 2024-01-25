#include<iostream>
#include<fstream>
#include<glew.h>
#include<string>

class ProgramLoader
{
public:
	static unsigned int load_shader(std::string vertex, std::string fragment)
	{
		std::ifstream vertex_file(vertex.c_str(), std::ios_base::in), fragment_file(fragment.c_str(), std::ios_base::in);

		std::string vertex_shader, fragment_shader, temp;
		if (!(vertex_file.is_open() && fragment_file.is_open()))
		{
			exit(4);
		}
		while (std::getline(vertex_file, temp))
		{
			vertex_shader.append(temp);
			vertex_shader += '\n';
		}
		while (std::getline(fragment_file, temp))
		{
			fragment_shader.append(temp);
			fragment_shader += '\n';
		}

		std::cout << vertex_shader << std::endl << fragment_shader;
		
		unsigned int program = glCreateProgram(), vs = create_shader(GL_VERTEX_SHADER, vertex_shader), fs = create_shader(GL_FRAGMENT_SHADER, fragment_shader);
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
private:
	static unsigned int create_shader(unsigned int type, std::string& shader_code)
	{
		unsigned int id = glCreateShader(type);
		const char* src = shader_code.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result = 0;

		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char)); 
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl << message << std::endl;
			glDeleteShader(id);
			exit(5);
		}

		return id;
	}
};