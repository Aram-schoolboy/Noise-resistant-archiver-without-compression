#include "ArgParser.h"
#include <limits>

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& name) {
    name_ = name;
}

ArgParser::StringArgument::StringArgument(const char short_name, const std::string& long_name, const std::string& description) {
    this->short_name = short_name;
    this->long_name = long_name;
    this->description = description;
}

ArgParser::StringArgument& ArgParser::AddStringArgument(const char short_name, const std::string& long_name, const std::string& description) {
    string_args_.emplace_back(short_name, long_name, description);
    return string_args_.back();
}

ArgParser::StringArgument& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
    string_args_.emplace_back(' ', long_name, description);
    return string_args_.back();
}

ArgParser::StringArgument& ArgParser::StringArgument::StoreValue(std::string& stored_value) {
    this->stored_value = &stored_value;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::StoreValues(std::vector<std::string>& stored_values) {
    this->stored_values = &stored_values;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::MultiValue(const uint16_t min_args_value) {
    is_multi_value = true;
    this->min_args_value = min_args_value;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::Default(const std::string& default_value) {
    has_default_value = true;
    this->default_value = default_value;
    if (!is_multi_value) {
        if (stored_value == nullptr) {
            value = default_value;
        } else {
            *stored_value = default_value;
        }
    } else {
        if (stored_values == nullptr) {
            for (int i = 0; i < min_args_value; ++i) {
                values.push_back(default_value);
            }
        } else {
            for (int i = 0; i < min_args_value; ++i) {
                (*stored_values).push_back(default_value);
            }
        }
    }
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::Positional() {
    is_positional = true;
    return *this;
}

ArgParser::IntArgument::IntArgument(const char short_name, const std::string& long_name, const std::string& description) {
    this->short_name = short_name;
    this->long_name = long_name;
    this->description = description;
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const char short_name, const std::string& long_name, const std::string& description) {
    int_args_.emplace_back(short_name, long_name, description);
    return int_args_.back();
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
    int_args_.emplace_back(' ', long_name, description);
    return int_args_.back();
}

ArgParser::IntArgument& ArgParser::IntArgument::StoreValue(int& stored_value) {
    this->stored_value = &stored_value;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::StoreValues(std::vector<int>& stored_values) {
    this->stored_values = &stored_values;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::MultiValue(const uint16_t min_args_value) {
    is_multi_value = true;
    this->min_args_value = min_args_value;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::Default(const int default_value) {
    has_default_value = true;
    this->default_value = default_value;
    if (!is_multi_value) {
        if (stored_value == nullptr) {
            value = default_value;
        } else {
            *stored_value = default_value;
        }
    } else {
        if (stored_values == nullptr) {
            for (int i = 0; i < min_args_value; ++i) {
                values.push_back(default_value);
            }
        } else {
            for (int i = 0; i < min_args_value; ++i) {
                (*stored_values).push_back(default_value);
            }
        }
    }
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::Positional() {
    is_positional = true;
    return *this;
}

ArgParser::Flag::Flag(const char short_name, const std::string& long_name, const std::string& description) {
    this->short_name = short_name;
    this->long_name = long_name;
    this->description = description;
}

ArgParser::Flag& ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
    flags_.emplace_back(short_name, long_name, description);
    return flags_.back();
}

ArgParser::Flag& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
    flags_.emplace_back(' ', long_name, description);
    return flags_.back();
}

ArgParser::Flag& ArgParser::Flag::StoreValue(bool& stored_value) {
    this->stored_value = &stored_value;
    return *this;
}

ArgParser::Flag& ArgParser::Flag::StoreValues(std::vector<bool>& stored_values) {
    this->stored_values = &stored_values;
    return *this;
}

ArgParser::Flag& ArgParser::Flag::MultiValue(const uint16_t min_args_value) {
    is_multi_value = true;
    this->min_args_value = min_args_value;
    return *this;
}

ArgParser::Flag& ArgParser::Flag::Default(const bool default_value) {
    has_default_value = true;
    this->default_value = default_value;
    if (!is_multi_value) {
        if (stored_value == nullptr) {
            value = default_value;
        } else {
            *stored_value = default_value;
        }
    } else {
        if (stored_values == nullptr) {
            for (int i = 0; i < min_args_value; ++i) {
                values.push_back(default_value);
            }
        } else {
            for (int i = 0; i < min_args_value; ++i) {
                (*stored_values).push_back(default_value);
            }
        }
    }
    return *this;
}

ArgParser::Flag& ArgParser::Flag::Positional() {
    is_positional = true;
    return *this;
}


ArgParser::Helper::Helper(const char short_name, const std::string& long_name, const std::string& description) {
    this->short_name = short_name;
    this->long_name = long_name;
    this->description = description;
}

void ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
    helper_ = Helper(short_name, long_name, description);
}

void ArgParser::AddHelp(const std::string& long_name, const std::string& description) {
    helper_ = Helper(' ', long_name, description);
}

bool ArgParser::Help() const {
    return helper_.is_active;
}

std::string ArgParser::MakeStringArgInfo(StringArgument& string_arg) const {
    std::string arg_info;
    if (string_arg.short_name != ' ' && (!string_arg.long_name.empty())) {
        arg_info = "-";
        arg_info += string_arg.short_name;
        arg_info += "\t--" + string_arg.long_name + "=<string>\t";
    } else if (string_arg.short_name != ' ') {
        arg_info = "-";
        arg_info += string_arg.short_name;
        arg_info += "=<string>\t\t";
    } else {
        arg_info = "\t--";
        arg_info += string_arg.long_name + "=<string>\t";
    }
    arg_info += string_arg.description + "\t";

    if (string_arg.is_multi_value) {
        arg_info += "[repeated, min args = " + std::to_string(string_arg.min_args_value) + "] ";
    }
    if (string_arg.is_positional) {
        arg_info += "[positional] ";
    }
    if (string_arg.has_default_value) {
        arg_info += "[default = " + string_arg.default_value + "] ";
    }
    if (string_arg.stored_value != nullptr || string_arg.stored_values != nullptr) {
        arg_info += "[stored] ";
    }
    arg_info += "\n";

    return arg_info;
}

std::string ArgParser::MakeIntArgInfo(IntArgument& int_arg) const {
    std::string arg_info;
    if (int_arg.short_name != ' ' && (!int_arg.long_name.empty())) {
        arg_info = "-";
        arg_info += int_arg.short_name;
        arg_info += "\t--" + int_arg.long_name + "=<integer>\t";
    } else if (int_arg.short_name != ' ') {
        arg_info = "-";
        arg_info += int_arg.short_name;
        arg_info += "=<integer>\t\t";
    } else {
        arg_info = "\t--";
        arg_info += int_arg.long_name + "=<integer>\t";
    }
    arg_info += int_arg.description + "\t";

    if (int_arg.is_multi_value) {
        arg_info += "[repeated, min args = " + std::to_string(int_arg.min_args_value) + "] ";
    }
    if (int_arg.is_positional) {
        arg_info += "[positional] ";
    }
    if (int_arg.has_default_value) {
        arg_info += "[default = " + std::to_string(int_arg.default_value) + "] ";
    }
    if (int_arg.stored_value != nullptr || int_arg.stored_values != nullptr) {
        arg_info += "[stored] ";
    }
    arg_info += "\n";

    return arg_info;
}

std::string ArgParser::MakeFlagInfo(Flag& flag) const {
    std::string arg_info;
    if (flag.short_name != ' ' && (!flag.long_name.empty())) {
        arg_info = "-";
        arg_info += flag.short_name;
        arg_info += "\t--" + flag.long_name + "=<bool>\t";
    } else if (flag.short_name != ' ') {
        arg_info = "-";
        arg_info += flag.short_name;
        arg_info += " <bool>\t\t";
    } else {
        arg_info = "\t--";
        arg_info += flag.long_name + "=<bool>\t";
    }
    arg_info += flag.description + "\t";

    if (flag.is_multi_value) {
        arg_info += "[repeated, min args = " + std::to_string(flag.min_args_value) + "] ";
    }
    if (flag.is_positional) {
        arg_info += "[positional] ";
    }
    if (flag.has_default_value) {
        arg_info += "[default = ";
        arg_info += flag.default_value ? "true" : "false";
        arg_info += "] ";
    }
    if (flag.stored_value != nullptr || flag.stored_values != nullptr) {
        arg_info += "[stored] ";
    }
    arg_info += "\n";

    return arg_info;
}

std::string ArgParser::MakeHelperInfo(Helper& helper) const {
    std::string arg_info;
    if (helper.short_name != ' ' && (!helper.long_name.empty())) {
        arg_info = "-";
        arg_info += helper.short_name;
        arg_info += "\t--" + helper.long_name + "\t";
    } else if (helper.short_name != ' ') {
        arg_info = "-";
        arg_info += helper.short_name;
        arg_info += "\t\t";
    } else {
        arg_info = "\t";
        arg_info += "--" + helper.long_name + "\t";
    }
    arg_info += "Display this help and exit\n";

    return arg_info;
}

std::string ArgParser::HelpDescription() {
    std::string program_description;
    program_description += name_ + "\n";
    program_description += helper_.description + "\n";

    for (int i = 0; i < string_args_.size(); ++i) {
        program_description += MakeStringArgInfo(string_args_[i]);
    }
    for (int i = 0; i < int_args_.size(); ++i) {
        program_description += MakeIntArgInfo(int_args_[i]);
    }
    for (int i = 0; i < flags_.size(); ++i) {
        program_description += MakeFlagInfo(flags_[i]);
    }

    program_description += MakeHelperInfo(helper_);
    return program_description;
}

bool ArgParser::GetFlag(const char flag_name) const {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].short_name == flag_name) {
            return (flags_[i].stored_value == nullptr) ? flags_[i].value : *(flags_[i].stored_value);
        }
    }
}

bool ArgParser::GetFlag(const std::string& flag_name) const {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].long_name == flag_name) {
            return (flags_[i].stored_value == nullptr) ? flags_[i].value : *(flags_[i].stored_value);
        }
    }
}

