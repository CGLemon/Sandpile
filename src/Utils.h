#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

#include <chrono>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <sstream>

class Timer {
public:
    void clock();

    int get_duration_seconds() const;
    int get_duration_milliseconds() const;
    int get_duration_microseconds() const;

private:
    std::chrono::steady_clock::time_point m_clock_time;

};

class CommandParser {
public:
    struct Reuslt {
        Reuslt(const std::string &s, const int i) : str(s), idx(i) {};

        Reuslt(const std::string &&s, const int i)
            : str(std::forward<decltype(s)>(s)), idx(i) {};

        std::string str;
        int idx;

        template<typename T> T get() const;
    };

    CommandParser() = delete;

    CommandParser(std::string input);

    CommandParser(int argc, char** argv);

    bool valid() const;

    size_t get_count() const;

    std::shared_ptr<Reuslt> get_command(size_t id) const;
    std::shared_ptr<Reuslt> get_commands(size_t begin = 0) const;
    std::shared_ptr<Reuslt> get_slice(size_t begin, size_t end) const;
    std::shared_ptr<Reuslt> find(const std::string input, int id = -1) const;
    std::shared_ptr<Reuslt> find(const std::vector<std::string> inputs, int id = -1) const;
    std::shared_ptr<Reuslt> find_next(const std::string input) const;
    std::shared_ptr<Reuslt> find_next(const std::vector<std::string> inputs) const;

private:
    std::vector<std::shared_ptr<const std::string>> m_commands;

    size_t m_count;

    void parser(std::string &&input);
};


static bool is_integer(std::string s) {
    for (const char &c : s) {
        if (!(c >= '0' && c <= '9')) {
            return false;
        } 
    }
    return true;
}

#endif
