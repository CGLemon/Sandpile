#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>

#include "SandModel.h"

SandModel::SandModel(const int width, const int height) {
    set_size(width, height);
}

void SandModel::set_size(int width, int height) {
    m_width = width > LIMIT_WIDTH ? LIMIT_WIDTH : width;
    m_height = height > LIMIT_HEIGHT ? LIMIT_HEIGHT : height;
    m_shift = m_width+2;
    m_dirs[0] = -m_shift; m_dirs[1] = -1;
    m_dirs[2] = +m_shift; m_dirs[3] = +1;
}

void SandModel::draw(const char *file_name) const {
    std::ofstream file;
    file.open(file_name);

    file << "P5" << std::endl;
    file << m_width << " " << m_height << std::endl;
    file << (int)LIMIT_SAND << std::endl;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const auto vtx = get_vertex(x, y);
            const auto p = m_set[vtx];
            file << p;
        }
    }
    file.close();
}

void SandModel::set_xy(const int x, const int y) {
    m_leak_x = x;
    m_leak_y = y;
}

int SandModel::get_x() const {
    return m_leak_x;
}

int SandModel::get_y() const {
    return m_leak_y;
}

int SandModel::get_width() const {
    return m_width;
}

int SandModel::get_height() const {
    return m_height;
}

void SandModel::alloc_set() {
    const auto set_size = (m_width+2) * (m_height+2);
    if (m_set.size() == set_size) {
        return;
    }
    m_set.resize(set_size);
    m_set.shrink_to_fit();
    std::fill(std::begin(m_set), std::end(m_set), 0U);
    
    m_valids.resize(set_size);
    m_valids.shrink_to_fit();
    std::fill(std::begin(m_valids), std::end(m_valids), false);
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const auto vtx = get_vertex(x, y);
            m_valids[vtx] = true;
        }
    }
}

void SandModel::iterate(const int leak_x, const int leak_y) {

    auto q = std::queue<int>{};
    const auto vtx = get_vertex(leak_x, leak_y);
    q.emplace(vtx);

    while (!q.empty()) {
        const auto v = q.front();
        q.pop();
        auto ptr = m_set.data() + v;
        (*ptr)++;
        if (*ptr >= LIMIT_SAND) {
            *ptr -= LIMIT_SAND;
            for (int k = 0; k < 4; ++k) {
                const auto av = v + m_dirs[k];
                if (m_valids[av]) {
                    q.emplace(av);
                }
            }
        }
    }
}

void SandModel::refresh(const int ite) {
    alloc_set();
    for (int i = 0; i < ite; ++i) {
        iterate(m_leak_x, m_leak_y);
    }
}
