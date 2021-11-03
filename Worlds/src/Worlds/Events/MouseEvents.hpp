#pragma once

#include "Worlds/Events/Event.hpp"

namespace Worlds {

class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(int x, int y) : x(x), y(y) {}

    inline int getX() { return x; }
    inline int getY() { return y; }

    EVENT_CLASS_TYPE(MouseMoved);
    EVENT_CLASS_CATEGORY(EventCategoryMouse);

  private:
    int x, y;
};

} // namespace Worlds
