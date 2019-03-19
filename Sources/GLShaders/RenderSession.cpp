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
#include <windows.h>
#include <map>
#include <vector>
#include <gl/glew.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#define NULL 0
typedef float FLOAT;
typedef unsigned char UBYTE;
typedef unsigned long  int ULONG;
typedef signed long int SLONG;
typedef ULONG COLOR;
typedef long int INDEX;
#define ENGINE_API __declspec(dllimport)
#define SE_INCL_GL_TYPES_H
#include <Engine/Graphics/Vertex.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Stream.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Projection.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Ska/ModelInstance.h>
#include "glm/math.h"
#include "glm/vec2.h"
#include "glm/vec3.h"
#include "glm/mat4.h"
#include "glm/mat_func.h"
#include "stb_image.h"
#include "ModelMatrix.h"
#include "ShaderSources.h"
#include "RenderSessionAPI.h"
#include "ShaderSettings.h"
#include "ShaderProfile.h"
#include "Hash.h"

namespace
{
  float hFOVtovFOV(float hFOV, float aspect)
  {
    return 2.0f*glm::atan(1.0f / aspect * glm::tan(glm::radians(hFOV)*0.5f));
  }

  struct RenderSession
  {
    RenderSession()
    {
      VBO = 0;
      UVBO1 = 0;
      UVBO2 = 0;
      TBO = 0;
      BTBO = 0;
      NBO = 0;
      EBO = 0;
      numVertices = 0;
    }

    GLuint VBO;
    GLuint UVBO1;
    GLuint UVBO2;
    GLuint TBO;
    GLuint BTBO;
    GLuint NBO;
    GLuint EBO;
    std::vector<GLuint> textures; // NON-OWNING CONTAINER
    std::vector<CTextureObject*> textureIDs;
    INDEX numVertices;
  };

  bool LoadTextureData(const char* path, unsigned char*& data, int& format, int& width, int& height)
  {
    int nrChannels;
    data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
      return false;
    switch (nrChannels)
    {
    case 4:
      format = GL_RGBA;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 2:
      format = GL_RGBA;
      stbi_image_free(data);
      data = stbi_load(path, &width, &height, &nrChannels, 4);
      break;
    default:
      format = GL_RGB;
      stbi_image_free(data);
      data = stbi_load(path, &width, &height, &nrChannels, 3);
      break;
    }
    return true;
  }

  int g_currentSession = -1;
  bool g_initialized = false;
  GLint g_previousArrayBuffer;
  GLint g_previousElementBuffer;
  std::vector<std::vector<RenderSession> > g_renderSessions;
  std::map<unsigned long, GLuint> g_textures;
  float* g_customFloatPool = NULL;
  float* g_customVecPool = NULL;
} // anonymous namespace

int AlexShaders_RegisterRenderSession()
{
  g_renderSessions.push_back(std::vector<RenderSession>());
  std::vector<RenderSession>& surfacesSessions = g_renderSessions.back();
  surfacesSessions.push_back(RenderSession());
  return g_renderSessions.size() - 1;
}

void AlexShaders_SetRenderSession(int renderSession)
{
  g_currentSession = renderSession;
}

bool AlexShaders_Initialize()
{
  if (g_initialized)
    return true;

  g_textures.clear();
  g_renderSessions.clear();
  g_currentSession = -1;
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
    return false;

  g_initialized = true;
  return true;
}

bool AlexShaders_Initialized()
{
  return g_initialized;
}

void AlexShaders_ClearAllSessions()
{
  for (int j = 0; j < g_renderSessions.size(); ++j)
  {
    std::vector<RenderSession>& surfaces = g_renderSessions[j];
    {for (int i = 0; i < surfaces.size(); ++i)
    {
      RenderSession& session = surfaces[i];
      glDeleteBuffers(1, &session.VBO);
      glDeleteBuffers(1, &session.UVBO1);
      glDeleteBuffers(1, &session.UVBO2);
      glDeleteBuffers(1, &session.TBO);
      glDeleteBuffers(1, &session.BTBO);
      glDeleteBuffers(1, &session.NBO);
      glDeleteBuffers(1, &session.EBO);
    }}
  }
  std::map<unsigned long, GLuint>::iterator it;
  for (it = g_textures.begin(); it != g_textures.end(); ++it)
    glDeleteTextures(1, &it->second);
  g_renderSessions.clear();
  g_textures.clear();
}

void AlexShaders_Finalize()
{
  g_initialized = false;
  ClearAllProfiles();
  AlexShaders_ClearAllSessions();
}

