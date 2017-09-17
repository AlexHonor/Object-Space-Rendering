#pragma once

#include "inc.h"
#include "texture.h"

class RenderableTexture {
public:
    RenderableTexture();

    bool Create(Texture &tex);
    bool Create(Texture &tex, Texture &depth);

    void Begin();
    void End();

    ~RenderableTexture();
private:
    GLuint id, rend_buf;
};