#include <windows.h>//包括大部分的Win32 API函数，以及一些Win32宏 
#include <GL/glut.h> //包含GL头文件
#include<cmath>
#include <iostream>
#include <string>
#include <map>
#include"Draw_man.h"
#define BUFFER_LENGTH 64 //select模式缓存区大小
using namespace std;
//不显示控制台窗口  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

GLdouble theta = 0;
GLint width=800, height=500;
int selected_part = -1,press_x=0,press_y=0;
int Cur_x, Cur_y;
Man *man;
double trans_theta(double theta) {
	return theta / 180 * acos(-1);
}
void rotate_xy(int &x, int &y) {
	printf("Init x %d y %d theda %.3lf\n", x, y,theta);
	//y = 500-y;
	//x -= 400; y -= 250;
	double tt = trans_theta(theta);
	printf("tt cos(%.2lf) = %.2lf\n", tt, cos(tt));
	int newx = cos(tt)*x + sin(tt)*y;
	int newy = -sin(tt)*x + cos(tt)*y;
	//x += 400; y += 250;
	//y = 500 - y;
	x = newx, y = newy;
	printf("Trans x %d y %d\n", x, y);
}

//绘制所有物体
void DrawObjects() {
	man->draw();
}
//绘制函数
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);// 清除屏幕及深度缓存设置 
	
	glLoadIdentity();//将当前的用户坐标系的原点移到了屏幕中心

	glTranslatef(400, 250, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);//绕系统原点
	glTranslatef(-400, -250, 0.0);

	DrawObjects();
	glutSwapBuffers(); //强制系统立刻在屏幕上显示输出图形(双缓冲)
	printf("myDisplay: Width:%d Height:%d\n", width, height);
}
//初始化mp和remap，清空面板
void myInit() {
	man = new Man(width, height);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
//改变窗口大小时、初始化时调用
void myRreshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	man->Reshape(w, h);
	//开启反走样  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	
}
//设置旋转角度
void myRotate(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		theta = theta + 1;
		break;
	case GLUT_KEY_DOWN:
		theta = theta - 1 + 360;
		break;
	}
	if (theta > 360)theta -= 360;

	//cout << "myRotate " << theta << endl;
	glutPostRedisplay();
}
//处理hits结果，返回选择的部分
int ProcessHits(GLint hits, GLuint selectBuff[]) {
	if (hits > 0)
	{

		int Choose = selectBuff[3];
		int depth = selectBuff[1];
		
		//printf("Hit 1 -  Choose: %d   depth: %d\n", Choose, depth);
		for (int loop = 1; loop<hits; loop++)
		{
			Choose = selectBuff[loop * 4 + 3];
			depth = selectBuff[loop * 4 + 1];
			//printf("Hit %d  Choose  %d   depth: %d\n", loop + 1, Choose, depth);
		}
		
		printf("FinalHit -  number: %d   Choose: %d  Name: ", hits, Choose);
		cout << man->GetRemp(Choose) << endl;
		puts("");//和下一次输出隔开
		//cout << man->remp[Choose] << endl;
		//ProcessPlanet(GLuint(Choose));
		return Choose;
	}
	return -1;
}
//处理鼠标点击，找到对应的部分
int ProcessSelection(GLint x, GLint y) {
	static GLuint selectBuff[BUFFER_LENGTH];
	GLint hits, viewport[4];
	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glRenderMode(GL_SELECT);//切换到select模式

	glLoadIdentity();
	gluPickMatrix(x, viewport[3] - y + viewport[1], 3, 3, viewport);//3个像素的偏差

	printf("View - Click on x: %d y: %d\n", x, viewport[3] - y + viewport[1]);

	gluOrtho2D(0, 800, 0, 500);
	//glutPostRedisplay();
	DrawObjects();
	hits = glRenderMode(GL_RENDER);
	//printf("Hits %d\n", hits);

	int res = ProcessHits(hits, selectBuff);//处理，找出选择对象

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();//还原

	glMatrixMode(GL_MODELVIEW);

	return res;
}
//鼠标响应  
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("Window - LeftClick on x: %d y: %d\n", x, y);
		press_x = x; press_y = y;
		selected_part = ProcessSelection(x, y);

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		
		int dx= x - press_x, dy = press_y - y;
		rotate_xy(dx, dy);
		man->AddBaseXY(selected_part, dx, dy);
		
		//man->AddBaseXY(selected_part, x - press_x, press_y - y);
		selected_part = -1;
	}
	/*
	GLuint selectBuf[20];
	GLint hits;
	GLint viewport[4];

	//进行颜色选择
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_down = false;
		// 创建名称缓冲区
		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(20, selectBuf);
		glRenderMode(GL_SELECT);
		// 初始化名称栈
		glInitNames();
		glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 3.0, 3.0, viewport);
		gluOrtho2D(0, 800, 0, 500);
		drawObjects(GL_SELECT);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		hits = glRenderMode(GL_RENDER);    //绘制模式(GL_RENDER),选择模式(GL_SELECT),反馈模式(GL_FEEDBACK)
		cout << hits << endl;
		processHits(hits, selectBuf);
		glutPostRedisplay();
	}
	*/
}

//鼠标摁下拖动时响应
void myMouseMove(int x, int y) {
	printf("MouseMove Choose:%d x:%d y:%d\n", selected_part, x, y);
	if (~selected_part) {
		
		int dx = x - press_x, dy = press_y - y;
		rotate_xy(dx, dy);
		man->ChangeMoveXY(selected_part, dx, dy);
		
		//man->ChangeMoveXY(selected_part, x - press_x, press_y - y);
	}
	glutPostRedisplay();
}
//不按下时拖动
void myMousePassiveMove(int x, int y) { 
	Cur_x = x, Cur_y = y; 
}
//处理每个菜单
void main_menu(int index) {
	printf("Cur_x: %d Cur_y: %d\n", Cur_x, Cur_y);
	selected_part = ProcessSelection(Cur_x, Cur_y);
	printf("index: %d Choose: %d\n", index, selected_part);
	if (index == -1)return;
	man->ChangeColor(selected_part, index);
	selected_part = -1;
	glutPostRedisplay();
}
//添加菜单
void addMenu() {
	//右击事件  
	glutCreateMenu(main_menu);
	glutAddMenuEntry("选择颜色", -1);
	glutAddMenuEntry("黑色", 0);
	glutAddMenuEntry("红色", 1);
	glutAddMenuEntry("绿色", 2);
	glutAddMenuEntry("蓝色", 3);
	glutAddMenuEntry("青色", 4);
	glutAddMenuEntry("紫色", 5);
	glutAddMenuEntry("黄色", 6);
	glutAddMenuEntry("白色", 7);
	glutAddMenuEntry("肉色", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInit(&argc, argv); // 初始化OPENGL
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("卡通人物"); //建立窗口，并设置窗口标题    
	myInit();
	glutReshapeFunc(myRreshape);
	glutDisplayFunc(myDisplay);   //调用回调显示函数  
	glutMouseFunc(myMouse);
	glutSpecialFunc(myRotate);
	glutMotionFunc(myMouseMove);
	glutPassiveMotionFunc(myMousePassiveMove);
	addMenu();
	glutMainLoop(); //主函数循环
	return 0;
}