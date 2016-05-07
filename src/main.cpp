#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>

#include "sdl.h"
#include "stringbuilder.h"

using namespace SDL;
using namespace Util;

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
        
        auto running = true;
        while (running) {
        
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
