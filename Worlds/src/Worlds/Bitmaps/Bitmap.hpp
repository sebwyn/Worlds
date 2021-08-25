#pragma once

namespace Worlds {
template <typename Base> class BitmapFactory {
  public:
    using TLoadMethod =
        std::function<void(Base *, const std::filesystem::path &)>;
    using TWriteMethod =
        std::function<void(const Base *, const std::filesystem::path &)>;
    using TRegistryMap =
        std::unordered_map<std::string, std::pair<TLoadMethod, TWriteMethod>>;

    virtual ~BitmapFactory() = default;

    static TRegistryMap &Registry() {
        static TRegistryMap impl;
        return impl;
    }

    template <typename T> class Registrar /*: public Base*/ {
      protected:
        template <typename... Args> static bool Register(Args &&...names) {
            for (std::string &&name : {names...})
                BitmapFactory::Registry()[name] =
                    std::make_pair(&T::Load, &T::Write);
            return true;
        }
    };
};

class Bitmap : public BitmapFactory<Bitmap> {
  public:
    Bitmap() = default;
    explicit Bitmap(std::filesystem::path filename);
    explicit Bitmap(const glm::uvec2 &size, uint32_t bytesPerPixel = 4);
    Bitmap(std::unique_ptr<uint8_t[]> &&data, const glm::uvec2 &size,
           uint32_t bytesPerPixel = 4);
    ~Bitmap() = default;

    void Load(const std::filesystem::path &filename);
    void Write(const std::filesystem::path &filename) const;

    explicit operator bool() const noexcept { return !m_data; }

    uint32_t GetLength() const;

    const std::filesystem::path &getFilename() const { return m_filename; }
    void setFilename(const std::filesystem::path &filename) {
        this->m_filename = filename;
    }

    const std::unique_ptr<uint8_t[]> &getData() const { return m_data; }
    std::unique_ptr<uint8_t[]> &GetData() { return m_data; }
    void setData(std::unique_ptr<uint8_t[]> &&data) {
        this->m_data = std::move(data);
    }

    const glm::uvec2 &GetSize() const { return m_size; }
    void setSize(const glm::uvec2 &size) { this->m_size = size; }

    uint32_t GetBytesPerPixel() const { return m_bytes_per_pixel; }
    void setBytesPerPixel(uint32_t bytesPerPixel) {
        this->m_bytes_per_pixel = bytesPerPixel;
    }

  private:
    static uint32_t calculateLength(const glm::uvec2 &size,
                                    uint32_t bytesPerPixel);

    std::filesystem::path m_filename;
    std::unique_ptr<uint8_t[]> m_data;
    glm::uvec2 m_size;
    uint32_t m_bytes_per_pixel = 0;
};

} // namespace Worlds
