#include<GL/glut.h>
#include<stdio.h>
#define imageWidth 50

//系统屏幕大小初始化
GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;
//窗口大小初始化
GLint window_width = 1024;
GLint window_height = 768;
//中心坐标计算
#define MIDWIDTH window_width/2
#define MIDHEIGHT window_height/2
//设置模型旋转角度
GLfloat xAngle = 0.0f;
GLfloat yAngle = 0.0f;
//设置模型初始距离
GLfloat distancex;
GLfloat distancey;
//受支持的点大小范围    
GLfloat sizes[2];
//受支持的点大小增量    
GLfloat step;

//出现的模型是茶壶
GLint flag = 1;

//线框|实体：状态默认为线框
GLint lsr = 0;

GLboolean mouseLeftDown = false;		//鼠标左键按下状态，默认是未按下

//当前鼠标位置
GLint mousex;
GLint mousey;

//默认启用光照
GLboolean lightEnable = true;

//定义光源的颜色和位置
GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };			//环境光
GLfloat position[] = { 10.0f, 3.0f, 2.0f, 0.0f };		//光源位置
GLfloat matEmission[] = { 1.0f, 1.0f, 1.0f, 1.0 };		//反射光颜色
GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };			    //漫射光
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };       //镜面反射

GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };   // 环境颜色
GLfloat mat_diffuse[] = { 0.2f, 0.5f, 0.8f, 1.0f };   // 散射颜色
GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // 镜面反射颜色
GLfloat high_shininess[] = { 100.0f };                // 镜面反射指数为100.0
GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };        // 无材质颜色

float xrot;
GLubyte stripeImage[3 * imageWidth];
//定义纹理图像
void makeStripeImage(void)
{
	int j;
	for (j = 0; j < imageWidth; j++)
	{
		stripeImage[3 * j] =255;
		stripeImage[3 * j + 1] = 255;
		stripeImage[3 * j + 2] = 255;
	}
}
/* 参数设置 */
GLfloat sgenparams[] = { 1.0, 1.0, 1.0, 0.0 };

void texture() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);//光暗处理
	makeStripeImage();//绘制纹理
	//函数设定从内存中读取纹理图并放到屏幕上的方式
	//指定内存中每个象素行起始的排列要求为字节排列
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//定义纹理环境参数：调整当前亮度和颜色信息，使之适应纹理图像
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//纹理绕转使用重复方式
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//定义纹理放大和缩小函数均为GL_LINEAR
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//定义纹理
	glTexImage1D(GL_TEXTURE_1D, 0, 3, imageWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, stripeImage);
	//控制纹理坐标的生成
	//指定单值纹理生成参数
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	//指定纹理坐标生成函数,系数由sgenparams指定
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);

	glEnable(GL_TEXTURE_GEN_S);//开启纹理坐标映射
	glEnable(GL_TEXTURE_1D);//开启纹理
}

void init() {			//光源、材质、真实感初始化

	//选择光照模型
	GLfloat local_view[] = { 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//设置环境光
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//设置漫射光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);		//设置材料反射指数（漫反射）->确定最终颜色
	//设置光源位置
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, local_view);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);  // 用来模拟物体发光的效果，但这不是光源
	//在OpenGL中，必须明确指出光照是否有效或无效。如果光照无效，则只是简单地将当前颜色映射到当前顶点上去

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

	glEnable(GL_DEPTH_TEST);		//深度测试
	//启动光照
	if (lightEnable) {
		glEnable(GL_LIGHTING);
		//启用指定光源
		glEnable(GL_LIGHT0);
	}
	else
		glDisable(GL_LIGHTING);

}

void translate()			//位移旋转操作
{
	glColor3f(0.0f, 1.0f, 1.0f);
	//glScalef(times, times, times);
	glTranslatef(distancex, distancey, 0);
	//坐标系绕x轴旋转xAngle度
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	//坐标系绕y轴旋转yAngle度  
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
}



