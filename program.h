#pragma once

#include "utility.h"
#include "shader.h"

shared_ptr<Program> ProgFromFile(string filename_v, string filename_f);

class Program : public GLResource {
public:
    enum VertexAttribute {
        POSITION,
        COLOR,
        NORMAL,
        TEXCOORD1,
        TEXCOORD2,
        NONE
    };
    
    Program();
	Program(const string &vs_filename, const string &fs_filename);

	void Use() const;

    void BindAttributeLocations();
	bool BuildFromSource(const string &vs_src, const string &fs_src);
    bool BuildFromFiles (const string &vs_src, const string &fs_src);
    bool TrySetUniform(string name, float44 mat) const;
    bool TrySetUniform(string name, GLuint mat) const;
    void Purge() override;

    bool IsValid() const;

	~Program();

protected:
	void Attach(const Shader &prog) const;
	void Detach(const Shader &prog) const;

	string name;

	GLid res;
};