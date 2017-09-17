#include "inc.h"

#include <iostream>
#include <stack>

#include "rendering_context.h"
#include "renderable_texture.h"
#include "texture.h"
#include "program.h"
#include "mesh.h"

using namespace std;

SDL_GLContext InitOpenGL(SDL_Window* window) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	return SDL_GL_CreateContext(window);
}

static const int DEFAULT_HEIGHT = 800;
static const int DEFAULT_WIDTH  = 600;

SDL_Window* InitSDLWindow() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw runtime_error(SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("RaY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL);

	if (!window) {
		throw runtime_error(SDL_GetError());
	}

	return window;
}

bool InitGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		throw runtime_error("Failed to init GLEW.");
	}
	return true;
}

Program test;
Texture texture;

bool InitResources() {
    test.BuildFromFiles("shaders/test.vs", "shaders/test.fs");
    texture.LoadFromFile("res/tex/test.jpg");
	
    return true;
}

void Draw() {    
    RenderableTexture frame;
    Texture screen;
    static const size_t w = 1000;
    static const size_t h = 1000;

    screen.CreateEmpty(w, h);
    frame.Create(screen);
    
    RenderingContext ctx;

    ctx.proj.Perspective(deg2rad(90.0f), 1, 0.01, 100);
    ctx.view.Identity();
    ctx.model.Identity();


    float3 vec;
    boost::qvm::X(vec) = 0;
    boost::qvm::Y(vec) = 0;
    boost::qvm::Z(vec) = -3;
    ctx.model.Translate(vec);
    ctx.model.RotateZ(deg2rad(SDL_GetTicks()/200.));

    frame.Begin();
    test.Use();
    
    ctx.ApplyContext(test);

    texture.SetDefaultParams();
    texture.BindToSlot(0);

    if (!test.TrySetUniform("t_diffuse", 0)) {
        throw runtime_error("Failed to set texture uniform");
    }

    glViewport(0, 0, w, h); GLERR;
    glDisable(GL_CULL_FACE); GLERR;
    glDisable(GL_DEPTH_TEST); GLERR;

    FullScreenQuad::Instance().Draw();

	glFinish();

    ctx.proj.Perspective(deg2rad(90.0f), (float)DEFAULT_WIDTH / DEFAULT_HEIGHT, 0.01, 100);
    ctx.model.Identity();
    ctx.view.Identity();

    frame.End();
    
    test.Use();
    ctx.ApplyContext(test);

    screen.SetDefaultParams();
    screen.BindToSlot(1);
    if (!test.TrySetUniform("t_diffuse", 1)) {
        throw runtime_error("Failed to set texture uniform");
    }

    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT); GLERR;
    glDisable(GL_CULL_FACE); GLERR;

    FullScreenQuad::Instance().Draw();
    glFinish();
}

void DestroyResources() {
    test.Purge();
}

void Tick() {

}

void Loop(SDL_Window *window) {
	bool quit = false;
	
    SDL_Event event;

	int last_frame_time = SDL_GetTicks();
	int time_to_process = 0;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
			}
		}

		int current_time = SDL_GetTicks();

		time_to_process += current_time - last_frame_time;
		last_frame_time = current_time;
		
		int FIXED_STEP = 1000.0f / 60.0f;

		while (time_to_process >= FIXED_STEP) {
			time_to_process -= FIXED_STEP;
			
			//Input
			Tick();
		}
		
		Draw();
		SDL_GL_SwapWindow(window);
	}
}

int main(int argv, const char **argc) {
	SDL_Window *window = nullptr;
	SDL_GLContext ctx;
	
	try {
		window = InitSDLWindow();

		ctx = InitOpenGL(window);

		InitGLEW();

		InitResources();

		Loop(window);
	} catch (runtime_error &ex) {
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL, "Fatal error occured: \"%s\"", ex.what());
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", ex.what(), NULL);
	}
    DestroyResources();

	if (ctx) {
		SDL_GL_DeleteContext(ctx);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();

	return 0;
}