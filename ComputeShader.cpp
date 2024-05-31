#include "ComputeShader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#define INFO_LENGTH 1024

namespace global
{
	ComputeShader::ComputeShader(std::string name)
	{
		mName = name;
	}

	ComputeShader::~ComputeShader()
	{
	}

	int32_t ComputeShader::BuildFromFile(std::string& path)
	{
		std::string code;
		std::ifstream file;
		std::stringstream stream;

		file.open(path);
		if (!file)
		{
			std::cout << "ERROR: Compute shader file failed: " << mName << std::endl;
			return -1;
		}
		stream << file.rdbuf();
		file.close();
		code = stream.str();
		const char* CodeBuffer = code.c_str();

		GLuint shader;
		shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shader, 1, &CodeBuffer, NULL);
		glCompileShader(shader);

		int success;
		char InfoLog[INFO_LENGTH];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, INFO_LENGTH, NULL, InfoLog);
			std::cout << "ERROR: Compile compute shader failed: " << mName << std::endl << InfoLog << std::endl;
			return -1;
		}
		
		mId = glCreateProgram();
		glAttachShader(mId, shader);
		glLinkProgram(mId);

		glGetProgramiv(mId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mId, INFO_LENGTH, NULL, InfoLog);
			std::cout << "ERROR: Link compute shader failed: " << mName << std::endl << InfoLog << std::endl;
			return -1;
		}

		glDeleteShader(shader);

		return 0;
	}

	int32_t ComputeShader::BuildFromFiles(std::vector<std::string>& paths)
	{
		std::vector<GLuint> shaders(paths.size());
		for (int i = 0; i < paths.size(); i++)
		{
			std::string code;
			std::ifstream file;
			std::stringstream stream;

			file.open(paths[i]);
			if (!file)
			{
				std::cout << "ERROR: Compute shader file failed: " << mName << std::endl;
				return -1;
			}
			stream << file.rdbuf();
			file.close();
			code = stream.str();
			const char* CodeBuffer = code.c_str();

			shaders[i] = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(shaders[i], 1, &CodeBuffer, NULL);
			glCompileShader(shaders[i]);

			int success;
			char InfoLog[INFO_LENGTH];
			glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaders[i], INFO_LENGTH, NULL, InfoLog);
				std::cout << "ERROR: Compile compute shader failed: " << mName << std::endl << InfoLog << std::endl;
				return -1;
			}
		}

		mId = glCreateProgram();
		for (int i = 0; i < shaders.size(); i++)
		{
			glAttachShader(mId, shaders[i]);
		}
		glLinkProgram(mId);

		int success;
		char InfoLog[INFO_LENGTH];
		glGetProgramiv(mId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mId, INFO_LENGTH, NULL, InfoLog);
			std::cout << "ERROR: Link compute shader failed: " << mName << std::endl << InfoLog << std::endl;
			return -1;
		}

		for (int i = 0; i < shaders.size(); i++)
		{
			glDeleteShader(shaders[i]);
		}

		return 0;
	}

	void ComputeShader::Use()
	{
		glUseProgram(mId);
	}

	void ComputeShader::unUse()
	{
		glUseProgram(0);
	}

	GLuint ComputeShader::GetId()
	{
		return mId;
	}

	void ComputeShader::SetBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(mId, name.c_str()), (int)value);
	}

	void ComputeShader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
	}

	void ComputeShader::SetUInt(const std::string& name, uint32_t value)
	{
		glUniform1ui(glGetUniformLocation(mId, name.c_str()), value);
	}

	void ComputeShader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
	}

	void ComputeShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
	}
	void ComputeShader::SetVec2(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(mId, name.c_str()), x, y);
	}

	void ComputeShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
	}

	void ComputeShader::SetVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z);
	}

	void ComputeShader::SetUVec3(const std::string& name, const glm::uvec3& value) {
		glUniform3uiv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
	}

	void ComputeShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
	}
	void ComputeShader::SetVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(mId, name.c_str()), x, y, z, w);
	}

	void ComputeShader::SetMat2(const std::string& name, const glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void ComputeShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void ComputeShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}