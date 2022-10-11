// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior
//	written consent of DigiPen Institute of Technology is prohibited.
//
//	File Name:		Shader.h
//	Purpose:		This file contains the declaration of the Shader &
//					ShaderProgram structs
//	Project:		cs300_alejandro.balea_0
//	Author:			Alejandro Balea Moreno - alejandro.balea - 540002118
// ----------------------------------------------------------------------------
#ifndef _SHADER__H_
#define _SHADER__H_

namespace BaleaEngine {
	namespace Graphics {
		class Light;
		struct Shader {
#pragma region // Constructors & Destructors
			Shader(const char* strShaderFile) noexcept;
			~Shader();
#pragma endregion

#pragma region // Members
			GLuint mShaderID;
#pragma endregion
		};

		class ShaderProgram
		{
#pragma region // Constructors & Destructors
		public:
			ShaderProgram(const stl::Array<BaleaEngine::Graphics::Shader>& shaderList) noexcept;
			~ShaderProgram() noexcept;
#pragma endregion

#pragma region // Methods
			void inline Bind() const noexcept;
			void inline SetUniform(const char* name, const bool val) const noexcept;
			void inline SetUniform(const char* name, const int val) const noexcept;
			void inline SetUniform(const char* name, const float val) const noexcept;
			void inline SetUniform(const char* name, const GLuint val) const noexcept;
			void inline SetUniform(const char* name, const Math::Vector3D& vec) const noexcept;
			void inline SetUniform(const char* name, const glm::vec2& vec) const noexcept;
			void inline SetUniform(const char* name, const glm::vec3& vec) const noexcept;
			void inline SetUniform(const char* name, const glm::mat3& mat) const noexcept;
			void inline SetUniform(const char* name, const glm::mat4& mat) const noexcept;
			void inline SetUniform(const char* name, const Color& col) const noexcept;
			void SetUniform(const char* name, const stl::Array<BaleaEngine::Graphics::Light*>& lightVec) const noexcept;

			void SetUniform(const GLuint loc, const bool val) const noexcept;
			void SetUniform(const GLuint loc, const int val) const noexcept;
			void SetUniform(const GLuint loc, const float val) const noexcept;
			void SetUniform(const GLuint loc, const GLuint val) const noexcept;
			void SetUniform(const GLuint loc, const glm::vec2& vec) const noexcept;
			void SetUniform(const GLuint loc, const glm::vec3& vec) const noexcept;
			void SetUniform(const GLuint loc, const glm::vec4& vec) const noexcept;
			void SetUniform(const GLuint loc, const glm::mat3& mat) const noexcept;
			void SetUniform(const GLuint loc, const glm::mat4& mat) const noexcept;

			template <typename T>
			void SetUniform(const std::string& name, const stl::Array<T>& vector) const noexcept;
#pragma endregion

#pragma region // Members
		private:
			GLuint mProgramID;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Uses this program to render
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::Bind() const noexcept {
			glUseProgram(mProgramID);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the bool uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const bool val) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform1i(idx, static_cast<int>(val));
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the int uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const int val) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform1i(idx, val);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the float uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const float val) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform1f(idx, val);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the GLuint uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const GLuint val) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform1i(idx, val);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the Vector3D uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const Math::Vector3D& vec) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform3f(idx, vec.x, vec.y, vec.z);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the vec2 uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const glm::vec2& vec) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform2f(idx, vec.x, vec.y);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the vec3 uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const glm::vec3& vec) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform3f(idx, vec.x, vec.y, vec.z);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the mat3 uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const glm::mat3& mat) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniformMatrix3fv(idx, 1, GL_FALSE, &mat[0][0]);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the mat4 uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const glm::mat4& mat) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniformMatrix4fv(idx, 1, GL_FALSE, &mat[0][0]);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the color uniform
		*/ // ---------------------------------------------------------------------
		void inline ShaderProgram::SetUniform(const char* name, const Color& col) const noexcept
		{
			auto idx = glGetUniformLocation(mProgramID, name);
			if (idx == -1) return;
			glUniform4f(idx, col.r, col.g, col.b, col.a);
		}

		// ------------------------------------------------------------------------
		/*! Set uniform
		*
		*   Sets the uniform in an Array form of any type
		*/ // ---------------------------------------------------------------------
		template<typename T>
		void ShaderProgram::SetUniform(const std::string& name, const stl::Array<T>& vector) const noexcept
		{
			for (size_t i = 0, size = vector.size(); i < size; i++)
				SetUniform((name + "[" + std::to_string(i) + "]").c_str(), vector[i]);
		}
	}
}

#endif