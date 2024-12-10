#pragma once
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>
class Matrix;
template<class T, uint size>
class Vec;

template<class T>
using Vec2 = Vec<T, 2>;
template<class T>
using Vec3 = Vec<T, 3>;

template<class T, uint size>
class Vec {
public:
    std::array<T, size> data;

    Vec(std::initializer_list<T> init): data(init) {
    }

    template<typename TV>
    Vec for_each(const Vec<TV, size> &V, std::function<T(const T &, const TV &)> input_function) {
        Vec result;
        for (int i = 0; i < size; i++) {
            result.data[i] = input_function(this->data[i], V.data[i]);
        }
        return result;
    }

    Vec map(std::function<T(const T &)> input_function) const {
        Vec result{*this};
        std::for_each(result.data.begin(), result.data.end(), input_function);
        return result;
    }

    template<typename TV>
    Vec operator+(const Vec<TV, size> &v) const {
        return for_each(v, [](const T &x, const TV &y) { return x + y; });
    }

    template<typename TV>
    Vec operator-(const Vec<TV, size> &v) const {
        return for_each(v, [](const T &x, const TV &y) { return x - y; });
    }

    template<typename TV>
    Vec operator*(const TV &v) const {
        return map([v](const T &x) { return x * v; });
    }

    T &operator[](unsigned const int i) { return data[i]; }

    bool operator==(const Vec &V) {
        for (int i = 0; i < size; i++) {
            if (data[i] != V.data[i]) {
                return false;
            }
        }
        return true;
    }

    auto get_size() const {
        T value{};
        map([&value](const T &x) {
            value += x * x;
            return 0;
        });
        return sqrt(value);
    }

    auto normalize() {
        return *this / get_size();
    }

    static Vec max(const Vec &a, const Vec &b) {
        return a.for_each(b, std::max);
    }

    template<typename... Args>
    static Vec max(const Vec &first, const Args &... rest) {
        return max(first, max(rest...));
    }

    static Vec min(const Vec &a, const Vec &b) {
        return a.for_each(b, std::min);
    }

    template<typename... Args>
    static Vec min(const Vec &first, const Args &... rest) {
        return max(first, min(rest...));
    }

    friend std::ostream &operator<<(std::ostream &s, const Vec &V) {
        s << "(";
        V.for_each([&s](const T &x) {
            s << x << ", ";
        });
        return s << ")";
    }

    static Vec ParallelDot(const Vec &V1, const Vec &V2) {
        return V1.for_each(V2, [](const T &a, const T &b) {
            return a * b;
        });
    }

    template<typename TV>
    friend auto operator^(Vec3<TV>, Vec3<TV>) {
        return Vec<TV, 3>();
    }
};


typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;


typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

class Matrix {
public:
    Vec2i shape;
    float *data;

    explicit Matrix(int);

    Matrix(int, int);

    explicit Matrix(const Vec3f &);

    explicit Matrix(const Vec3f *);

    float &operator[](const int i) const {
        return data[i];
    }

    Matrix(const Matrix &M) : shape(M.shape) {
        data = new float[shape.x * shape.y];
        for (int i = 0; i < shape.x * shape.y; i++)
            data[i] = M.data[i];
    };

    Matrix identity(int n);

    ~Matrix();

    const Matrix operator*(const Matrix &);

    friend std::ostream &operator<<(std::ostream &s, const Matrix &V) {
        for (int i = 0; i < V.shape.x * V.shape.y; i++) {
            std::cout << V.data[i] << "\t";
        }
        return s;
    }
};
