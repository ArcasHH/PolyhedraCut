#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#include "structures.h"
//#include "tests.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool WireframeMode = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("C:\\Users\\Arcasha\\Desktop\\tmp\\cut\\Clip\\shaders\\model.vs", "C:\\Users\\Arcasha\\Desktop\\tmp\\cut\\Clip\\shaders\\model.fs", WireframeMode);

    // INITIAL TRIANGULATED MODEL
    mdl::Model ourModel("C:\\Users\\Arcasha\\Desktop\\tmp\\cut\\Clip\\models\\cube.obj");

    // ................................................................................................................................................................









    mdl::Mesh mesh = ourModel.meshes[0];//convert mdl::Mesh to Mesh
    Mesh m;
    for(int i = 0; i  <mesh.vertices.size(); ++i){
        Vertex v;
        v.x = mesh.vertices[i].Position.x;
        v.y = mesh.vertices[i].Position.y;
        v.z = mesh.vertices[i].Position.z;
        m.Vertices.push_back(v);
    }
    for(int i = 0; i < mesh.indices.size() ; i +=3 ){
        Face new_face;
        for(int n = 0; n < 3; ++n)
            new_face.Indices.push_back(mesh.indices[i + n]) ;
        m.Faces.push_back(new_face);
    }








    Flat plane1;

    // plane1.p = {{1}, {0}, {0}};
    // plane1.n = Vector{{-1}, {0.0003}, {0}}.normalize();
    // Intersect(m, plane1);


    plane1.p = {{1}, {1.0001}, {1}};
    plane1.n = Vector{{-1}, {0.5}, {0}}.normalize();
    Intersect(m, plane1);



    // plane1.p = {{1}, {0.9998}, {1}};
    // plane1.n = Vector{{-1}, {-1}, {-1}}.normalize();
    // Intersect(m, plane1);

    // plane1.p = {{1}, {0.9998}, {1}};
    // plane1.n = Vector{{1}, {1}, {1}}.normalize();
    // Intersect(m, plane1);



    // plane1.p = {{0.99999}, {0}, {0}};
    // plane1.n = Vector{{1}, {0}, {0}}.normalize();
    // Intersect(m, plane1);

    // plane1.p = {{0.99999}, {0}, {0}};
    // plane1.n = Vector{{-1}, {0}, {0}}.normalize();
    // Intersect(m, plane1);




    // plane1.p = {{0}, {0}, {0}};
    // plane1.n = Vector{{1}, {1}, {0}}.normalize();
    // Intersect(m, plane1);

    // plane1.p = {{0}, {0}, {0}};
    // plane1.n = Vector{{1}, {1}, {1}}.normalize();
    // Intersect(m, plane1);




    // Flat plane2;
    // plane2.p = {{-0.5}, {-0.5}, {-0.5}};
    // plane2.n = Vector{{0}, {1}, {0}}.normalize();
    // //PointClassify(m, plane1);
    // Intersect(m, plane2);





    std::vector<mdl::Vertex> ver; //convert Mesh back to mdl::Mesh
    for(int i =0; i < m.Vertices.size(); ++i){
        mdl::Vertex v;
        v.Position = {{m.Vertices[i].x}, {m.Vertices[i].y}, {m.Vertices[i].z}};
        ver.push_back(v);
    }
    for(int i = 0; i < m.Faces.size(); ++i){
        Vector n = Vector{{m.Vertices[m.Faces[i].Indices[0]]},{m.Vertices[m.Faces[i].Indices[1]]}}.cross(Vector{{m.Vertices[m.Faces[i].Indices[1]]},{m.Vertices[m.Faces[i].Indices[2]]}}).normalize();
        for(int j = 0; j < m.Faces[i].Indices.size(); ++j){
            ver[m.Faces[i].Indices[j]].Normal = {{n.x},{n.y},{n.z}};
        }
    }
    std::vector<unsigned int> indices;
    for(int i =0; i < m.Faces.size(); ++i){
        for( int j = 0; j < m.Faces[i].Indices.size(); ++j){
            indices.push_back(m.Faces[i].Indices[j]);
        }
    }
    bool IsTr = Check(m);
    std::cout<<"is correct  "<< IsTr<<std::endl;
    
    mdl::Mesh new_res = {{ver}, {indices},IsTr};
    ourModel.meshes[0] = new_res;


    // ................................................................................................................................................................



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        // hardcoded light
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightDir", 1.f, 1.f, 1.f);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        WireframeMode = !WireframeMode;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}