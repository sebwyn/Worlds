#pragma once

#include "Worlds/Core/Module.hpp"

struct PHYSFS_File;

namespace Worlds {
enum class FileMode { Read, Write, Append };

class BaseFStream {
  public:
    explicit BaseFStream(PHYSFS_File *file);
    virtual ~BaseFStream();

    size_t length();

  protected:
    PHYSFS_File *file;
};

class IFStream : public BaseFStream, public std::istream {
  public:
    explicit IFStream(const std::filesystem::path &filename);
    virtual ~IFStream();
};

class OFStream : public BaseFStream, public std::ostream {
  public:
    explicit OFStream(const std::filesystem::path &filename,
                      FileMode writeMode = FileMode::Write);
    virtual ~OFStream();
};

class FStream : public BaseFStream, public std::iostream {
  public:
    explicit FStream(const std::filesystem::path &filename,
                     FileMode openMode = FileMode::Read);
    virtual ~FStream();
};

/**
 * @brief Module used for managing files on engine updates.
 */
class Files : public Module::Registrar<Files> {
    inline static const bool Registered = Register(Stage::Post);

  public:
    Files();
    ~Files();

    void Update() override;

    void AddSearchPath(const std::string &path);
    void RemoveSearchPath(const std::string &path);
    void ClearSearchPath();

    static bool ExistsInPath(const std::filesystem::path &path);

    static std::optional<std::string> Read(const std::filesystem::path &path);
    static std::vector<unsigned char>
    ReadBytes(const std::filesystem::path &path);
    static std::vector<std::string>
    FilesInPath(const std::filesystem::path &path, bool recursive = true);

    static std::istream &SafeGetLine(std::istream &is, std::string &t);

  private:
    std::vector<std::string> searchPaths;
};

} // namespace Worlds
