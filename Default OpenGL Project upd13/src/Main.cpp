#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <streambuf>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#include "MyShaders.h"
#include "MyHexagon.h"
#define LOG(x) std::cout << x << std::endl;
#define PI() 3.14159265f
#define Sound() PlaySound(TEXT("mix3.wav"), NULL, SND_ASYNC);

//some variables for circle positioning and color:

float increment = 0.05f;
float incrPos = 0.05f;
float positions[724];
unsigned int indices[362];
unsigned int buffer; //id of buffer
unsigned int ibo;
float IncX = 0.0f;
float IncY = 0.0f;
float scale = 1.0f;
short length = 1360;
short heigth = 768;
float ratio = (float)length / (float)heigth;
float R;
int location;
float RED[4] = { 0.0f , 0.0f , 0.3f, 0.5f };
float GREEN[4] = { 0.0f , 0.0f , 0.8f, 0.3f };
float BLUE[4] = { 0.6f , 0.0f , 0.2f, 0.6f };
float scaleHex = 0.02f;

void drawArrays(
	bool isHexagon,
	int locationVar,
	float redVar, float greenVar, float blueVar,
	int arrayType,
	unsigned int nVert
)

{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	isHexagon ?
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionsHex), positionsHex, GL_STATIC_DRAW) :
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2,
		0);

	glUniform4f(locationVar, redVar, greenVar, blueVar, 1.0f);
	glDrawArrays(arrayType, 0, nVert);
	glDeleteBuffers(1,
		&buffer);
}
//this function will draw a circle:

class RectangleClass
{
public:
	float a;
	float abRatio;
	float angle;
	float positionsRec[8];
};
RectangleClass shot;

void drawShot(
	float centerX, float centerY,
float aVar,
float abRatioVar,
float angleVar
)
{
	shot.a = aVar;
	shot.abRatio = abRatioVar;
	shot.angle = angleVar;
	shot.positionsRec[0] = 0.5f * shot.a;
	shot.positionsRec[1] = 0.5f * shot.a/ shot.abRatio;

	shot.positionsRec[2] = -0.5f * shot.a;
	shot.positionsRec[3] = 0.5f * shot.a / shot.abRatio;

	shot.positionsRec[4] = 0.5f * shot.a;
	shot.positionsRec[5] = -0.5f * shot.a / shot.abRatio;

	shot.positionsRec[6] = -0.5f * shot.a;
	shot.positionsRec[7] = -0.5f * shot.a / shot.abRatio;

	for (short i = 0; i < 4; i++)
	{
		positionsHex[2 * i] = shot.positionsRec[2 * i] * cos(shot.angle)
			- shot.positionsRec[2 * i + 1] * sin(shot.angle) ;
		positionsHex[2 * i] = positionsHex[2 * i] / ratio + centerX;

		positionsHex[2 * i + 1] = shot.positionsRec[2 * i] * sin(shot.angle)
			+ shot.positionsRec[2 * i + 1] * cos(shot.angle) + centerY;
		
	}
	drawArrays(
		1,
		location,
		1.0f, 1.0f, 1.0f,
		GL_TRIANGLE_STRIP,
		4
	);
}

void circle(float centerX, float centerY,
	float radius,
	int locationVar,
	bool isHexagon,
	unsigned short elementNumber)
{
	positions[0] = centerX;
	positions[1] = centerY;

	for (int i = 0; i < 361; i++)
	{
		positions[(i + 1) * 2] = cos(PI() / 180 * i) * radius * scale / ratio + centerX;
		positions[2 * (i + 1) + 1] = sin(PI() / 180 * i) * radius * scale + centerY;
	}

	drawArrays(
		isHexagon,
		locationVar,
		RED[elementNumber], GREEN[elementNumber], BLUE[elementNumber],
		GL_TRIANGLE_FAN,
		362
	);

}

