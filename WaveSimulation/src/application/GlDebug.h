#pragma once

#include <iostream>
#include <algorithm>
#include <glad/glad.h>

static const char* GL_DEBUG_SOURCE_STR(GLenum source)
{
    static const char* sources[] = {
      "API",   "Window System", "Shader Compiler", "Third Party", "Application",
      "Other", "Unknown"
    };

    int str_idx =
        std::min(source - GL_DEBUG_SOURCE_API,
            (unsigned int)(sizeof(sources) / sizeof(const char*) - 1));

    return sources[str_idx];
}

static const char* GL_DEBUG_TYPE_STR(GLenum type)
{
    static const char* types[] = {
      "Error",       "Deprecated Behavior", "Undefined Behavior", "Portability",
      "Performance", "Other",               "Unknown"
    };

    int str_idx =
        std::min(type - GL_DEBUG_TYPE_ERROR,
            (unsigned int)(sizeof(types) / sizeof(const char*) - 1));

    return types[str_idx];
}

static const char* GL_DEBUG_SEVERITY_STR(GLenum severity)
{
    static const char* severities[] = {
      "High", "Medium", "Low", "Notification"
    };

 
    int str_idx =
        std::min(severity - GL_DEBUG_SEVERITY_HIGH,
            (unsigned int)(sizeof(severities) / sizeof(const char*) - 1));

    return severities[str_idx];
}

void GLAPIENTRY GL_ERROR_CALLBACK(GLenum        source,
    GLenum        type,
    GLuint        id,
    GLenum        severity,
    GLsizei       length,
    const GLchar* message,
    GLvoid* userParam)
{
    std::cout << "OpenGL Error:" << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << " Object ID: " << id << std::endl;
    std::cout << " Severity:  " << GL_DEBUG_SEVERITY_STR(severity) << std::endl;
    std::cout << " Type:      " << GL_DEBUG_TYPE_STR(type) << std::endl;
    std::cout << " Source:    " << GL_DEBUG_SOURCE_STR(source) << std::endl;
    std::cout << " Message:   " << message << std::endl;
}