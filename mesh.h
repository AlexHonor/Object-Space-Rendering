#pragma once

#include "inc.h"
#include "utility.h"
#include "program.h"
#include "renderable_texture.h"
#include "texture.h"
#include <type_traits>

using namespace std;

class VertexAttrib : public GLResource {
public:
    VertexAttrib();

    template<typename type>
    bool Set(const vector<type> &data, uint32_t dim, Program::VertexAttribute attrib);

    void Purge();

    void BindIndices() const;

    size_t GetSize() const;

    ~VertexAttrib();
private:
    size_t size;
    GLid res;
};

class Mesh : public GLResource {
public:
    Mesh();
    
    void SetIndex(vector<unsigned> data);

    void SetPosition(vector<float3> data);
    void SetColor(vector<float3> data);
    void SetNormal(vector<float3> data);
    void SetTexCoord1(vector<float2> data);
    void SetTexCoord2(vector<float2> data);

    GLint GetCurrentVAO();
    void Draw() const;
    void DrawStrips();

    void BakeMaps(size_t map_w, size_t map_h);

    void BindPositionTex(unsigned id);
    void BindNormalTex(unsigned id);
    void BindTexCoordTex(unsigned id);

    void Purge() override;

    ~Mesh();
private:
    void Bind() const;
    void UnBind() const;

    shared_ptr<Texture> position_tex, normal_tex, texcoord_tex;

    VertexAttrib position, index, normal, color, texcoord1, texcoord2;
    GLid res;
};

class FullScreenQuad : public Mesh {
public:
    FullScreenQuad();
    ~FullScreenQuad();
    static shared_ptr<FullScreenQuad> Instance();
private:

    FullScreenQuad(FullScreenQuad const&) = delete;
    FullScreenQuad& operator=(FullScreenQuad const&) = delete;
};