void display()			//绘图函数
{
	init();
	//初始化窗口背景颜色
	glClearColor(0.8f, 0.8f, 0.8f, 0.75f);
	//将模板缓冲区值全部清理为1    
	glClearStencil(1);
	//开启使用模板测试并且更新模版缓存    
	glEnable(GL_STENCIL_TEST);
	//把整个窗口清理为当前清理颜色：黑色。清除深度缓冲区、模板缓冲区    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glPushMatrix()和glPopMatrix()的配对使用可以消除上一次的变换对本次变换的影响。
	//使本次变换是以世界坐标系的原点为参考点进行
	glPushMatrix();

	//进行平滑处理　    
	glEnable(GL_POINT_SMOOTH);		//点 反走样，使平滑
	glHint(GL_POINT_SMOOTH, GL_NICEST);		//指示平滑程度为最高质量
	glEnable(GL_LINE_SMOOTH);			//线反走样
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);			//多边形反走样
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	switch (flag) {
	case 1:
		translate();
		if (lsr == 0) {
			glutWireTeapot(6.5f);
		}
		else if (lsr == 1) {
			glutSolidTeapot(6.5f);
		}
		break;
	}

	//恢复压入栈的Matrix    
	glPopMatrix();
	//交换两个缓冲区的指针    
	glutSwapBuffers();
}

void setupRederingState(void) {				//渲染初始化
	//设置清理颜色为黑色    
	glClearColor(0.0f, 0.0, 0.0, 1.0f);
	//设置绘画颜色为绿色    
	glColor3f(1.0f, 1.0f, 1.0f);
	//使能深度测试    
	glEnable(GL_DEPTH_TEST);
}

