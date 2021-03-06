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

void Texture::Purge() {
    if (res.IsPurged()) {
        GLuint glid = res;
        glDeleteTextures(1, &glid);
        res.Purge();
    }
}

bool Texture::IsValid() const {
    auto ret = glIsTexture(res); GLERR;
    return ret;
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
    glBindTexture(GL_TEXTURE_2D, res); GLERR;
}

bool Texture::CreateEmpty(size_t w, size_t h) {
    Create();
    
    width = w;
    height = h;

    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); GLERR;
    glBindTexture(GL_TEXTURE_2D, 0); GLERR;

    return true;
}

bool Texture::LoadFromFile(const string &filename) {
    Create();
    
    FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType(filename.c_str(), 0),
        filename.c_str());

    FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
    width = FreeImage_GetWidth(pImage);
    height = FreeImage_GetHeight(pImage);

    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage)); GLERR;
    glBindTexture(GL_TEXTURE_2D, 0); GLERR;

    FreeImage_Unload(pImage);
    
    return true;
}

bool Texture::SaveToFile(const string &filename) const {
    glBindTexture(GL_TEXTURE_2D, res); GLERR;
    vector<BYTE> pixels(3 * width * height, 0);

    //glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data()); GLERR;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.data()); GLERR;
    
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels.data(), width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    bool result = FreeImage_Save(FIF_PNG, image, filename.c_str(), 0);

    FreeImage_Unload(image);
    
    return result;
}

Texture::~Texture() {
    if (!res.IsPurged()) {
        GLuint id = res;
        glDeleteTextures(1, &id);
    }
}