GLuint LoadTexture2D(CTextureObject* texObject)
{
  CTFileName absTextureNoExt;
  ExpandFilePath(0, texObject->GetName().NoExt(), absTextureNoExt);
  unsigned long hashValue = HashValue(static_cast<const char*>(absTextureNoExt));
  std::map<unsigned long, GLuint>::iterator foundPos = g_textures.find(hashValue);
  if (foundPos != g_textures.end())
    return foundPos->second;

  GLuint tex;
  std::string srcTexture = std::string(static_cast<const char*>(absTextureNoExt)) + ".tga";
  unsigned char* data;
  int format;
  int width;
  int height;
  if (!LoadTextureData(srcTexture.c_str(), data, format, width, height))
  {
    g_textures[hashValue] = 0;
    return 0;
  }
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  static bool hasAnisotropic = glewGetExtension("GL_EXT_texture_filter_anisotropic") == GL_TRUE;
  if (hasAnisotropic)
  {
    float maxAnisotropic = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmapEXT(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
  g_textures[hashValue] = tex;
  return tex;
}

GLuint LoadTextureCube(CTextureObject* texObject)
{
  CTFileName absTextureNoExt;
  ExpandFilePath(0, texObject->GetName().NoExt(), absTextureNoExt);
  unsigned long hashValue = HashValue(static_cast<const char*>(absTextureNoExt));
  std::map<unsigned long, GLuint>::iterator foundPos = g_textures.find(hashValue);
  if (foundPos != g_textures.end())
    return foundPos->second;

  GLuint cubeTex;
  const std::string srcTexture = static_cast<const char*>(absTextureNoExt);
  std::vector<std::string> cubeTextures;
  cubeTextures.reserve(6);
  cubeTextures.push_back(srcTexture + "_right.tga");
  cubeTextures.push_back(srcTexture + "_left.tga");
  cubeTextures.push_back(srcTexture + "_top.tga");
  cubeTextures.push_back(srcTexture + "_bottom.tga");
  cubeTextures.push_back(srcTexture + "_front.tga");
  cubeTextures.push_back(srcTexture + "_back.tga");
  glGenTextures(1, &cubeTex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
  {for (unsigned int i = 0; i < cubeTextures.size(); i++)
  {
    unsigned char* data;
    int format;
    int width;
    int height;
    if (!LoadTextureData(cubeTextures.at(i).c_str(), data, format, width, height))
    {
      glDeleteTextures(1, &cubeTex);
      g_textures[hashValue] = 0;
      return 0;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }}
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  g_textures[hashValue] = cubeTex;
  return cubeTex;
}

void AlexShaders_Precache(CModelInstance& modelInstance)
{
  {FOREACHINSTATICARRAY(modelInstance.mi_aMeshInst, MeshInstance, mesh)
  {
    {FOREACHINSTATICARRAY(mesh->mi_tiTextures, TextureInstance, tex)
    {
      (void)LoadTexture2D(&(tex->ti_toTexture));
    }}
  }}
}

float* AlexShaders_SetCustomFloatPool(float* customFloatPool)
{
  float* prevPool = g_customFloatPool;
  g_customFloatPool = customFloatPool;
  return prevPool;
}

float* AlexShaders_SetCustomVecPool(float* customVecPool)
{
  float* prevPool = g_customVecPool;
  g_customVecPool = customVecPool;
  return prevPool;
}

GLuint LoadTexture(GLenum textureType, CTextureObject* texObject)
{
  if (!texObject)
    return 0;

  GLuint tex = 0;
  switch (textureType)
  {
  case GL_TEXTURE_CUBE_MAP:
    tex = LoadTextureCube(texObject);
    break;
  case GL_TEXTURE_2D:
    tex = LoadTexture2D(texObject);
    break;
  default:
    throw std::runtime_error("Unknown sampler type for provided texture");
  }
  return tex;
}

void CreateTangentBuffers(RenderSession& session)
{
  const INDEX numVertices = shaGetVertexCount();
  glm::vec3* tangents = new glm::vec3[numVertices];
  glm::vec3* bitangents = new glm::vec3[numVertices];

  glm::mat4 mat;
  memcpy(&mat[0][0], shaGetObjToAbsMatrix(), sizeof(FLOAT) * 12);
  mat = glm::transpose(mat);

  const INDEX numIndices = shaGetIndexCount();
  const INDEX* indices = shaGetIndexArray();
  GFXVertex4* vertices = shaGetVertexArray();
  GFXNormal4* normals = shaGetNormalArray();
  GFXTexCoord* uvs = shaGetUVMap(0);
  {for (INDEX ind = 0; ind < numIndices; ind += 3)
  {
    GFXVertex4& sv0 = vertices[indices[ind + 0]];
    GFXVertex4& sv1 = vertices[indices[ind + 1]];
    GFXVertex4& sv2 = vertices[indices[ind + 2]];
    glm::vec3 v0 = mat * glm::vec4(sv0.x, sv0.y, sv0.z, 1.0f);
    glm::vec3 v1 = mat * glm::vec4(sv1.x, sv1.y, sv1.z, 1.0f);
    glm::vec3 v2 = mat * glm::vec4(sv2.x, sv2.y, sv2.z, 1.0f);

    GFXTexCoord& uv0 = uvs[indices[ind + 0]];
    GFXTexCoord& uv1 = uvs[indices[ind + 1]];
    GFXTexCoord& uv2 = uvs[indices[ind + 2]];

    glm::vec3 deltaPos1 = v1 - v0;
    glm::vec3 deltaPos2 = v2 - v0;

    glm::vec2 deltaUV1(uv1.u - uv0.u, uv1.v - uv0.v);
    glm::vec2 deltaUV2(uv2.u - uv0.u, uv2.v - uv0.v);

    FLOAT r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    tangents[indices[ind + 0]] = tangent;
    tangents[indices[ind + 1]] = tangent;
    tangents[indices[ind + 2]] = tangent;
    bitangents[indices[ind + 0]] = bitangent;
    bitangents[indices[ind + 1]] = bitangent;
    bitangents[indices[ind + 2]] = bitangent;
  }}

  {for (INDEX ind = 0; ind < numVertices; ++ind)
  {
    glm::vec3 n = mat * glm::vec4(normals[ind].nx, normals[ind].ny, normals[ind].nz, 0.0f);
    glm::vec3& t = tangents[ind];
    glm::vec3& b = bitangents[ind];

    t = t - n * glm::dot(n, t);
    t = glm::normalize(t);

    if (glm::dot(glm::cross(n, t), b) < 0.0f)
      t *= -1.0f;
  }}

  glGenBuffers(1, &session.TBO);
  glBindBuffer(GL_ARRAY_BUFFER, session.TBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), tangents, GL_STATIC_DRAW);

  glGenBuffers(1, &session.BTBO);
  glBindBuffer(GL_ARRAY_BUFFER, session.BTBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), bitangents, GL_STATIC_DRAW);

  delete[] tangents;
  delete[] bitangents;
}

void UpdateVertexBuffer(RenderSession& session, bool create = false)
{
  glm::mat4 mat;
  memcpy(&mat[0][0], shaGetObjToAbsMatrix(), sizeof(FLOAT) * 12);
  mat = glm::transpose(mat);
  const INDEX numVertices = shaGetVertexCount();
  GFXVertex4* vertices = shaGetVertexArray();
  glm::vec3* verts = new glm::vec3[numVertices];
  {for (INDEX i = 0; i < numVertices; ++i) {
    verts[i] = mat * glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);
  }}
  glBindBuffer(GL_ARRAY_BUFFER, session.VBO);
  if (create) {
    session.numVertices = numVertices;
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), verts, GL_DYNAMIC_DRAW);
  }
  else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(glm::vec3), verts);
  }
  delete[] verts;

  GFXNormal* norms = shaGetNormalArray();
  glm::vec3* normals = new glm::vec3[numVertices];
  {for (INDEX i = 0; i < numVertices; ++i) {
    normals[i] = mat * glm::vec4(norms[i].nx, norms[i].ny, norms[i].nz, 0.0f);
  }}
  glBindBuffer(GL_ARRAY_BUFFER, session.NBO);
  if (create)
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), normals, GL_DYNAMIC_DRAW);
  else
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(glm::vec3), normals);
  delete[] normals;
}

