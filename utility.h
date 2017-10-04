#pragma once

#include "inc.h"

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <type_traits>

using namespace std;

string ReadFile(string filename);


double deg2rad(double degrees);

class GLid {
public:
    GLid();

    GLid(GLuint _id);

    GLid &operator=(GLid res);

    operator GLuint() const;

    bool IsPurged() const;

    void Purge();

    friend void swap(GLid &a, GLid &b);

protected:
    bool is_purged;
    GLuint id;
};

void swap(GLid &a, GLid &b);

class GLResource;

class GLResourceManager {
public:
    static GLResourceManager& Instance();

    void FreeResources();

    template<typename ConcreteGLResource>
    shared_ptr<typename ConcreteGLResource> New() {
        static_assert(is_base_of<GLResource, ConcreteGLResource>::value, "Method new can only create instances with GLResource base class.");

        shared_ptr<ConcreteGLResource> ptr = make_shared<ConcreteGLResource>();

        auto free = find_if(resources.begin(), resources.end(), [](const weak_ptr<GLResource> &ptr) { return ptr.expired(); });

        if (free == resources.end()) {
            resources.push_back(ptr);
        } else {
            *free = ptr;
        }

        return ptr;
    }


    friend class GLResource;
private:
    GLResourceManager() = default;
    GLResourceManager(const GLResourceManager &manager) = delete;
    GLResourceManager& operator=(GLResourceManager &manager) = delete;
    ~GLResourceManager() = default;

    vector<weak_ptr<GLResource>> resources;
};

class GLResource {
public:
    GLResource();

    virtual void Purge() = 0;

    ~GLResource();
private:
    GLid id;
};

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