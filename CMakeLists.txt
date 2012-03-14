project (nutmeg CXX C)
cmake_minimum_required (VERSION 2.8)

add_definitions(-DNUTMEG_DEBUG)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  link_directories (${nutmeg_SOURCE_DIR}/Engine/Library/Newton/vc)
else()
  link_directories (${nutmeg_SOURCE_DIR}/Engine/Library/Newton/linux)
endif()

FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

include_directories (
  Engine/CoreSrc/Common
  Engine/CoreSrc/Engine
  Engine/CoreSrc/Math
  Engine/CoreSrc/Render
  Engine/CoreSrc/Audio
  Engine/CoreSrc/Scene
  Engine/CoreSrc/Physics
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

include (FindDevIL)

if (NOT IL_FOUND)
  if (MSVC)
    set (IL_LIBRARIES  ${nutmeg_SOURCE_DIR}/Engine/Library/DevIL/vc/DevIL.lib)
    set (ILU_LIBRARIES ${nutmeg_SOURCE_DIR}/Engine/Library/DevIL/vc/ILU.lib)
    set (ILUT_LIBRARIES ${nutmeg_SOURCE_DIR}/Engine/Library/DevIL/vc/ILUT.lib)
    set (IL_FOUND TRUE)
    message("Using vendored MSVC DevIL")
  endif()
endif ()

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
  Engine/Subsystems/Platform/PlatformWindows.cpp)

set_source_files_properties(${NUTMEG_SOURCES} PROPERTIES LANGUAGE CXX)

add_library (nutmeg_engine STATIC ${NUTMEG_SOURCES})
add_executable (nutmeg_test Samples/example/main.cpp)

set(NUTRIS_SOURCES
    Samples/nutris/game.cpp
    Samples/nutris/main.cpp
    Samples/nutris/st_autor.cpp
    Samples/nutris/st_game.cpp
    Samples/nutris/st_mainmenu.cpp)

add_executable(nutris ${NUTRIS_SOURCES})


TARGET_LINK_LIBRARIES(nutmeg_engine Newton pthread ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(nutmeg_test ${OPENGL_gl_LIBRARY} nutmeg_engine)
TARGET_LINK_LIBRARIES(nutris ${OPENGL_gl_LIBRARY} nutmeg_engine)

if (NOT SDL_FOUND)
  set (SDL_INCLUDE_DIR ${nutmeg_SOURCE_DIR}/Engine/Library/SDL)
  set (SDL_LIBRARY ${nutmeg_SOURCE_DIR}/Engine/Library/SDL/vc/SDL.lib ${nutmeg_SOURCE_DIR}/Engine/Library/SDL/vc/SDLmain.lib)
  set (SDL_FOUND TRUE)
endif()

if (SDL_FOUND)
  include_directories (${SDL_INCLUDE_DIR})
  target_link_libraries (nutmeg_test ${SDL_LIBRARY})
  target_link_libraries (nutris ${SDL_LIBRARY})
  IF (MSVC)
    set_target_properties (nutmeg_test PROPERTIES LINK_FLAGS /nodefaultlib:msvcrt)
    set_target_properties (nutris PROPERTIES LINK_FLAGS /nodefaultlib:msvcrt)
  ENDIF (MSVC)  

  add_definitions (-DNUTMEG_PLATFORM_SDL)
endif (SDL_FOUND)

if (IL_FOUND)
  target_link_libraries (nutmeg_test ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES})
  target_link_libraries (nutris ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES})
endif (IL_FOUND)

INSTALL(TARGETS nutmeg_test DESTINATION ${nutmeg_SOURCE_DIR}/Binary)