bool ArgParser::GetFlag(const std::string& flag_name, const uint32_t index) const {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].long_name == flag_name) {
            return (flags_[i].stored_values == nullptr) ? flags_[i].values[index] : (*(flags_[i].stored_values))[index];
        }
    }
}

bool ArgParser::GetFlag(const char flag_name, const uint32_t index) const {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].short_name == flag_name) {
            return (flags_[i].stored_values == nullptr) ? flags_[i].values[index] : (*(flags_[i].stored_values))[index];
        }
    }
}

std::string ArgParser::GetStringValue(const char string_arg_name) const {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].short_name == string_arg_name) {
            return (string_args_[i].stored_value == nullptr) ? string_args_[i].value : *(string_args_[i].stored_value);
        }
    }
}

std::string ArgParser::GetStringValue(const std::string& string_arg_name) const {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].long_name == string_arg_name) {
            return (string_args_[i].stored_value == nullptr) ? string_args_[i].value : *(string_args_[i].stored_value);
        }
    }
}

std::string ArgParser::GetStringValue(const std::string& string_arg_name, const uint32_t index) const {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].long_name == string_arg_name) {
            return (string_args_[i].stored_values == nullptr) ? string_args_[i].values[index] : (*(string_args_[i].stored_values))[index];
        }
    }
}

