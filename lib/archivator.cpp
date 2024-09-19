#include <fstream>
#include <filesystem>
#include <iostream>
#include "archivator.h"
#include "hamming.h"

void Archive::AddByteInBlock(std::vector<bool>& arr, uint8_t byte) {
        for (int i = 0; i < 8; ++i) {
            arr.push_back(byte & 128);
            byte <<= 1;
        }
    }

void Archive::SetCodingInfo() {
        std::vector<bool> test_block(bytes_per_block_ * 8);
        std::vector<bool> coded_test_block = ToCode(test_block);
        added_bits_in_coded_block = (8 - coded_test_block.size() % 8) % 8;
        bytes_per_coded_block_ = (coded_test_block.size() + added_bits_in_coded_block) / 8;
    }

void Archive::WriteFileNameInHeader(std::vector<bool>& header, FileInfo& file_info) {
        int length = file_info.name.size();

        for (int i = 0; i < kSizeOfFileNameInfo - length; ++i) {
            AddByteInBlock(header, '\0');
        }

        for (int i = 0; i < length; ++i) {
            AddByteInBlock(header, static_cast<uint8_t>(file_info.name[i]));
        }
    }

size_t Archive::GetSizeOfFile(std::ifstream& file) {
        file.seekg(0, file.end);
        size_t size_of_file = file.tellg();
        file.seekg(0, file.beg);
        return size_of_file;
    }

void Archive::WriteFileCodedSizeInHeader(std::vector<bool>& header, FileInfo& file_info) {
        std::vector<uint8_t> bytes;
        bytes.resize(8);
        for (int i = 0; i < 8; ++i) {
            bytes[7 - i] = (file_info.size_of_coded_file >> (i * 8)) & 255;
        }

        for (int i = 0; i < 8; ++i) {
            AddByteInBlock(header, bytes[i]);
        }
    }

void Archive::WriteFileAddedBytesCountInHeader(std::vector<bool>& header, FileInfo& file_info) {
        std::vector<uint8_t> bytes;
        bytes.resize(8);
        for (int i = 0; i < 8; ++i) {
            bytes[7 - i] = (file_info.added_bytes >> (i * 8)) & 255;
        }

        for (int i = 0; i < 8; ++i) {
            AddByteInBlock(header, bytes[i]);
        }
    }

void Archive::SetFileInfo(FileInfo& file_info, const size_t size_of_file, const std::string& file_name) {
        file_info.size_of_file = size_of_file;
        file_info.added_bytes = (bytes_per_block_ - file_info.size_of_file % bytes_per_block_) % bytes_per_block_;
        file_info.blocks_count = (file_info.size_of_file + file_info.added_bytes) / bytes_per_block_;
        file_info.size_of_coded_file = file_info.blocks_count * bytes_per_coded_block_;
        file_info.name = file_name;
    }

void Archive::WriteBlock(std::vector<bool>& block, std::ofstream& output) {
        uint8_t bytes_block[block.size() / 8];

        for (int i = 0; i < block.size(); i += 8) {
            uint8_t byte = 0;
            for (int j = i; j < i + 7; ++j) {
                byte += block[j];
                byte <<= 1;
            }
            byte += block[i + 7];
            bytes_block[i / 8] = byte;
            output << byte;
        }
        //output.write(reinterpret_cast<char*>(bytes_block), block.size() / 8);
    }

void Archive::WriteFile(FileInfo& file_info, std::ofstream& output, std::ifstream& input) {
        for (size_t i = 0; i < file_info.size_of_file; i += bytes_per_block_) {
            std::vector<bool> block;
            block.reserve(bytes_per_block_);

            for (size_t j = i; j < i + bytes_per_block_; ++j) {
                if (j >= file_info.size_of_file) {
                    AddByteInBlock(block, 0);
                    continue;
                }
                uint8_t byte;
                input.read(reinterpret_cast<char *>(&byte), 1);
                AddByteInBlock(block, byte);
            }
            std::vector<bool> coded_block = ToCode(block);
            for (int j = 0; j < added_bits_in_coded_block; ++j) {
                coded_block.push_back(false);
            }

            WriteBlock(coded_block, output);
        }
    }

