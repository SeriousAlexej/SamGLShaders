<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/logo.png">
</p>

---

<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/demo.gif">
</p>

# SamGLShaders

A small utility library for using OpenGL 3.3 shaders in vanilla 
*Serious Sam The Second Encounter v1.07* game.

1. [Who is this for?](#brief)
2. [Installation](#install)
   1. [For regular users](#install_regular)
   2. [For developers](#install_devs)
3. [Default shaders](#default_shaders_example)
4. [Basic usage](#usage)
   1. [For regular users](#usage_regular)
      1. [Shader entry point](#entry_point)
      2. [GLDefaultProfile](#default_profile)
      3. [Shader profile](#shader_profile)
      4. [Shader description](#shader_descr)
      5. [Shader sources](#shader_sources)
      6. [Default shader uniforms and variables](#default_uniforms)
      7. [Custom entities](#custom_entities)
         1. [GLModelHolder](#glmodel_holder)
         2. [GLModelHolderAnimationChanger](#glanim_changer)
         3. [GLCubeMapBaker](#glcubemap_baker)
      8. [Demo level](#demo_level)
      9. [So how do I create my own shader?](#halpme)
   2. [For developers](#usage_devs)
5. [What does not work](#notwork)
6. [License](#license)
7. [3rd party notice](#thirdparty)

---

# Who is this for?<a name="brief"></a>

This library is usable both for those who are creating custom maps and for those who develop mods 
using Serious Sam's SDK 1.07 via Visual Studio 6.0 compiler.

This library is not well tested, but has been confirmed to work on multiple NVIDIA cards.

# Installation<a name="install"></a>
### For regular users<a name="install_regular"></a>
To install this library, go to the [releases](https://github.com/SeriousAlexej/SamGLShaders/releases) github page and download the latest zip file. Extract it to your Serious Sam TSE 1.07 root directory. Steam version should work too.

### For developers<a name="install_devs"></a>
Clone this repository and place content of **Sources** directory inside SDK's *Sources* directory, 
so that 'GLShaders' directory and, for example, 'EntitiesMP' are on the same level.
  If you are willing to recompile this library from sources, add **GLShaders.dsp** project to your workspace as a dependency of **EntitiesMP**, provide link 
and include directories for **glew** and **jansson** third-party libraries and you are ready to go.

# Default shaders<a name="default_shaders_example"></a>
**SamGLShaders** provides some shaders as example on how to use this library. Here is their description:

 - `GL3 Base` - simple shader that tries to mimic the look and feel of Serious Sam's default `Base` shader in SKAStudio
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/base.png">
 
 - `GL3 Lul` - shader that demonstrates usage of sprite in vertex program, and dynamic texture mapping in fragment program
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/lul.png">
 
 - `GL3 Reflection` - shader that uses cubemaps to render reflections
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/ref.png">
 
 - `GL3 Sprite` - this shader is meant to show how shader profiles work in terms of directory and file structure
 
 - `GL3 SS2 Standard` - shader that tries to mimic basic look and feel of Serious Sam 2 model rendering
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/ss2.png">
 
 - `GL3 SS3 Standard` - shader that tries to mimic basic look and feel of Serious Sam 3 model rendering
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/ss3.png">
 
 - `GL3 Water` - this shader is an example of how to render a simple water surface
 
 <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/water.png">

# Basic usage<a name="usage"></a>
### For regular users<a name="usage_regular"></a>
**SamGLShaders** uses SKA models. They are very convenient, mostly because they enable this whole project 
to work. With SKA models, a 'shader' concept was introduced to the Serious Engine.
  While default *shaders* are not shaders at all in our modern sense of this word,
  they provide a very important property - ability to modify rendering pipeline. 
  **SamGLShaders**, due to limitations of it being an add-on on top of the vanilla engine,
  introduces the concept of **'shaders profile'**.
  
  A **Shader profile** is simply a set of 20 shaders. While number 20 is pretty limited and has been picked 
  arbitrarily, the number of profiles is not limited at all.
  
  ---
  
**IMPORTANT** - Before we continue, it must be noted that **SamGLShaders** library does *NOT* use textures that are provided by Serious Engine. Instead, it tries to load **.tga** source of that texture to support greater resolution. However, regular **.tex** textures are still required as placeholders for SKAStudio and the engine itself.
  
  ---
  
  Here is a structure of *Shaders* directory, that is used to store your shaders:
  
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/structure.png">

  #### Shader entry point<a name="entry_point"></a>
  Is a simple text file, that defines index of a shader that it describes.
  Shader entry point is not linked to a particular **shader profile**, it only tells engine the name of a shader,
  and its number in currently enabled profile.
  
  Here is a content of _**GL3 Base.sha**_ file:
  ```
  Package: TFNM Bin\GLShadersMP.module
  Name: Shader_GLShader_1
  Info: Shader_Desc_GLShader_1
  ```
  
  In general, proper content should strictly follow this pattern:
  
  ```
  Package: TFNM Bin\GLShadersMP.module
  Name: Shader_GLShader_<NUMBER_BETWEEN_1_AND_20>
  Info: Shader_Desc_GLShader_<SAME_NUMBER_AS_ABOVE>
  ```
  
  #### GLDefaultProfile<a name="default_profile"></a>
  As stated above, **shader entry point** is not linked to a profile,
  the profile itself is bound dynamically by an external entity, like **GLModelHolder**, which is set up by user.
  Because SKAStudio cannot have entities and only serves as a model viewer, the contents of this file
  are interpreted as a name of a profile that should be enabled by default. That means, when you want to create
  your new custom shader profile (which you should always do to be compatible with all other possible shaders),
  you should edit this file and make sure that your profile's name is saved in it. While one profile is enabled,
  SKAStudio *CAN NOT* work with shaders from other profiles! The game itself works fine though.
  
  #### Shader profile<a name="shader_profile"></a>
  Basically, its a directory that can contain up to 20 shaders.
  Here is a structure of a typical shader profile:
  
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/profile.png">
  
  #### Shader description<a name="shader_descr"></a>
   Is a *json* plain text file, that describes shader and defines is properties. All shader description files should be called by this pattern:
   ```
   GLShader_<NUMBER_BETWEEN_1_AND_20>.json
   ```
    
  Here is a quick overview of what can be written in a typical *json* for your shader:
  
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/shader.png">
  
  Lets look closer at each field:
  
  - `vertex_shader` - relative path to your vertex shader.
  
  - `fragment_shader` - relative path to your fragment shader.
  
  - `name` - shader name, not really used, just a quick description of shader.
  
  - `texture_wrapping_x`
  `texture_wrapping_y` - these two parameters describe how textures are wrapped. Possible values: **clamp**, **repeat**
  
  - `depth_test` - enables depth testing.
  
  - `depth_write` - enables depth writing.
  
  - `depth_func` - comparison that determines whether fragment is close enough to be visible. Possible values: **never**, **less**, **less or equal**, **equal**, **not equal**, **greater or equal**, **greater**, **always**.
  
  - `culling` - determines which faces are culled. Possible values: **front**, **back**, **none**.
  
  - `alpha_test` - if enabled, fragmens with less than half alpha channel are considered transparent.
  
  - `blending` - enables blending.
  
  - `blend_src_mode`
  `blend_dst_mode` - these two parameters specify coefficients that are applied to blend function. Possible values: **one**, **zero**, **src color**, **inv src color**, **dst color**, **inv dst color**, **src alpha**, **inv src alpha**.
  
  - `uv_map_1`
  `uv_map_2` - these two parameters describe GLSL uniforms that are used to identify UV maps. Only two UV maps are supported simultaniously. All uniform values are *json* objects that have two fields:
  
    + `shader_name` - name of corresponding variable in your shader source code.
    + `name` - how it is displayed in SKAStudio GUI.
     
  - `custom_floats` - number of custom float uniforms, provided for shader. Useful only for programmers, more details about this below.
  
  - `custom_vecs` - numver of custom vec3 uniforms, provided for shader. Similar to *custom_floats*.
  
  - `textures` - array of objects that describe texture uniform variables. Quantity potentially unlimited. Supported texture samplers are: **sampler2D**, **samplerCube**.
  
  - `floats` - array of objects that describe float uniform variables. Quantity potentially unlimited.
  
  - `colors` - array of objects that describe vec3 uniform variables for colors. Quantity potentially unlimited.
  
  - `flags` - array of objects that describe boolean uniform variables. Maximum number is 8.

 #### Shader sources<a name="shader_sources"></a>
   Are actual GLSL code that does all the job. To learn more about GLSL, please read proper documentation, books, use google, whatever.
  GLSL help is not provided here and to use this library you *have* to know how to write OpenGL shaders.
  
 #### Default shader uniforms and variables<a name="default_uniforms"></a>
   Obviously, for shaders to actually work, library has to provide some predefined uniforms and variables:
   
   - `uniform mat4 projMx` - projection matrix
   - `uniform mat4 viewMx` - view matrix
   - `uniform mat4 modelMx` - model matrix
   - `uniform vec3 lightDir` - light direction in world-space
   - `uniform vec3 colLight` - light color
   - `uniform vec3 colAmbient` - light ambient color
   - `uniform float time` - current ingame time
   - `layout(location = 0) in vec3 posModel` - vertex coordinates, _**already** in world-space_
   - `layout(location = 1) in vec2 vertexUV1` - UV Map 1 coordinates
   - `layout(location = 2) in vec3 normalModel` - fragment normal vector, _in model-space_
   - `layout(location = 3) in vec3 tangentModel` - fragment tangent vector (used in normal mapping)
   - `layout(location = 4) in vec3 bitangentModel` - fragment bitangent vector (used in normal mapping)
   - `layout(location = 5) in vec2 vertexUV2` - UV Map 2 coordinates

#### Custom entities<a name="custom_entities"></a>
**GLShaders** library provides three new entities for use in Serious Editor. They can be added to your Editor's virtual tree through *ecl* files in **Classes** directory:

  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/entities.png">
  
  #### GLModelHolder<a name="glmodel_holder"></a>
  This entity gives ability to properly use SKA models in your level with your shaders.
  
  GLModelHolder entity also has a special field `GL Profile` that defines directory, from which its shaders should be loaded. Set this field *before* loading proper .smc model.
  
  #### GLModelHolderAnimationChanger<a name="glanim_changer"></a>
  This entity provides ability to play animations for targeted **GLModelHolder**. Desired animation is provided in form of string value, that is stored in `Animation Name` property.
  
  #### GLCubeMapBaker<a name="glcubemap_baker"></a>
  This is a utility entity, used only to create cubemaps.
  
  To easily create a cubemap, place this entity in the desired position and check its `Press to bake` property.
  
  A "Save File" dialog shall appear, asking you where to save the cubemap. In case of success, you should get something similar to this:
  
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/cubemap.png">
  
  _**NOTE**_: the generated .tex file is just a placeholder.
  
  #### Demo level<a name="demo_level"></a>
  To show all new custom entities in action, an example level is provided:
  
  ```
  GLResources\Examples\ExampleLevel.wld
  ```
  
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SamGLShaders/master/images/level.png">

#### So how do I create my own shader?<a name="halpme"></a>
Follow these simple steps:

 - Create your custom shader profile directory inside `Shaders` directory. For example, `Shaders\MyProfile`
 - Create a shader entry point file with desired name and index that is free in your profile. For example, `Shaders\My Shader.sha` 
 - Create shader description json file and properly set it up. For example, `Shaders\MyProfile\GLShader_1.json`
 - Create actual shader source files. For example, `Shaders\MyProfile\MyShader.vert` and `Shaders\MyProfile\MyShader.frag`
 - Before launching SKAStudio, edit the `Shaders\GLDefaultProfile` file so it contains your profile name, i.e. `MyProfile`
 - Launch SKAStudio, setup your model and shader and work on them until you are happy with the results.
 - Launch SeriousEditor, place a **GLModelHolder** entity on the map, edit its `GL Profile` property so it is `MyProfile` and select your model with your custom shader.
 
 As an exercise you can use and explore provided **GL3 Sprite** shader.

### For developers<a name="usage_devs"></a>
  To use GLShaders in your custom dll for Serious Sam 1.07, a **GLShadersMP.lib** and **GLShadersMPD.lib** libraries are provided in *bin* directory. But you can always recompile them from source code if you desire so.
  
  There are three main include files that provide API:
  ```cpp
#include <GLShaders/RenderSessionAPI.h>
#include <GLShaders/ShaderProfileAPI.h>
#include <GLShaders/ModelMatrix.h>
  ```
  
  File **ModelMatrix.h** provides variables that are used to compute model matrix for shader. As engine premultiplies vertex data with model matrix internally, there is no easy way to get that info at the library level, so the easiest way is to provide this information manually before draw call.
  ```cpp
extern ALEX_API float g_AlexShaders_modelMatrix_pos0;
extern ALEX_API float g_AlexShaders_modelMatrix_pos1;
extern ALEX_API float g_AlexShaders_modelMatrix_pos2;
extern ALEX_API float g_AlexShaders_modelMatrix_ang0; // angles are in degrees and not radians
extern ALEX_API float g_AlexShaders_modelMatrix_ang1;
extern ALEX_API float g_AlexShaders_modelMatrix_ang2;
  ```
  
  File **RenderSessionAPI.h** exposes these functions:
  
  ```cpp
  // call this function to initialize this library. Returns true on success
ALEX_API bool   AlexShaders_Initialize(); 
// call this function when you want to deinitialize the library
ALEX_API void   AlexShaders_Finalize(); 
// basic check
ALEX_API bool   AlexShaders_Initialized();
// render session is a unique set of parameters used to draw one particular model
// create unique render session for each model and always set it before drawing
ALEX_API int    AlexShaders_RegisterRenderSession();
// set render session to be used
ALEX_API void   AlexShaders_SetRenderSession(int renderSession);
// reset all sessions. This function is not normally used
ALEX_API void   AlexShaders_ClearAllSessions();
// a helper function to load textures for particular CModelInstance onto GPU
ALEX_API void   AlexShaders_Precache(CModelInstance& modelInstance);
// set address, from which custom float uniforms are taken
// number of these floats is determined in JSON config
ALEX_API float* AlexShaders_SetCustomFloatPool(float* customFloatPool);
// set address, from which custom vec3 uniforms are taken
// number of these vec3's is determined in JSON config. vec3 is simply three consequent floats
ALEX_API float* AlexShaders_SetCustomVecPool(float* customVecPool);
  ```
  
  And finally **ShaderProfileAPI.h** provides following APIs:
  
  ```cpp
// call this function to create new shader profile, which will load shaders from Shaders/<loadingDir>
ALEX_API int AlexShaders_CreateShaderProfile(const char* loadingDir);
// set render profile to be used. Return value is previously active profile
ALEX_API int AlexShaders_SetCurrentProfile(int newProfile);
// gets currently active profile. This function is not normally used
ALEX_API int AlexShaders_GetCurrentProfile();
  ```
  
  A good example of how to use these API's is source code of **GLModelHolder** entity, located in **GLModelHolder.es** file.
  

## What does not work<a name="notwork"></a>
 - Haze
 - Fog
 - DirectX (nobody sane needs DirectX anyway)

## License<a name="license"></a>
This library uses GPL v3, for more info, read file LICENSE

## 3rd party notice<a name="thirdparty"></a>
glew - Modified BSD license

glm - Modified MIT license

jansson - MIT license

stb_image - public domain