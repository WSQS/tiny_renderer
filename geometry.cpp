#include "geometry.h"
#include "const.h"

template <class T>
Vec2<T>::Vec2(Vec3<T> Vec): a(Vec.a), b(Vec.b)
{
}

template Vec2<float>::Vec2(Vec3<float> Vec);

template <class T>
template <class U>
Vec2<T>::Vec2(Vec2<U> Vec): a(Vec.a), b(Vec.b)
{
}

template Vec2<float>::Vec2(Vec2<int> Vec);
template Vec2<int>::Vec2(Vec2<float> Vec);

template <class T>
Vec3<T>::Vec3(const Matrix &M) : x(M.data[0]), y(M.data[1]), z(M.data[2]) {}

template <class T>
template <class U>
Vec3<T>::Vec3(Vec3<U> Vec): a(Vec.a), b(Vec.b), c(Vec.c)
{
}

template Vec3<float>::Vec3(Vec3<int> Vec);
template Vec3<int>::Vec3(Vec3<float> Vec);

template <class T>
T Vec3<T>::GetPhi() const
{
    return 1.f / (1.f - z / GetC());
}

template class Vec3<float>;

Matrix::Matrix(int n = 3) : shape(n, n)
{
    data = new float[n * n];
    for (int i = 0; i < n * n; i++)
    {
        data[i] = 0;
    }
}

Matrix::Matrix(int m, int n) : shape(m, n)
{
    data = new float[m * n];
    for (int i = 0; i < m * n; i++)
    {
        data[i] = 0;
    }
}

Matrix::Matrix(const Vec3f &v) : shape(1, 3)
{
    data = new float[3];
    data[0] = v.x;
    data[1] = v.y;
    data[2] = v.z;
}

Matrix::Matrix(const Vec3f *v) : shape(3, 3)
{
    data = new float[9];
    for (int i = 0; i < 9; i++)
    {
        data[i] = v[i / 3].data[i % 3];
    }
}

Matrix::~Matrix()
{
    delete[] data;
}

const Matrix Matrix::operator*(const Matrix &M)
{
    if (shape.x != M.shape.y)
    {
        std::cerr << "error" << std::endl;
        return Matrix();
    }
    // std::cout << shape << M.shape << std::endl;
    Matrix result(M.shape.x, shape.y);
    for (int i = 0; i < M.shape.x; i++)
        for (int j = 0; j < shape.y; j++)
            for (int k = 0; k < shape.x; k++)
            {
                result[i * M.shape.x + j] += data[j * shape.x + k] * M[k * M.shape.x + i];
                // std::cout << j * shape.x + k << k * M.shape.x + i << std::endl;
            }

    // std::cout << result << std::endl;
    return result;
}

Matrix Matrix::identity(int n)
{
    Matrix m(n);
    for (int i = 0; i < n; i++)
    {
        m.data[i * n + i] = 1;
    }
    return m;
}
