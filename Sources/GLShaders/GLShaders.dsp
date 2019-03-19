# Microsoft Developer Studio Project File - Name="GLShaders" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GLShaders - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GLShaders.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GLShaders.mak" CFG="GLShaders - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLShaders - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GLShaders - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLShaders - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLSHADERS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLSHADERS_EXPORTS" /Fr /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 jansson.lib Engine.lib EngineGUI.lib glew32.lib glu32.lib opengl32.lib User32.lib /nologo /dll /map /debug /machine:I386 /out:"Release/GLShaders.module" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\GLShaders.module
InputName=GLShaders
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).module" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).module "$(ENGINE_DIR)\Bin" >nul 
	copy Release\$(InputName).map "$(ENGINE_DIR)\Bin" >nul 
	copy Release\$(InputName).lib "$(ENGINE_DIR)\Bin" >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GLShaders - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLSHADERS_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLSHADERS_EXPORTS" /FR /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 janssond.lib EngineD.lib EngineGUID.lib glew32.lib glu32.lib opengl32.lib User32.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/GLShadersD.module" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\Debug\GLShadersD.module
InputName=GLShadersD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).module" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).module "$(ENGINE_DIR)\Bin\Debug" >nul 
	copy Debug\$(InputName).map "$(ENGINE_DIR)\Bin\Debug" >nul 
	copy Debug\$(InputName).lib "$(ENGINE_DIR)\Bin\Debug" >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "GLShaders - Win32 Release"
# Name "GLShaders - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GLCubeMapBaker.cpp
# End Source File
# Begin Source File

SOURCE=.\GLModelHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\GLModelHolderAnimationChanger.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderSession.cpp
# End Source File
# Begin Source File

SOURCE=.\ShaderEntryPoints.cpp
# End Source File
# Begin Source File

SOURCE=.\ShaderProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\ShaderSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\stb.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlexShaders.h
# End Source File
# Begin Source File

SOURCE=.\DeclDllExport.h
# End Source File
# Begin Source File

SOURCE=.\DeclDllImport.h
# End Source File
# Begin Source File

SOURCE=.\GLCubeMapBaker.h
# End Source File
# Begin Source File

SOURCE=.\GLCubeMapBaker_tables.h
# End Source File
# Begin Source File

SOURCE=.\GLModelHolder.h
# End Source File
# Begin Source File

SOURCE=.\GLModelHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\GLModelHolderAnimationChanger.h
# End Source File
# Begin Source File

SOURCE=.\GLModelHolderAnimationChanger_tables.h
# End Source File
# Begin Source File

SOURCE=.\Hash.h
# End Source File
# Begin Source File

SOURCE=.\glm\mat3.h
# End Source File
# Begin Source File

SOURCE=.\glm\mat4.h
# End Source File
# Begin Source File

SOURCE=.\glm\mat_func.h
# End Source File
# Begin Source File

SOURCE=.\glm\math.h
# End Source File
# Begin Source File

SOURCE=.\ModelMatrix.h
# End Source File
# Begin Source File

SOURCE=.\RenderSessionAPI.h
# End Source File
# Begin Source File

SOURCE=.\Shader.h
# End Source File
# Begin Source File

SOURCE=.\ShaderProfile.h
# End Source File
# Begin Source File

SOURCE=.\ShaderProfileAPI.h
# End Source File
# Begin Source File

SOURCE=.\ShaderSettings.h
# End Source File
# Begin Source File

SOURCE=.\ShaderSources.h
# End Source File
# Begin Source File

SOURCE=.\stb_image.h
# End Source File
# Begin Source File

SOURCE=.\glm\vec2.h
# End Source File
# Begin Source File

SOURCE=.\glm\vec3.h
# End Source File
# Begin Source File

SOURCE=.\glm\vec4.h
# End Source File
# End Group
# Begin Group "Classes"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\GLCubeMapBaker.es

!IF  "$(CFG)" == "GLShaders - Win32 Release"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLCubeMapBaker.es
InputName=GLCubeMapBaker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GLShaders - Win32 Debug"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLCubeMapBaker.es
InputName=GLCubeMapBaker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLModelHolder.es

!IF  "$(CFG)" == "GLShaders - Win32 Release"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLModelHolder.es
InputName=GLModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GLShaders - Win32 Debug"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLModelHolder.es
InputName=GLModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLModelHolderAnimationChanger.es

!IF  "$(CFG)" == "GLShaders - Win32 Release"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLModelHolderAnimationChanger.es
InputName=GLModelHolderAnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GLShaders - Win32 Debug"

# Begin Custom Build - GLShaders/$(InputName).es
InputPath=.\GLModelHolderAnimationChanger.es
InputName=GLModelHolderAnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                  ..\  
	ecc GLShaders/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
