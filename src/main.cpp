#include "harken/sdl.h"
#include "harken/shader.h"
#include "harken/shaderprogram.h"
#include "harken/stringbuilder.h"
#include "harken/vector.h"

#include <GL/glew.h>
#include <SDL.h>

#include <array>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace Harken;

enum VAOName {
    TriangleVAO = 0,
    VAOCount
};

enum BufferName {
    ArrayBuffer = 0,
    BufferCount
};

enum AttribName {
    PositionAttrib = 0
};

GLuint vaos[VAOCount];
GLuint buffers[BufferCount];

void render(SDLWindow& window) {

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaos[TriangleVAO]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFlush();
    window.swapBuffers();
}

int main() {

    try {

        SDLManager sdl;
        sdl.setOpenGLVersion(3, 3);
        auto window = sdl.createWindow("Harken", 1024, 768);

        glewExperimental = true;
        const auto glewResult = glewInit();
        if (glewResult != GLEW_OK) {
            throw std::runtime_error{StringBuilder{} << "Could not initialise GLEW. Error: " << glewGetErrorString(glewResult)};
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glGenVertexArrays(VAOCount, vaos);
        glBindVertexArray(vaos[TriangleVAO]);

        GLfloat vertices[3][2] = {
            { 0.0f,  0.433f},
            { 0.5f, -0.433f},
            {-0.5f, -0.433f}
        };
        
        glGenBuffers(BufferCount, buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ArrayBuffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        const auto vertexShader   = std::make_shared<Shader>(GL_VERTEX_SHADER, "passthrough.vert");
        const auto fragmentShader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "red.frag");

        ShaderProgram shaderProgram{vertexShader, fragmentShader};
        shaderProgram.use();

        glVertexAttribPointer(PositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(PositionAttrib);

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
