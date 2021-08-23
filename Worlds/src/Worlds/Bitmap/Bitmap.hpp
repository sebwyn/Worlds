#pragma once

#include <glm/glm.hpp>

namespace Worlds {

class Bitmap {
  public:
    Bitmap() = default;
    Bitmap(std::string filename);

    void load(std::string filename);

    uint32_t getLength() const;
    const glm::uvec2 &getSize() const { return m_size; }
    uint32_t getBytesPerPixel() const { return m_bytes_per_pixel; }

    const Scope<uint8_t[]> &getData() const { return m_data; }

  private:
    static uint32_t calculateLength(const glm::ivec2 &size,
                                    uint32_t bytesPerPixel);

  private:
    std::string m_filename;

    Scope<uint8_t[]> m_data;
    glm::uvec2 m_size;
    uint32_t m_bytes_per_pixel;
};

}; // namespace Worlds
