#pragma once
#include <iostream>
#include <cmath>
class Matrix;
template<class T>
class Vec3;

template <class T>
class Vec2
{
public:
    union {
        struct {
            T x, y;
        };

        struct {
            T u, v;
        };

        struct {
            T a, b;
        };

        T data[2];
    };

    Vec2() : x(0), y(0) {
    }

    Vec2(T x, T y) : a(x), b(y) {
    }

    explicit Vec2(Vec3<T> Vec);

    template<class U>
    explicit Vec2(Vec2<U> Vec);

    Vec2 operator+(const Vec2 &V) const { return Vec2<T>(x + V.x, y + V.y); }
    Vec2 operator-(const Vec2 &V) { return Vec2<T>(x - V.x, y - V.y); }
    T operator*(const Vec2 &V) { return x * V.x + y * V.y; }
    Vec2 operator*(const T &r) { return Vec2<T>(x * r, y * r); }
    T &operator[](unsigned const int i) { return data[i]; }
    bool operator==(const Vec2 &V) { return x == V.x && y == V.y; }
    static Vec2 max(const Vec2 &a, const Vec2 &b) { return {std::max(a.x, b.x), std::max(a.y, b.y)}; }
    static Vec2 max(const Vec2 &a, const Vec2 &b, const Vec2 &c) { return max(max(a, b), c); }
    static Vec2 min(const Vec2 &a, const Vec2 &b) { return {std::min(a.x, b.x), std::min(a.y, b.y)}; }
    static Vec2 min(const Vec2 &a, const Vec2 &b, const Vec2 &c) { return min(min(a, b), c); }

    friend std::ostream &operator<<(std::ostream &s, const Vec2<T> &V) { return s << "(" << V.x << ", " << V.y << ")"; }
};

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

template<class T>
class Vec3 {
public:
    union {
        struct {
            T x, y, z;
        };

        struct {
            T a, b, c;
        };

        T data[3];
    };

    Vec3() : x(0), y(0), z(0) {
    }

    Vec3(T x, T y, T z) : a(x), b(y), c(z) {
    }

    explicit Vec3(const Matrix &M);

    template<class U>
    explicit Vec3(Vec3<U> Vec);

    Vec3 operator+(const Vec3 &V) { return Vec3<T>(x + V.x, y + V.y, z + V.z); }
    Vec3 operator-(const Vec3 &V) const { return Vec3<T>(x - V.x, y - V.y, z - V.z); }
    T operator*(const Vec3 &V) { return x * V.x + y * V.y + z * V.z; }

    T GetPhi() const;

    Vec3 operator*(const T &r) { return Vec3<T>(x * r, y * r, z * r); }
    Vec3 operator/(const T &r) { return Vec3<T>(x / r, y / r, z / r); }

    Vec3 operator^(const Vec3 &V) const {
        // std::cout << y * V.z - z * V.y << std::endl;
        return Vec3<T>(y * V.z - z * V.y, z * V.x - x * V.z, x * V.y - y * V.x);
    }

    inline T &operator[](unsigned const int i) { return data[i]; }
    bool operator==(const Vec3 &V) { return x == V.x && y == V.y && z == V.z; }

    Vec3 &normalize(T l = 1) {
        *this = (*this) * (l / std::sqrt(x * x + y * y + z * z));
        return *this;
    }

    T sum() { return x + y + z; }

    static Vec3 ParallelDot(const Vec3 &V1, const Vec3 &V2) { return {V1.x * V2.x, V1.y * V2.y, V1.z * V2.z}; }

    friend std::ostream &operator<<(std::ostream &s, const Vec3<T> &V) {
        return s << "(" << V.x << ", " << V.y << ", " << V.z << ")";
    }

    friend Vec3<T> operator*(const T &r, const Vec3 &V) { return Vec3<T>(V.x * r, V.y * r, V.z * r); }
};

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
