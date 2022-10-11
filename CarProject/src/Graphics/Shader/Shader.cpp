//
//	Factory.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Constructs a Shader from a Path (Deduces shader type)
		*/ // ---------------------------------------------------------------------
		Shader::Shader(const char* strShaderFile) noexcept
		{
			GLenum _type;

			std::string_view fn = strShaderFile;
			if (fn.substr(fn.find_last_of(".") + 1) == "vert")
				_type = GL_VERTEX_SHADER;
			else
				_type = GL_FRAGMENT_SHADER;

			// Create shader
			mShaderID = glCreateShader(_type);

			std::ifstream tFile(strShaderFile);

			assertThis(!tFile.is_open() || !tFile.good(), std::string("File not read in" + std::string(strShaderFile)).c_str());

			// parse shader file
			std::stringstream _shaderCode;
			char line[256];

			//Read the entire file
			while (tFile.getline(line, 256))
				_shaderCode << line << '\n';

			const std::string _finalCode = _shaderCode.str();

			const char* strFileData = _finalCode.data();
			glShaderSource(mShaderID, 1, &strFileData, NULL);

			glCompileShader(mShaderID);

			GLint _status;

			GLint bufflen;
			glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &bufflen);
			if (bufflen > 1)
			{

				GLchar* log_string = reinterpret_cast<GLchar*>(alloca((bufflen + 1) * sizeof(GLchar)));
				glGetShaderInfoLog(mShaderID, bufflen, 0, log_string);
				AConsole->Log("Log found for '%s.vert':\n%s", strShaderFile, log_string);
			}

			glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &_status);


			//If we got compilation errors
			if (_status == GL_FALSE)
			{
				GLint _infoLogLength;
				glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &_infoLogLength);

				GLchar* _strInfoLog = reinterpret_cast<GLchar*>(alloca((_infoLogLength + 1) * sizeof(GLchar)));
				glGetShaderInfoLog(mShaderID, _infoLogLength, NULL, _strInfoLog);

				// Get shader type
				const char* _strShaderType = NULL;

				//Switch by the shader type
				switch (_type)
				{
				case GL_VERTEX_SHADER: _strShaderType = "vertex"; break;
				case GL_GEOMETRY_SHADER: _strShaderType = "geometry"; break;
				case GL_FRAGMENT_SHADER: _strShaderType = "fragment"; break;
				}

				AConsole->Log("Compile failure in %s shader:\n%s\n", _strShaderType, _strInfoLog);
			}
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Prevents Shaders from leaking onto the GPU
		*/ // ---------------------------------------------------------------------
		Shader::~Shader()
		{
			glDeleteShader(mShaderID);
		}

		// ------------------------------------------------------------------------
		/*! Converstion Constructor
		*
		*   Constructs a ShaderProgram from all the shaders to link
		*/ // ---------------------------------------------------------------------
		ShaderProgram::ShaderProgram(const stl::Array<BaleaEngine::Graphics::Shader>& shaderList)
			noexcept
		{
			mProgramID = glCreateProgram();

			// Attach all shaders passed from the vector
			for (size_t iLoop = 0, size = shaderList.size(); iLoop < size; iLoop++)
				glAttachShader(mProgramID, shaderList[iLoop].mShaderID);

			glLinkProgram(mProgramID);

			GLint _status;
			glGetProgramiv(mProgramID, GL_LINK_STATUS, &_status);

			//If we got a linking error
			if (_status == GL_FALSE)
			{
				GLint infoLogLength;
				glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);

				GLchar* strInfoLog = reinterpret_cast<GLchar*>(alloca((infoLogLength + 1) * sizeof(GLchar)));
				glGetProgramInfoLog(mProgramID, infoLogLength, NULL, strInfoLog);
				std::cout << strInfoLog << std::endl;
				AConsole->Log("Linker failure: %s\n", strInfoLog);
			}


			//Detach the Shaders
			for (size_t iLoop = 0, size = shaderList.size(); iLoop < size; iLoop++)
				glDetachShader(mProgramID, shaderList[iLoop].mShaderID);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Prevents Shader Programs from leaking onto the GPU
		*/ // ---------------------------------------------------------------------
		ShaderProgram::~ShaderProgram() noexcept {
			glDeleteProgram(mProgramID);
		}

		// ------------------------------------------------------------------------
		/*! Set Uniform
		*
		*   Sets all the uniforms for the lights at the scene
		*/ // ---------------------------------------------------------------------
		void ShaderProgram::SetUniform(const char* name, const stl::Array<BaleaEngine::Graphics::Light*>& lightVec)
			const noexcept {
			const size_t _size = lightVec.size();

			SetUniform("uLightNum", static_cast<int>(_size));

			const std::string _name(name);
			const float _deg2rad = static_cast<float>(PI) / 180.f;

			//FOr each light, upload the uniforms to the GPU
			for (size_t i = 0; i < _size;)
			{
				const std::string _indexstr = std::to_string(i);

				Light& _templight = *lightVec[i++];

				SetUniform((_name + "[" + _indexstr + "].type").c_str(), static_cast<unsigned char>(_templight.GetLightType()));
				SetUniform((_name + "[" + _indexstr + "].ambient").c_str(), _templight.GetAmbientColor());
				SetUniform((_name + "[" + _indexstr + "].diffuse").c_str(), _templight.GetDiffuseColor());
				SetUniform((_name + "[" + _indexstr + "].specular").c_str(), _templight.GetSpecullarColor());
				SetUniform((_name + "[" + _indexstr + "].position").c_str(), _templight.GetOwner()->GetPosition());
				SetUniform((_name + "[" + _indexstr + "].spot_falloff").c_str(), _templight.GetFallOff());
				SetUniform((_name + "[" + _indexstr + "].spot_innerAngle").c_str(), _deg2rad * _templight.GetInnerAngle());
				SetUniform((_name + "[" + _indexstr + "].spot_outerAngle").c_str(), _deg2rad * _templight.GetInnerAngle());
				SetUniform((_name + "[" + _indexstr + "].const_Att").c_str(), _templight.GetAttenuation1());
				SetUniform((_name + "[" + _indexstr + "].linear_Att").c_str(), _templight.GetAttenuation2());
				SetUniform((_name + "[" + _indexstr + "].quadratic_Att").c_str(), _templight.GetAttenuation3());
			}
		}

		void ShaderProgram::SetUniform(const GLuint loc, const bool val) const noexcept
		{
			glUniform1i(loc, static_cast<int>(val));
		}

		void ShaderProgram::SetUniform(const GLuint loc, const int val) const noexcept
		{
			glUniform1i(loc, val);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const float val) const noexcept
		{
			glUniform1f(loc, val);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const GLuint val) const noexcept
		{
			glUniform1i(loc, val);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const glm::vec2& vec) const noexcept
		{
			glUniform2f(loc, vec.x, vec.y);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const glm::vec3& vec) const noexcept
		{
			glUniform3f(loc, vec.x, vec.y, vec.z);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const glm::vec4& vec) const noexcept
		{
			glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const glm::mat3& mat) const noexcept
		{
			glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
		}

		void ShaderProgram::SetUniform(const GLuint loc, const glm::mat4& mat) const noexcept
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
	}
}