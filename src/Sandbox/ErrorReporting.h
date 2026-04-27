#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

// Modified version of glDebugOutput in https://learnopengl.com/In-Practice/Debugging for C

void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	if (
        0
        || id == 131169 // TODO: replace magic constants
        || id == 131185
        || id == 131218
        || id == 131204
		|| id == 131222
        || type == GL_DEBUG_TYPE_PERFORMANCE
	) return;

	printf("+- OpenGL ----------------------------------------------------------------------\n| \n");
	printf("| Debug message (%u): %s\n", id, message);

    printf("| ");
	switch (source)
	{
	    case GL_DEBUG_SOURCE_API:             printf("Source: API");             break;
	    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System");   break;
	    case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
	    case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party");     break;
	    case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application");     break;
	    case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other");           break;
	    default:                              printf("Source: ???");             break;
	} putchar('\n');

    printf("| ");
	switch (type)
	{
	    case GL_DEBUG_TYPE_ERROR:               printf("Type: Error");                break;
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour");  break;
	    case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability");          break;
	    case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance");          break;
	    case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker");               break;
	    case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group");           break;
	    case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group");            break;
	    case GL_DEBUG_TYPE_OTHER:               printf("Type: Other");                break;
	    default:                                printf("Type: ???");                  break;
	} putchar('\n');

    printf("| ");
	switch (severity)
	{
        case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high");         break;
        case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium");       break;
        case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low");          break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
        default:                             printf("Severity: ???");          break;
	} putchar('\n');
    
    printf("| \n+-------------------------------------------------------------------------------\n");
    
    if ( severity == GL_DEBUG_SEVERITY_HIGH )
        exit(1);
}

void enableReportGlErrors()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}