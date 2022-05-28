﻿// HaGame3D_v1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifndef HAGAME_H
#define HAGAME_H

#define NOMINMAX true
#define WITH_SDL2

#include <iostream>

#include "Utils/Memory.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Hypercube.h"
#include "Math/Functions.h"
#include "Math/Quaternion.h"
#include "Math/Sample.h"
#include "Math/Perlin.h"
#include "Math/Triangle.h"
#include "Math/Line.h"
#include "Math/LineSegment.h"
#include "Math/Ray.h"
#include "Math/Plane.h"
#include "Math/Interval.h"
#include "Math/NSphere.h"
#include "Math/Collisions.cpp"

#include "Graphics/OpenGL.cpp"
#include "Graphics/Monitors.h"
#include "Graphics/Window.cpp"
#include "Graphics/Shader.cpp"
#include "Graphics/ShaderProgram.cpp"
#include "Graphics/Mesh.h"
#include "Graphics/Triangle.hpp"
#include "Graphics/Quad.hpp"
#include "Graphics/RectPrism.hpp"
#include "Graphics/TriangularPrism.hpp"
#include "Graphics/Stairs.hpp"
#include "Graphics/Circle.hpp"
#include "Graphics/Cylinder.hpp"
#include "Graphics/Cone.hpp"
#include "Graphics/Sphere.hpp"
#include "Graphics/Camera.h"
#include "Graphics/PerspectiveCamera.hpp"
#include "Graphics/OrthographicCamera.hpp"
#include "Graphics/Primitives.h"
#include "Graphics/Font.h"
#include "Graphics/Text.h"
#include "Graphics/Texture.cpp"
#include "Graphics/LineBuffer.cpp"
#include "Graphics/Material.cpp"
#include "Graphics/Color.h"
#include "Graphics/Color.cpp"
#include "Graphics/Sprite.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Routines.cpp"
#include "Graphics/Line.h"
#include "Graphics/Plane.h"
#include "Graphics/PixelGrid.h"
#include "Graphics/GridLines.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Components/AxisRenderer.h"
#include "Graphics/Components/BoundingBoxRenderer.h"
#include "Graphics/Components/RigidBodyRenderer.h"
#include "Graphics/Components/MeshRenderer.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/PixelGridRenderer.h"
#include "Graphics/Components/SpriteRenderer.h"
#include "Graphics/Components/AnimatedSpriteRenderer.h"
#include "Graphics/Components/BoxRenderer.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Graphics/Systems/CameraSystem.h"
#include "Graphics/Systems/LightingSystem.h"
#include "Graphics/MeshBuffer.cpp"

#include "Audio/Audio.h"

#include "Input/Device.h"
#include "Input/Devices/Gamepad.h"
#include "Input/Devices/KeyboardMouse.h"

#include "Utils/UUID.h"
#include "Utils/Watchable.hpp"
#include "Utils/Path.h"
#include "Utils/File.cpp"
#include "Utils/FileSystem.cpp"
#include "Utils/ConfigParser.h"
#include "Utils/Clock.h"
#include "Utils/Timer.h"
#include "Utils/Aliases.h"
#include "Utils/String.cpp"
#include "Utils/Random.h"
#include "Utils/Testing.h"
#include "Utils/GridMap.h"
#include "Utils/RectGrid.h"
#include "Utils/Promise.h"
#include "Utils/TimedCallback.h"
#include "Utils/Algorithms/AStar.h"

#include "Physics/Components/RigidBody.h"
#include "Physics/Components/SimpleMovement.h"
#include "Physics/Components/RotationMovement.h"
#include "Physics/Components/Collider.h"
#include "Physics/Systems/PhysicsSystem.h"
#include "Physics/Systems/MovementSystem.h"
#include "Physics/Systems/CollisionSystem.h"
#include "Physics/Systems/PartitionSystem.h"
#include "Physics/BoundingVolume.h"


#include "Core/Game.cpp"
#include "Core/ResourceManager.cpp"
#include "Core/Transform.cpp"
#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"
#include "Core/ECS/System.h"
#include "Core/ECS/Component.h"
#include "Core/Scene.h"
#include "Core/Subscriber.h"
#include "Core/Publisher.h"

#include "UI/Systems/UISystem.h"

#ifndef HAGAME_IMPLEMENTATION
#define HAGAME_IMPLEMENTATION

#include "Physics/Collisions.cpp"
#include "Core/ECS/Entity.cpp"

#endif

#include "Vendor/tinyexr/tinyexr.cc"

#include "Vendor/json.hpp"

#include "Vendor/entt/entt.hpp"

#include "Vendor/stb/stb_image.cpp"
// #include "Vendor/stb/stb_truetype.cpp"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui.cpp"
#include "Vendor/imgui/imgui_draw.cpp"
#include "Vendor/imgui/imgui_tables.cpp"
#include "Vendor/imgui/imgui_widgets.cpp"
#include "Vendor/imgui/imgui_demo.cpp"
#include "Vendor/imgui/backends/imgui_impl_sdl.h"
#include "Vendor/imgui/backends/imgui_impl_sdl.cpp"
#include "Vendor/imgui/backends/imgui_impl_opengl3.h"
#include "Vendor/imgui/backends/imgui_impl_opengl3.cpp"


// #include "Vendor/soloud/soloud.h"
// #include "Vendor/soloud/soloud_wav.h"
#include "Vendor/soloud/soloud.cpp"
#include "Vendor/soloud/soloud_sdl2.cpp"
#include "Vendor/soloud/soloud_sdl2_dll.c"
#include "Vendor/soloud/soloud_audiosource.cpp"
#include "Vendor/soloud/soloud_bus.cpp"
#include "Vendor/soloud/soloud_core_3d.cpp"
#include "Vendor/soloud/soloud_core_basicops.cpp"
#include "Vendor/soloud/soloud_core_faderops.cpp"
#include "Vendor/soloud/soloud_core_filterops.cpp"
#include "Vendor/soloud/soloud_core_getters.cpp"
#include "Vendor/soloud/soloud_core_setters.cpp"
#include "Vendor/soloud/soloud_core_voicegroup.cpp"
#include "Vendor/soloud/soloud_core_voiceops.cpp"
#include "Vendor/soloud/soloud_fader.cpp"
#include "Vendor/soloud/soloud_fft.cpp"
#include "Vendor/soloud/soloud_fft_lut.cpp"
#include "Vendor/soloud/soloud_file.cpp"
#include "Vendor/soloud/soloud_filter.cpp"
#include "Vendor/soloud/soloud_misc.cpp"
#include "Vendor/soloud/soloud_queue.cpp"
#include "Vendor/soloud/soloud_thread.cpp"
#include "Vendor/soloud/stb_vorbis.c"
#include "Vendor/soloud/soloud_wav.cpp"
#include "Vendor/soloud/soloud_wavstream.cpp"
#include "Vendor/soloud/dr_impl.cpp"
// #include "Vendor/soloud/dr_mp3.h"
// #include "Vendor/soloud/dr_wav.h"

#include "Vendor/ImGuiFileDialog/ImGuiFileDialog.h"
#include "Vendor/ImGuiFileDialog/ImGuiFileDialog.cpp"

// TODO: Reference additional headers your program requires here.

#endif