std::string ArgParser::GetStringValue(const char string_arg_name, const uint32_t index) const {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].short_name == string_arg_name) {
            return (string_args_[i].stored_values == nullptr) ? string_args_[i].values[index] : (*(string_args_[i].stored_values))[index];
        }
    }
}

int ArgParser::GetIntValue(const char int_arg_name) const {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].short_name == int_arg_name) {
            return (int_args_[i].stored_value == nullptr) ? int_args_[i].value : *(int_args_[i].stored_value);
        }
    }
}

int ArgParser::GetIntValue(const std::string& int_arg_name) const {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].long_name == int_arg_name) {
            return (int_args_[i].stored_value == nullptr) ? int_args_[i].value : *(int_args_[i].stored_value);
        }
    }
}

int ArgParser::GetIntValue(const std::string& int_arg_name, const uint32_t index) const {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].long_name == int_arg_name) {
            return (int_args_[i].stored_values == nullptr) ? int_args_[i].values[index] : (*(int_args_[i].stored_values))[index];
        }
    }
}

int ArgParser::GetIntValue(const char int_arg_name, const uint32_t index) const {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].short_name == int_arg_name) {
            return (int_args_[i].stored_values == nullptr) ? int_args_[i].values[index] : (*(int_args_[i].stored_values))[index];
        }
    }
}

ArgParser::StringArgument* ArgParser::FindPositionalStringArgument() {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].is_positional) {
            return &string_args_[i];
        }
    }
    return nullptr;
};

ArgParser::IntArgument* ArgParser::FindPositionalIntArgument() {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].is_positional) {
            return &int_args_[i];
        }
    }
    return nullptr;
};

ArgParser::Flag* ArgParser::FindPositionalFlag() {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].is_positional) {
            return &flags_[i];
        }
    }
    return nullptr;
};

