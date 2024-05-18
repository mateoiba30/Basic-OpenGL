#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Compilar shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"


"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"

"}";

const char* fragmentShaderSource = "#version 330 core\n"

"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0,0.0,0.0, 1.0);\n"
"}";


int main() {
    //inicializo glfw
    glfwInit();
    //le indico la version de opengl
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    //creo la ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PhysicsEngine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //vinculo la ventana al contexto actual
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ///le indico a opengl el tamaño del lienzo
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    GLfloat vertex[15] = {
        //x,   y       //R,    G,   B
        0.f, 0.5f,
        0.5f,-0.5f,
        -0.5f, -0.5f,
    };

    GLuint buffer_id; //Es el VBO_id
    glGenBuffers(1, &buffer_id); //gener 1 buffer y guardo su id
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id); // le digo a opengl que voy a usar el buffer de id: buffer_id
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW); //cargo la informacion en el buffer

    //Si tuviera un solo atributo por vertice usaria un solo vertex attrib pointer(vap), pero como tengo 2 es mas comodo usar un vertex array object que contiene varios vap (posicion y color)
    GLuint VAO_id;
    glGenVertexArrays(1, &VAO_id); //genero 1 vertex array object y guardo su id en VAO_id
    glBindVertexArray(VAO_id); //le digo a opengl que voy a usar el vao de id: VAO_id
    //cada posicion del VAO es un layout, en cada layout se guarda un tipo de atributo de los vertices ej: layout1 guarda las posiciones, el layout2 guarda los colors

    //guardo las posiciones en el layout 0
    glEnableVertexAttribArray(0); //habilito el layout 0 del VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //creo shader objects y guardo sus ids
    GLuint vertexShader_id= glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader_id = glCreateShader(GL_FRAGMENT_SHADER);

    //cargo los codigos de las shader en el shader object de la id que genere anteriormente
    glShaderSource(vertexShader_id, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader_id, 1, &fragmentShaderSource, NULL);

    //compilo los shaders, el parametro es la id
    glCompileShader(vertexShader_id);
    glCompileShader(fragmentShader_id);

    //creo el shader program y guardo su id
    GLuint shaderProgram_id = glCreateProgram();
    //le asigno los shader objects al shader program
    glAttachShader(shaderProgram_id, vertexShader_id);
    glAttachShader(shaderProgram_id, fragmentShader_id);
    //vinculo todos los shader del shader program en un mismo pipline
    glLinkProgram(shaderProgram_id);
    //borro los shader objects porque ya tengo las shader en un shader program y no las necesito individualmente
    glDeleteShader(vertexShader_id);
    glDeleteShader(fragmentShader_id);


    while (!glfwWindowShouldClose(window))
    {
        //definir color de fondo
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //le asigno un color al color buffer
        glClear(GL_COLOR_BUFFER_BIT); //uso el color buffer para asignarle un color a la pantalla

        //uso el programa de shader de id: shaderProgram_id
        glUseProgram(shaderProgram_id);
        
        //le pasamos a opengl los vertices de la forma especificada en el VAO que creamos antes, por eso lo bindeamos
        glBindVertexArray(VAO_id);

        //le decimos a opengl como unir los vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
