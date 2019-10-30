#include <stdafx.h>
#include "engine.h"

GLboolean Engine::firstMouse = true;
GLfloat Engine::lastX = 0.0f;
GLfloat Engine::lastY = 0.0f;
GLfloat Engine::deltaTime = 0.0f;
GLfloat Engine::lastFrame = 0.0f;

std::unique_ptr<Engine>& Engine::Instance()
{
	static std::unique_ptr<Engine> instance = nullptr;

	if (!instance)
	{
		instance.reset(new Engine());
	}

	return instance;
}

Engine::Engine()
{
	//��ʼ��
	Initialize();
		
	//����������Դ
	AssetsManager::Instance();

	//���ûص�����
	SetCallback();
}

GLFWwindow* Engine::Window()
{
	if (window != nullptr)
	{
		return window;
	}
}

void Engine::Initialize()
{
	//��ʼ��GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//OpenGL���汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ������Ҫ������������Ⱦģʽ


	//����GLFW����
	window = glfwCreateWindow(800, 600, "TestOpenGL", NULL, NULL);
	if (window == NULL)
	{
		throw 0;
	}
	glfwMakeContextCurrent(window);//���´����Ĵ��ڵ�����������Ϊ��ǰ�̵߳ĵ���������
	
	//��ȡ��Ļλ��
	GLint width, height;
	glfwGetWindowSize(window, &width, &height);
	lastX = width / 2;
	lastY = height / 2;


	//��ʼ��GLAD
	//��GLAD����(��������(ϵͳ��ص�OpenGL����ָ���ַ)��)������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw 1;
	}
}

void Engine::SetCallback()
{
	//ע�ᴰ�ڴ�С�仯�ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��������ƶ��ص�����
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//����������

	//���������ֻص�����
	glfwSetScrollCallback(window, scroll_callback);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//OpenGL���꣨-1��1���������ӿ����ã�ӳ�䵽��Ļ����
	glViewport(0, 0, width, height);//����ڴ������½ǵ����ꡢ������
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	Camera::Active()->ProcessMouseMovement(xoffset, yoffset);
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::Active()->ProcessMouseScroll(yoffset);
}

void Engine::processInput(GLFWwindow* window)
{
	//ÿ��������ע��ɻص�����̫�鷳������һ�����ֶ���飬ͳһ��һ������Ҳ��ֱ��
	//����״̬�ڼ��֮ǰ�ᱣ�֣������ڼ��ǰ�����Ѿ��ͷŵ����⣩
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	auto& camera = Camera::Active();

	//��������ķ����
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
}





void Engine::RenderLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		//����ÿ֡�ļ��ʱ��
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);//������������

		AssetsManager::Instance()->renderers["Voxelization"]->Render();

		glfwSwapBuffers(window);//������ɫ����
		glfwPollEvents();//����Ƿ񴥷��¼�����������ע��Ķ�Ӧ�Ļص�����

	}
}

void Engine::Terminate()
{
	delete Instance().release();
}

Engine::~Engine()
{
	AssetsManager::Terminate();
}
