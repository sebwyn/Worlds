#pragma once

#include "Worlds/Models/Model.hpp"

namespace Worlds {

class Cube : public Model {
  public:
      Cube(glm::vec3 extents);
};

} // namespace Worlds
