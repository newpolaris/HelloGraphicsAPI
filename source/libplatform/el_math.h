#ifndef __MATH_TYPES_H__
#define __MATH_TYPES_H__

namespace el
{
    namespace math
    {
    
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

            template <typename A, typename B, typename C, typename D>
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
    }
}

#endif //__MATH_TYPES_H__
