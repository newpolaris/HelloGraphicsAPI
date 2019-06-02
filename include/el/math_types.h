#ifndef __EL_MATH_TYPES_H__
#define __EL_MATH_TYPES_H__

#include <el/debug.h>

namespace el {
namespace math {
    
template <typename T>
struct vec2
{
    vec2() {}
    
    template <typename A>
    vec2(A v) : x(v), y(v) {} 

    template <typename A, typename B>
    vec2(A x, B y) : x(x), y(y) {}

    const T& operator[](size_t i) const {
        return v[i];
    }

    T& operator[](size_t i) {
        return v[i];
    }

    union {
        T v[2];
        struct { T x, y; };
        struct { T s, t; };
    };
};

template <typename T>
struct vec3
{
    vec3() {}
    
    template <typename A>
    vec3(A v) : x(v), y(v), z(v) {} 

    template <typename A, typename B, typename C>
    vec3(A x, B y, C z) : x(x), y(y), z(z) {}

    const T& operator[](size_t i) const {
        return v[i];
    }

    T& operator[](size_t i) {
        return v[i];
    }

    union {
        float v[3];
        struct { float x, y, z; };
    };
};

template <typename T>
struct vec4
{
    vec4() {}
    
    template <typename A>
    vec4(A v) : x(v), y(v), z(v), w(v) {} 

    template <typename A, typename B, typename C, typename D>
    vec4(A x, B y, C z, D w) : 
        x(static_cast<T>(x)),
        y(static_cast<T>(y)),
        z(static_cast<T>(z)),
        w(static_cast<T>(w)) {}

    const T& operator[](size_t i) const {
        return v[i];
    }

    T& operator[](size_t i) {
        return v[i];
    }

    union {
        T v[4];
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };
};

typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;

typedef float scalar;
typedef vec2<scalar> vect2;
typedef vec3<scalar> vect3;
typedef vec4<scalar> vect4;

enum EnumZero { Zero };
enum EnumIdentity { Identity };

struct mat4
{
    typedef float data_type;
    typedef vec4<data_type> row_type;
    typedef vec4<data_type> col_type;

    static const int numRows = 4;
    static const int numCols = 4;

    col_type _data[numRows];

    mat4();
    explicit mat4(EnumZero);
    explicit mat4(EnumIdentity);

    template <
        typename A, typename B, typename C, typename D,
        typename E, typename F, typename G, typename H,
        typename I, typename J, typename K, typename L,
        typename M, typename N, typename O, typename P>
    explicit mat4(A m00, B m01, C m02, D m03,
                    E m10, F m11, G m12, H m13,
                    I m20, J m21, K m22, L m23,
                    M m30, N m31, O m32, P m33);

    const col_type& operator[](size_t col) const
    {
        EL_ASSERT(col< numCols);
        return _data[col];
    }

    col_type& operator[](size_t col)
    {
        EL_ASSERT(col < numCols);
        return _data[col];
    }

};

inline mat4::mat4()
{
}

inline mat4::mat4(EnumZero)
{
    for (int i = 0; i < 4; i++)
        _data[i] = col_type(0, 0, 0, 0);
}

inline mat4::mat4(EnumIdentity)
{
    _data[0] = col_type(1, 0, 0, 0);
    _data[1] = col_type(0, 1, 0, 0);
    _data[2] = col_type(0, 0, 1, 0);
    _data[3] = col_type(0, 0, 0, 1);
}

template <
    typename A, typename B, typename C, typename D,
    typename E, typename F, typename G, typename H,
    typename I, typename J, typename K, typename L,
    typename M, typename N, typename O, typename P>
inline mat4::mat4(A m00, B m01, C m02, D m03,
            E m10, F m11, G m12, H m13,
            I m20, J m21, K m22, L m23,
            M m30, N m31, O m32, P m33)
{
    _data[0] = col_type(m00, m01, m02, m03);
    _data[1] = col_type(m10, m11, m12, m13);
    _data[2] = col_type(m20, m21, m22, m23);
    _data[3] = col_type(m30, m31, m32, m33);
}

} // namespace math
} // namespace el

#endif //__EL_MATH_TYPES_H__
