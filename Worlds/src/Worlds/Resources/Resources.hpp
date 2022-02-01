#pragma once

#include "Worlds/Core/Module.hpp"

namespace Worlds {

class Resources : Module::Registrar<Resources> {
  public:
    template <typename T> Ref<T> Find() {}
    

  private:
};

} // namespace Worlds
