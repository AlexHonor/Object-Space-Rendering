#include "renderable_texture.h"

RenderableTexture::RenderableTexture() {
    glGenFramebuffers(1, &id); GLERR;
}

bool RenderableTexture::Create(Texture &tex) {
    glBindFramebuffer(GL_FRAMEBUFFER, id); GLERR;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.res, 0); GLERR;
    glGenRenderbuffers(1, &rend_buf); GLERR;
    glBindRenderbuffer(GL_RENDERBUFFER, rend_buf); GLERR;
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex.width, tex.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rend_buf); GLERR;

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); GLERR;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR;

    return status == GL_FRAMEBUFFER_COMPLETE;
}

// todo

bool RenderableTexture::Create(Texture &tex, Texture &depth) {
    return false;
}

void RenderableTexture::Begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, id); GLERR;
}
void RenderableTexture::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR;
}

RenderableTexture::~RenderableTexture() {
    glDeleteRenderbuffers(1, &rend_buf); GLERR;
    glDeleteFramebuffers(1, &id); GLERR;
}
