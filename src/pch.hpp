/// Precompiled Header ///
#pragma once

/// Platform specific includes ///
/// Windows
#if defined(_WIN32)
  #include <Windows.h>
	#include <TlHelp32.h>
#endif
///


/// OpenGL: GLAD (must be included first)
#include <glad/glad.h>
///

/// Math: glm
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/transform.hpp>
///

/// Graphic library framework: GLFW
#include <GLFW/glfw3.h>
///

/// UI: ImGui
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
///


/// stb tools: stb_image
#include <stb/stb_image.h>
///


/// the C++ Standard Libraries
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <stack>
#include <thread>
#include <memory>
#include <queue>
#include <deque>
#include <utility>
#include <list>
#include <future>
#include <functional>
#include <iomanip>
#include <numeric>
#include <regex>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>
#include <cstdint>
#include <cstring>

///


