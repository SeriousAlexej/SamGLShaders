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
#ifndef SAM_GL_SHADERS_INIT_H
#define SAM_GL_SHADERS_INIT_H
#include "AlexShaders.h"

class CModelInstance;
struct ShaderSettings;

ALEX_API bool   AlexShaders_Initialize();
ALEX_API void   AlexShaders_Finalize();
ALEX_API bool   AlexShaders_Initialized();
ALEX_API int    AlexShaders_RegisterRenderSession();
ALEX_API void   AlexShaders_SetRenderSession(int renderSession);
ALEX_API void   AlexShaders_ClearAllSessions();
ALEX_API void   AlexShaders_Precache(CModelInstance& modelInstance);
ALEX_API float* AlexShaders_SetCustomFloatPool(float* customFloatPool);
ALEX_API float* AlexShaders_SetCustomVecPool(float* customVecPool);

void AlexRenderWithShader(const ShaderSettings& settings);

#endif