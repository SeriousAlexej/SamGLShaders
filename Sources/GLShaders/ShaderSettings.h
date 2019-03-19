/*
    SamGLShaders - a free software for rendering Serious Sam models with OpenGL 3.3 shaders
    Copyright (C) 2019 Oleksii Sierov (seriousalexej@gmail.com)
	
    SamGLShaders is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    SamGLShaders is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with SamGLShaders.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SAM_GL_SHADERS_SHADER_SETTINGS_H
#define SAM_GL_SHADERS_SHADER_SETTINGS_H
#include <string>
#include <vector>
#include <gl/glew.h>

struct ShaderSettings
{
  struct ShaderField
  {
    std::string name;
    std::string shaderName;
  };

  struct ShaderUniform : ShaderField
  {
    ShaderUniform();

    void Bind(GLuint program);

    GLint location;
  };

  struct ShaderTexture : ShaderUniform
  {
    void LoadTextureType(GLuint program);

    GLenum textureType;
  };

  ShaderSettings();
  ~ShaderSettings();

  void CompileShader();

  bool valid;
  bool compiled;
  bool loaded;
  std::string name;
  int textureWrappingX;
  int textureWrappingY;
  int culling;
  int depthFunc;
  int blendSrcMode;
  int blendDstMode;
  int numUVMaps;
  bool blending;
  bool depthTest;
  bool depthWrite;
  bool alphaTest;
  ShaderField uvMap1;
  ShaderField uvMap2;
  std::vector<ShaderTexture> textures;
  std::vector<ShaderUniform> floats;
  std::vector<ShaderUniform> colors;
  std::vector<ShaderUniform> flags;
  std::vector<ShaderUniform> customFloats;
  std::vector<ShaderUniform> customVecs;

  std::string vertexShaderFile;
  std::string fragmentShaderFile;
  
  GLuint shaderProgram;
  GLint  projectionLocation;
  GLint  viewLocation;
  GLint  modelLocation;
  GLint  lightDirLocation;
  GLint  colLightLocation;
  GLint  colAmbientLocation;
  GLint  timeLocation;
};

void LoadShader(const std::string& path, const std::string& filename, ShaderSettings& shaderSettings);

#endif
