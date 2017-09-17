#pragma once

#include "inc.h"

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

string ReadFile(string filename);


double deg2rad(double degrees);

class GLResource {
public:
    GLResource();
    
    GLResource(GLuint _id);

    GLResource &operator=(GLResource res);

    operator GLuint() const;

    bool IsPurged() const;

    void Purge();

    friend void swap(GLResource &a, GLResource &b);

protected:
    bool is_purged;
    GLuint id;
};

void swap(GLResource &a, GLResource &b);

#define GLERR ThrowExceptionOnGLError(__LINE__, __FILE__)

void ThrowExceptionOnGLError(int line, const char *file);

#include <boost/qvm/vec_traits.hpp>

#define BOOOST_VECTOR_TYPE_TRAIT_DEF(type_name, dim_num)						                            	    \
struct type_name##dim_num { type_name a[dim_num]; };                                                                \
                                                                                                                    \
namespace boost {                                                                                                   \
    namespace qvm {                                                                                                 \
        template <>                                                                                                 \
        struct vec_traits<type_name##dim_num> {                                                                     \
            static int const dim = dim_num;                                                                         \
            typedef type_name scalar_type;                                                                          \
                                                                                                                    \
            template <int I> static inline scalar_type & write_element(type_name##dim_num & v) { return v.a[I]; }   \
            template <int I> static inline scalar_type read_element(type_name##dim_num const & v) { return v.a[I]; }\
                                                                                                                    \
            static inline scalar_type & write_element_idx(int i, type_name##dim_num & v) { return v.a[i]; }         \
            static inline scalar_type read_element_idx(int i, type_name##dim_num const & v) { return v.a[i]; }      \
        };                                                                                                          \
    }                                                                                                               \
}

#include <boost/qvm/mat_traits.hpp>

#define BOOOST_MATRIX_TYPE_TRAIT_DEF(type_name, col_num, row_num)	                                                                    \
struct type_name##col_num##row_num{ type_name a[row_num][col_num]; };                                                                   \
                                                                                                                                        \
namespace boost {                                                                                                                       \
    namespace qvm {                                                                                                                     \
        template <>                                                                                                                     \
        struct mat_traits<type_name##col_num##row_num> {                                                                                \
            static int const rows = col_num;                                                                                            \
            static int const cols = row_num;                                                                                            \
            typedef type_name scalar_type;                                                                                              \
                                                                                                                                        \
            template <int I, int J> static inline scalar_type & write_element(type_name##col_num##row_num & v) { return v.a[J][I]; }    \
            template <int I, int J> static inline scalar_type read_element(type_name##col_num##row_num const & v) { return v.a[J][I]; } \
                                                                                                                                        \
            static inline scalar_type & write_element_idx(int i, int j, type_name##col_num##row_num & v) { return v.a[j][i]; }          \
            static inline scalar_type read_element_idx(int i, int j, type_name##col_num##row_num const & v) { return v.a[j][i]; }       \
        };                                                                                                                              \
    }                                                                                                                                   \
}

BOOOST_VECTOR_TYPE_TRAIT_DEF(float, 2)
BOOOST_VECTOR_TYPE_TRAIT_DEF(float, 3)
BOOOST_VECTOR_TYPE_TRAIT_DEF(float, 4)

BOOOST_VECTOR_TYPE_TRAIT_DEF(int, 2)
BOOOST_VECTOR_TYPE_TRAIT_DEF(int, 3)
BOOOST_VECTOR_TYPE_TRAIT_DEF(int, 4)

BOOOST_MATRIX_TYPE_TRAIT_DEF(float, 2, 2)
BOOOST_MATRIX_TYPE_TRAIT_DEF(float, 3, 3)
BOOOST_MATRIX_TYPE_TRAIT_DEF(float, 4, 4)