#ifndef SANDMODEL_H_INCLUDE
#define SANDMODEL_H_INCLUDE

#include <vector>
#include <array>

class SandModel {
public:
    SandModel() = default;
    SandModel(int width, int height);

    void draw(const char *file_name) const;
    void refresh(const int ite);

    void set_xy(const int x, const int y);
    void set_size(int width, int height);

    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;

private:
    constexpr static int DEFAULT_WIDTH = 50;
    constexpr static int DEFAULT_HEIGHT = 50;
  
    constexpr static int LIMIT_WIDTH = 10000;
    constexpr static int LIMIT_HEIGHT = 10000;

    constexpr static unsigned char LIMIT_SAND = 4U;

    void alloc_set();
    void iterate(const int x, const int y);
    int get_vertex(const int x, const int y) const;

    int m_width{DEFAULT_WIDTH};
    int m_height{DEFAULT_HEIGHT};
    int m_shift{0};
    int m_leak_x{0};
    int m_leak_y{0};

    std::array<int, 4> m_dirs;
    std::vector<unsigned char> m_set;
    std::vector<bool> m_valids;
};

inline int SandModel::get_vertex(const int x, const int y) const {
    return (y+1) * m_shift + (x+1);
};

#endif
