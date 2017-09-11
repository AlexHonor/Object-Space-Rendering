#pragma once

#include "inc.h"
#include "utility.h"
#include "program.h"
#include <type_traits>

using namespace std;

class VertexAttrib {
public:
    VertexAttrib();

    template<typename type>
    bool Set(const vector<type> &data, uint32_t dim, Program::VertexAttribute attrib);

    void BindIndices();

    size_t GetSize();

    ~VertexAttrib();
private:
    size_t size;
    GLResource res;
};

class Mesh {
public:
    Mesh();
    
    void SetIndex(vector<unsigned> data);

    void SetPosition(vector<float3> data);
    void SetColor(vector<float3> data);
    void SetNormal(vector<float3> data);
    void SetTexCoord1(vector<float2> data);
    void SetTexCoord2(vector<float2> data);

    GLint GetCurrentVAO();
    void Draw();

    ~Mesh();
private:
    void Bind();
    void UnBind();

    VertexAttrib position, index, normal, color, texcoord1, texcoord2;
    GLResource res;
};