#include<GL\glut.h>
#include<iostream>
#include<cmath>
using namespace std;

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);  
    glMatrixMode(GL_PROJECTION);       
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}


void LineDDA(int x1,int y1,int x2,int y2)
{
    glColor3f(1.0, 0.0, 0.0);       // 红色
    glPointSize(2.0f);



    int dm = 0;
    if (abs(x2 - x1) >= abs(y2 - y1))
    {
        dm = abs(x2 - x1);              // x 为计长方向
    }
    else
    {
        dm = abs(y2 - y1);              // y 为计长方向
    }
    float dx = (float)(x2 - x1) / dm;   // 当 x 为计长方向，dx = 1
    float dy = (float)(y2 - y1) / dm;   // 当 y 为计长方向，dy = 1
    float x = x1;
    float y = y1;

    for (int i = 0; i < dm; ++i)
    {
        glBegin(GL_POINTS);
        glVertex2f((int)x, (int)y);
        glEnd();
        glFlush();
        x += dx;
        y += dy;
    }
    return;
}

/*
    交换两个int 类型的变量的值
*/
void swap_value(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}



// 窗口大小改变时调用的登记函数
void ChangeSize(GLsizei w, GLsizei h)
{

    if (h == 0)     h = 1;

    // 设置视区尺寸
    glViewport(0, 0, w, h);

    // 重置坐标系统
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 建立修剪空间的范围
    if (w <= h)
        glOrtho(0.0f, 250.0f, 0.0f, 250.0f * h / w, 1.0, -1.0);
    else
        glOrtho(0.0f, 250.0f * w / h, 0.0f, 250.0f, 1.0, -1.0);

}


void display(void)
{
    // 用当前背景色填充窗口，如果不写这句会残留之前的图像
    glClear(GL_COLOR_BUFFER_BIT);
    int x1, y1, x2, y2;
    do {
        cout << "请输入两点的坐标：" << endl;
        cin >> x1 >> y1 >> x2 >> y2;
    } while (0);
    LineDDA(x1,y1,x2,y2);
    return;
    
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Line");
    do {
        glutDisplayFunc(display);
    } while (0);
    glutReshapeFunc(ChangeSize);
    init();
    glutMainLoop();
    return 0;
}