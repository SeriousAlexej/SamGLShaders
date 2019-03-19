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
#include <fstream>
#include <jansson.h>
#include "ShaderSettings.h"
#include "Shader.h"
#include "RenderSessionAPI.h"

#define JSON_LOAD_FIELD_OPT(field, from, type, logger, required) \
json_t* field = json_object_get(from, #field); \
if (!field && required) \
{ \
  if (logger.is_open()) \
    logger << "Required field '" << #field << "' is not present!\n"; \
    return; \
} \
if (field && !json_is_##type(field)) \
{ \
  if (logger.is_open()) \
    logger << "Field '" << #field << "' should be " << #type << "\n"; \
  return; \
}

#define JSON_LOAD_FIELD(field, from, type, logger) \
  JSON_LOAD_FIELD_OPT(field, from, type, logger, false)

#define JSON_LOAD_UNIFORMS_CONTAINER(containerType, targetContainer, container, log) \
{const unsigned int num##container = json_array_size(container); \
{for (unsigned int i = 0; i < num##container; ++i) \
{ \
  json_t* uniformVal = json_array_get(container, i); \
  if (!json_is_object(uniformVal)) \
  { \
    if (log.is_open()) \
      log << "'" #container "' array should contain objects!\n"; \
    return; \
  } \
  containerType uniform; \
  bool ok = false; \
  ReadFieldValue(uniform, uniformVal, false, log, ok); \
  if (!ok) \
    return; \
  targetContainer.container.push_back(uniform); \
}}}

namespace
{
  struct JSONScope
  {
  public:
    JSONScope(json_t* inputJson)
      : json(inputJson)
    {}
    ~JSONScope()
    {
      if (json)
        json_decref(json);
    }

  private:
    json_t* json;
  };

  std::vector<char> ReadTextFile(
    const std::string& path,
    const std::string& filename,
    std::ofstream& log)
  {
    std::ifstream input;
    input.open((path + filename).c_str());
    if (!input.is_open())
    {
      if (log.is_open())
        log << "Failed to open '" << filename << "' file!\n";
      return std::vector<char>();
    }
    input.seekg(0, std::ifstream::end);
    const unsigned int textSize = static_cast<int>(input.tellg());
    input.seekg(0, std::ifstream::beg);
    std::vector<char> content(textSize, '\0');
    input.read(&content[0], textSize);
    return content;
  }

  void ReadFieldValue(
    ShaderSettings::ShaderField& uniform,
    json_t* container,
    bool canBeEmpty,
    std::ofstream& log,
    bool& ok)
  {
    ok = false;
    JSON_LOAD_FIELD_OPT(shader_name, container, string, log, !canBeEmpty)
    JSON_LOAD_FIELD_OPT(name, container, string, log, !canBeEmpty)
    if ((!shader_name || !name) && canBeEmpty)
    {
      ok = true;
      return;
    }
    uniform.shaderName = json_string_value(shader_name);
    uniform.name = json_string_value(name);
    if (!canBeEmpty && (uniform.shaderName.empty() || uniform.name.empty()))
    {
      if (log.is_open())
        log << "Uniforms 'shader_name' and 'name' fields can not be empty!\n";
      return;
    }
    ok = true;
  }

  int ReadTextureWrapping(const std::string& val)
  {
    if (val == "repeat")
      return GFX_REPEAT;
    if (val == "clamp")
      return GFX_CLAMP;
    return -1;
  }

  int ReadCulling(const std::string& val)
  {
    if (val == "front")
      return GFX_FRONT;
    if (val == "back")
      return GFX_BACK;
    if (val == "none")
      return GFX_NONE;
    return -1;
  }

  int ReadDepthFunc(const std::string& val)
  {
    if (val == "never")
      return GFX_NEVER;
    if (val == "less")
      return GFX_LESS;
    if (val == "less or equal")
      return GFX_LESS_EQUAL;
    if (val == "equal")
      return GFX_EQUAL;
    if (val == "not equal")
      return GFX_NOT_EQUAL;
    if (val == "greater or equal")
      return GFX_GREATER_EQUAL;
    if (val == "greater")
      return GFX_GREATER;
    if (val == "always")
      return GFX_ALWAYS;
    if (val == "")
      return 0;
    return -1;
  }

  int ReadBlendMode(const std::string& val)
  {
    if (val == "one")
      return GFX_ONE;
    if (val == "zero")
      return GFX_ZERO;
    if (val == "src color")
      return GFX_SRC_COLOR;
    if (val == "inv src color")
      return GFX_INV_SRC_COLOR;
    if (val == "dst color")
      return GFX_DST_COLOR;
    if (val == "inv dst color")
      return GFX_INV_DST_COLOR;
    if (val == "src alpha")
      return GFX_SRC_ALPHA;
    if (val == "inv src alpha")
      return GFX_INV_SRC_ALPHA;
    if (val == "")
      return 0;
    return -1;
  }
} // namespace anonymous

ShaderSettings::ShaderUniform::ShaderUniform()
  : location(0)
{
}

void ShaderSettings::ShaderUniform::Bind(GLuint program)
{
  if (!shaderName.empty())
    location = glGetUniformLocation(program, shaderName.c_str());
}

void ShaderSettings::ShaderTexture::LoadTextureType(GLuint program)
{
  GLsizei dummyLength;
  GLint dummySize;
  GLenum samplerType;
  glGetActiveUniform(
    program,
    location,
    0,
    &dummyLength,
    &dummySize,
    &samplerType,
    NULL);
  switch (samplerType)
  {
  case GL_SAMPLER_CUBE:
    textureType = GL_TEXTURE_CUBE_MAP;
    break;
  case GL_SAMPLER_2D:
    textureType = GL_TEXTURE_2D;
    break;
  default:
    throw std::runtime_error("Unknown sampler type for provided texture");
  }
}

ShaderSettings::ShaderSettings()
  : valid(false)
  , compiled(false)
  , loaded(false)
  , name("GLShader_Invalid")
  , shaderProgram(0)
  , projectionLocation(0)
  , viewLocation(0)
  , modelLocation(0)
  , lightDirLocation(0)
  , colLightLocation(0)
  , colAmbientLocation(0)
  , timeLocation(0)
{
}

ShaderSettings::~ShaderSettings()
{
  if (valid)
    glDeleteProgram(shaderProgram);
}

void ShaderSettings::CompileShader()
{
  std::ofstream log;
  log.open((vertexShaderFile + ".log").c_str());

  std::vector<char> vertexShaderSrc = ReadTextFile("", vertexShaderFile.c_str(), log);
  std::vector<char> fragmentShaderSrc = ReadTextFile("", fragmentShaderFile.c_str(), log);
  if (vertexShaderSrc.empty() || fragmentShaderSrc.empty())
  {
    if (log.is_open())
      log << "Shader sources can not be empty!\n";
    return;
  }

  GLuint vertexShader;
  GLuint fragmentShader;

  GLint logLength;
  GLint result;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char* vertexShaderSrcData = &vertexShaderSrc[0];
  glShaderSource(vertexShader, 1, &vertexShaderSrcData, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    if (log.is_open())
    {
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
      if (logLength > 0)
      {
        GLchar* logMessage = new GLchar[logLength + 1];
        glGetShaderInfoLog(vertexShader, logLength, NULL, logMessage);
        logMessage[logLength] = '\0';
        log << "Error compiling vertex shader:\n" << logMessage << "\n";
        delete[] logMessage;
      }
    }
    glDeleteShader(vertexShader);
    return;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragmentShaderSrcData = &fragmentShaderSrc[0];
  glShaderSource(fragmentShader, 1, &fragmentShaderSrcData, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    if (log.is_open())
    {
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
      if (logLength > 0)
      {
        GLchar* logMessage = new GLchar[logLength + 1];
        glGetShaderInfoLog(fragmentShader, logLength, NULL, logMessage);
        logMessage[logLength] = '\0';
        log << "Error compiling fragment shader:\n" << logMessage << "\n";
        delete[] logMessage;
      }
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return;
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if (result == GL_FALSE)
  {
    if (log.is_open())
    {
      glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
      if (logLength > 0)
      {
        GLchar* logMessage = new GLchar[logLength + 1];
        glGetProgramInfoLog(shaderProgram, logLength, NULL, logMessage);
        logMessage[logLength] = '\0';
        log << "Error linking shader program:\n" << logMessage << "\n";
        delete[] logMessage;
      }
    }
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
    return;
  }

  std::vector<ShaderTexture>::iterator itt;
  for (itt = textures.begin(); itt != textures.end(); ++itt)
  {
    itt->Bind(shaderProgram);
    itt->LoadTextureType(shaderProgram);
  }
  std::vector<ShaderUniform>::iterator it;
  for (it = floats.begin(); it != floats.end(); ++it)
    it->Bind(shaderProgram);
  for (it = colors.begin(); it != colors.end(); ++it)
    it->Bind(shaderProgram);
  for (it = flags.begin(); it != flags.end(); ++it)
    it->Bind(shaderProgram);
  for (it = customFloats.begin(); it != customFloats.end(); ++it)
    it->Bind(shaderProgram);
  for (it = customVecs.begin(); it != customVecs.end(); ++it)
    it->Bind(shaderProgram);

  projectionLocation = glGetUniformLocation(shaderProgram, "projMx");
  viewLocation       = glGetUniformLocation(shaderProgram, "viewMx");
  modelLocation      = glGetUniformLocation(shaderProgram, "modelMx");
  lightDirLocation   = glGetUniformLocation(shaderProgram, "lightDir");
  colLightLocation   = glGetUniformLocation(shaderProgram, "colLight");
  colAmbientLocation = glGetUniformLocation(shaderProgram, "colAmbient");
  timeLocation       = glGetUniformLocation(shaderProgram, "time");

  compiled = true;
}

void LoadShader(
  const std::string& path,
  const std::string& filename,
  ShaderSettings& shaderSettings)
{
  shaderSettings.loaded = true;
  std::ofstream log;
  log.open((path + filename + ".log").c_str());
  std::vector<char> content = ReadTextFile(path, filename, log);
  if (content.empty())
    return;

  json_error_t error;
  json_t* root = json_loads(&content[0], JSON_REJECT_DUPLICATES, &error);
  JSONScope scope(root);
  if (!root)
  {
    if (log.is_open())
      log << "JSON parse error on line " << error.line << ": " << error.text << "\n";
    return;
  }
  if (!json_is_object(root))
  {
    if (log.is_open())
      log << "Root config element should be object!\n";
    return;
  }
  JSON_LOAD_FIELD(vertex_shader, root, string, log)
  JSON_LOAD_FIELD(fragment_shader, root, string, log)
  JSON_LOAD_FIELD(name, root, string, log)
  JSON_LOAD_FIELD(texture_wrapping_x, root, string, log)
  JSON_LOAD_FIELD(texture_wrapping_y, root, string, log)
  JSON_LOAD_FIELD(depth_test, root, boolean, log)
  JSON_LOAD_FIELD(depth_write, root, boolean, log)
  JSON_LOAD_FIELD(depth_func, root, string, log)
  JSON_LOAD_FIELD(culling, root, string, log)
  JSON_LOAD_FIELD(alpha_test, root, boolean, log)
  JSON_LOAD_FIELD(blending, root, boolean, log)
  JSON_LOAD_FIELD(blend_src_mode, root, string, log)
  JSON_LOAD_FIELD(blend_dst_mode, root, string, log)
  JSON_LOAD_FIELD(uv_map_1, root, object, log)
  JSON_LOAD_FIELD(uv_map_2, root, object, log)
  JSON_LOAD_FIELD(textures, root, array, log)
  JSON_LOAD_FIELD(floats, root, array, log)
  JSON_LOAD_FIELD(colors, root, array, log)
  JSON_LOAD_FIELD(flags, root, array, log)
  JSON_LOAD_FIELD(custom_floats, root, integer, log)
  JSON_LOAD_FIELD(custom_vecs, root, integer, log)

  if (json_array_size(flags) > 8)
  {
    if (log.is_open())
      log << "Cannot have more than 8 flags!\n";
    return;
  }

  shaderSettings.name = json_string_value(name);
  shaderSettings.blending = json_boolean_value(blending);
  shaderSettings.depthTest = json_boolean_value(depth_test);
  shaderSettings.depthWrite = json_boolean_value(depth_write);
  shaderSettings.alphaTest = json_boolean_value(alpha_test);
  const int numCustomFloats = json_integer_value(custom_floats);
  if (numCustomFloats > 0)
    shaderSettings.customFloats.resize(numCustomFloats, ShaderSettings::ShaderUniform());
  {for (int i = 0; i < numCustomFloats; ++i)
  {
    char buffer[10];
    buffer[sprintf(buffer, "%d", i + 1)] = '\0';
    shaderSettings.customFloats[i].shaderName = std::string("customFloat_") + buffer;
  }}
  const int numCustomVecs = json_integer_value(custom_vecs);
  if (numCustomVecs > 0)
    shaderSettings.customVecs.resize(numCustomVecs, ShaderSettings::ShaderUniform());
  {for (int i = 0; i < numCustomVecs; ++i)
  {
    char buffer[10];
    buffer[sprintf(buffer, "%d", i + 1)] = '\0';
    shaderSettings.customVecs[i].shaderName = std::string("customVec_") + buffer;
  }}
  shaderSettings.textureWrappingX = ReadTextureWrapping(json_string_value(texture_wrapping_x));
  if (shaderSettings.textureWrappingX < 0)
  {
    if (log.is_open())
      log << "'texture_wrapping_x' is invalid!\n";
    return;
  }
  shaderSettings.textureWrappingY = ReadTextureWrapping(json_string_value(texture_wrapping_y));
  if (shaderSettings.textureWrappingY < 0)
  {
    if (log.is_open())
      log << "'texture_wrapping_y' is invalid!\n";
    return;
  }
  shaderSettings.culling = ReadCulling(json_string_value(culling));
  if (shaderSettings.culling < 0)
  {
    if (log.is_open())
      log << "'culling' is invalid!\n";
    return;
  }
  shaderSettings.depthFunc = ReadDepthFunc(json_string_value(depth_func));
  if (shaderSettings.depthFunc < 0 ||
      (shaderSettings.depthTest && shaderSettings.depthFunc == 0))
  {
    if (log.is_open())
      log << "'depth_func' is invalid!\n";
    return;
  }
  shaderSettings.blendSrcMode = ReadBlendMode(json_string_value(blend_src_mode));
  if (shaderSettings.blendSrcMode < 0 ||
      (shaderSettings.blending && shaderSettings.blendSrcMode == 0))
  {
    if (log.is_open())
      log << "'blend_src_mode' is invalid!\n";
    return;
  }
  shaderSettings.blendDstMode = ReadBlendMode(json_string_value(blend_dst_mode));
  if (shaderSettings.blendDstMode < 0 ||
    (shaderSettings.blending && shaderSettings.blendDstMode == 0))
  {
    if (log.is_open())
      log << "'blend_dst_mode' is invalid!\n";
    return;
  }

  JSON_LOAD_UNIFORMS_CONTAINER(ShaderSettings::ShaderTexture, shaderSettings, textures, log)
  JSON_LOAD_UNIFORMS_CONTAINER(ShaderSettings::ShaderUniform, shaderSettings, floats, log)
  JSON_LOAD_UNIFORMS_CONTAINER(ShaderSettings::ShaderUniform, shaderSettings, colors, log)
  JSON_LOAD_UNIFORMS_CONTAINER(ShaderSettings::ShaderUniform, shaderSettings, flags, log)

  bool dummy = true;
  shaderSettings.numUVMaps = 0;
  ReadFieldValue(shaderSettings.uvMap1, uv_map_1, true, log, dummy);
  ReadFieldValue(shaderSettings.uvMap2, uv_map_2, true, log, dummy);
  if (!shaderSettings.uvMap1.shaderName.empty())
    shaderSettings.numUVMaps++;
  if (!shaderSettings.uvMap2.shaderName.empty())
    shaderSettings.numUVMaps++;

  shaderSettings.vertexShaderFile = path + json_string_value(vertex_shader);
  shaderSettings.fragmentShaderFile = path + json_string_value(fragment_shader);

  if (AlexShaders_Initialized())
    shaderSettings.CompileShader();
  shaderSettings.valid = true;
}
