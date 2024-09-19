#pragma once
#include <vector>
#include <cstdint>
#include <iostream>

namespace ArgumentParser {

class ArgParser {
private:
    std::string name_;

    class StringArgument {
    public:
        StringArgument(char short_name, const std::string& long_name, const std::string& description);

        StringArgument& Default(const std::string& default_value);

        StringArgument& StoreValue(std::string& stored_value);

        StringArgument& StoreValues(std::vector<std::string>& stored_values);

        StringArgument& MultiValue(const uint16_t min_args_value = 0);

        StringArgument& Positional();

        std::vector<std::string> values;
        std::string value = "";
        std::string long_name = "";
        char short_name = ' ';
        std::string description = "";
        bool is_multi_value = false;
        bool is_positional = false;
        bool has_default_value = false;
        std::string default_value;
        std::string* stored_value = nullptr;
        std::vector<std::string>* stored_values = nullptr;
        int new_value_index = 0;
        uint16_t min_args_value = 0;
    };

    class IntArgument {
    public:
        IntArgument(char short_name, const std::string& long_name, const std::string& description);

        IntArgument& Default(const int default_value);

        IntArgument& StoreValue(int& stored_value);

        IntArgument& StoreValues(std::vector<int>& stored_values);

        IntArgument& MultiValue(const uint16_t min_args_value = 0);

        IntArgument& Positional();

        std::vector<int> values;
        int value;
        std::string long_name = "";
        char short_name = ' ';
        std::string description = "";
        bool is_multi_value = false;
        bool is_positional = false;
        bool has_default_value = false;
        int default_value;
        int* stored_value = nullptr;
        std::vector<int>* stored_values = nullptr;
        int new_value_index = 0;
        uint16_t min_args_value = 0;
    };

    class Flag {
    public:
        Flag(char short_name, const std::string& long_name, const std::string& description);

        Flag& Default(const bool default_value);

        Flag& StoreValue(bool& stored_value);

        Flag& StoreValues(std::vector<bool>& stored_values);

        Flag& MultiValue(const uint16_t min_args_value = 0);

        Flag& Positional();

        std::vector<bool> values;
        bool value;
        std::string long_name = "";
        char short_name = ' ';
        std::string description = "";
        bool is_multi_value = false;
        bool is_positional = false;
        bool has_default_value = false;
        bool default_value;
        bool* stored_value = nullptr;
        std::vector<bool>* stored_values = nullptr;
        int new_value_index = 0;
        uint16_t min_args_value = 0;
    };

    class Helper {
    public:
        Helper() = default;
        Helper(const char short_name, const std::string& long_name, const std::string& description);

        std::string long_name = "";
        char short_name = ' ';
        std::string description = "";
        bool is_active = false;
    };

    std::vector<StringArgument> string_args_;
    std::vector<IntArgument> int_args_;
    std::vector<Flag> flags_;
    Helper helper_;
public:
    ArgParser(const std::string& name);

    StringArgument& AddStringArgument(const char short_name = ' ', const std::string& long_name = "", const std::string& description = "");

    StringArgument& AddStringArgument(const std::string& long_name = "", const std::string& description = "");

    IntArgument& AddIntArgument(const char short_name = ' ', const std::string& long_name = "", const std::string& description = "");

    IntArgument& AddIntArgument(const std::string& long_name = "", const std::string& description = "");

    Flag& AddFlag(const char short_name = ' ', const std::string& long_name = "", const std::string& description = "");

    Flag& AddFlag(const std::string& long_name = "", const std::string& description = "");

    void AddHelp(const char short_name = ' ', const std::string& long_name = "", const std::string& description = "");

    void AddHelp(const std::string& long_name = "", const std::string& description = "");

    bool Help() const;

    std::string HelpDescription();

    std::string MakeStringArgInfo(StringArgument& string_arg) const;

    std::string MakeIntArgInfo(IntArgument& int_arg) const;

    std::string MakeFlagInfo(Flag& flag) const;

    std::string MakeHelperInfo(Helper& string_arg) const;

    bool GetFlag(const char flag_name) const;

    bool GetFlag(const std::string& flag_name) const;

    bool GetFlag(const std::string& flag_name, const uint32_t index) const;

    bool GetFlag(const char flag_name, const uint32_t index) const;

    std::string GetStringValue(const char string_arg_name) const;

    std::string GetStringValue(const std::string& string_arg_name) const;

    std::string GetStringValue(const std::string& string_arg_name, const uint32_t index) const;

    std::string GetStringValue(const char string_arg_name, const uint32_t index) const;

    int GetIntValue(const char int_arg_value) const;

    int GetIntValue(const std::string& int_arg_value) const;

    int GetIntValue(const std::string& int_arg_value, const uint32_t index) const;

    int GetIntValue(const char int_arg_value, const uint32_t index) const;

    StringArgument* FindPositionalStringArgument();

    IntArgument* FindPositionalIntArgument();

    Flag* FindPositionalFlag();

    StringArgument* FindStringArgumentByShortName(const char name);

    IntArgument* FindIntArgumentByShortName(const char name);

    Flag* FindFlagByShortName(const char name);

    StringArgument* FindStringArgumentByLongName(const std::string& name);

    IntArgument* FindIntArgumentByLongName(const std::string& name);

    Flag* FindFlagByLongName(const std::string& name);

    void SetStringValue(StringArgument& string_arg, const std::string& value);

    void SetIntValue(IntArgument& int_arg, const int value);

    void SetFlagValue(Flag& flag, const bool value);

    bool IsValidNumber(const char* string_number);

    bool ArgumentsAreInitialized();

    bool Parse(std::vector<std::string> args);

    bool Parse(int argc, char** argv);
};

}