void Archive::WriteHeader(FileInfo& file_info, std::ofstream& output) {
        std::vector<bool> header;
        header.reserve(kSizeOfHeader * 8);

        WriteFileNameInHeader(header, file_info);
        WriteFileCodedSizeInHeader(header, file_info);
        WriteFileAddedBytesCountInHeader(header, file_info);

        std::vector<bool> coded_header = ToCode(header);
        for (int i = 0; i < kSizeOfAddedBitsInHeader; ++i) {
            coded_header.push_back(false);
        }
        std::vector<bool> cope = coded_header;
        for (int i = 0; i < kSizeOfAddedBitsInHeader; ++i) {
            coded_header.pop_back();
        }

        WriteBlock(coded_header, output);
    }

void Archive::AppendFile(std::ofstream& output, std::ifstream& input, const std::string& file_name) {
        FileInfo file_info;
        SetFileInfo(file_info, GetSizeOfFile(input), file_name);
        WriteHeader(file_info, output);
        WriteFile(file_info, output, input);
    }

void Archive::CopyFile(std::ifstream& source, std::ofstream& out) {
        size_t size_of_source = GetSizeOfFile(source);
        for (size_t i = 0; i < size_of_source; ++i) {
            uint8_t byte;
            source.read(reinterpret_cast<char *>(&byte), 1);
            out << byte;
        }
    }

Archive::Archive(size_t bytes_per_block, const std::string& output_name): bytes_per_block_(bytes_per_block), output_name_(output_name) {
        SetCodingInfo();
    }

void Archive::Create(const std::vector<std::string>& file_names) {
        std::ofstream output(output_name_, std::ios::out | std::ios::binary);
        for (int i = 0; i < file_names.size(); ++i) {
            std::ifstream input(file_names[i], std::ifstream::binary);
            AppendFile(output, input, file_names[i]);
        }
    }

void Archive::AppendFile(const std::string& input_name) {
        std::ifstream input(input_name, std::ifstream::binary);
        std::ofstream output(output_name_, std::ios::out | std::ios::app | std::ios::binary);
        AppendFile(output, input, input_name);
    }

void Archive::Concatenate(const std::string& name1, const std::string& name2) {
        std::ifstream input1(name1, std::ifstream::binary);
        std::ifstream input2(name2, std::ifstream::binary);
        std::ofstream output(output_name_, std::ios::out | std::ios::binary);
        CopyFile(input1, output);
        CopyFile(input2, output);
    }

uint8_t Archive::FixBlockInArchive(std::vector<bool>& block, std::ofstream& source, size_t pos) {
        if (!IsDamaged(block)) {
            return ZERO_ERROR;
        }

        uint8_t res = FixErrors(block);
        if (res == MULTIPLE_ERRORS) {
            return res;
        }
        for (int j = 0; j < block.size(); j += 8) {
            uint8_t correct_byte = 0;
            for (int k = j; k < j + 7; ++k) {
                correct_byte += block[k];
                correct_byte <<= 1;
            }
            correct_byte += block[j + 7];
            source.seekp(pos + j / 8, source.beg);
            source.put(correct_byte);
        }

        return res;
    }

std::string Archive::GetNameFromHeader(std::vector<bool>& header) {
        std::string name;
        int start = 0;
        int end = kSizeOfFileNameInfo * 8;
        for (int i = start; i < end; i += 8) {
            uint8_t byte = 0;
            for (int j = i; j < i + 7; ++j) {
                byte += header[j];
                byte <<= 1;
            }
            byte += header[i + 7];
            if (byte != 0) {
                name += byte;
            }
        }
        return name;
    }

