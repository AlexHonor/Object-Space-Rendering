#include "inc.h"

#include <iostream>
#include <stack>
#include <memory>

#include "rendering_context.h"
#include "renderable_texture.h"
#include "texture.h"
#include "program.h"
#include "mesh.h"
#include "utility.h"

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

shared_ptr<Program> test, render_position_prog;
shared_ptr<Texture> texture;
shared_ptr<Mesh> cube;

bool InitResources() {
    texture = GLResourceManager::Instance().New<Texture>();
    test = GLResourceManager::Instance().New<Program>();
    render_position_prog = GLResourceManager::Instance().New<Program>();
    
    test->BuildFromFiles("shaders/test.vs", "shaders/test.fs");
    render_position_prog->BuildFromFiles("shaders/render_position.vs", "shaders/render_position.fs");
    texture->LoadFromFile("res/tex/test.jpg");
	
    vector<float3> vertices =
    {
        {-1, -1, -1},
        {1, -1, -1 },
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, 1},
        {-1, 1, 1}
    };

    float dx = 1.0f / 3.0f;
    float dy = 1.0f / 2.0f;

    vector<float> tex_coords =
    {
        0.000059f, 1.0f - 0.000004f,
        0.000103f, 1.0f - 0.336048f,
        0.335973f, 1.0f - 0.335903f,
        1.000023f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.336024f, 1.0f - 0.671877f,
        0.667969f, 1.0f - 0.671889f,
        1.000023f, 1.0f - 0.000013f,
        0.668104f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.000059f, 1.0f - 0.000004f,
        0.335973f, 1.0f - 0.335903f,
        0.336098f, 1.0f - 0.000071f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.336024f, 1.0f - 0.671877f,
        1.000004f, 1.0f - 0.671847f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.668104f, 1.0f - 0.000013f,
        0.335973f, 1.0f - 0.335903f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.668104f, 1.0f - 0.000013f,
        0.336098f, 1.0f - 0.000071f,
        0.000103f, 1.0f - 0.336048f,
        0.000004f, 1.0f - 0.671870f,
        0.336024f, 1.0f - 0.671877f,
        0.000103f, 1.0f - 0.336048f,
        0.336024f, 1.0f - 0.671877f,
        0.335973f, 1.0f - 0.335903f,
        0.667969f, 1.0f - 0.671889f,
        1.000004f, 1.0f - 0.671847f,
        0.667979f, 1.0f - 0.335851f
    };

    vector<float3> normals =
    {
        {0, 0, 1},
        {1, 0, 0},
        {0, 0, -1},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0}
    };
   
    vector<unsigned> indices =
    {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };

    vector<float3> vertexBuffer;
    for (auto index : indices) {
        vertexBuffer.push_back(vertices[index]);
    }

    vector<float2> textureBuffer;
    for (int i = 0; i < 12; i++) {
        auto v1 = vertices[indices[i * 3 + 0]];
        auto v2 = vertices[indices[i * 3 + 1]];
        auto v3 = vertices[indices[i * 3 + 2]];
        
        bool coord[3] = {false, false, false};
        int sign = 1;
        int p = 0;

        for (int j = 0; j < 3; j++) {
            coord[j] = v1.a[j] == v2.a[j] && v1.a[j] == v3.a[j];
            if (coord[j]) {
                p = j;
                sign = vertices[indices[i * 3 + 2]].a[j];
            }
        }

        float2 s = { sign / 4.0f + 0.25f, p / 3.0f };
        float2 f = { 2.0f, 3.0f };

        cout << endl;
        for (int j = 0; j < 3; j++) {
            int t = 0;
            float2 texcoord;
            for (int k = 0; k < 3; k++) {
                if (!coord[k]) {
                    texcoord.a[t] = s.a[t] + (vertices[indices[i * 3 + j]].a[k] / 2.0f + 0.5f) / f.a[t];
                    t++;
                }
            }
            cout << texcoord.a[0] * 2 << " " << texcoord.a[1] * 3 << endl;

            textureBuffer.push_back(texcoord);
        }
    }

    vector<float3> normalBuffer;
    for (int i = 0; i < 36; i++) {
        normalBuffer.push_back(normals[indices[i / 6]]);
    }

    for (int i = 0; i < indices.size(); i++) {
        indices[i] += 8 * (i / 6);
    }

    cube = GLResourceManager::Instance().New<Mesh>();
    cube->SetPosition(vertexBuffer);
    cube->SetTexCoord1(textureBuffer);
    cube->SetNormal(normalBuffer);
    cube->SetIndex(indices);

    cube->BakeMaps(100, 100);

    return true;
}

void Draw() {
    RenderableTexture frame;
    shared_ptr<Texture> screen = GLResourceManager::Instance().New<Texture>();
    static const size_t w = 1000;
    static const size_t h = 1000;

    screen->CreateEmpty(w, h);
    frame.Create(*screen);

    RenderingContext ctx;

    ctx.proj.Perspective(deg2rad(90.0f), 1, 0.01, 100);
    ctx.view.Identity();
    ctx.model.Identity();


    float3 vec = { 0, 0, -3 };
    ctx.model.Translate(vec);
    ctx.model.RotateX(SDL_GetTicks() / 500.0f);
    frame.Begin();
    render_position_prog->Use();

    ctx.ApplyContext(render_position_prog);

    texture->SetDefaultParams();
    texture->BindToSlot(0);

    render_position_prog->TrySetUniform("t_diffuse", 0);

    glClearColor(0, 0, 0, 0); GLERR;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR;

    glViewport(0, 0, w, h); GLERR;
    glDisable(GL_CULL_FACE); GLERR;
    glDisable(GL_DEPTH_TEST); GLERR;

    //FullScreenQuad::Instance()->Draw();

    glFinish();

    frame.End();

    test->Use();
    ctx.ApplyContext(test);

    static auto simple_prog = ProgFromFile("shaders/simple.vs", "shaders/simple.fs");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT); GLERR;
    glDisable(GL_CULL_FACE); GLERR;
    glEnable(GL_DEPTH_TEST); GLERR;

    simple_prog->Use();
    ctx.ApplyContext(simple_prog);
    cube->BindPositionTex(2);
    simple_prog->TrySetUniform("t_diffuse", 2);
    cube->DrawStrips();
    
    screen->SetDefaultParams();
    screen->BindToSlot(1);
    test->TrySetUniform("t_diffuse", 1);

    
    //FullScreenQuad::Instance()->Draw();
    glFinish();
}

void DestroyResources() {
    GLResourceManager::Instance().FreeResources();
    FullScreenQuad::Instance()->Purge();
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
    
        DestroyResources();
    } catch (runtime_error &ex) {
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL, "Fatal error occured: \"%s\"", ex.what());
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", ex.what(), NULL);
	}
    
	if (ctx) {
		SDL_GL_DeleteContext(ctx);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();

	return 0;
}