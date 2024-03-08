#ifndef IMGUI_SFML_H
#define IMGUI_SFML_H

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Joystick.hpp>

#include "imgui-SFML_export.h"

#if __cplusplus >= 201703L // C++17 and above
#define IMGUI_SFML_NODISCARD [[nodiscard]]
#else
#define IMGUI_SFML_NODISCARD
#endif

#if __cplusplus >= 201402L // C++14 and above
#define IMGUI_SFML_DEPRECATED [[deprecated]]
#else
#define IMGUI_SFML_DEPRECATED
#endif

namespace sf {
class Event;
class RenderTarget;
class RenderTexture;
class RenderWindow;
class Sprite;
class Texture;
class Window;
}

namespace ImGui {
namespace SFML {
IMGUI_SFML_NODISCARD IMGUI_SFML_API bool Init(sf::RenderWindow& window,
                                              bool loadDefaultFont = true);
IMGUI_SFML_NODISCARD IMGUI_SFML_API bool Init(sf::Window& window, sf::RenderTarget& target,
                                              bool loadDefaultFont = true);
IMGUI_SFML_NODISCARD IMGUI_SFML_API bool Init(sf::Window& window, const sf::Vector2f& displaySize,
                                              bool loadDefaultFont = true);

IMGUI_SFML_API void SetCurrentWindow(const sf::Window& window);

// DEPRECATED: Use ProcessEvent(const sf::Window& window, const sf::Event& event)
IMGUI_SFML_DEPRECATED IMGUI_SFML_API void ProcessEvent(const sf::Event& event);
IMGUI_SFML_API void ProcessEvent(const sf::Window& window, const sf::Event& event);

IMGUI_SFML_API void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize,
                           sf::Time dt);

IMGUI_SFML_API void Render();

IMGUI_SFML_API void Shutdown(const sf::Window& window);
// Shuts down all ImGui contexts
IMGUI_SFML_API void Shutdown();

// joystick functions
IMGUI_SFML_API void SetActiveJoystickId(unsigned int joystickId);
IMGUI_SFML_API void SetJoystickDPadThreshold(float threshold);
IMGUI_SFML_API void SetJoystickLStickThreshold(float threshold);
IMGUI_SFML_API void SetJoystickRStickThreshold(float threshold);
IMGUI_SFML_API void SetJoystickLTriggerThreshold(float threshold);
IMGUI_SFML_API void SetJoystickRTriggerThreshold(float threshold);

IMGUI_SFML_API void SetJoystickMapping(int key, unsigned int joystickButton);
IMGUI_SFML_API void SetDPadXAxis(sf::Joystick::Axis dPadXAxis, bool inverted = false);
IMGUI_SFML_API void SetDPadYAxis(sf::Joystick::Axis dPadYAxis, bool inverted = false);
IMGUI_SFML_API void SetLStickXAxis(sf::Joystick::Axis lStickXAxis, bool inverted = false);
IMGUI_SFML_API void SetLStickYAxis(sf::Joystick::Axis lStickYAxis, bool inverted = false);
IMGUI_SFML_API void SetRStickXAxis(sf::Joystick::Axis rStickXAxis, bool inverted = false);
IMGUI_SFML_API void SetRStickYAxis(sf::Joystick::Axis rStickYAxis, bool inverted = false);
IMGUI_SFML_API void SetLTriggerAxis(sf::Joystick::Axis lTriggerAxis);
IMGUI_SFML_API void SetRTriggerAxis(sf::Joystick::Axis rTriggerAxis);
} // end of namespace SFML

} // end of namespace ImGui

#endif // # IMGUI_SFML_H
