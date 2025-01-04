#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <iostream>
using uint8 = std::uint_fast8_t;
template <typename T, uint8 row, uint8 col> class Matrix {
    std::array<std::array<T, col>, row> data;

  public:
    Matrix() = default;
    Matrix(std::array<std::array<T, col>, row> data) : data(data) {}
    Matrix(std::initializer_list<std::initializer_list<T>> list) : data{} {}
    Matrix(std::initializer_list<Matrix<T, row, 1>> list) : data{} {}
    template <typename T_> Matrix(Matrix<T_, row, col> list) : data{} {}
    Matrix operator*(const int &a) {
        std::for_each(data.begin(), data.end(), [a](std::array<T, col> &r) {
            std::transform(r.begin(), r.end(), r.begin(), [a](T &x) { return x * a; });
        });
        return *this;
    }
    template <uint8 row_, uint8 col_>
    Matrix<T, row, col_> operator*(const Matrix<T, row_, col_> &a) const {
        return {};
    }
    template <uint8 row_> Matrix(const Matrix<T, row_, 1> m) : data{} {}
    Matrix operator/(const int &a) {
        std::for_each(data.begin(), data.end(), [a](std::array<T, col> &r) {
            std::transform(r.begin(), r.end(), r.begin(), [a](T &x) { return x / a; });
        });
        return *this;
    }
    template <typename T_, uint8 row_>
    friend T_ operator*(const Matrix<T_, row_, 1> &ml, const Matrix<T_, row_, 1> &mr);

    Matrix operator-(const Matrix &m) const {
        Matrix result{*this};
        return result;
    }

    Matrix operator+(const Matrix &m) const {
        Matrix result{*this};
        return result;
    }

    // TODO
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
