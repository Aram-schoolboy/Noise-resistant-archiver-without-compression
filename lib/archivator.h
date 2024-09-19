#include <cstdint>
#include <string>
#include <vector>

struct FileInfo {
    size_t size_of_file = -1;
    size_t size_of_coded_file = -1;
    size_t blocks_count = -1;
    size_t added_bytes = -1;
    std::string name;
};

class Archive {
private:
    void AddByteInBlock(std::vector<bool>& arr, uint8_t byte);

    void SetCodingInfo();

    void WriteFileNameInHeader(std::vector<bool>& header, FileInfo& file_info);

    size_t GetSizeOfFile(std::ifstream& file);

    void WriteFileCodedSizeInHeader(std::vector<bool>& header, FileInfo& file_info);

    void WriteFileAddedBytesCountInHeader(std::vector<bool>& header, FileInfo& file_info);

    void SetFileInfo(FileInfo& file_info, const size_t size_of_file, const std::string& file_name);

    void WriteBlock(std::vector<bool>& block, std::ofstream& output);

    void WriteFile(FileInfo& file_info, std::ofstream& output, std::ifstream& input);

    void WriteHeader(FileInfo& file_info, std::ofstream& output);

    void AppendFile(std::ofstream& output, std::ifstream& input, const std::string& file_name);

    void CopyFile(std::ifstream& source, std::ofstream& out);

    uint8_t FixBlockInArchive(std::vector<bool>& block, std::ofstream& source, size_t pos);

    std::string GetNameFromHeader(std::vector<bool>& header);

    size_t GetSizeOfCodedFileFromHeader(std::vector<bool>& header);

    size_t GetAddedBytesCountFromHeader(std::vector<bool>& header);

    std::vector<bool> GetCodedHeader(std::ifstream& reader);

    bool IsInList(const std::vector<std::string>& names, const std::string& name);

    void Rewrite(std::ifstream& read, std::ofstream& source, size_t size, size_t pos, size_t size_of_coded_file);

    uint8_t WriteOriginalFile(std::ofstream& extracted_file, std::ifstream& reader, std::ofstream& source, size_t blocks_count, size_t& pos, size_t added_bytes_count);

    std::vector<bool> ReadBlock(std::ifstream& reader);

    const std::string output_name_;
    size_t bytes_per_block_;
    size_t bytes_per_coded_block_;
    uint8_t added_bits_in_coded_block;
    // если я знаю bytes_per_block, то я знаю сколько надо битов докидывать каждому
    static const uint16_t kSizeOfHeader = 80; // in bytes
    static const uint16_t kSizeOfAddedBitsInHeader = 6; // in bits
    static const uint16_t kSizeOfCodedHeader = 82; // in bytes
    static const uint16_t kSizeOfFileNameInfo = 64; // in bytes
    static const uint16_t kSizeOfCodedFileInfo = 8; // in bytes
    static const uint16_t kSizeOfAddedBytesInfo = 8; // in bytes ALL IN BYTES

public:
    Archive(size_t bytes_per_block, const std::string& output_name);

    void Create(const std::vector<std::string>& file_names);

    void AppendFile(const std::string& input_name);

    void Concatenate(const std::string& name1, const std::string& name2);

    std::vector<std::string> GetListOfFiles();

    void Delete(const std::string& name);

    void ExtractFiles(const std::vector<std::string>& file_names);
};