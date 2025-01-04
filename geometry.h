#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <iostream>
using uint8 = std::uint_fast8_t;
template <typename T, uint8 row, uint8 col> class Matrix {
  public:
    std::array<std::array<T, col>, row> data{};

    Matrix() = default;
    Matrix(std::array<std::array<T, col>, row> data) : data(data) {}
    Matrix(std::initializer_list<std::initializer_list<T>> list) : data{} {}
    Matrix(std::initializer_list<Matrix<T, row, 1>> list) {
        for (uint8 i = 0; i < std::min(row,static_cast<uint8>(list.size())); i++)
            for (uint8 j = 0; j < col; j++)
                data[i][j] = list.begin()[i].data[j][0];
    }
    template <uint8 row_> Matrix(const Matrix<T, row_, 1> m) : data{} {
        for (int i = 0; i < std::min(row, row_); i++)
            data[i][0] = m.data[i][0];
    }
    template <typename T_> Matrix(Matrix<T_, row, col> list) : data{} {
        for (uint8 i = 0; i < row; i++)
            for (uint8 j = 0; j < col; j++)
                data[i][j] = list.data[i][j];
    }
    Matrix operator*(const int &a) const {
        Matrix result{*this};
        std::for_each(result.data.begin(), result.data.end(), [a](std::array<T, col> &r) {
            std::transform(r.begin(), r.end(), r.begin(), [a](T &x) { return x * a; });
        });
        return result;
    }
    template <typename T_, uint8 row_, uint8 t_, uint8 col_>
    friend Matrix<T_, row_, col_> operator*(const Matrix<T_, row_, t_> &ml,
                                            const Matrix<T_, t_, col_> &mr);
    template <typename T_, uint8 row_>
    friend T_ operator*(const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr);
    Matrix operator/(const int &a) const {
        Matrix result{*this};
        std::for_each(result.data.begin(), result.data.end(), [a](std::array<T, col> &r) {
            std::transform(r.begin(), r.end(), r.begin(), [a](T &x) { return x / a; });
        });
        return result;
    }
    Matrix operator-(const Matrix &m) const {
        Matrix result{*this};
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
        return result;
    }
    Matrix normalize() { return *this; }
    Matrix operator^(const Matrix &m) const {
        Matrix result{*this};
        return result;
    }
    auto &get(uint8 i, uint8 j) { return data[i][j]; }
    auto get(uint8 i, uint8 j) const { return data[i][j]; }
    auto sum() { return 0; }
    auto GetPhi() { return 0; }
    static auto ParallelDot(Matrix a, Matrix b) { return a; }
    static auto min(Matrix a, Matrix b) { return a; }
    static auto min(Matrix a, Matrix b, Matrix c) { return a; }
    static auto max(Matrix a, Matrix b) { return a; }
    static auto max(Matrix a, Matrix b, Matrix c) { return a; }
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
Matrix<T_, row_, col_> operator*(const Matrix<T_, row_, t_> &ml, const Matrix<T_, t_, col_> &mr) {
    Matrix<T_, row_, col_> result;
    for (uint8 i = 0; i < row_; i++) {
        for (uint8 j = 0; j < col_; j++)
            for (uint8 k = 0; k < t_; k++)
                result.data[i][j] += ml.data[i][k] * mr.data[k][j];
    }
    return result;
}
