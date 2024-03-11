#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 400

//VAO es el encapsulamienot de la info en forma de variables que se pasa a través de la tarjeta gráfica (POO -> un vao es un objeto)
//Un VAO es un modelo, encapsula N vertices
//VBO son las varaiables/atributos del objeto que pertenecen a cada VAO, cada vertice de un VAO tiene un VBO

struct ShaderProgram 
{
	GLuint vertexShader = 0;
};

//Función para cargar un archivo
std::string LoadFile(const std::string& filepath)
{
	//Cargamos archivo
	std::ifstream file(filepath);
	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se abre
	if (!file.is_open())
	{
		std::cerr << "No se ha podido abrir el archivo" << filepath << std::endl;
		std::exit(EXIT_FAILURE);

	}
	//Leemos el contenido
	while (std::getline(file, line))
	{
		fileContent += line + "\n";

	}

	file.close();
	return fileContent;
	
	//std::exit(EXIT_FAILURE);

}

GLuint LoadVertexShader(const std::string& file)
{
	//Crear un espacio para un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Usamos la función creada anteriormente para leer el vertex shader y almacenarlo
	std::string sShaderCode = LoadFile(file);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	//Compila el vertex shader
	glCompileShader(vertexShader);
	
	//Verificar el estado del shader compilado
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//Si la compilación ha sido exitosa devolvemos la referencia del vector shader
	if (success) {
		return vertexShader;
	}
	else{
		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());
		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el programa: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);

	}
}

void ResizableWindow(GLFWwindow * window, int _iFrameBufferWidth, int _iFrameBufferHeight)
{
	glViewport(0, 0, _iFrameBufferWidth, _iFrameBufferHeight);
}


GLuint CreateProgram(const ShaderProgram& shaders)
{
	//Creamos programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar si existe algun vertex shader
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}
	//Linkear el program
	glLinkProgram(program);

	//Obtener el estado dle programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(success)
	{
		//Liberamos recursos
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}
		return program;
	
	}
	else
	{
		//Obtenemos longitud del log
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());
		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);

	}
}


void main()
{
	// Inicializa la semilla del generador de números aleatorios
	srand(static_cast<unsigned>(time(NULL)));

	int iFrameBufferWidth = 0;
	int iFrameBufferHeight = 0;

	//std::cout << LoadFile("DELETEME.txt") << std::endl;

	//Window config
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Init window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos function de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, ResizableWindow);

	//Obtenemos tamaño del frame buffer para asignarlo al viewport (front buffer)
	//glfwGetFramebufferSize(window, &iFrameBufferWidth, &iFrameBufferHeight);

	//Definimos tamaño del viewport (front buffer)
	//glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Definimos que GLEW use funcoiones experimentales
	glewExperimental = GL_TRUE;
	//Activa el culling
	glEnable(GL_CULL_FACE);
	//Indicamos que la cara que queremos ocultar es la trasera
	glCullFace(GL_BACK);

	if(glewInit()==GLEW_OK)
	{
		//Compilar los shaders
		ShaderProgram myFirstProgram;
		myFirstProgram.vertexShader	= LoadVertexShader("MyFirstVertexShader.glsl");

		//Compilar el programa
		GLuint myFirstCompileProgram;
		myFirstCompileProgram = CreateProgram(myFirstProgram);

		//Obtengo referencia a la variable uniform
		GLint offsetReference = glGetUniformLocation(myFirstCompileProgram,"offset");


		//Definimos color para limpiar el buffer de color
		glClearColor(1.f, 1.f, 1.f, 1.f);


		//Definimos mi VAO y VBO
		GLuint vaoPuntos, vboPuntos, vboAleatorios;
		//Defino la cantidad de VAOs a crear
		glGenVertexArrays(1, &vaoPuntos);
	

		//Indico que el VAO se active para trabajar 
		glBindVertexArray(vaoPuntos);

		//Definimos la cantidad de VBOs a crear
		glGenBuffers(1, &vboPuntos);
		glGenBuffers(1, &vboAleatorios);

		//Indico el VBO activo
		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos);
		

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		//Posicion X e Y
		GLfloat punto[] = 
		{ 
			-0.5f,-0.5f,
			+0.5f,-0.5f,
			-0.5f,+0.5f,
			+0.5f,+0.5f,
			
		};
		//---------PRIMER VBO-----------
		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(punto),punto,GL_STATIC_DRAW);

		//Indicamos donde almacenar y como distribuir la info
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//DesvinculamosVBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//---------SEGUNDO VBO-----------
		//Crear offset para cada vertice
		GLfloat randomData[12];
		for(int i=0; i<12; i++)
		{
			randomData[i] = static_cast<GLfloat>(rand()) / RAND_MAX * 0.5f;
		}
		
		//Indicamos que le nuevo VBO activo es el aleatorio
		glBindBuffer(GL_ARRAY_BUFFER, vboAleatorios);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(randomData), randomData, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como distribuir la info
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		
		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(1);

		//Desvinculamos el segundo VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		//Indicamos a la tarjeta gfráfica que programa usar
		glUseProgram(myFirstCompileProgram);

		//Declaro la variable
		glm::vec2 offset = glm::vec2(0.0f, 0.0f);

		//Asigno valor a la variable
		glUniform2fv(offsetReference, 1, &offset[0]);

		//Game loop
		while(!glfwWindowShouldClose(window))
		{
			//Pulleamos eventos (botones, teclas)
			glfwPollEvents();

			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Definimos que equeremos usar el vao con los puntos
			glBindVertexArray(vaoPuntos);

			//Definimos que queremos dibujar
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//Dejamos de usar el VAO
			glBindVertexArray(0);

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		
		}
		
		//Desactivamos el programa
		glUseProgram(0);
		glDeleteProgram(myFirstCompileProgram);
	}
	else 
	{
		std::cout << "no me vas a copiar xd";
		glfwTerminate();
	}

}