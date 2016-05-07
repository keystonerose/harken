#ifndef HARKEN_APP_SDL_H
#define HARKEN_APP_SDL_H

#include <stdexcept>
#include <string>
#include <SDL.h>
#include "harken/size.h"

namespace SDL {
    
    /**
     * Exception type thrown when an error is reported by the SDL API. In addition to the message provided to the
     * constructor, objects of this class query SDL_GetError() and report the information it returns in the string
     * returned by what().
     */
    
    class SDLException : public std::runtime_error {
        public:
            SDLException(const char * const message);
    };
    
    class SDLWindow;
    
    /**
     * Manages the initialisation and destruction of the SDL subsystem. Has no data of its own, but facilitates RAII and
     * ensures that SDL is properly initialised before any windows are created. Only one instance of this class should
     * be constructed per application, and its lifetime will determine the time over which the SDL subsystem is active.
     */
    
    class SDLManager {
        public:
            
            SDLManager();  /** Initialises the SDL subsystem and sets some default OpenGL attributes. */
            ~SDLManager(); /** Quits the SDL subsystem. */
            
            /**
             * Creates a full-screen SDL window for OpenGL rendering.
             * @param title The title to create the window with.
             */
            
            SDLWindow createFullscreenWindow(const char * title);
            
            /**
             * Creates a (non-full-screen) SDL window for OpenGL rendering, positioned at the center of the screen.
             * @param title The title to create the window with.
             * @param width The horizontal dimension of the window, in pixels.
             * @param height The vertical dimension of the window, in pixels.
             */
            
            SDLWindow createWindow(const char * title, int width, int height);
            
            /**
             * Sets OpenGL window attributes specifying the version of OpenGL to be used. If called, should be done
             * before createWindow() or createFullscreenWindow().
             * @param major The major version number of OpenGL to use.
             * @param minor The minor version number of OpenGL to use.
             */
            
            void setOpenGLVersion(int major, int minor);
    };
    
    /**
     * Resource management class for an SDL window and its OpenGL rendering context. These resources can only be
     * acquired once the SDL subsystem is active; this is enforced by not allowing SDLWindow instances to be constructed
     * directly but rather requiring them to be created via SDLManager::createWindow() and
     * SDLManager::createFullscreenWindow(). SDLWindow instances will (almost) always act as handles for valid windows;
     * they create their windows at construction and delete them upon destruction. The exception is when an SDLWindow is
     * moved from; then, it no longer tracks any window (but should not be used anyway).
     */
  
    class SDLWindow {
        friend class SDLManager;
        
        public:
            
            /**
             * Performs the necessary SDL calls to delete the window managed by this SDLWindow.
             */

            ~SDLWindow();
            
            SDLWindow(const SDLWindow&) = delete;
            SDLWindow& operator=(const SDLWindow&) = delete;
            
            /**
             * Constructs a new SDLWindow instance that replaces @p rhs as a handle to an existing SDL window and OpenGL
             * rendering context. After the new SDLWindow is constructed, @p rhs is no longer a handle to the window,
             * and will not delete it when its lifetime ends.
             */
            
            SDLWindow(SDLWindow&& rhs);
            
            /**
             * @see SDLWindow(SDLWindow&&)
             */
            
            SDLWindow& operator=(SDLWindow&& rhs);
            
            float aspectRatio() const;    /** Gets the aspect ratio of the drawable portion of the window. */
            Harken::Size2i size() const;  /** Gets the pixel dimensions of the drawable area of the window. */ 
            void swapBuffers();           /** Swaps the buffers used to render to the window. */
            
        private:
            
            /**
             * Invokes the relevant SDL calls to create a window for OpenGL rendering, and assumes ownership of that
             * window and its OpenGL context. The created window is positioned at the center of the screen if it is not
             * full-screen.
             * @param title The title to create the window with.
             * @param width The horizontal dimension of the window, in pixels.
             * @param height The vertical dimension of the window, in pixels.
             * @param flags The flags to pass to SDL_CreateWindow(). Automatically includes SDL_WINDOW_OPENGL.
             */
            
            SDLWindow(const char * title, int width, int height, Uint32 flags);
            
            SDL_GLContext m_glContext = nullptr;
            SDL_Window * m_handle = nullptr;
    };
}

#endif
