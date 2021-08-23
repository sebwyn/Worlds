#include "Worlds/Bitmap/Bitmap.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Worlds;

Bitmap::Bitmap(std::string filename) : m_filename(std::move(filename)) {
    load(m_filename);
}

void Bitmap::load(std::string filename) {
    stbi_uc *raw = stbi_load(filename.c_str(), &m_size.x, &m_size.y,
                             &m_bytes_per_pixel, STBI_rgb_alpha);

    m_data = CreateScope<uint8_t[]>(getLength());
    std::memcpy(m_data, raw, getLength());

    stbi_image_free(raw);
}

uint32_t Bitmap::getLength() const {
    return m_size.x * m_size.y * m_bytes_per_pixel;
}

uint32_t calculateLength(const glm::ivec2 &size, uint32_t bytesPerPixel){
    return size.x * size.y * bytesPerPixel;
}
