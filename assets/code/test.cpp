#include <stdafx.h>
#include "scene/model.h"
#include "render/prog_test.h"
#include "scene/camera.h"

//��ɫ��Դ�룺��ʹ���ļ���������
//#pragma region ������ɫ��Դ��
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in float aColor;\n"
//"layout (location = 2) in vec2 aTexCoord;\n"
//"out float outColor;\n"
//"out vec2 TexCoord;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"   outColor = aColor;\n"
//"   TexCoord = aTexCoord;\n"
//"}\0";
//#pragma endregion
//
//#pragma region ƬԪ��ɫ��Դ��
//const char* fragmentShaderSource = "#version 330 core\n"
//"#extension GL_ARB_explicit_uniform_location : enable\n"
//"out vec4 FragColor;\n"
//"in float outColor;\n"
//"in vec2 TexCoord;;\n"
//"layout (location = 0) uniform float blueValue;\n"
//"layout (location = 1) uniform sampler2D ourTexture;\n"
//"void main()\n"
//"{\n"
//"   FragColor =  mix(texture(ourTexture, TexCoord), vec4(1.0f * outColor, 0.5 * outColor, blueValue * outColor, 1.0f), 0.4);\n"
//"}\n\0";
//#pragma endregion

#pragma region ȫ�ֵ��������
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
#pragma endregion

#pragma region ���ڴ�С�仯�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//OpenGL���꣨-1��1���������ӿ����ã�ӳ�䵽��Ļ����
	glViewport(0, 0, width, height);//����ڴ������½ǵ����ꡢ����
}
#pragma endregion

#pragma region ����ƶ��ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
#pragma endregion

#pragma region �����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
#pragma endregion

