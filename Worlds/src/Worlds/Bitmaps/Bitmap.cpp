#include "Bitmap.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Worlds/Files/Files.hpp"

namespace Worlds {
Bitmap::Bitmap(std::filesystem::path filename)
    : m_filename(std::move(filename)) {
    Load(this->m_filename);
}

Bitmap::Bitmap(const glm::uvec2 &size, uint32_t bytesPerPixel)
    : m_data(std::make_unique<uint8_t[]>(calculateLength(size, bytesPerPixel))),
      m_size(size), m_bytes_per_pixel(bytesPerPixel) {}

Bitmap::Bitmap(std::unique_ptr<uint8_t[]> &&data, const glm::uvec2 &size,
               uint32_t bytesPerPixel)
    : m_data(std::move(data)), m_size(size), m_bytes_per_pixel(bytesPerPixel) {}

void Bitmap::Load(const std::filesystem::path &filename) {
    // Registry()[filename.extension().string()].first(this, filename);

    auto fileLoaded = Files::Read(filename);

    if (!fileLoaded) {
        W_ERROR("Bitmap could not be loaded: ", filename, '\n');
        return;
    }

    m_data = std::unique_ptr<uint8_t[]>(stbi_load_from_memory(
        reinterpret_cast<uint8_t *>(fileLoaded->data()),
        static_cast<uint32_t>(fileLoaded->size()),
        reinterpret_cast<int32_t *>(&m_size.x),
        reinterpret_cast<int32_t *>(&m_size.y),
        reinterpret_cast<int32_t *>(&m_bytes_per_pixel), STBI_rgb_alpha));
    m_bytes_per_pixel = 4;
}

void Bitmap::Write(const std::filesystem::path &filename) const {
    if (auto parentPath = filename.parent_path(); !parentPath.empty())
        std::filesystem::create_directories(parentPath);

    // Registry()[filename.extension().string()].second(this, filename);

    std::ofstream os(filename, std::ios::binary | std::ios::out);
    int32_t len;
    std::unique_ptr<uint8_t[]> png(
        stbi_write_png_to_mem(m_data.get(), m_size.x * m_bytes_per_pixel,
                              m_size.x, m_size.y, m_bytes_per_pixel, &len));
    os.write(reinterpret_cast<char *>(png.get()), len);
}

uint32_t Bitmap::GetLength() const {
    return m_size.x * m_size.y * m_bytes_per_pixel;
}

uint32_t Bitmap::calculateLength(const glm::uvec2 &size,
                                 uint32_t bytesPerPixel) {
    return size.x * size.y * bytesPerPixel;
}
} // namespace Worlds
