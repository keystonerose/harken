#include "harken/sdl.h"
#include "harken/shader.h"
#include "harken/shaderprogram.h"
#include "harken/vector.h"
#include "harken/stringbuilder.h"

#include <GL/glew.h>
#include <SDL.h>

#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace Harken;

void render(SDLWindow& window) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    glPushMatrix();        
        glTranslatef(0.0f, 0.0f, -1.5f);
        glTranslatef(0.0f, -0.433f, 0.0f);
        glBegin(GL_POLYGON);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f, 0.866f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( 0.5f, 0.000f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, 0.000f, 0.0f);
        glEnd();
    glPopMatrix();
    
    window.swapBuffers();
}

int main() {
    
    try {
        
        SDLManager sdl;
        sdl.setOpenGLVersion(3, 3);
        auto window = sdl.createWindow("Harken", 1024, 768);
        
        const auto glewResult = glewInit();
        if (glewResult != GLEW_OK) {
            throw std::runtime_error{StringBuilder{} << "Could not initialise GLEW. Error: " << glewGetErrorString(glewResult)};
        }
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        Shader vertexShader{GL_VERTEX_SHADER, "passthrough.vert"};
        Shader fragmentShader{GL_FRAGMENT_SHADER, "red.frag"};
        
        /*
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        std::array<Vector3f, 1> vertices{
            Vector3f{0.0f, 0.0f, 0.0f}
        };
        
        const auto * const vertexData = vertices.data();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        */
        
        auto running = true;
        while (running) {
        
            // TODO: move this into SDLManager
            
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }
            
            render(window);
        }
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
