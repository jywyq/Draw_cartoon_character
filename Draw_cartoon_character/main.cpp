#include <windows.h>//�����󲿷ֵ�Win32 API�������Լ�һЩWin32�� 
#include <GL/glut.h> //����GLͷ�ļ�
#include<cmath>
#include <iostream>
#include <string>
#include <map>
#include"Draw_man.h"
#define BUFFER_LENGTH 64 //selectģʽ��������С
using namespace std;
//����ʾ����̨����  
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

//������������
void DrawObjects() {
	man->draw();
}
//���ƺ���
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);// �����Ļ����Ȼ������� 
	
	glLoadIdentity();//����ǰ���û�����ϵ��ԭ���Ƶ�����Ļ����

	glTranslatef(400, 250, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);//��ϵͳԭ��
	glTranslatef(-400, -250, 0.0);

	DrawObjects();
	glutSwapBuffers(); //ǿ��ϵͳ��������Ļ����ʾ���ͼ��(˫����)
	printf("myDisplay: Width:%d Height:%d\n", width, height);
}
//��ʼ��mp��remap��������
void myInit() {
	man = new Man(width, height);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
//�ı䴰�ڴ�Сʱ����ʼ��ʱ����
void myRreshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	man->Reshape(w, h);
	//����������  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	
}
//������ת�Ƕ�
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
//����hits���������ѡ��Ĳ���
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
		puts("");//����һ���������
		//cout << man->remp[Choose] << endl;
		//ProcessPlanet(GLuint(Choose));
		return Choose;
	}
	return -1;
}
//������������ҵ���Ӧ�Ĳ���
int ProcessSelection(GLint x, GLint y) {
	static GLuint selectBuff[BUFFER_LENGTH];
	GLint hits, viewport[4];
	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glRenderMode(GL_SELECT);//�л���selectģʽ

	glLoadIdentity();
	gluPickMatrix(x, viewport[3] - y + viewport[1], 3, 3, viewport);//3�����ص�ƫ��

	printf("View - Click on x: %d y: %d\n", x, viewport[3] - y + viewport[1]);

	gluOrtho2D(0, 800, 0, 500);
	//glutPostRedisplay();
	DrawObjects();
	hits = glRenderMode(GL_RENDER);
	//printf("Hits %d\n", hits);

	int res = ProcessHits(hits, selectBuff);//�����ҳ�ѡ�����

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();//��ԭ

	glMatrixMode(GL_MODELVIEW);

	return res;
}
//�����Ӧ  
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

	//������ɫѡ��
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_down = false;
		// �������ƻ�����
		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(20, selectBuf);
		glRenderMode(GL_SELECT);
		// ��ʼ������ջ
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
		hits = glRenderMode(GL_RENDER);    //����ģʽ(GL_RENDER),ѡ��ģʽ(GL_SELECT),����ģʽ(GL_FEEDBACK)
		cout << hits << endl;
		processHits(hits, selectBuf);
		glutPostRedisplay();
	}
	*/
}

//��������϶�ʱ��Ӧ
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
//������ʱ�϶�
void myMousePassiveMove(int x, int y) { 
	Cur_x = x, Cur_y = y; 
}
//����ÿ���˵�
void main_menu(int index) {
	printf("Cur_x: %d Cur_y: %d\n", Cur_x, Cur_y);
	selected_part = ProcessSelection(Cur_x, Cur_y);
	printf("index: %d Choose: %d\n", index, selected_part);
	if (index == -1)return;
	man->ChangeColor(selected_part, index);
	selected_part = -1;
	glutPostRedisplay();
}
//��Ӳ˵�
void addMenu() {
	//�һ��¼�  
	glutCreateMenu(main_menu);
	glutAddMenuEntry("ѡ����ɫ", -1);
	glutAddMenuEntry("��ɫ", 0);
	glutAddMenuEntry("��ɫ", 1);
	glutAddMenuEntry("��ɫ", 2);
	glutAddMenuEntry("��ɫ", 3);
	glutAddMenuEntry("��ɫ", 4);
	glutAddMenuEntry("��ɫ", 5);
	glutAddMenuEntry("��ɫ", 6);
	glutAddMenuEntry("��ɫ", 7);
	glutAddMenuEntry("��ɫ", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInit(&argc, argv); // ��ʼ��OPENGL
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("��ͨ����"); //�������ڣ������ô��ڱ���    
	myInit();
	glutReshapeFunc(myRreshape);
	glutDisplayFunc(myDisplay);   //���ûص���ʾ����  
	glutMouseFunc(myMouse);
	glutSpecialFunc(myRotate);
	glutMotionFunc(myMouseMove);
	glutPassiveMotionFunc(myMousePassiveMove);
	addMenu();
	glutMainLoop(); //������ѭ��
	return 0;
}