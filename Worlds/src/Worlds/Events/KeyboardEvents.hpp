#pragma once

#include <volk.h>
#include <GLFW/glfw3.h>
#include <sstream>

#include "Worlds/Events/Event.hpp"

namespace Worlds {

class KeyEvent : public Event {
  public:
    KeyEvent(int key) : key(key) {}

    int getKey() const { return key; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard);

  protected:
    int key;
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(int key, bool isRepeat)
        : KeyEvent(key), isRepeat(isRepeat){};

    bool getIsRepeat() const { return isRepeat; }

    EVENT_CLASS_TYPE(KeyPressed);

  private:
    bool isRepeat = false;
};

class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(int key) : KeyEvent(key){};

    EVENT_CLASS_TYPE(KeyReleased);
};

} // namespace Worlds
