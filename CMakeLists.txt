project (nutmeg CXX C)
cmake_minimum_required (VERSION 2.6)

FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

include_directories (
  Engine/CoreSrc/Common
  Samples/CBTemplate/Source
  Engine/CoreSrc/Engine
  Engine/CoreSrc/Math
  Engine/CoreSrc/Render
  Engine/CoreSrc/Audio
  Engine/CoreSrc/Scene
  Engine/CoreSrc/Physics
  Engine/Library/Tokamak
  Engine/Library/Tokamak/math
  Engine/Library/Newton
  Engine/Library/OpenGL
  Engine/Library/DevIL
  Engine/CoreSrc/Platform
  Engine/CoreSrc
  Engine/CoreSrc/Game
  Engine/Subsystems/Platform
  Engine/Subsystems/Audio
  Engine/Library/Squall
  Engine/Library/SDL
)

include (FindSDL)

set (NUTMEG_SOURCES
  Engine/CoreSrc/Audio/AbstractAudio.cpp
  Engine/CoreSrc/Common/Debug.cpp
  Engine/CoreSrc/Common/File.cpp
  Engine/CoreSrc/Common/Log.cpp
  Engine/CoreSrc/Common/Resource.cpp
  Engine/CoreSrc/Common/Str.cpp
  Engine/CoreSrc/Common/Xml.cpp
  Engine/CoreSrc/Engine/Application.cpp
  Engine/CoreSrc/Engine/Console.cpp
  Engine/CoreSrc/Engine/Engine.cpp
  Engine/CoreSrc/Engine/Profiler.cpp
  Engine/CoreSrc/Engine/Subsystem.cpp
  Engine/CoreSrc/Engine/Timer.cpp
  Engine/CoreSrc/Game/Entity.cpp
  Engine/CoreSrc/Game/Game.cpp
  Engine/CoreSrc/Game/Level.cpp
  Engine/CoreSrc/Math/MathCore.cpp
  Engine/CoreSrc/Physics/AbstractPhysics.cpp
  Engine/CoreSrc/Physics/PhysicsNewton.cpp
  Engine/CoreSrc/Physics/PhysicsTokamak.cpp
  Engine/CoreSrc/Platform/AbstractPlatform.cpp
  Engine/CoreSrc/Platform/Input.cpp
  Engine/CoreSrc/Render/AbstractRender.cpp
  Engine/CoreSrc/Render/Camera.cpp
  Engine/CoreSrc/Render/Light.cpp
  Engine/CoreSrc/Render/Material.cpp
  Engine/CoreSrc/Render/Mesh.cpp
  Engine/CoreSrc/Render/RenderBadaOpenGL1.cpp
  Engine/CoreSrc/Render/RenderOpenGL1.cpp
  Engine/CoreSrc/Render/RenderOpenGL2.cpp
  Engine/CoreSrc/Render/Skeleton.cpp
  Engine/CoreSrc/Scene/BoneNode.cpp
  Engine/CoreSrc/Scene/CameraNode.cpp
  Engine/CoreSrc/Scene/ControllerNode.cpp
  Engine/CoreSrc/Scene/EntityNode.cpp
  Engine/CoreSrc/Scene/LightNode.cpp
  Engine/CoreSrc/Scene/MeshNode.cpp
  Engine/CoreSrc/Scene/Node.cpp
  Engine/CoreSrc/Scene/OriginNode.cpp
  Engine/CoreSrc/Scene/ParticleNode.cpp
  Engine/CoreSrc/Scene/ParticleSystemNode.cpp
  Engine/CoreSrc/Scene/PhysicsNode.cpp
  Engine/CoreSrc/Scene/Scene.cpp
  Engine/CoreSrc/Scene/SkinnedMeshNode.cpp
  Engine/CoreSrc/Scene/TextNode.cpp
  Engine/CoreSrc/Scene/TriggerNode.cpp
  Engine/Library/OpenGL/gl/GLee.c
  Engine/Subsystems/Audio/AudioBada.cpp
  Engine/Subsystems/Audio/AudioSquall.cpp
  Engine/Subsystems/Platform/PlatformBada.cpp
  Engine/Subsystems/Platform/PlatformSDL.cpp
  Engine/Subsystems/Platform/PlatformWindows.cpp
  Samples/CBTemplate/Source/main.cpp)

set_source_files_properties(${NUTMEG_SOURCES} PROPERTIES LANGUAGE CXX)
add_executable (nutmeg_test WIN32 ${NUTMEG_SOURCES})

TARGET_LINK_LIBRARIES(nutmeg_test ${OPENGL_gl_LIBRARY})

if (SDL_FOUND)
  include_directories (${SDL_INCLUDE_DIR})
  target_link_libraries (nutmeg_test ${SDL_LIBRARY})
  IF (MSVC)
    set_target_properties (nutmeg_test PROPERTIES LINK_FLAGS /nodefaultlib:msvcrt)
  ENDIF (MSVC)  
endif (SDL_FOUND)