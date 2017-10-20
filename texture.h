#pragma once

#include "inc.h"
#include "utility.h"

#include <FreeImage.h>

class RenderableTexture;

class Texture : public GLResource {
public:
    Texture();

    bool SetDefaultParams();
    bool CreateEmpty(size_t w, size_t h);

    void Purge() override;

    bool IsValid() const;

    void BindToSlot(GLuint slot);
    bool SaveToFile(const string &filename) const;
    bool LoadFromFile(const string &filename);

    ~Texture();

    friend class RenderableTexture;
private:
    bool Create();
    
    GLid res;
    size_t width, height;
};