// Copyright 2025 Sophomore Wang
#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <type_traits>
using uint8 = std::uint_fast8_t;
template <typename T, uint8 row, uint8 col> class Matrix {
public:
    std::array<std::array<T, col>, row> data{};

    Matrix() = default;
    constexpr Matrix(const Matrix &m) { std::copy(m.data.begin(), m.data.end(), data.begin()); }
    constexpr Matrix(std::initializer_list<std::initializer_list<T>> list) {
        for (uint8 i = 0; i < std::min(row, static_cast<uint8>(list.size())); i++)
            for (uint8 j = 0; j < col; j++)
                data[i][j] = list.begin()[i].begin()[j];
    }
    constexpr Matrix(std::initializer_list<T> list)
        requires(col == 1)
    {
        std::transform(list.begin(), list.end(), data.begin(),
            [](const T &x) { return std::array<T, col>{x}; });
    }
    static Matrix merge(std::initializer_list<Matrix<T, row, 1>> list) {
        Matrix result{};
        for (uint8 i = 0; i < std::min(row, static_cast<uint8>(list.size())); i++)
            for (uint8 j = 0; j < col; j++)
                result.data[i][j] = list.begin()[i].data[j][0];
        return result;
    }
    template <uint8 row_>
    Matrix(const Matrix<T, row_, 1> m)
        requires(col == 1)
    {
        for (int i = 0; i < std::min(row, row_); i++)
            data[i][0] = m.data[i][0];
    }
    template <typename T_> Matrix(Matrix<T_, row, col> list) {
        for (uint8 i = 0; i < row; i++)
            for (uint8 j = 0; j < col; j++)
                data[i][j] = list.data[i][j];
    }
    constexpr Matrix operator*(const T &a) const {
        Matrix result(*this);
        std::for_each(result.data.begin(), result.data.end(), [a](std::array<T, col> &r) {
            std::transform(r.begin(), r.end(), r.begin(), [a](T &x) { return x * a; });
        });
        return result;
    }
    Matrix operator/(const T &a) const {
        Matrix result(*this);
        std::transform(result.data.begin(), result.data.end(), result.data.begin(),
            [a](const std::array<T, col> &r1) {
                std::array<T, col> result;
                std::transform(
                    r1.begin(), r1.end(), result.begin(), [a](const T &x) { return x / a; });
                return result;
            });
        return result;
    }
    constexpr Matrix operator-(const Matrix &m) const {
        Matrix result(*this);
        std::transform(result.data.begin(), result.data.end(), m.data.begin(), result.data.begin(),
            [](const std::array<T, col> &r1, const std::array<T, col> &r2) {
                std::array<T, col> result;
                std::transform(r1.begin(), r1.end(), r2.begin(), result.begin(),
                    [](const T &x, const T &y) { return x - y; });
                return result;
            });
        return result;
    }

    Matrix operator+(const Matrix &m) const {
        Matrix result{*this};
        std::transform(result.data.begin(), result.data.end(), m.data.begin(), result.data.begin(),
            [](const std::array<T, col> &r1, const std::array<T, col> &r2) {
                std::array<T, col> result;
                std::transform(r1.begin(), r1.end(), r2.begin(), result.begin(),
                    [](const T &x, const T &y) { return x + y; });
                return result;
            });
        return result;
    }
    constexpr Matrix normalize() {
        T result{};
        for (auto r : data)
            for (auto c : r)
                result += c * c;
        result = std::sqrt(result);
        return *this / result;
    }
    template <typename T_, uint8 row_>
    friend constexpr Matrix<T_, row_, 1> operator^(
        const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr);
    constexpr auto &get(uint8 i, uint8 j) { return data[i][j]; }
    auto get(uint8 i, uint8 j) const { return data[i][j]; }
    T sum() {
        T result{};
        for (auto r : data)
            for (auto c : r)
                result += c;
        return result;
    }
    static Matrix ParallelDot(Matrix a, Matrix b) {
        Matrix result{};
        std::transform(a.data.begin(), a.data.end(), b.data.begin(), result.data.begin(),
            [](const std::array<T, col> &r1, const std::array<T, col> &r2) {
                std::array<T, col> result;
                std::transform(r1.begin(), r1.end(), r2.begin(), result.begin(),
                    [](const T &x, const T &y) { return x * y; });
                return result;
            });
        return result;
    }
    static auto min(Matrix a, Matrix b) {
        Matrix result{};
        std::transform(a.data.begin(), a.data.end(), b.data.begin(), result.data.begin(),
            [](const std::array<T, col> &r1, const std::array<T, col> &r2) {
                std::array<T, col> result;
                std::transform(r1.begin(), r1.end(), r2.begin(), result.begin(),
                    [](const T &x, const T &y) { return std::min(x, y); });
                return result;
            });
        return result;
    }
    static auto min(Matrix a, Matrix b, Matrix c) { return min(min(a, b), c); }
    static auto max(Matrix a, Matrix b) {
        Matrix result{};
        std::transform(a.data.begin(), a.data.end(), b.data.begin(), result.data.begin(),
            [](const std::array<T, col> &r1, const std::array<T, col> &r2) {
                std::array<T, col> result;
                std::transform(r1.begin(), r1.end(), r2.begin(), result.begin(),
                    [](const T &x, const T &y) { return std::max(x, y); });
                return result;
            });
        return result;
    }
    static auto max(Matrix a, Matrix b, Matrix c) { return max(max(a, b), c); }
    template <typename T_, uint8 row_, uint8 t_, uint8 col_>
    friend constexpr Matrix<T_, row_, col_> operator*(
        const Matrix<T_, row_, t_> &ml, const Matrix<T_, t_, col_> &mr);
    template <typename T_, uint8 row_>
    friend T_ operator*(const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr);
    template <typename T_, uint8 row_>
    friend constexpr Matrix<T_, row_, 1> operator^(
        const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr);
    template <typename T_, uint8 row_>
    friend Matrix<T_, row_ + 1, 1> operator<<(const Matrix<T_, row_, 1> &m, const T_ &t);
};

