#include <sstream>
#include "sdl.h"

namespace SDL {
    
    SDLException::SDLException(const char * const message)
        : runtime_error{message} {
            
        std::ostringstream buffer;
        buffer << runtime_error::what() << " Error: " << SDL_GetError();
        m_formattedMessage = buffer.str();
    }
    
    const char * SDLException::what() const noexcept {
        return m_formattedMessage.c_str();
    }

    SDLManager::SDLManager() {
        auto result = SDL_Init(SDL_INIT_VIDEO);
        if (result != 0) {
            throw SDLException{"Could not initialise SDL."};
        }
    }
    
    SDLManager::~SDLManager() {
        SDL_Quit();
    }
    
    SDLWindow SDLManager::createFullscreenWindow(const char* title) {
        return SDLWindow{title, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP};
    }
    
    SDLWindow SDLManager::createWindow(const char * const title, const int width, const int height) {
        return SDLWindow{title, width, height, 0};
    }

    SDLWindow::SDLWindow(const char * const title, const int width, const int height, const Uint32 flags) {
        
        m_handle = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags | SDL_WINDOW_OPENGL);
        if (!m_handle) {
            throw SDLException{"Could not create an SDL window."};
        }
        
        // The OpenGL context is another candidate for factoring out into an RAII class of its own, but it's not much
        // effort to just incorporate it into SDLWindow for now.
        
        m_glContext = SDL_GL_CreateContext(m_handle);
        if (!m_glContext) {
            SDL_DestroyWindow(m_handle);
            throw SDLException{"Could not create an SDL OpenGL context."};
        }
        
        SDL_GL_SetSwapInterval(1);
    }
    
    SDLWindow::SDLWindow(SDLWindow&& rhs)
        : m_glContext{rhs.m_glContext}, m_handle{rhs.m_handle} {

        rhs.m_glContext = nullptr;
        rhs.m_handle    = nullptr;
    }
    
    SDLWindow::~SDLWindow() {
        
        if (m_glContext) {
            SDL_GL_DeleteContext(m_glContext);
        }
        
        if (m_handle) {
            SDL_DestroyWindow(m_handle);
        }
    }
    
    SDLWindow& SDLWindow::operator=(SDLWindow&& rhs) {
        
        m_glContext = rhs.m_glContext;
        m_handle    = rhs.m_handle;
        
        rhs.m_glContext = nullptr;
        rhs.m_handle    = nullptr;
        
        return *this;
    }
    
    float SDLWindow::aspectRatio() const {
        const auto sizeResult = size();
        return sizeResult.height() ? static_cast<float>(sizeResult.width()) / static_cast<float>(sizeResult.height()) : 0.0f;
    }
    
    void SDLWindow::swapBuffers() {
        if (m_handle) {
            SDL_GL_SwapWindow(m_handle);
        }
    }
    
    Harken::Size2i SDLWindow::size() const {

        auto width  = 0;
        auto height = 0;
        
        if (m_handle) {
            SDL_GL_GetDrawableSize(m_handle, &width, &height);
        }
        
        return {width, height};
    }
}