void changeSize(GLint width, GLint height)			//窗口改变尺寸时调用
{
	//横宽比率    
	GLfloat ratio;
	//设置坐标系为x(-100.0f,100.0f)、y(-100.0f,100.0f)、z(-100.0f,100.0f)    
	GLfloat coordinatesize = 16.0f;
	//窗口宽高为零直接返回    
	if ((width == 0) || (height == 0))
		return;
	//设置视口和窗口大小一致    
	glViewport(0, 0, width, height);
	//对投影矩阵应用随后的矩阵操作    
	glMatrixMode(GL_PROJECTION);
	//重置当前指定的矩阵为单位矩阵　    
	glLoadIdentity();
	ratio = (GLfloat)width / (GLfloat)height;
	//正交投影    
	if (width < height)
		glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
	else
		glOrtho(-coordinatesize * ratio, coordinatesize * ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
	//对模型视图矩阵堆栈应用随后的矩阵操作    
	glMatrixMode(GL_MODELVIEW);
	//重置当前指定的矩阵为单位矩阵　    
	glLoadIdentity();
}

void menuFunc(int key)		//菜单回调函数
{
	switch (key) {
	case 1:
		lsr = 0;
		break;
	case 2:
		lsr = 1;
		break;
	case 3:
		ambient[0] = 1.0f; ambient[1] = 0.0f; ambient[2] = 0.0f;
		break;
	case 4:
		ambient[0] = 1.0f; ambient[1] = 0.0f; ambient[2] = 1.0f;
		break;
	case 5:
		ambient[0] = 0.0f; ambient[1] = 1.0f; ambient[2] = 0.0f;
		break;
	case 6:
		ambient[0] = 0.0f; ambient[1] = 0.0f; ambient[2] = 1.0f;
		break;
	case 7:
		ambient[0] = 1.0f; ambient[1] = 1.0f; ambient[2] = 1.0f;
		break;
	case 8:
		ambient[0] = 0.0f; ambient[1] = 0.0f; ambient[2] = 0.0f;
		break;
	case 9:
		lightEnable = true;
		break;
	case 10:
		lightEnable = false;
		break;
	case 11:
		position[0] -= 10.0;
		break;
	case 12:
		position[0] += 10.0;
		break;
	case 13:
		position[2] += 10.0;
		break;
	case 14:
		position[2] -= 10.0;
		break;
	}
	glutPostRedisplay();
}

void setMenu()			//设置菜单
{
	int slrMenu;
	int mainMenu;
	int colorMenu;
	int lightMenu;
	int lightPositionMenu;

	slrMenu = glutCreateMenu(menuFunc);
	glutAddMenuEntry("线框", 1);
	glutAddMenuEntry("实体", 2);

	colorMenu = glutCreateMenu(menuFunc);
	glutAddMenuEntry("红色", 3);
	glutAddMenuEntry("品红色", 4);
	glutAddMenuEntry("绿色", 5);
	glutAddMenuEntry("蓝色", 6);
	glutAddMenuEntry("白色", 7);
	glutAddMenuEntry("黑色", 8);

	lightMenu = glutCreateMenu(menuFunc);
	glutAddMenuEntry("启用", 9);
	glutAddMenuEntry("关闭", 10);

	lightPositionMenu = glutCreateMenu(menuFunc);
	glutAddMenuEntry("光源向下移动", 11);
	glutAddMenuEntry("光源向上移动", 12);
	glutAddMenuEntry("光源向外移动", 13);
	glutAddMenuEntry("光源向内移动", 14);

	mainMenu = glutCreateMenu(menuFunc);
	glutAddSubMenu("线框|实体", slrMenu);
	glutAddSubMenu("环境光颜色选择", colorMenu);
	glutAddSubMenu("启用光照", lightMenu);
	glutAddSubMenu("光照位置", lightPositionMenu);


	//将菜单链接到鼠标右键上    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyFunc(int key, int x, int y)			//键盘控制
{
	//旋转
	if (key == GLUT_KEY_UP)
		xAngle -= 5.0f;			//绕x轴顺时针转
	else if (key == GLUT_KEY_DOWN)
		xAngle += 5.0f;			//绕x轴逆时针转
	else if (key == GLUT_KEY_LEFT)
		yAngle -= 5.0f;			//绕y轴顺时针转
	else if (key == GLUT_KEY_RIGHT)
		yAngle += 5.0f;			//绕y轴逆时针转

	glutPostRedisplay();
}

void mouseFunc(int key, int state, int x, int y)			//鼠标点击事件
{

	if (key == 0)
		if (state == 0)
			mouseLeftDown = true;
		else if (state == GLUT_UP)
			mouseLeftDown = false;

}

void mouseMove(int x, int y)			//鼠标拖动则在相应位置进行旋转
{
	mousex = x;
	mousey = y;

	if ((x > 0 && x < MIDWIDTH / 2) && (y > MIDHEIGHT / 2 && y < MIDHEIGHT * 3 / 2))
		yAngle -= 1.0f;			//向上转
	if ((x<window_width && x>MIDWIDTH * 3 / 2) && (y > MIDHEIGHT / 2 && y < MIDHEIGHT * 3 / 2))
		yAngle += 1.0f;			//向下转
	if ((y > 0 && y < MIDHEIGHT / 2) && (x > MIDWIDTH / 2 && x < MIDWIDTH * 3 / 2))
		xAngle -= 1.0f;			//向左转
	if ((x > MIDWIDTH / 2 && x < MIDWIDTH * 3 / 2) && (y<window_height && y > MIDHEIGHT * 3 / 2))
		xAngle += 1.0f;			//向右转

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);//完成系统初始化工作
	//设置初始显示模式：使用 双缓冲区 、RGBA颜色模式、 深缓冲区 、 模板缓冲区
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	//设置屏幕大小为系统获取的宽度和高度
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	//设置窗口名称
	glutCreateWindow("OpenGL三维图形渲染");
	//设置窗口的大小
	glutReshapeWindow(window_width, window_height);
	//窗口居中显示
	glutPositionWindow((SCREEN_WIDTH - window_width) / 2, (SCREEN_HEIGHT - window_height) / 2);
	//设置菜单操作
	setMenu();
	//设置窗口大小改变后执行的函数
	glutReshapeFunc(changeSize);
	//显示回调函数
	glutDisplayFunc(display);
	//键鼠函数
	glutSpecialFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMove);
	//设置渲染参数
	setupRederingState();
    //纹理映射
	texture();
	//设置主循环
	glutMainLoop();
}