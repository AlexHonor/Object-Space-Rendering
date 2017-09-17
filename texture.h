#pragma once

#include "inc.h"
#include "utility.h"

#include <FreeImage.h>

class RenderableTexture;

class Texture {
public:
    Texture();

    bool SetDefaultParams();
    bool CreateEmpty(size_t w, size_t h);

    bool IsValid() const;

    void BindToSlot(GLuint slot);
    bool LoadFromFile(string filename);

    ~Texture();

    friend class RenderableTexture;
private:
    bool Create();
    
    GLResource res;
    size_t width, height;
};