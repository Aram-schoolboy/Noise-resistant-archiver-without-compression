#include "lib/archivator.h"
#include "lib/ArgParser.h"

struct Options {
    bool create = false;
    std::vector<std::string> file_names;
    bool list = false;
    bool extract = false;
    bool append = false;
    bool to_delete = false;
    bool concatenate = false;
    std::string arch_name;
    int bytes_per_block = -1;
};

int main(int argc, char** argv) {
    Options options;
    ArgumentParser::ArgParser parser("Program");
    parser.AddFlag('c', "create", "create a new archive").StoreValue(options.create);
    parser.AddFlag('l', "list", "get file names in archive").StoreValue(options.list);
    parser.AddFlag('x', "extract", "extract files from archive").StoreValue(options.extract);
    parser.AddFlag('a', "append", "append file in archive").StoreValue(options.append);
    parser.AddFlag('d', "delete", "delete file from archive").StoreValue(options.to_delete);
    parser.AddFlag('A', "concatenate", "merge two archives").StoreValue(options.concatenate);
    parser.AddIntArgument('b', "block", "bytes per hamming block").StoreValue(options.bytes_per_block);
    parser.AddStringArgument('f', "file", "archive name").StoreValue(options.arch_name);
    parser.AddStringArgument('i', "items", "file names").MultiValue().StoreValues(options.file_names).Positional();
    parser.AddHelp('h', "help", "Archivator, using hamming code");

    if (!parser.Parse(argc, argv)) {
        std::cerr << "Invalid arguments\n";
    }
    if (options.arch_name.empty()) {
        std::cerr << "No archive name" << '\n';
        return 1;
    }
    if (options.bytes_per_block <= 0) {
        std::cerr << "Invalid bytes per block" << '\n';
        return 1;
    }

    Archive archive(options.bytes_per_block, options.arch_name);

    if (options.create) {
        std::cout << "Make archive with name " << options.arch_name;
        std::cout << "Add these files:\n";
        for (int i = 0; i < options.file_names.size(); ++i) {
            std::cout << options.file_names[i] << '\n';
        }
        archive.Create(options.file_names);
    } else if (options.list) {
        std::cout << "Get list of files\n";
        std::vector<std::string> names = archive.GetListOfFiles();
        std::cout << names.size() << " files:\n";
        for (int i = 0; i < names.size(); ++i) {
            std::cout << names[i] << '\n';
        }
    } else if (options.extract) {
        std::cout << "Get those files:\n";
        if (options.file_names.empty()) {
            options.file_names = archive.GetListOfFiles();
        }
        archive.ExtractFiles(options.file_names);
        for (int i = 0; i < options.file_names.size(); ++i) {
            std::cout << options.file_names[i] << '\n';
        }

    } else if (options.append) {
        std::cout << "Append these files:\n";
        for (int i = 0; i < options.file_names.size(); ++i) {
            archive.AppendFile(options.file_names[i]);
            std::cout << options.file_names[i] << '\n';
        }
    } else if (options.to_delete) {
        std::cout << "Delete these files:\n";
        for (int i = 0; i < options.file_names.size(); ++i) {
            archive.Delete(options.file_names[i]);
            std::cout << options.file_names[i] << '\n';
        }
    } else if (options.concatenate) {
        std::cout << "Merge these archives:\n";
        if ( options.file_names.size() != 2) {
            std::cerr << "Can concatenate only 2 archives";
        }
        archive.Concatenate(options.file_names[0], options.file_names[1]);
        std::cout << options.file_names[0] << '\n';
        std::cout << options.file_names[1] << '\n';
    }
    return 0;
}