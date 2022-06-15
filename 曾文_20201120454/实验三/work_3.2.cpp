#include<GL/glut.h>
#include<iostream>
#include<math.h>
using namespace std;

GLfloat pointsize = 1.0f;

void drawOneLine(GLint x, GLint y, GLint x1, GLint y1) {
	GLint a = x;
	GLfloat m = (y1 - y) * 1.0 / (x1 - x);       //斜率
	GLfloat b = y - m * x;
	GLfloat thethay = m * a + b - y;             //thetha y
	GLfloat d0 = 2 * thethay - 1;              //初始化d0
	glPointSize(pointsize);
	GLint cx = x, cy = y;
	glVertex2i(x, y);                         //画第一个点
	while (a <= x1) {
		a++;
		thethay = m * a + b - cy;                //更新thetha y
		if (d0 <= 0) {                          //更新d0
			d0 += 2 * thethay;
			cy = cy;
		}
		else {
			d0 += 2 * thethay - 2;
			cy = cy + 1;
		}
		glVertex2i(a, cy);
	}

}


void display(void) {          //设置圆的半径
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0f, 0.0f);
	glBegin(GL_POINTS);

	drawOneLine(2, 2, 400, 800);
	

	glEnd();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Bresenhamplus");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-500.0, 800.0, -500.0, 800.0);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}