#ifndef HARKEN_GLOBAL_H
#define HARKEN_GLOBAL_H

#include <GL/glew.h>
#include <type_traits>

namespace Harken {
 
    // We in various places need OpenGL's GLchar type to be interoperable with the C++ char, both
    // when passing string information in to OpenGL and when reading it out from OpenGL and feeding
    // it into (say) an std::string.
    // 
    // A char is required by the standard to be the smallest addressable type, so
    // sizeof(char) <= sizeof(GLchar). Furthermore, the standard apparently [1] says that char is at
    // least 8 bits, and OpenGL apparently [2] specifies that GLchar must be 8 bits. Tying this all
    // together, on any platform with a conforming OpenGL implementation, char and GLchar are both
    // 8 bits. So the following condition isn't an unreasonable assumption to depend upon.

    static_assert(std::is_same<GLchar, char>::value, "Incompatible OpenGL / C++ character types.");
}

#endif
