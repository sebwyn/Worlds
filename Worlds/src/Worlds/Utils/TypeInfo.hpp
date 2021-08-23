#pragma once

#include <typeindex>

namespace Worlds {
using TypeId = std::size_t;

template <typename T> class TypeInfo {
  public:
    TypeInfo() = delete;

    template <typename K,
              typename = std::enable_if<std::is_convertible_v<K *, T *>>>
    static TypeId getTypeId() noexcept {
        std::type_index typeIndex(typeid(K));
        if (auto it = s_type_map.find(typeIndex); it != s_type_map.end())
            return it->second;

        const auto id = nextTypeId();
        s_type_map[typeIndex] = id;
        return id;
    }

  private:
    static TypeId nextTypeId(){
        const auto id = s_next_type_id;
        ++s_next_type_id;
        return id;
    }
  private:
    static TypeId s_next_type_id;
    static std::unordered_map<std::type_index, TypeId> s_type_map;
};

template<typename K>
TypeId TypeInfo<K>::s_next_type_id = 0;

template<typename K>
std::unordered_map<std::type_index, TypeId> TypeInfo<K>::s_type_map = {};

} // namespace Worlds
