#include "harken_sdl.h"
#include "harken_shader.h"
#include "harken_shaderprogram.h"
#include "harken_stringbuilder.h"
#include "harken_vector.h"
#include "harken_vertexarrayobject.h"
#include "harken_vertexbufferobject.h"

#include <GL/glew.h>
#include <SDL.h>

#include <array>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace Harken;

enum AttribName {
    PositionAttrib = 0
};

std::atomic<float> scale{0.0f};

void render(SDLWindow& window, VertexArrayObject& triangleVAO) {

    glClear(GL_COLOR_BUFFER_BIT);

    triangleVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFlush();
    window.swapBuffers();
}

Uint32 update(const Uint32 interval, void *) {

    scale = scale + 0.01f;
    return interval;
}

int main() {

    try {

        SDLManager sdl{SDL_INIT_TIMER};
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

        const auto scaleLocation = shaderProgram.uniformLocation("scale");

        SDL_AddTimer(1000 / 60, update, nullptr);

        auto running = true;
        while (running) {

            // TODO: move this into SDLManager

            SDL_Event event;
            while (SDL_PollEvent(&event)) {

                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            glUniform1f(scaleLocation, std::sin(scale.load()));
            render(window, triangleVAO);
        }
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
