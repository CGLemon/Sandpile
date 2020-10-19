#include "Utils.h"

void Timer::clock() {
    m_clock_time = std::chrono::steady_clock::now();
}

int Timer::get_duration_seconds() const {
    const auto end_time = std::chrono::steady_clock::now();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - m_clock_time).count();
    return seconds;
}

int Timer::get_duration_milliseconds() const {
    const auto end_time = std::chrono::steady_clock::now();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_clock_time).count();
    return milliseconds;
}

int Timer::get_duration_microseconds() const {
    const auto end_time = std::chrono::steady_clock::now();
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - m_clock_time).count();
    return microseconds;
}

CommandParser::CommandParser(std::string input) {
    parser(std::move(input));
}

CommandParser::CommandParser(int argc, char** argv) {
    auto out = std::ostringstream{};
    for (int i = 0; i < argc; ++i) {
        out << argv[i] << " ";
    }
    parser(out.str());
}

bool CommandParser::valid() const {
    return m_count != 0;
}

void CommandParser::parser(std::string &&input) {

    m_count = 0;
    auto stream = std::istringstream{input};
    auto in = std::string{};
    while (stream >> in) {
        m_commands.emplace_back(std::make_shared<std::string>(in));
        m_count++;
    }
}

size_t CommandParser::get_count() const {
    return m_count;
}


std::shared_ptr<CommandParser::Reuslt> CommandParser::get_command(size_t id) const {

    if (!valid() || id > m_count) {
        return nullptr;
    }
    return std::make_shared<Reuslt>(Reuslt(*m_commands[id], (int)id));
}

std::shared_ptr<CommandParser::Reuslt> CommandParser::get_commands(size_t b) const {
    return get_slice(b, m_count);
}

std::shared_ptr<CommandParser::Reuslt> CommandParser::get_slice(size_t b, size_t e) const {

     if (!valid() || b >= m_count || e > m_count || b >= e) {
         return nullptr;
     }

     auto out = std::ostringstream{};
     auto begin = std::next(std::begin(m_commands), b);
     auto end = std::next(std::begin(m_commands), e);
     auto stop = std::prev(end, 1);

     if (begin != end) {
         std::for_each(begin, stop, [&](auto in)
                                        {  out << *in << " "; });
     }

     out << **stop;
     return std::make_shared<Reuslt>(Reuslt(out.str(), -1));
}


std::shared_ptr<CommandParser::Reuslt> CommandParser::find(const std::string input, int id) const {

    if (!valid()) {
        return nullptr;
    }

    if (id < 0) {
        for (auto i = size_t{0}; i < get_count(); ++i) {
            const auto res = get_command((size_t)i);
            if (res->str == input) {
                return res;
            }
        }
    } else {
        if (const auto res = get_command((size_t)id)) {
            return res->str == input ? res : nullptr;
        }
    }
    return nullptr;
}

std::shared_ptr<CommandParser::Reuslt> CommandParser::find(const std::vector<std::string> inputs, int id) const {

    for (const auto &in : inputs) {
        if (const auto res = find(in, id)) {
            return res;
        }
    }
    return nullptr;
}

std::shared_ptr<CommandParser::Reuslt> CommandParser::find_next(const std::string input) const {

    const auto res = find(input);

    if (!res || res->idx+1 > (int)get_count()) {
        return nullptr;
    }
    return get_command(res->idx+1);
}

std::shared_ptr<CommandParser::Reuslt> CommandParser::find_next(const std::vector<std::string> inputs) const {

    for (const auto &in : inputs) {
        if (const auto res = find_next(in)) {
            return res;
        }
    }
    return nullptr;
}

template<>
std::string CommandParser::Reuslt::get<std::string>() const {
    return str;
}

template<>
int CommandParser::Reuslt::get<int>() const {
    return std::stoi(str);
}

template<>
float CommandParser::Reuslt::get<float>() const{
    return std::stof(str);
}

template<>
const char* CommandParser::Reuslt::get<const char*>() const{
    return str.c_str();
}
