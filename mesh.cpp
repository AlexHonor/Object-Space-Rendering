#include "mesh.h"

FullScreenQuad::FullScreenQuad() {
    vector<float3> position = {
        {-1.0,  1.0, 0.0f },
        { 1.0,  1.0, 0.0f },
        { 1.0, -1.0, 0.0f },
        {-1.0, -1.0, 0.0f }
    };

    vector<float3> colors = {
        { 1.0,  0.0,  0.0 },
        { 0.0,  1.0,  0.0 },
        { 0.0,  0.0,  1.0 },
        { 1.0,  1.0,  1.0 }
    };

    vector<unsigned> indices = {
        1,  2,  3,
        1,  0,  3
    };

    vector<float2> texcoord1 = {
        { 0.0, 1.0 },
        { 1.0, 1.0 },
        { 1.0, 0.0 },
        { 0.0, 0.0 }
    };

    SetPosition(position);
    SetTexCoord1(texcoord1);
    SetColor(colors);

    SetIndex(indices);
}

shared_ptr<FullScreenQuad> FullScreenQuad::Instance() {
    static auto instance = GLResourceManager::Instance().New<FullScreenQuad>();
    return instance;
}

FullScreenQuad::~FullScreenQuad() = default;

Mesh::Mesh() {
    GLuint id;
    glGenVertexArrays(1, &id);
    res = id;

    position_tex = GLResourceManager::Instance().New<Texture>();
    normal_tex   = GLResourceManager::Instance().New<Texture>();
    texcoord_tex = GLResourceManager::Instance().New<Texture>();
}

void Mesh::Purge() {
    if (!res.IsPurged()) {
        position.Purge();
        index.Purge();
        normal.Purge();
        color.Purge();
        texcoord1.Purge();
        texcoord2.Purge();

        GLuint glid = res;
        glDeleteVertexArrays(1, &glid);
        res.Purge();
    }
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

void Mesh::Draw() const {
    Bind();
    index.BindIndices();
    glDrawElements(GL_TRIANGLES, index.GetSize(), GL_UNSIGNED_INT, nullptr); GLERR;
    UnBind();
}

void Mesh::DrawStrips() {
    Bind();
    glDrawArrays(GL_TRIANGLES, 0, position.GetSize()); GLERR;
    UnBind();
}

void Mesh::BindPositionTex(unsigned id) {
    position_tex->BindToSlot(id);
}

void Mesh::BindNormalTex(unsigned id) {
    normal_tex->BindToSlot(id);
}

void Mesh::BindTexCoordTex(unsigned id) {
    texcoord_tex->BindToSlot(id);
}


void Mesh::BakeMaps(size_t map_w, size_t map_h) {
    position_tex->CreateEmpty(map_w, map_h);
    normal_tex->CreateEmpty(map_w, map_h);
    texcoord_tex->CreateEmpty(map_w, map_h);

    position_tex->SetDefaultParams();
    normal_tex->SetDefaultParams();
    texcoord_tex->SetDefaultParams();

    RenderableTexture bake_target;

    static shared_ptr<Program> prog_position = ProgFromFile("shaders/position.vs","shaders/position.fs");
    static shared_ptr<Program> prog_normal = ProgFromFile("shaders/normal.vs", "shaders/normal.fs");
    static shared_ptr<Program> prog_texcoord = ProgFromFile("shaders/texcoord.vs", "shaders/texcoord.fs");
    
    glViewport(0, 0, map_w, map_h); GLERR;
    glDisable(GL_DEPTH_TEST); GLERR;
    glDisable(GL_CULL_FACE); GLERR;

    bake_target.Create(*position_tex);
    bake_target.Begin();
        prog_position->Use();
        DrawStrips();
    bake_target.End();
    
    bake_target.Create(*normal_tex);
    bake_target.Begin();
        prog_normal->Use();
        DrawStrips();
    bake_target.End();

    bake_target.Create(*texcoord_tex);
    bake_target.Begin();
        prog_texcoord->Use();
        DrawStrips();
    bake_target.End();
}

void Mesh::Bind() const {
    glBindVertexArray(res); GLERR;
}

void Mesh::UnBind() const {
    glBindVertexArray(0); GLERR;
}

Mesh::~Mesh() {
    Purge();
}

VertexAttrib::VertexAttrib() {
    GLuint id;
    glGenBuffers(1, &id); GLERR;
    res = id;
}

void VertexAttrib::Purge() {
    if (!res.IsPurged()) {
        GLuint id = res;
        glDeleteBuffers(1, &id); GLERR;
        res.Purge();
    }
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

size_t VertexAttrib::GetSize() const {
    return size;
}

void VertexAttrib::BindIndices() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res); GLERR;
}


VertexAttrib::~VertexAttrib() {
    Purge();
}