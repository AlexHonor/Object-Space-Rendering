#include "utility.h"

GLResource::GLResource() {
    id.Purge();
    
}

GLResource::~GLResource() {

}

GLResourceManager& GLResourceManager::Instance() {
    static GLResourceManager instance;
    return instance;
}

void GLResourceManager::FreeResources() {
    for (auto resource : resources) {
        if (!resource.expired()) {
            (static_cast<shared_ptr<GLResource>>(resource))->Purge();
        }
    }
    resources.clear();
}

string ReadFile(string filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		throw runtime_error("Failed to open file.");
	}

	file.seekg(0, ios::end);
	size_t size = file.tellg();
	file.seekg(0, ios::beg);
	
	vector<char> buffer(size + 1);
	
	file.read(buffer.data(), size);
	*buffer.rbegin() = '\0';
	
	return buffer.data();
}

GLid::GLid() {
	is_purged = true;
}

GLid::GLid(GLuint _id) {
	id = _id;
	is_purged = false;
}

bool GLid::IsPurged() const {
	return is_purged;
}

GLid &GLid::operator=(GLid res) {
	swap(*this, res);
	return *this;
}

GLid::operator GLuint() const {
	if (!IsPurged()) {
		return id;
	} else {
		return 0;
	}
}

void GLid::Purge() {
	is_purged = true;
}

double deg2rad(double degrees) {
    return degrees * 4.0 * atan(1.0) / 180.0;
}

void swap(GLid &a, GLid &b) {
	swap(a.is_purged, b.is_purged);
	swap(a.id, b.id);
}

void ThrowExceptionOnGLError(int line, const char *file) {

	static char error_message[512];

	GLenum gl_error = glGetError();

	if (gl_error == GL_NO_ERROR)
		return;

	switch (gl_error) {
		case GL_INVALID_ENUM:
		sprintf(error_message, "GL_INVALID_ENUM file %s line %d\n", file, line);
		break;

		case GL_INVALID_VALUE:
		sprintf(error_message, "GL_INVALID_VALUE file %s line %d\n", file, line);
		break;

		case GL_INVALID_OPERATION:
		sprintf(error_message, "GL_INVALID_OPERATION file %s line %d\n", file, line);
		break;

		case GL_STACK_OVERFLOW:
		sprintf(error_message, "GL_STACK_OVERFLOW file %s line %d\n", file, line);
		break;

		case GL_STACK_UNDERFLOW:
		sprintf(error_message, "GL_STACK_UNDERFLOW file %s line %d\n", file, line);
		break;

		case GL_OUT_OF_MEMORY:
		sprintf(error_message, "GL_OUT_OF_MEMORY file %s line %d\n", file, line);
		break;

		case GL_TABLE_TOO_LARGE:
		sprintf(error_message, "GL_TABLE_TOO_LARGE file %s line %d\n", file, line);
		break;

		case GL_NO_ERROR:
		break;

		default:
		sprintf(error_message, "Unknown error @ file %s line %d\n", file, line);
		break;
	}

	if (gl_error != GL_NO_ERROR)
		throw runtime_error(error_message);
}