void ArgParser::SetStringValue(StringArgument& string_arg, const std::string& value) {
    if (!string_arg.is_multi_value) {
        if (string_arg.stored_value != nullptr) {
            *(string_arg.stored_value) = value;
        } else {
            string_arg.value = value;
        }
    } else {
        if (string_arg.stored_values != nullptr) {
            if (string_arg.new_value_index < (*string_arg.stored_values).size()) {
                (*string_arg.stored_values)[string_arg.new_value_index] = value;
            } else {
                (*string_arg.stored_values).emplace_back(value);
            }
        } else {
            if (string_arg.new_value_index < string_arg.values.size()) {
                string_arg.values[string_arg.new_value_index] = value;
            } else {
                string_arg.values.emplace_back(value);
            }
        }
    }
    ++string_arg.new_value_index;
}

void ArgParser::SetIntValue(IntArgument& int_arg, const int value) {
    if (!int_arg.is_multi_value) {
        if (int_arg.stored_value != nullptr) {
            *(int_arg.stored_value) = value;
        } else {
            int_arg.value = value;
        }
    } else {
        if (int_arg.stored_values != nullptr) {
            if (int_arg.new_value_index < (*int_arg.stored_values).size()) {
                (*int_arg.stored_values)[int_arg.new_value_index] = value;
            } else {
                (*int_arg.stored_values).emplace_back(value);
            }
        } else {
            if (int_arg.new_value_index < int_arg.values.size()) {
                int_arg.values[int_arg.new_value_index] = value;
            } else {
                int_arg.values.emplace_back(value);
            }
        }
    }
    ++int_arg.new_value_index;
}

void ArgParser::SetFlagValue(Flag& flag, const bool value) {
    if (!flag.is_multi_value) {
        if (flag.stored_value != nullptr) {
            *(flag.stored_value) = value;
        } else {
            flag.value = value;
        }
    } else {
        if (flag.stored_values != nullptr) {
            if (flag.new_value_index < (*flag.stored_values).size()) {
                (*flag.stored_values)[flag.new_value_index] = value;
            } else {
                (*flag.stored_values).emplace_back(value);
            }
        } else {
            if (flag.new_value_index < flag.values.size()) {
                flag.values[flag.new_value_index] = value;
            } else {
                flag.values.emplace_back(value);
            }
        }
    }
    ++flag.new_value_index;
}

bool ArgParser::IsValidNumber(const char* string_number) {
    errno = 0;
    char* p_end;
    long long number = std::strtoll(string_number, &p_end, 10);
    if ((*p_end != '\0') || (errno == ERANGE)) {
        return false;
    }
    return true;
}

ArgParser::StringArgument* ArgParser::FindStringArgumentByShortName(const char name) {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].short_name == name) {
            return &string_args_[i];
        }
    }
    return nullptr;
}

ArgParser::IntArgument* ArgParser::FindIntArgumentByShortName(const char name) {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].short_name == name) {
            return &int_args_[i];
        }
    }
    return nullptr;
}

ArgParser::Flag* ArgParser::FindFlagByShortName(const char name) {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].short_name == name) {
            return &flags_[i];
        }
    }
    return nullptr;
}

ArgParser::StringArgument* ArgParser::FindStringArgumentByLongName(const std::string& name) {
    for (int i = 0; i < string_args_.size(); ++i) {
        if (string_args_[i].long_name == name) {
            return &string_args_[i];
        }
    }
    return nullptr;
}

ArgParser::IntArgument* ArgParser::FindIntArgumentByLongName(const std::string& name) {
    for (int i = 0; i < int_args_.size(); ++i) {
        if (int_args_[i].long_name == name) {
            return &int_args_[i];
        }
    }
    return nullptr;
}

ArgParser::Flag* ArgParser::FindFlagByLongName(const std::string& name) {
    for (int i = 0; i < flags_.size(); ++i) {
        if (flags_[i].long_name == name) {
            return &flags_[i];
        }
    }
    return nullptr;
}