float digitPos[2][14]
=
{
	{
	-0.5f, 1.0f,
	0.5f, 1.0f,
	-0.5f, -1.0f,
	0.5f, -1.0f

	}
	,
	{
		0.0f, 2.0f,
		0.0f, 0.0f,
		0.0f, -2.0f,
		NULL, NULL
	}
};
void hexagon1(
	float centerX, float centerY,
	float scaleVar,
	short hexagonNum,
	bool vh
)
{
	hexagon(
		centerX + scaleHex * 1.2f * digitPos[vh][2 * hexagonNum],
		centerY + scaleHex * 1.1f  * digitPos[vh][2 * hexagonNum + 1],
		location,
		0.75f,
		1.0f,
		ratio,
		vh,
		scaleHex
	);
	drawArrays(
		1,
		location,
		RED[2], GREEN[2], BLUE[2],
		GL_TRIANGLE_STRIP,
		6
	);
}
void zero(float centerX, float centerY)
{
	for (short i = 0; i < 4; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			0
		);
	}
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			2 * i,
			1
		);
	}
}

void one(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			2 * i + 1,
			0
		);
	}
}

void two(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i + 1,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
}

void three(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			2 * i + 1,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
}

void four(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			2 * i + 1,
			0
		);
	}

	hexagon1(
		centerX, centerY,
		scaleHex,
		0,
		0
	);
	hexagon1(
		centerX, centerY,
		scaleHex,
		1,
		1
	);
}

void five(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i * 3,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
}

void six(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i * 3,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
	hexagon1(
		centerX, centerY,
		scaleHex,
		2,
		0
	);
}

void seven(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			2 * i + 1,
			0
		);
	}
	hexagon1(
		centerX, centerY,
		scaleHex,
		0,
		1
	);
}

void eight(float centerX, float centerY)
{
	for (short i = 0; i < 4; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
}

void nine(float centerX, float centerY)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			0
		);
	}
	hexagon1(
		centerX, centerY,
		scaleHex,
		3,
		0
	);

	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			centerX, centerY,
			scaleHex,
			i,
			1
		);
	}
}

void digit(short digitVar, float centerX, float centerY)
{
	switch (digitVar)
	{
	case 0:zero(centerX, centerY);
		break;
	case 1:one(centerX, centerY);
		break;
	case 2:two(centerX, centerY);
		break;
	case 3:three(centerX, centerY);
		break;
	case 4:four(centerX, centerY);
		break;
	case 5:five(centerX, centerY);
		break;
	case 6:six(centerX, centerY);
		break;
	case 7:seven(centerX, centerY);
		break;
	case 8:eight(centerX, centerY);
		break;
	case 9:nine(centerX, centerY);
		break;
	}
}

void hits(std::string strVar)
{
	for (short i = 0; i < strVar.length(); i++)
		digit((short)strVar.at(i) - 48, 0.9f + i * 0.035f, 0.9f);
}

