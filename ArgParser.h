#pragma once

#include <vector>
#include <iostream>
#include <string>


namespace ArgumentParser {

    struct Parameter {
        std::string name;
        char short_name;
        bool is_string = true;
        std::string def;
        std::string value;
        int int_value = -1000000007;
        size_t multi = 0;
        std::vector<std::string> values;
        std::vector<int> int_values;
        std::string *value1 = nullptr;
        std::vector<int> *int_values1 = nullptr;
        bool pos;
        std::string info;
    };

    struct Flag {
        std::string name;
        char short_name;
        bool to_make = false;
        bool def;
        bool *value1 = nullptr;
        std::string info;
    };

    struct Helppp {
        std::string name;
        char short_name{};
        std::string info;
        bool need_help = false;
    };

    class ArgParser { //поменять на class
    public:
        std::string name;
        std::vector<Parameter> Parameters;
        std::vector<Flag> Flags;
        std::vector<Helppp> Helps;
        bool need_help_global = false;

        ArgParser(const std::string &name1);

        bool Parse(const std::vector<std::string> &split);

        bool Parse(int argc, char **argv);

        ArgParser &AddStringArgument(const std::string &func);

        ArgParser &AddStringArgument(const char &ch, const std::string &func);

        ArgParser &AddStringArgument(const char &ch, const std::string &func, const std::string &inform);

        std::string GetStringValue(const std::string &func);

        void StoreValue(bool &flag1);

        void DefaultString(const std::string &func);

        void Default(bool is_true);

        void Default(char *func);

        void StoreValue(std::string &func);

        ArgParser &AddIntArgument(const std::string &func);

        ArgParser &AddIntArgument(const char ch, const std::string &func);

        ArgParser &AddIntArgument(const std::string &func, const std::string &inform);

        int GetIntValue(const std::string &func);

        int GetIntValue(const std::string &func, int pos);

        void StoreValues(std::vector<int> &vec);

        ArgParser &MultiValue();

        ArgParser &MultiValue(size_t count);

        ArgParser &AddFlag(char ch, const std::string &func);

        ArgParser &AddFlag(char ch, const std::string &func, const std::string &inform);

        ArgParser &AddFlag(const std::string &func, const std::string &inform);

        ArgParser &AddHelp(char ch, const std::string &func, const std::string &inform);

        bool GetFlag(const std::string &func);


        ArgParser &Positional();

        bool Help();

        std::string HelpDescription();

        // Your Implementation here!
    };


} // namespace ArgumentParser

