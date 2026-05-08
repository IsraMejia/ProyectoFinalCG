#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	mueveMustang = 8.0f;
	mueveHelicoptero = 0.0f;
	lamparaEncendida = true;
	farolesEncendidos = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Obtener resolucion del monitor primario y calcular dimensiones
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
	
	// Ventana ocupa mitad derecha con 5% de margen arriba y abajo
	width  = (GLint)(videoMode->width  * 0.5f);  // 50% del ancho (mitad derecha)
	height = (GLint)(videoMode->height * 0.9f);  // 90% del alto (5% arriba, 5% abajo)

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	
	// Establecer color de fondo verde oscuro casi gris (RGB: 40, 50, 45)
	glClearColor(0.157f, 0.196f, 0.176f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(mainWindow);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);

	// Posicionar la ventana en la mitad derecha del monitor con 5% de margen arriba
	int monitorX, monitorY;
	glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
	int windowPosX = monitorX + (videoMode->width / 2);  // Mitad derecha
	int windowPosY = monitorY + (int)(videoMode->height * 0.05f);  // 5% de margen arriba
	glfwSetWindowPos(mainWindow, windowPosX, windowPosY);

	// Centrar el mouse en la ventana al iniciar
	glfwFocusWindow(mainWindow);
	glfwSetCursorPos(mainWindow, (double)(width / 2.0), (double)(height / 2.0));
	lastX = (GLfloat)(width / 2.0);
	lastY = (GLfloat)(height / 2.0);
	mouseFirstMoved = false;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{ 
		theWindow->muevex -= 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex += 1.0; 
	}
	 
	if (key == GLFW_KEY_J)
	{
		theWindow->mueveMustang += 1.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->mueveMustang -= 1.0;
	}
	// Helicoptero: M avanza, N retrocede
	if (key == GLFW_KEY_M)
	{
		theWindow->mueveHelicoptero += 1.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->mueveHelicoptero -= 1.0;
	}
	// Z: toggle lampara
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		theWindow->lamparaEncendida = !theWindow->lamparaEncendida;
	}
	// L: toggle faroles
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		theWindow->farolesEncendidos = !theWindow->farolesEncendidos;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = (GLfloat)xPos;
		theWindow->lastY = (GLfloat)yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = (GLfloat)xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - (GLfloat)yPos;

	theWindow->lastX = (GLfloat)xPos;
	theWindow->lastY = (GLfloat)yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