bool ArgParser::ArgumentsAreInitialized() {
    for (int i = 0; i < string_args_.size(); ++i) {
        StringArgument* arg = &string_args_[i];
        if (arg->has_default_value) {
            continue;
        }
        if (arg->new_value_index >= arg->min_args_value && arg->new_value_index > 0) {
            continue;
        }
        if (arg->is_multi_value && arg->min_args_value == 0) {
            continue;
        }
        return false;
    }
    for (int i = 0; i < int_args_.size(); ++i) {
        IntArgument* arg = &int_args_[i];
        if (arg->has_default_value) {
            continue;
        }
        if (arg->new_value_index >= arg->min_args_value) {
            continue;
        }
        return false;
    }
    for (int i = 0; i < flags_.size(); ++i) {
        Flag* arg = &flags_[i];
        if (arg->has_default_value) {
            continue;
        }
        if (arg->new_value_index >= arg->min_args_value) {
            continue;
        }
        return false;
    }
    return true;
}

bool ArgParser::Parse(std::vector<std::string> args) {
    if (args.empty() || args[0] != "app") {
        return false;
    }

    for (int i = 1; i < args.size(); ++i) {
        std::string arg = args[i];

        if ((arg[0] == '-' && arg[1] == helper_.short_name) || (arg[0] == '-' && arg[1] == '-' && arg.substr(2) == helper_.long_name)) {
            helper_.is_active = true;
            return true;
        }

        if (arg[0] != '-') {
            StringArgument* positional_string_argument = FindPositionalStringArgument();
            if (positional_string_argument != nullptr) {
                SetStringValue(*positional_string_argument, arg);
                continue;
            }
            IntArgument* positional_int_argument = FindPositionalIntArgument();
            if (positional_int_argument != nullptr) {
                if (!IsValidNumber(arg.c_str())) {
                    return false;
                }
                SetIntValue(*positional_int_argument, stoi(arg));
                continue;
            }
            Flag* positional_flag = FindPositionalFlag();
            if (positional_flag != nullptr) {
                if (arg == "true" || arg == "1") {
                    SetFlagValue(*positional_flag, true);
                    continue;
                }
                if (arg == "false" || arg == "0") {
                    SetFlagValue(*positional_flag, false);
                    continue;
                }
            }
            return false;
        }

        int equality_sign_index = arg.find('=');
        if (equality_sign_index == std::string::npos) {
            if (arg[1] != '-') {
                char name = arg[1];

                StringArgument* string_arg = FindStringArgumentByShortName(name);
                if (string_arg != nullptr) {
                    std::string string_value = args[i + 1];
                    SetStringValue(*string_arg, string_value);
                    ++i;
                    continue;
                }

                IntArgument* int_arg = FindIntArgumentByShortName(name);
                if (int_arg != nullptr) {
                    std::string str_number =  args[i + 1];
                    if (!IsValidNumber(str_number.c_str())) {
                        return false;
                    }
                    int number = std::stoi(str_number);
                    SetIntValue(*int_arg, number);
                    ++i;
                    continue;
                }

                for (int j = 1; j < arg.size(); ++j) {
                    Flag* flag = FindFlagByShortName(arg[j]);
                    if (flag == nullptr) {
                        return false;
                    }
                    SetFlagValue(*flag, true);
                }
            } else {
                Flag* flag = FindFlagByLongName(arg.substr(2));
                if (flag == nullptr) {
                    return false;
                }
                SetFlagValue(*flag, true);
            }
            continue;
        }

        if (arg[1] == '-') {
            std::string name = arg.substr(2, equality_sign_index - 2);
            std::string value = arg.substr(equality_sign_index + 1);

            StringArgument* string_arg = FindStringArgumentByLongName(name);
            if (string_arg != nullptr) {
                SetStringValue(*string_arg, value);
                continue;
            }

            IntArgument* int_arg = FindIntArgumentByLongName(name);
            if (int_arg != nullptr) {
                if (!IsValidNumber(value.c_str())) {
                    return false;
                }
                int number = std::stoi(value);
                SetIntValue(*int_arg, number);
            } else {
                return false;
            }
        } else {
            char name = arg[1];

            StringArgument* string_arg = FindStringArgumentByShortName(name);
            if (string_arg != nullptr) {
                SetStringValue(*string_arg, arg.substr(3, arg.size() - 3));
                continue;
            }

            IntArgument* int_arg = FindIntArgumentByShortName(name);
            if (int_arg != nullptr) {
                std::string str_number = arg.substr(3, arg.size() - 3);
                if (!IsValidNumber(str_number.c_str())) {
                    return false;
                }
                int number = std::stoi(str_number);
                SetIntValue(*int_arg, number);
            } else {
                return false;
            }
        }
    }

    return ArgumentsAreInitialized();
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> args;
    args.emplace_back("app");
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    return Parse(args);
}