#pragma region �������봦����
void processInput(GLFWwindow *window)
{
	//ÿ��������ע��ɻص�����̫�鷳������һ�����ֶ���飬ͳһ��һ������Ҳ��ֱ��
	//����״̬�ڼ��֮ǰ�ᱣ�֣������ڼ��ǰ�����Ѿ��ͷŵ����⣩
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//��������ķ����
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
#pragma endregion



int main()
{
	
	#pragma region ��ʼ��GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//OpenGL���汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ������Ҫ������������Ⱦģʽ
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Mac OS Xϵͳ��Ҫ
	#pragma endregion

	#pragma region ����GLFW����
	//���ô��ڿ��ߡ����ƣ���������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TestOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//���´����Ĵ��ڵ�����������Ϊ��ǰ�̵߳ĵ���������
	#pragma endregion

	#pragma region ��ʼ��GLAD
	//��GLAD����(��������(ϵͳ��ص�OpenGL����ָ���ַ)��)������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	#pragma endregion

	#pragma region ע��ص�����
	//ע�ᴰ�ڴ�С�仯�ص�������
	//���ڵ�һ����ʾ���С�仯ʱ�����øú�����ʵʱ�����ӿڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��������ƶ��ص�����
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//����������

	//���������ֻص�����
	glfwSetScrollCallback(window, scroll_callback);

	#pragma endregion

	#pragma region ���Է�װ�õ���ɫ��������
	ProgTest progTest = ProgTest();
	progTest.AttachShader(GL_VERTEX_SHADER, "assets/code/render/shader/test.vert");
	progTest.AttachShader(GL_FRAGMENT_SHADER, "assets/code/render/shader/test.frag");
	progTest.Link();

	#pragma region ��ע�ʹ���
	//#pragma region ���ö�����ɫ��
	//GLuint vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);
	//GLint success;
	//GLchar infoLog[512];
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//��ȡ���һ�α����Ƿ�ɹ�����Ϣ
	//if (!success)
	//{
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//��ȡ��Ϣ��־
	//	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	//}
	//#pragma endregion

	//#pragma region ����ƬԪ��ɫ��
	//GLuint fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//��ȡ���һ�α����Ƿ�ɹ�����Ϣ
	//if (!success)
	//{
	//	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);//��ȡ��ɫ��������Ϣ��־
	//	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	//}
	//#pragma endregion

	//#pragma region ����������ɫ��������ɫ������
	//GLuint shaderProgram;
	//shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);
	//glLinkProgram(shaderProgram);
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);//��ȡ���һ�������Ƿ�ɹ�����Ϣ
	//if (!success) {
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);//��ȡ��ɫ������������Ϣ��־
	//	std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	//}
	////�����ţ�ֻ�Ǳ��Ϊɾ�����ѣ�û�и������κγ���ʱ�Ż�����ɾ��
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
	//#pragma endregion
	#pragma endregion

	#pragma endregion

	#pragma region ���Է�װ�õ�ģ����

	Model testModel("assets/model/test/nanosuit.obj");

	#pragma region ��ע�ʹ���
	//#pragma region �Ѵ���ЩVBO����λ�ȡ���㣬�����ĸ�EBO����Ϣ���浽VAO��
	//GLuint VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//	#pragma region �Ѷ���������������괫�䵽�Դ棨VBO����
	//	//�����ε�������������
	//	GLfloat vertices[] = {
	//		//     ---- λ�� ----     - �������� -
	//			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // ����
	//			 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // ����
	//			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // ����
	//			-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // ����
	//	};
	//	GLuint VBO;
	//	glGenBuffers(1, &VBO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	//һ���Ӱ����ж��㴫���Դ��У���һ����һ�������Ч�ʸ�
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	#pragma endregion

	//	#pragma region ������ɫ�������Դ棨VBO���ж��붥���������������
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);//����λ��Ϊ0�Ķ������ԣ�Ĭ�Ͻ��ã�
	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//	glEnableVertexAttribArray(2);//����λ��Ϊ0�Ķ������ԣ�Ĭ�Ͻ��ã�
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);//�չ��������Դ�����ݻ��ڣ�ֻ����ʱ������������
	//	#pragma endregion

	//	#pragma region ��ͬ���Կ����в�ͬ��������Դ���ٿ�һ��VBO�����涥����ɫ������
	//	float inColor[] = {
	//		0.3f,0.6f,0.9f,0.8f
	//	};
	//	GLuint VBO2;
	//	glGenBuffers(1, &VBO2);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(inColor), inColor, GL_STATIC_DRAW);
	//	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(1);//����λ��Ϊ0�Ķ������ԣ�Ĭ�Ͻ��ã�
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);//�չ��������Դ�����ݻ��ڣ�ֻ����ʱ������������
	//	#pragma endregion

	//	#pragma region �����������EBO
	//	GLuint indices[] = {
	//		0, 1, 3, // ��һ��������
	//		1, 2, 3  // �ڶ���������
	//	};
	//	GLuint EBO;
	//	glGenBuffers(1, &EBO);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//	#pragma endregion
	
	//glBindVertexArray(0);//������ϣ����չ�������Ҫ���Ƶ�ʱ���ٰ�
	//#pragma endregion

	//#pragma region ������ͼ����������
	//GLuint texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	////Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////����ͼƬ
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);//��תY��
	//unsigned char* data = stbi_load("assets/img/picture.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);
	//#pragma endregion


	#pragma endregion

	#pragma endregion

	glEnable(GL_DEPTH_TEST);

	#pragma region ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//����ÿ֡�ļ��ʱ��
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);//�����������

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ�������Ȼ���
		
		#pragma region ����
		progTest.Use();//ʹ�����úõ���ɫ������

		//����projection����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		progTest.setMat4("projection", projection);

		//����view����
		glm::mat4 view = camera.GetViewMatrix();
		progTest.setMat4("view", view);
		
		//����model����
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		progTest.setMat4("model", model);

		testModel.Draw(progTest);//����
		#pragma endregion

		glfwSwapBuffers(window);//������ɫ����
		glfwPollEvents();//����Ƿ񴥷��¼�����������ע��Ķ�Ӧ�Ļص�����

		
	}
	#pragma endregion
	
	glfwTerminate();//�ͷ�/ɾ��֮ǰ�����������Դ
	return 0;
}