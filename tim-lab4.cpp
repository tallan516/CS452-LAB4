#include "timShader.h"

#include "SDL2/SDL.h"

using namespace std;

GLuint abuffer;
GLuint buffer[2];
GLuint ebuffer;
GLuint program;

GLfloat pit = 1;
GLfloat yaw = 1;


GLfloat vertices[] = {	-5.0f,-5.0f,-5.0f,	//0 Left, Bottom, Far
				5.0f,-5.0f,-5.0f,		//1 Right, Bottom, Far
				5.0f,-5.0f,5.0f,		//2 Right, Bottom, Near
				-5.0f,-5.0f,5.0f,		//3 Left, Bottom, Near
				-5.0f,5.0f,-5.0f,		//4 Left, Top, Far
				5.0f,5.0f,-5.0f,		//5 Right, Top, Far
				5.0f,5.0f,5.0f,		//6 Right, Top, Near
				-5.0f,5.0f,5.0f	};	//7 Left, Top, Near


				//R, B, G, A (transparency)
GLfloat colors[] = {	1.0f,0.0f,0.0f,1.0f,		//0 Red
				1.0f,1.0f,0.0f,1.0f,		//1 Purple
				0.0f,1.0f,0.0f,1.0f,		//2 Blue
				0.0f,1.0f,1.0f,1.0f,		//3 Cyan
				0.0f,0.0f,1.0f,1.0f,		//4 Green
				1.0f,0.0f,1.0f,1.0f,		//5 Yellow
				0.0f,0.0f,0.0f,1.0f,		//6 Black
				1.0f,1.0f,1.0f,1.0f	};	//7 White

GLubyte elems[] = {	2,3,0,1,2,0,		//Bottom
				5,4,7,6,5,7,		//Top
				6,7,3,2,6,3,		//Front
				4,5,1,0,4,1,		//Back
				7,4,0,3,7,0,		//Left
				5,6,2,1,5,2		};	//Right


//Declare functions
void init();
void display(SDL_Window* window);
void input(SDL_Window* window);


int main(int argc, char **argv)
{
	//Main method stuff
	SDL_Window *window;	//SDL window
	if(SDL_Init(SDL_INIT_VIDEO) < 0)	//Tries to intitiate SDL
	{
		cerr << "Error, cannot initialize SDL." << endl;
		SDL_Quit();	//Close out of SDL
		exit(0);	//End program
	}

	//Create window
	window = SDL_CreateWindow("CS452-LAB4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL);

	//Check window creation
	if(window == NULL)
	{
		cerr << "Error, cannot create window." << endl;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);	//Creates opengl context associated with the window
	glewInit();	//Initializes glew

	init();	//Calls function to initialize the shaders and set up buffers

	//Keep looping through to make sure
	while(true)
	{
		input(window);	//Check keyboard input
		display(window);	//Render
	}

	//Close out of SDL stuff
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	glFrontFace(GL_CW);

	return 0;
}

void display(SDL_Window* window)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	//Clears the frame buffer

	glm::mat4 trans;	//Matrix for transformations

	trans = glm::rotate(trans, pit, glm::vec3(1, 0, 0));	//rotate the cube around the x axis
	trans = glm::rotate(trans, yaw, glm::vec3(0, 1, 0));	//rotate the cube arround the y axis
	
	GLint tempLoc = glGetUniformLocation(program, "modelMatrix");	//Matrix that handles the transformations
	glUniformMatrix4fv(tempLoc, 1 ,GL_FALSE,&trans[0][0]);
	
	GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat light1_dir[] = {-10.0f, -10.0f, -10.0f};	//Light pointing at top right near corner
	GLfloat light1_color[] = {0.1f, 1.0f, 0.1f};
	GLfloat light2_dir[] = {10.0f, -10.0f, -10.0f};		//Light pointing at top left near corner
	GLfloat light2_color[] = {0.1f, 0.1f, 1.0f};
	
	tempLoc = glGetUniformLocation(program,"ambient");
	glUniform4fv(tempLoc, 1, ambient);
	tempLoc = glGetUniformLocation(program, "light1Color");
	glUniform3fv(tempLoc, 1, light1_color);
	tempLoc = glGetUniformLocation(program, "light1Dir");
	glUniform3fv(tempLoc, 1, light1_dir);
	tempLoc = glGetUniformLocation(program, "hVector1");
	glUniform3fv(tempLoc, 1, light1_dir);
	tempLoc = glGetUniformLocation(program, "light2Color");
	glUniform3fv(tempLoc, 1, light2_color);
	tempLoc = glGetUniformLocation(program, "light2Dir");
	glUniform3fv(tempLoc, 1, light2_dir);
	tempLoc = glGetUniformLocation(program, "hVector2");
	glUniform3fv(tempLoc, 1, light2_dir);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
	glFlush();	//Makes sure all data is rendered as soon as possible
	SDL_GL_SwapWindow(window);	//Updates the window
}

void input(SDL_Window* window)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))	//Handling the keyboard
	{	
		if(event.type == SDL_QUIT)
		{
			exit(0);
		}
		else if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: exit(0);
			case SDLK_i: pit+=2; break;
			case SDLK_k: pit-=2; break;
			case SDLK_j: yaw+=2; break;
			case SDLK_l: yaw-=2; break;
			}
		}
	}
}

void init()
{
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_COLOR_MATERIAL);
      glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
      
	program = glCreateProgram();	//Creates program
	initShaders(program);	//Calls the initialize shader function in the header file

	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	glGenBuffers(2, buffer);

	//Sets up pointers and stuff
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &ebuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

	//Enables vertex arrays to draw stuff
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
