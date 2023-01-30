#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

//bool IsExtensionSupported(const char *name)
//{
//        GLint n=0;
//        glGetIntegerv(GL_NUM_EXTENSIONS, &n);
//        for (GLint i=0; i<n; i++) {
//                auto extension = reinterpret_cast<const char*>(glad_glGetStringi(GL_EXTENSIONS, i));
//                if (!strcmp(name, extension)) {
//                        return true;
//                }
//        }
//        return false;
//}

static void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void error_callback(int /*error*/, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int, char*[])
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "GLFW3 failed to initialize." << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "get all GL extensions", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW context." << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Load GL extensions using glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        exit(EXIT_FAILURE);
    } else
        std::cout << "OpenGL version loaded" << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glfwSwapInterval(1);

    int NumberOfExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
    std::cout << "OpenGL Num of Extensions: " << NumberOfExtensions << std::endl;
    for(auto i=0; i < NumberOfExtensions; i++) {
        auto extension =  glad_glGetStringi(GL_EXTENSIONS, i);
        printf("OpenGL extension #%d: %s\n", i, extension);
    }

    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.9f, 0.1f, 1.);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}