size_t Archive::GetSizeOfCodedFileFromHeader(std::vector<bool>& header) {
        size_t size_of_coded_file = 0;
        int start = kSizeOfFileNameInfo * 8;
        int end = kSizeOfFileNameInfo * 8 + kSizeOfCodedFileInfo * 8;
        for (int i = start; i < end; i += 8) {
            uint8_t byte = 0;
            for (int j = i; j < i + 7; ++j) {
                byte += header[j];
                byte <<= 1;
            }
            byte += header[i + 7];
            size_of_coded_file += byte;
            if (i + 8 < end) {
                size_of_coded_file <<= 8;
            }
        }
        return size_of_coded_file;
    }

size_t Archive::GetAddedBytesCountFromHeader(std::vector<bool>& header) {
        size_t added_bytes_count = 0;
        int start = kSizeOfFileNameInfo * 8 + kSizeOfCodedFileInfo * 8;
        int end = kSizeOfFileNameInfo * 8 + kSizeOfCodedFileInfo * 8 + kSizeOfAddedBytesInfo * 8;
        for (int i = start; i < end; i += 8) {
            uint8_t byte = 0;
            for (int j = i; j < i + 7; ++j) {
                byte += header[j];
                byte <<= 1;
            }
            byte += header[i + 7];
            added_bytes_count += byte;
            if (i + 8 < kSizeOfFileNameInfo * 8 + kSizeOfCodedFileInfo * 8 * 2) {
                added_bytes_count <<= 8;
            }
        }
        return added_bytes_count;
    }

std::vector<bool> Archive::GetCodedHeader(std::ifstream& reader) {
        std::vector<bool> coded_header_block;
        coded_header_block.reserve(kSizeOfCodedHeader * 8);

        for (int i = 0; i < kSizeOfCodedHeader; ++i) {
            uint8_t byte;
            reader.read(reinterpret_cast<char*>(&byte), 1);
            AddByteInBlock(coded_header_block, byte);
        }

        for (int i = 0; i < kSizeOfAddedBitsInHeader; ++i) {
            coded_header_block.pop_back();
        }
        return coded_header_block;
    }

bool Archive::IsInList(const std::vector<std::string>& names, const std::string& name) {
        for (int i = 0; i < names.size(); ++i) {
            if (names[i] == name) {
                return true;
            }
        }
        return false;
    }

void Archive::Rewrite(std::ifstream& read, std::ofstream& source, size_t size, size_t pos, size_t size_of_coded_file) {
        pos -= kSizeOfCodedHeader;
        size_t delta = kSizeOfCodedHeader + size_of_coded_file;
        size_t final_size = size - kSizeOfCodedHeader - size_of_coded_file;
        size_t pos_read = pos + delta;
        read.seekg(pos_read, read.beg);

        size_t copy_bytes_count = final_size - pos;
        for (size_t i = 0; i < copy_bytes_count; ++i) {
            uint8_t byte;
            read.read(reinterpret_cast<char*>(&byte), 1);
            source.seekp(pos, source.beg);
            source.put(byte);
            ++pos;
        }
        source.close();
        read.close();
        std::filesystem::resize_file(output_name_, final_size);
    }

uint8_t Archive::WriteOriginalFile(std::ofstream& extracted_file, std::ifstream& reader, std::ofstream& source, size_t blocks_count, size_t& pos, size_t added_bytes_count) {
        for (size_t i = 0; i < blocks_count; ++i) {
            std::vector<bool> coded_block = ReadBlock(reader);

            uint8_t res = FixBlockInArchive(coded_block, source, pos);
            if (res == MULTIPLE_ERRORS) {
                return res;
            }

            std::vector<bool> block = ToDecode(coded_block);
            if (i == blocks_count - 1) {
                for (size_t j = 0; j < added_bytes_count * 8; ++j) {
                    block.pop_back(); // этой информации на самом деле не существовало
                }
            }

            WriteBlock(block, extracted_file);
            pos += bytes_per_coded_block_;
        }
        return ZERO_ERROR;
    }