//main function. creating window, context, etc.:
int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(length, heigth, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "error" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	for (int i = 0; i < 362; i++)
	{
		indices[i] = i;
	}
	ossvs << ssvs.rdbuf();
	ossfs << ssfs.rdbuf();

	vertexShader = ossvs.str();
	fragmentShader = ossfs.str();
	
	double valueDCX, valueDCY;

	float CX[4] = { 0.0f, 0.0f, 0.0f };
	float CY[4] = { 0.0f, 0.0f, 0.0f };
	R = 0.03f;
	const float IncX = 0.04f/ratio;
	const float IncY = 0.04f;
	bool check[6] = { true, false, false, false, false, false };
	bool checkIfLMBUp = true;
	bool checkIfPressedOnce = false;
	short hitCounter = 0;
	std::string hitCounterString;
	hitCounterString = std::to_string(hitCounter);
	short wait = 15;
	short waitProjectile = 0;
	int intRnd;
	float flRndX, flRndY;
	float projectileSpeed=0.03f;
	float angle = 0.0f;
	bool angleQ = 0;
	unsigned int  shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	location = glGetUniformLocation(shader, "u_Color");
	glfwGetPrimaryMonitor();
	std::cout << scale << std::endl;
	std::cout << cos(0.0f) << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		check[4] = false;
		checkIfPressedOnce = false;
		glfwSetInputMode(window,
			GLFW_CURSOR,
			GLFW_CURSOR_NORMAL
		);
		// 5) timer:
		if (check[0])
		{
			wait++;
		}
		if (wait > 25)
		{
			check[1] = false;
			check[0] = false;
			wait = 0;
		}

		//controls:

		GLFWmousebuttonfun(window) GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, NULL);
		


		if (check[5])
		{
			waitProjectile++;
		}
		if (waitProjectile > 20)
			waitProjectile = 0;
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			check[2] = true;
			if (checkIfLMBUp == true)
			{
				checkIfPressedOnce = true;
				check[5] = true;
			}
				
		}
		else
		{
			check[2] = false;
			checkIfLMBUp = false;
		}

		if (checkIfPressedOnce)
			std::cout << 'nice' << std::endl;

		glfwGetCursorPos(window,
			&valueDCX,
			&valueDCY
		);

		CX[0] = (float)((valueDCX - 680) / 680);
		CY[0] = (float)(-(valueDCY - 384) / 384);
		//std::cout << valueDCX <<' '<< valueDCY<<' ' << CX << ' ' << CY << std::endl;
		//std::cout << (short)hitCounterString.at(0) << std::endl;
		if (RED[0] > 1.0f)
			increment = -0.05f;
		if (RED[0] < 0.0f)
			increment = 0.05f;
		RED[0] = RED[0] + increment;
		glClear(GL_COLOR_BUFFER_BIT);
		angleQ = (CX[0] < CX[1]);
		if (check[4])
		{
			angle = atan((CY[0] - CY[1]) / (CX[0] * ratio - CX[1] * ratio)) + angleQ * PI();
			CX[3] = CX[1] + 0.25f * cos(angle) / ratio;
			CY[3] = CY[1] + 0.25f * sin(angle);
			
		}
			
		if(check[4])
		drawShot(
			CX[1] + 0.25f * cos(angle) / ratio,
			CY[1] + 0.25f * sin(angle),
			0.2f,
			2.0f,
			atan((CY[0] - CY[1])/(CX[0]*ratio - CX[1] * ratio))
		);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && CY[1] < 1.0f - 0.13f)
			CY[1] = CY[1] + IncY;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && CY[1] > -1.0f + 0.13f)
			CY[1] = CY[1] - IncY;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && CX[1]  > -1.0f + 0.13f / ratio)
			CX[1] = CX[1] - IncX;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && CX[1]  < 1.0f - 0.13f / ratio)
			CX[1] = CX[1] + IncX;

		circle(CX[1], CY[1], 0.13f, location, 0, 3);

		hits(hitCounterString);

		// 2) generate random coordinates for "lootable" circle:
		if (!check[1])
		{
			flRndX = ((float)((rand() & 80) - (rand() & 80))) / 100.0f;
			flRndY = ((float)((rand() & 80) - (rand() & 80))) / 100.0f;
			check[1] = true;
		}

		// 3) draw "lootable" circle:
		if (!check[0])
		{
			circle(flRndX, flRndY, 0.13f, location, 0, 1);
		}

		circle(CX[0], CY[0], 0.06f, location, 0, 0);

		//std::cout << CX[0] << ' ' << CX[1] <<' '<< 0.5f * (CX[0] + CX[1])<< std::endl;
		//check pressed mouse left button
		

		// 4) check collision of circles:
		if (1.0f > (((flRndX - CX[0] / 1) / (0.1f / ratio)) * ((flRndX - CX[0] / 1) / (0.1f / ratio)) +
			((flRndY - CY[0]) / 0.1f) * ((flRndY - CY[0]) / 0.1f)) && check[2])
		{
			check[2] = false;
			if (!check[0])
			{
				PlaySound(TEXT("mix3.wav"), NULL, SND_ASYNC);
				hitCounter++;
				hitCounterString = std::to_string(hitCounter);
			}

			check[0] = true; //llotable circle won't be drawn
		}

		//calling a circle function( a circle will be drawn):

		GREEN[1] = GREEN[1] + increment;
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}