using Vec2f = Matrix<float, 2, 1>;
using Vec2i = Matrix<int, 2, 1>;
using Vec3f = Matrix<float, 3, 1>;
using Vec3i = Matrix<int, 3, 1>;

template <typename T_, uint8 row_>
T_ operator*(const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr) {
    T_ result = 0;
    for (uint8 i = 0; i < row_; i++)
        result += ml.data[0][i] * mr.data[0][i];
    return result;
}

template <typename T_, uint8 row_, uint8 t_, uint8 col_>
constexpr Matrix<T_, row_, col_> operator*(
    const Matrix<T_, row_, t_> &ml, const Matrix<T_, t_, col_> &mr) {
    Matrix<T_, row_, col_> result;
    for (uint8 i = 0; i < row_; i++) {
        for (uint8 j = 0; j < col_; j++)
            for (uint8 k = 0; k < t_; k++)
                result.data[i][j] += ml.data[i][k] * mr.data[k][j];
    }
    return result;
}

template <typename T_, uint8 row_>
constexpr Matrix<T_, row_, 1> operator^(
    const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr) {
    return {ml.get(1, 0) * mr.get(2, 0) - ml.get(2, 0) * mr.get(1, 0),
        ml.get(2, 0) * mr.get(0, 0) - ml.get(0, 0) * mr.get(2, 0),
        ml.get(0, 0) * mr.get(1, 0) - ml.get(1, 0) * mr.get(0, 0)};
}

template <typename T_, uint8 row_>
Matrix<T_, row_ + 1, 1> operator<<(const Matrix<T_, row_, 1> &m, const T_ &t) {
    Matrix<T_, row_ + 1, 1> result;
    for (uint8 i = 0; i < row_; i++)
        result.data[i][0] = m.data[i][0];
    result.data[row_][0] = t;
    return result;
}
