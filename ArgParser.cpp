
#include <vector>
#include <iostream>
#include <string>
#include "ArgParser.h"


ArgumentParser::ArgParser::ArgParser(const std::string &name1) {
    name = name1;
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string> &split) {
    int now_pos_of_parameter = 0;
    std::string now_parameter;
    char now_short_parameter;
    if (split[0] == "app") {
        for (int i = 1; i < split.size(); ++i) {
            if (split[i][0] == '-') {
                if (split[i][1] == '-') {
                    int k = 1;
                    std::string real_func = split[i].substr(2);
                    for (int u = 1; u < split[i].size(); ++u) {
                        if (split[i][u] == '=') {
                            k = u;
                            break;
                        }
                    }
                    if (k == 1) {
                        int r = 0;
                        for (int u = 0; u < Helps.size(); ++u) {
                            if (Helps[u].name == real_func) {
                                Helps[u].need_help = true;
                                need_help_global = true;
                                k = u;
                                ++r;
                                break;
                            }
                        }
                        if (r == 0) {
                            for (int q = 0; q < Flags.size(); ++q) {
                                if (Flags[q].name == real_func) {
                                    Flags[q].to_make = true;
                                    if (Flags[q].value1 != nullptr) {
                                        *Flags[q].value1 = Flags[q].to_make;
                                    }
                                    break;
                                }
                            }
                        }

                    } else {
                        now_parameter = split[i].substr(2, k - 2);
                        for (int q = 0; q < Parameters.size(); ++q) {
                            if (Parameters[q].name == now_parameter) {
                                now_pos_of_parameter = q;
                            }
                        }

                        std::string arg;
                        arg = split[i].substr(k + 1);
                        if (Parameters[now_pos_of_parameter].is_string) {
                            Parameters[now_pos_of_parameter].values.push_back(arg);
                            Parameters[now_pos_of_parameter].value = arg;
                        } else {
                            Parameters[now_pos_of_parameter].int_values.push_back(stoi(arg));
                            Parameters[now_pos_of_parameter].int_value = stoi(arg);
                        }

                        if (Parameters[now_pos_of_parameter].value1 != nullptr) {
                            *Parameters[now_pos_of_parameter].value1 = arg;
                        }

                        if (Parameters[now_pos_of_parameter].int_values1 != nullptr) {
                            Parameters[now_pos_of_parameter].int_values1->push_back(stoi(arg));
                        }


                    }
                } else {
                    if (split[i][2] == '=') {
                        now_short_parameter = split[i][1];
                        for (int q = 0; q < Parameters.size(); ++q) {
                            if (Parameters[q].short_name == now_short_parameter) {
                                now_pos_of_parameter = q;
                            }
                        }
                        std::string arg;
                        arg = split[i].substr(3);
                        Parameters[now_pos_of_parameter].values.push_back(arg);
                        Parameters[now_pos_of_parameter].value = arg;
                    } else {
                        for (int k = 1; k < split[i].size(); ++k) {
                            char ch = split[i][k];
                            for (int u = 0; u < Flags.size(); ++u) {
                                if (Flags[u].short_name == ch) {
                                    Flags[u].to_make = true;
                                    if (Flags[u].value1 != nullptr) {
                                        *Flags[u].value1 = Flags[u].to_make;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            } else {
                int val;
                val = stoi(split[i]);
                for (int y = 0; y < Parameters.size(); ++y) {
                    if (Parameters[y].pos) {
                        Parameters[y].int_values.push_back(val);
                        Parameters[y].int_values1->push_back(val);
                    }
                }
            }
        }

        if (split.size() < 2) {
            for (int p = 0; p < Parameters.size(); ++p) {
                if ((Parameters[p].value.empty() and Parameters[p].int_value == -1000000007)) {
                    return false;
                }
            }
        }
        if (!need_help_global) {
            for (int p = 0; p < Parameters.size(); ++p) {
                if (Parameters[p].int_values.size() < Parameters[p].multi) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }

}

bool ArgumentParser::ArgParser::Parse(int argc, char **argv) {
    std::vector<std::string> split;
    for (int i = 1; i < argc; ++i) {
        split.emplace_back(argv[i]);
    }
    if (split.size() + 1 == argc) {
        Parse(split);
        return true;
    } else {
        return false;
    }
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddStringArgument(const std::string &func) {
    ArgumentParser::Parameter param;
    param.name = func;
    Parameters.push_back(param);
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddStringArgument(const char &ch, const std::string &func) {
    ArgumentParser::Parameter param;
    param.name = func;
    param.short_name = ch;
    Parameters.push_back(param);
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddStringArgument(const char &ch, const std::string &func, const std::string &inform) {
    ArgumentParser::Parameter param;
    param.name = func;
    param.short_name = ch;
    Parameters.push_back(param);
    param.info = inform;
    return *this;
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string &func) {
    for (int i = 0; i < Parameters.size(); ++i) {
        if (Parameters[i].name == func and Parameters[i].is_string) {
            return Parameters[i].value;
        }
    }
    return "";
}

void ArgumentParser::ArgParser::StoreValue(bool &flag1) {
    Flags[Flags.size() - 1].value1 = &flag1;
}

void ArgumentParser::ArgParser::DefaultString(const std::string &func) {
    Parameters[Parameters.size() - 1].def = func;
    Parameters[Parameters.size() - 1].value = func;
}

void ArgumentParser::ArgParser::Default(bool is_true) {
    Flags[Flags.size() - 1].def = is_true;
    Flags[Flags.size() - 1].to_make = is_true;
};

void ArgumentParser::ArgParser::Default(char *func) {
    Parameters[Parameters.size() - 1].def = func;
    Parameters[Parameters.size() - 1].value = func;
}


void ArgumentParser::ArgParser::StoreValue(std::string &func) {
    Parameters[Parameters.size() - 1].value1 = &func;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddIntArgument(const std::string &func) {
    ArgumentParser::Parameter param;
    param.name = func;
    param.is_string = false;
    Parameters.push_back(param);
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddIntArgument(const char ch, const std::string &func) {
    ArgumentParser::Parameter param;
    param.name = func;
    param.is_string = false;
    param.short_name = ch;
    Parameters.push_back(param);
    return *this;
}

ArgumentParser::ArgParser &
ArgumentParser::ArgParser::AddIntArgument(const std::string &func, const std::string &inform) {
    ArgumentParser::Parameter param;
    param.name = func;
    param.is_string = false;
    param.info = inform;
    Parameters.push_back(param);
    return *this;
}

int ArgumentParser::ArgParser::GetIntValue(const std::string &func) {
    for (int i = 0; i < Parameters.size(); ++i) {
        if (Parameters[i].name == func and !Parameters[i].is_string) {
            return Parameters[i].int_value;
        }
    }
    return 0;
}

int ArgumentParser::ArgParser::GetIntValue(const std::string &func, int pos) {
    for (int i = 0; i < Parameters.size(); ++i) {
        if (Parameters[i].name == func and !Parameters[i].is_string) {
            return Parameters[i].int_values[pos];
        }
    }
    return 0;
}

void ArgumentParser::ArgParser::StoreValues(std::vector<int> &vec) {
    Parameters[Parameters.size() - 1].int_values1 = &vec;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::MultiValue() {
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::MultiValue(size_t count) {
    Parameters[Parameters.size() - 1].multi = count;
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddFlag(char ch, const std::string &func) {
    ArgumentParser::Flag flag;
    flag.short_name = ch;
    flag.name = func;
    Flags.push_back(flag);
    return *this;
}

ArgumentParser::ArgParser &
ArgumentParser::ArgParser::AddFlag(char ch, const std::string &func, const std::string &inform) {
    ArgumentParser::Flag flag;
    flag.short_name = ch;
    flag.name = func;
    flag.info = inform;
    Flags.push_back(flag);
    return *this;
}

ArgumentParser::ArgParser &ArgumentParser::ArgParser::AddFlag(const std::string &func, const std::string &inform) {
    ArgumentParser::Flag flag;
    flag.name = func;
    flag.info = inform;
    Flags.push_back(flag);
    return *this;
}

ArgumentParser::ArgParser &
ArgumentParser::ArgParser::AddHelp(char ch, const std::string &func, const std::string &inform) {
    ArgumentParser::Helppp help1;
    help1.short_name = ch;
    help1.name = func;
    help1.info = inform;
    Helps.push_back(help1);
    help1.need_help = false;
    return *this;
}

bool ArgumentParser::ArgParser::GetFlag(const std::string &func) {
    for (int i = 0; i < Flags.size(); ++i) {
        if (Flags[i].name == func) {
            return true;
        }
    }
    return false;
}


ArgumentParser::ArgParser &ArgumentParser::ArgParser::Positional() {
    Parameters.back().pos = true;
    return *this;
}

bool ArgumentParser::ArgParser::Help() {
    bool need = false;
    for (int i = 0; i < Helps.size(); ++i) {
        if (Helps[i].need_help) {
            need = true;
            break;
        }
    }
    return need;
}

std::string ArgumentParser::ArgParser::HelpDescription() {
    return Helps.back().info;
}