void InitSession(RenderSession& session)
{
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &g_previousArrayBuffer);
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &g_previousElementBuffer);

  glGenBuffers(1, &session.VBO);
  glGenBuffers(1, &session.NBO);
  UpdateVertexBuffer(session, true);
  CreateTangentBuffers(session);

  if (shaGetUVMap(0))
  {
    glGenBuffers(1, &session.UVBO1);
    glBindBuffer(GL_ARRAY_BUFFER, session.UVBO1);
    glBufferData(GL_ARRAY_BUFFER, shaGetVertexCount() * sizeof(GFXTexCoord), shaGetUVMap(0), GL_STATIC_DRAW);
  }

  if (shaGetUVMap(1))
  {
    glGenBuffers(1, &session.UVBO2);
    glBindBuffer(GL_ARRAY_BUFFER, session.UVBO2);
    glBufferData(GL_ARRAY_BUFFER, shaGetVertexCount() * sizeof(GFXTexCoord), shaGetUVMap(1), GL_STATIC_DRAW);
  }

  glGenBuffers(1, &session.EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, session.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, shaGetIndexCount() * sizeof(INDEX), shaGetIndexArray(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_previousElementBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_previousArrayBuffer);
}

RenderSession& FindOrCreateProperSessionForSurface(std::vector<RenderSession>& surfaces, const INDEX vertexCount)
{
  for (int i = 0; i < surfaces.size(); ++i)
    if (surfaces[i].numVertices == 0 || surfaces[i].numVertices == vertexCount)
      return surfaces[i];
  surfaces.push_back(RenderSession());
  return surfaces.back();
}

