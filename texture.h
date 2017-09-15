#pragma once

#include "inc.h"
#include "utility.h"

#include <FreeImage.h>

class Texture {
public:
    Texture();

    bool SetDefaultParams();
    bool CreateEmpty(size_t w, size_t h);
    
    void BindToSlot(GLuint slot);
    bool LoadFromFile(string filename);

    ~Texture();
private:
    bool Create();
    
    GLResource res;
    size_t width, height;
};