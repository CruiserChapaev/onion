#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <thread>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include <windef.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int FRAME_RATE_LOCK = 120;
const unsigned int FRAME_LOCK_PERIOD = 1000 / FRAME_RATE_LOCK;

// Камера
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// Тайминги
static float deltaTime = 0.0;
static float lastFrame = 0.0;



int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL windowd", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Сообщаем stb_image.h, чтобы он перевернул загруженные текстуры относительно y-оси (до загрузки модели)
    stbi_set_flip_vertically_on_load(true);

    // Конфигурирование глобального состояния OpenGL
    glEnable(GL_DEPTH_TEST);

    // Компилирование нашей шейдерной программы
    Shader lightSourceShader("shaders/modelSource.vs", "shaders/modelSource.fs");
    Shader planetShader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");

    // Загрузка моделей
    Model solarSystem_mars("models/mars.obj");
//    Model solarSystem_mars("textures/backpack/backpack.obj");
    Model solarSystem_star("models/sun.obj");
    Model solarSystem_milkyWay("models/milkyWay.obj");


    // Отрисовка в режиме каркаса
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Позиция источника света.
    glm::vec3 lightPosition(0.0f, 0.0f, 10.0f);

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Логическая часть работы со временем для каждого кадра
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (deltaTime < FRAME_LOCK_PERIOD)
        {
            this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_LOCK_PERIOD - deltaTime)));
        }

        // Обработка ввода
        processInput(window);

        // Рендеринг
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        lightSourceShader.use();

        // Преобразования Вида/Проекции
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);

        // Рендеринг загруженной модели
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition); // смещаем вниз чтобы быть в центре сцены
        model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
//        model = glm::rotate(model, currentFrame/10, glm::vec3(0.0f, 1.0f, 0.0f));
        lightSourceShader.setMat4("model", model);
        solarSystem_star.Draw(lightSourceShader);

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        planetShader.use();

        // Преобразования Вида/Проекции
        glm::mat4 projectionbp = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        glm::mat4 viewbp = camera.GetViewMatrix();
        planetShader.setMat4("projection", projectionbp);
        planetShader.setMat4("view", viewbp);

        // Рендеринг загруженной модели
        glm::mat4 modelbp = glm::mat4(1.0f);
        modelbp = glm::translate(modelbp, glm::vec3(0.0f, 0.0f, 0.0f)); // смещаем вниз чтобы быть в центре сцены
        modelbp = glm::scale(modelbp, glm::vec3(1.f, 1.f, 1.f));
        float rotationAngle = static_cast<float>(currentFrame)/10;
        modelbp = glm::rotate(modelbp, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 sourceLightRotationMatrix(1.0f);
        sourceLightRotationMatrix = glm::rotate(sourceLightRotationMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 currentLightSourcePosition((glm::vec4(lightPosition, 1.0f) * sourceLightRotationMatrix));

        planetShader.setVec3("sourceLightPos", currentLightSourcePosition);

        planetShader.setMat4("model", modelbp);
        solarSystem_mars.Draw(planetShader);

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        lightSourceShader.use();

        // Преобразования Вида/Проекции
        glm::mat4 projectionw = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        glm::mat4 vieww = camera.GetViewMatrix();
        lightSourceShader.setMat4("projection", projectionw);
        lightSourceShader.setMat4("view", vieww);

        // Рендеринг загруженной модели
        glm::mat4 modelw = glm::mat4(1.0f);
        modelw = glm::translate(model, lightPosition); // смещаем вниз чтобы быть в центре сцены
        modelw = glm::scale(modelw, glm::vec3(50.f, 50.f, 50.f));
        lightSourceShader.setMat4("model", modelw);
        solarSystem_milkyWay.Draw(lightSourceShader);



        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // перевернуто, так как y-координаты идут снизу вверх

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void) window;
    (void) xoffset;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
