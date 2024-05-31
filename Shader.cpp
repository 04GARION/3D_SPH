#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#define INFO_LENGTH 1024

namespace global
{
	Shader::Shader()
	{
		mId = 0;
	}

	Shader::~Shader()
	{
		if (mId != 0)
		{
			glDeleteProgram(mId);
		}
	}

	int32_t Shader::BuildFromFile(std::string& VertPath, std::string& FragPath, std::string& GeomPath)
	{
        const char* VertCode, * FragCode, * GeomCode;
        std::ifstream VertFile, FragFile, GeomFile;
        std::stringstream VertStream, FragStream, GeomStream;

        VertFile.open(VertPath);
        if (!VertFile)
        {
            std::cout << "ERROR: Open vertex shader file failed." << std::endl;
            return -1;
        }
        VertStream << VertFile.rdbuf();
        VertFile.close();
        auto VertStr = VertStream.str();
        VertCode = VertStr.c_str();

        unsigned int vert;
        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &VertCode, NULL);
        glCompileShader(vert);

        int success;
        char InfoLog[INFO_LENGTH];
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vert, INFO_LENGTH, NULL, InfoLog);
            std::cout << "ERROR: Compile vertex shader failed." << std::endl << InfoLog << std::endl;
            return -1;
        }

        FragFile.open(FragPath);
        if (!FragFile)
        {
            std::cout << "ERROR: Open fragment file failed." << std::endl;
            return -1;
        }
        FragStream << FragFile.rdbuf();
        FragFile.close();
        auto FragStr = FragStream.str();
        FragCode = FragStr.c_str();

        unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &FragCode, NULL);
        glCompileShader(frag);

        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(frag, INFO_LENGTH, NULL, InfoLog);
            std::cout << "ERROR: Compile fragment shader failed." << std::endl << InfoLog << std::endl;
            return -1;
        }

        unsigned int geom = 0;
        if (GeomPath != std::string(""))
        {
            GeomFile.open(GeomPath);
            if (!GeomFile)
            {
                std::cout << "ERROR: Open geometry file failed." << std::endl;
                return -1;
            }
            GeomStream << GeomFile.rdbuf();
            GeomFile.close();
            auto GeomStr = GeomStream.str();
            GeomCode = GeomStr.c_str();

            geom = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geom, 1, &GeomCode, NULL);
            glCompileShader(geom);

            glGetShaderiv(geom, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(geom, INFO_LENGTH, NULL, InfoLog);
                std::cout << "ERROR: Compile geometry shader failed." << std::endl << InfoLog << std::endl;
                return -1;
            }
        }

        mId = glCreateProgram();
        glAttachShader(mId, vert);
        glAttachShader(mId, frag);
        if (GeomPath != std::string(""))
        {
            glAttachShader(mId, geom);
        }
        glLinkProgram(mId);

        glGetProgramiv(mId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mId, INFO_LENGTH, NULL, InfoLog);
            std::cout << "ERROR: Link failed." << std::endl << InfoLog << std::endl;
            return -1;
        }

        glDeleteShader(vert);
        glDeleteShader(frag);
        if (GeomPath != std::string(""))
        {
            glDeleteShader(geom);
        }

        return 0;
	}

    void Shader::Use() {
        glUseProgram(mId);
    }

    void Shader::unUse() {
        glUseProgram(0);
    }

    GLuint Shader::GetId() {
        return mId;
    }

    void Shader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(mId, name.c_str()), (int)value);
    }

    void Shader::SetInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
    }

    void Shader::SetVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec2(const std::string& name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(mId, name.c_str()), x, y);
    }

    void Shader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z);
    }

    void Shader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(mId, name.c_str()), x, y, z, w);
    }

    void Shader::SetMat2(const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}