void AlexRenderWithShader(const ShaderSettings& settings)
{
  CAnyProjection3D* proj = shaGetProjection();
  if (!proj->IsPerspective())
    return;

  if (!g_initialized)
    if (!AlexShaders_Initialize())
      return;

  if (g_currentSession < 0)
    AlexShaders_SetRenderSession(AlexShaders_RegisterRenderSession());

  std::vector<RenderSession>& surfaces = g_renderSessions[g_currentSession];
  RenderSession& currentSession = FindOrCreateProperSessionForSurface(surfaces, shaGetVertexCount());
  if (currentSession.VBO == 0)
    InitSession(currentSession);

  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &g_previousArrayBuffer);
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &g_previousElementBuffer);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);
  glUseProgram(settings.shaderProgram);

  UpdateVertexBuffer(currentSession);

  glBindBuffer(GL_ARRAY_BUFFER, currentSession.VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  if (currentSession.UVBO1 != 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, currentSession.UVBO1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
  }
  if (currentSession.UVBO2 != 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, currentSession.UVBO2);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
  }
  glBindBuffer(GL_ARRAY_BUFFER, currentSession.NBO);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glBindBuffer(GL_ARRAY_BUFFER, currentSession.TBO);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glBindBuffer(GL_ARRAY_BUFFER, currentSession.BTBO);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

  // bind default uniform variables
  glm::mat4 viewMx;
  if (settings.projectionLocation >= 0)
  {
    CPerspectiveProjection3D* proj3D = (CPerspectiveProjection3D*)(proj->operator CProjection3D *());
    FLOAT ratio = proj3D->pr_ScreenCenter(1) / proj3D->pr_ScreenCenter(2);
    glm::mat4 projMx = glm::perspective(hFOVtovFOV(proj3D->ppr_FOVWidth, ratio), ratio, proj3D->pr_NearClipDistance, proj3D->pr_FarClipDistance);
    glUniformMatrix4fv(settings.projectionLocation, 1, GL_FALSE, &projMx[0][0]);
  }
  if (settings.viewLocation >= 0)
  {
    CPerspectiveProjection3D* proj3D = (CPerspectiveProjection3D*)(proj->operator CProjection3D *());
    glm::mat3 proxy;
    memcpy(&proxy[0][0], &proj3D->pr_ViewerRotationMatrix.matrix[0][0], sizeof(FLOAT) * 9);
    viewMx = glm::transpose(proxy);
    glm::mat4 translation(1.0f);
    translation[3] = glm::vec4(-proj3D->pr_vViewerPosition.vector[0], -proj3D->pr_vViewerPosition.vector[1], -proj3D->pr_vViewerPosition.vector[2], 1.0f);
    viewMx = viewMx * translation;
    glUniformMatrix4fv(settings.viewLocation, 1, GL_FALSE, &viewMx[0][0]);
  }
  if (settings.modelLocation >= 0)
  {
    FLOATmatrix3D rotMx;
    MakeRotationMatrix(rotMx, ANGLE3D(g_AlexShaders_modelMatrix_ang0, g_AlexShaders_modelMatrix_ang1, g_AlexShaders_modelMatrix_ang2));
    glm::mat3 proxy;
    memcpy(&proxy[0][0], &rotMx.matrix[0][0], sizeof(FLOAT) * 9);
    glm::mat4 modelMx(glm::transpose(proxy));
    glm::mat4 translation(1.0f);
    translation[3] = glm::vec4(g_AlexShaders_modelMatrix_pos0, g_AlexShaders_modelMatrix_pos1, g_AlexShaders_modelMatrix_pos2, 1.0f);
    modelMx = translation * modelMx;
    glUniformMatrix4fv(settings.modelLocation, 1, GL_FALSE, &modelMx[0][0]);
  }
  if (settings.lightDirLocation >= 0)
  {
    const FLOAT3D& lightDir = shaGetLightDirection();
    const glm::vec3 lightDirWorldSpace = glm::inverse(viewMx) * glm::vec4(lightDir(1), lightDir(2), lightDir(3), 0.0f);
    glUniform3fv(settings.lightDirLocation, 1, &lightDirWorldSpace[0]);
  }
  if (settings.colLightLocation >= 0)
  {
    const COLOR& light = shaGetLightColor();
    glm::vec3 colLight(
      (light & 0x000000ff) / 127.0f,
      ((light & 0x0000ff00) >> 8) / 127.0f,
      ((light & 0x00ff0000) >> 16) / 127.0f);
    glUniform3fv(settings.colLightLocation, 1, &colLight[0]);
  }
  if (settings.colAmbientLocation >= 0)
  {
    const COLOR& ambient = shaGetAmbientColor();
    glm::vec3 colAmbient(
      glm::clamp((ambient & 0x000000ff) / 127.0f, 0.0f, 1.0f),
      glm::clamp(((ambient & 0x0000ff00) >> 8) / 127.0f, 0.0f, 1.0f),
      glm::clamp(((ambient & 0x00ff0000) >> 16) / 127.0f, 0.0f, 1.0f));
    glUniform3fv(settings.colAmbientLocation, 1, &colAmbient[0]);
  }
  if (settings.timeLocation >= 0)
  {
    glUniform1f(settings.timeLocation, _pTimer->CurrentTick());
  }

  // bind user-provided uniform variables
  {for (unsigned int i = 0; i < settings.floats.size(); ++i)
  {
    glUniform1f(settings.floats.at(i).location, shaGetFloat(i));
  }}
  {for (unsigned int i = 0; i < settings.colors.size(); ++i)
  {
    const COLOR& col = shaGetColor(i);
    glm::vec4 colValue(
      ((col & 0xff000000) >> 24) / 255.0f,
      ((col & 0x00ff0000) >> 16) / 255.0f,
      ((col & 0x0000ff00) >> 8) / 255.0f,
       (col & 0x000000ff) / 255.0f);
    glUniform4fv(settings.colors.at(i).location, 1, &colValue[0]);
  }}
  {for (unsigned int i = 0; i < settings.flags.size(); ++i)
  {
    bool flagValue = shaGetFlags() & (1ul << i);
    glUniform1f(settings.flags.at(i).location, flagValue ? GL_TRUE : GL_FALSE);
  }}

  // bind programatically provided floats
  if (g_customFloatPool)
    {for (unsigned int i = 0; i < settings.customFloats.size(); ++i)
    {
      glUniform1f(settings.customFloats.at(i).location, g_customFloatPool[i]);
    }}

  // bind programatically provided vectors
  if (g_customVecPool)
    {for (unsigned int i = 0; i < settings.customVecs.size(); ++i)
    {
      glUniform3fv(settings.customVecs.at(i).location, 1, &g_customVecPool[i*3]);
    }}

  // bind textures
  std::vector<CTextureObject*> boundTextures;
  boundTextures.reserve(settings.textures.size());
  {for (unsigned int i = 0; i < settings.textures.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    boundTextures.push_back(shaGetTexture(i));
    if (currentSession.textures.size() <= i)
    {
      currentSession.textures.push_back(LoadTexture(settings.textures.at(i).textureType, boundTextures.back()));
      currentSession.textureIDs.push_back(boundTextures.back());
    }
    else if (currentSession.textureIDs.at(i) != boundTextures.back())
    {
      currentSession.textures[i] = LoadTexture(settings.textures.at(i).textureType, boundTextures.back());
      currentSession.textureIDs[i] = boundTextures.back();
    }
    glBindTexture(settings.textures.at(i).textureType, currentSession.textures.at(i));
    glUniform1i(settings.textures.at(i).location, static_cast<GLint>(i));
  }}

  // actual drawing
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentSession.EBO);
  glDrawElements(GL_TRIANGLES, shaGetIndexCount(), GL_UNSIGNED_INT, 0);

  // unbind textures
  {for (int i = boundTextures.size() - 1; i >= 0; --i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(settings.textures.at(i).textureType, 0);
  }}

  // cleanup
  glUseProgram(0);
  glDisableVertexAttribArray(5);
  glDisableVertexAttribArray(4);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_previousElementBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_previousArrayBuffer);
}
