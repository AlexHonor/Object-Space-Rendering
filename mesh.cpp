#include "mesh.h"

Mesh::Mesh() {
    GLuint id;
    glGenVertexArrays(1, &id);
    res = id;
}

void Mesh::SetIndex(vector<unsigned> data) {
    Bind();
    index.Set(data, 1, Program::VertexAttribute::NONE);
    UnBind();
}

void Mesh::SetPosition(vector<float3> data) {
    Bind();
    position.Set(data, 3, Program::VertexAttribute::POSITION);
    UnBind();
}

void Mesh::SetColor(vector<float3> data) {
    Bind();
    color.Set(data, 3, Program::VertexAttribute::COLOR);
    UnBind();
}

void Mesh::SetNormal(vector<float3> data) {
    Bind();
    normal.Set(data, 3, Program::VertexAttribute::NORMAL);
    UnBind();
}

void Mesh::SetTexCoord1(vector<float2> data) {
    Bind();
    texcoord1.Set(data, 2, Program::VertexAttribute::TEXCOORD1);
    UnBind();
}

void Mesh::SetTexCoord2(vector<float2> data) {
    Bind();
    texcoord2.Set(data, 2, Program::VertexAttribute::TEXCOORD2);
    UnBind();
}

GLint Mesh::GetCurrentVAO() {
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    return current_vao;
}

void Mesh::Draw() {
    Bind();
    index.BindIndices();
    glDrawElements(GL_TRIANGLES, index.GetSize(), GL_UNSIGNED_INT, nullptr); GLERR;
    UnBind();
}

void Mesh::Bind() {
    glBindVertexArray(res); GLERR;
}

void Mesh::UnBind() {
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (!res.IsPurged()) {
        GLuint id = res;
        glDeleteVertexArrays(1, &id); GLERR;
    }
}

VertexAttrib::VertexAttrib() {
    GLuint id;
    glGenBuffers(1, &id); GLERR;
    res = id;
}

template<typename type>
bool VertexAttrib::Set(const vector<type> &data, uint32_t dim, Program::VertexAttribute attrib) {
    size = data.size();
    
    glBindBuffer(is_same<type,float3>::value || is_same<type, float2>::value ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, res); GLERR;

    glBufferData(is_same<type, float3>::value || is_same<type, float2>::value ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(type), data.data(), GL_STATIC_DRAW); GLERR;
    if (is_same<type, float3>::value || is_same<type, float2>::value) {
        glVertexAttribPointer(attrib, dim, GL_FLOAT, GL_FALSE, 0, 0); GLERR;
        glEnableVertexAttribArray(attrib); GLERR;
    }

    glBindBuffer(is_floating_point<type>::value ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, 0); GLERR;

    return true;
}

size_t VertexAttrib::GetSize() {
    return size;
}

void VertexAttrib::BindIndices() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res); GLERR;
}


VertexAttrib::~VertexAttrib() {
    if (!res.IsPurged()) {
        GLuint id = res;
        glDeleteBuffers(1, &id); GLERR;
    }
}