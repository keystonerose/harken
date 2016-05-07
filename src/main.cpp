#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>

#include "sdl.h"

using namespace SDL;

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
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0); //3
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        
        auto window = sdl.createWindow("Harken", 1024, 768);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective (60.0, window.aspectRatio(), 1.0, 200.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glewExperimental = GL_TRUE;
        glewInit();
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
