#include "texture.h"

Texture::Texture() {
    res.Purge();
}

bool Texture::Create() {
    if (res.IsPurged()) {
        GLuint id;
        glGenTextures(1, &id); GLERR;
        res = id;
        return true;
    }
    return false;
}

bool Texture::SetDefaultParams() {
    Create();
    
    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GLERR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GLERR;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLERR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLERR;

    glBindTexture(GL_TEXTURE_2D, 0); GLERR;
    
    return true;
}

void Texture::BindToSlot(GLuint slot) {
    glActiveTexture(GL_TEXTURE0 + slot); GLERR;
    glBindTexture(GL_TEXTURE_2D, slot); GLERR;
}

bool Texture::CreateEmpty(size_t w, size_t h) {
    Create();
    
    width = w;
    height = h;

    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); GLERR;
    glBindTexture(GL_TEXTURE_2D, 0); GLERR;

    return true;
}

bool Texture::LoadFromFile(string filename) {
    Create();
    
    FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType(filename.c_str(), 0),
        filename.c_str());

    FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
    int nWidth = FreeImage_GetWidth(pImage);
    int nHeight = FreeImage_GetHeight(pImage);

    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage)); GLERR;
    glBindTexture(GL_TEXTURE_2D, 0); GLERR;

    FreeImage_Unload(pImage);
    
    return true;
}

Texture::~Texture() {
    if (!res.IsPurged()) {
        GLuint id = res;
        glDeleteTextures(1, &id);
        res.Purge();
    }
}