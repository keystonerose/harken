#include "harken/sdl.h"
#include "harken/shader.h"
#include "harken/shaderprogram.h"
#include "harken/stringbuilder.h"
#include "harken/vector.h"
#include "harken/vertexarrayobject.h"
#include "harken/vertexbufferobject.h"

#include <GL/glew.h>
#include <SDL.h>

#include <array>
#include <cmath>
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

void render(SDLWindow& window, VertexArrayObject& triangleVAO) {

    glClear(GL_COLOR_BUFFER_BIT);

    triangleVAO.bind();
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

        VertexArrayObject triangleVAO;
        triangleVAO.bind();
        
        VertexBufferObject arrayBuffer{GL_ARRAY_BUFFER};
        arrayBuffer.bind();
        
        GLfloat vertices[3][2] = {
            { 0.0f,  0.433f},
            { 0.5f, -0.433f},
            {-0.5f, -0.433f}
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        const auto vertexShader   = std::make_shared<Shader>(GL_VERTEX_SHADER, "uniform-scale.vert");
        const auto fragmentShader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "red.frag");

        ShaderProgram shaderProgram{vertexShader, fragmentShader};
        shaderProgram.use();
        
        glVertexAttribPointer(PositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(PositionAttrib);
        
        auto scale = 0.0f;
        const auto scaleLocation = shaderProgram.uniformLocation("scale");

        auto running = true;
        while (running) {

            // TODO: move this into SDLManager

            SDL_Event event;
            while (SDL_PollEvent(&event)) {

                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            glUniform1f(scaleLocation, std::sin(scale));
            render(window, triangleVAO);
            scale += 0.01f;
        }
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