std::vector<bool> Archive::ReadBlock(std::ifstream& reader) {
        std::vector<bool> coded_block;
        coded_block.reserve(bytes_per_coded_block_ * 8);

        for (size_t j = 0; j < bytes_per_coded_block_; ++j) {
            uint8_t byte_;
            reader.read(reinterpret_cast<char*>(&byte_), 1);
            AddByteInBlock(coded_block, byte_);
        }

        // уберем лишние биты
        for (int j = 0; j < added_bits_in_coded_block; ++j) {
            coded_block.pop_back();
        }

        return coded_block;
    }

std::vector<std::string> Archive::GetListOfFiles() {
        std::ifstream read(output_name_, std::ios::binary);
        std::ofstream source(output_name_, std::ios::in | std::ios::binary);
        size_t size = GetSizeOfFile(read);
        std::vector<std::string> names;
        size_t pos = 0;

        while (pos < size) {
            std::vector<bool> coded_header_block = GetCodedHeader(read);
            if (FixBlockInArchive(coded_header_block, source, pos) == MULTIPLE_ERRORS) {
                std::cerr << "Archive is damaged";
                return names;
            }

            pos += kSizeOfCodedHeader;
            std::vector<bool> header = ToDecode(coded_header_block);
            std::string file_name = GetNameFromHeader(header);
            names.push_back(file_name);
            pos += GetSizeOfCodedFileFromHeader(header);
            read.seekg(pos, read.beg);
        }
        return names;
    }

void Archive::Delete(const std::string& name) {
        std::ifstream read(output_name_, std::ios::binary);
        std::ofstream source(output_name_, std::ios::in | std::ios::binary);
        size_t size = GetSizeOfFile(read);
        size_t pos = 0;

        while (pos < size) {
            std::vector<bool> coded_header_block = GetCodedHeader(read);
            if (FixBlockInArchive(coded_header_block, source, pos) == MULTIPLE_ERRORS) {
                std::cerr << "Archive is damaged";
                return;
            }

            pos += kSizeOfCodedHeader;
            std::vector<bool> header = ToDecode(coded_header_block);
            std::string file_name = GetNameFromHeader(header);
            size_t size_of_coded_file = GetSizeOfCodedFileFromHeader(header);

            if (file_name == name) {
                Rewrite(read, source, size, pos, size_of_coded_file);
                return;
            } else {
                pos += size_of_coded_file;
                read.seekg(pos, read.beg);
            }
        }


    }

void Archive::ExtractFiles(const std::vector<std::string>& file_names) {
        std::ifstream read(output_name_, std::ios::binary);
        std::ofstream source(output_name_, std::ios::in | std::ios::binary);
        size_t size = GetSizeOfFile(read);
        size_t pos = 0;

        while (pos < size) {
            std::vector<bool> coded_header_block = GetCodedHeader(read);
            if (FixBlockInArchive(coded_header_block, source, pos) == MULTIPLE_ERRORS) {
                std::cerr << "Archive is damaged";
                return;
            }

            pos += kSizeOfCodedHeader;
            std::vector<bool> header = ToDecode(coded_header_block);
            std::string file_name = GetNameFromHeader(header);
            size_t size_of_coded_file = GetSizeOfCodedFileFromHeader(header);
            if (!IsInList(file_names, file_name)) {
                pos += size_of_coded_file;
                read.seekg(pos, read.beg);
                continue;
            }

            size_t added_bytes_count = GetAddedBytesCountFromHeader(header);
            std::ofstream extracted_file(file_name, std::ios::binary); // сюда буду записывать файлик, который сейчас буду декодировать
            size_t blocks_count = size_of_coded_file / bytes_per_coded_block_;

            if (WriteOriginalFile(extracted_file, read, source, blocks_count, pos, added_bytes_count) == MULTIPLE_ERRORS) {
                std::cerr << "Archive is damaged";
                return;
            }
        }
    }
