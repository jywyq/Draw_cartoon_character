#include <windows.h>//�����󲿷ֵ�Win32 API�������Լ�һЩWin32�� 
#include <GL/glut.h> //����GLͷ�ļ�
#include<cmath>
#include <iostream>
#include <map>
using namespace std;
class Man {
private:
	GLint Width = 500, Height = 500;
	GLint Mx[20], My[20], Bx[20], By[20];
	GLdouble Pi = acos(-1);
	map<string, int> mp;
	string remp[20];
	int paint_color[20] = { 0,7,8,1,2,4,4,6,5,3,3 };
	void GetDxDy(int choose, int &dx, int &dy) {
		dx = Mx[choose] + Bx[choose], dy = My[choose] + By[choose];
	}
	GLfloat colors[9][3] = {
		{ 0.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0 },{ 1.0, 1.0, 0.0 },{ 1.0, 1.0, 1.0 },{1.0,125./256,64./256}
	};
	//��ɫ0  ��ɫ1  ��ɫ2  ��ɫ3  ��ɫ4  ��ɫ5  ��ɫ6  ��ɫ7 
public:

	Man() { 
		initMpAndRemap(); 
	}
	Man(GLint W, GLint H):Width(H), Height(W){
		initMpAndRemap();
	}
	string GetRemp(int choose) {
		return remp[choose];
	}
	void Reshape(GLint W, GLint H) {
		Width = W; Height = H;
	}
	//��ʼ������ٵ����ֵ�ӳ�����ӳ��
	void initMpAndRemap() 
	{
		mp["��"] = 1;   remp[1] = "��";
		mp["����"] = 2;  remp[2] = "����";
		mp["ͷ��"] = 3;   remp[3] = "ͷ��";
		mp["üë"] = 4;   remp[4] = "üë";
		mp["����"] = 5;   remp[5] = "����";
		mp["����"] = 6;   remp[6] = "����";
		mp["����"] = 7;   remp[7] = "����";
		mp["��"] = 8;   remp[8] = "��";
		mp["���"] = 9;   remp[9] = "���";
		mp["�Ҷ�"] = 10;   remp[10] = "�Ҷ�";
		memset(Mx, 0, sizeof Mx);
		memset(My, 0, sizeof My);
		memset(Bx, 0, sizeof Bx);
		memset(By, 0, sizeof By);

	}
	//�϶�ʱ��ƫ����
	void ChangeMoveXY(int Choose, int x, int y) {
		Mx[Choose] = x;
		My[Choose] = y;
	}
	//�ɿ���ı����ƫ����
	void AddBaseXY(int Choose, int x, int y) {
		Bx[Choose] += x;
		By[Choose] += y;
		Mx[Choose] = 0;
		My[Choose] = 0;
	}
	//��װ���޸���ɫ
	void ChangeColor(int Choose, int colorid) {
		paint_color[Choose] = colorid;
		printf("Change_color Choose: %d  %s colorid: %d\n", Choose ,remp[Choose], colorid);
	}
	//��
	void draw() {
		/*
		int mod = GL_RENDER_MODE;
		if (mod = GL_SELECT)cout << "select" << endl;
		if (mod = GL_RENDER)cout << "render" << endl;
		*/

		//��ʼ��namestack
		glInitNames();
		glPushName(0);

		//glPushMatrix();glLoadIdentity();
		glLoadName(mp["��"]); drawFace();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["����"]); drawExtraFace();

		//glPushMatrix();glLoadIdentity();
		glLoadName(mp["ͷ��"]);drawHair();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["üë"]); drawBrow();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["����"]); drawLeftEye();

		///glPushMatrix(); glLoadIdentity();
		glLoadName(mp["����"]); drawRightEye();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["����"]); drawNose();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["��"]); drawMouse();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["���"]); drawLeftEar();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["�Ҷ�"]); drawRightEar();


		//��ԭ��ɫ
		glColor3f(1, 1, 1);
	}
	void drawFace() {
		GLint Ax, Ay; 
		int id = mp["��"], colorid=paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//���û������        
		glVertex2f(Ax + 125, Ay + 150);
		glVertex2f(Ax + 125, Ay + 360);
		glVertex2f(Ax + 375, Ay + 360);
		glVertex2f(Ax + 375, Ay + 150);
		glVertex2f(Ax + 250, Ay +100);
		glEnd(); //���û�����ν���
	}
	void drawHair() {
		GLint Ax, Ay;
		int id = mp["ͷ��"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_TRIANGLE_FAN);

		glVertex2i(Ax + 160, Ay + 330);
		glVertex2f(Ax + 140, Ay + 300);
		for (int i = 100; i <= 400; i = i + 10)
			glVertex2f(Ax + i, Ay + 440 - (i - 250)*(i - 250)/225);
		glVertex2f(Ax + 380, Ay + 300);
		glEnd();
	}
	void drawBrow() {
		GLint Ax, Ay;
		int id = mp["üë"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex2f(Ax + 180, Ay + 290);
		glVertex2f(Ax + 210, Ay + 270);
		glVertex2f(Ax + 290, Ay + 270);
		glVertex2f(Ax + 320, Ay + 290);
		glEnd();
	}
	void drawLeftEye() {
		GLint Ax, Ay;
		int id = mp["����"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		GLdouble R = 10;
		glBegin(GL_POLYGON);
		for (GLdouble i = 0; i<100; ++i)
			glVertex2f(Ax + 200 + R*cos(2 * Pi / 100 * i), Ay + 260 + R*sin(2 * Pi / 100 * i));
		glEnd();

	}
	void drawRightEye() {
		GLint Ax, Ay;
		int id = mp["����"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		GLdouble R = 10;
		glBegin(GL_POLYGON);
		for (GLdouble i = 0; i<100; ++i)
			glVertex2f(Ax + 300 + R*cos(2 * Pi / 100 * i), Ay + 260 + R*sin(2 * Pi / 100 * i));
		glEnd();
	}
	void drawNose() {
		GLint Ax, Ay;
		int id = mp["����"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_TRIANGLES);
		glVertex2f(Ax + 250, Ay + 250);
		glVertex2f(Ax + 225, Ay + 200);
		glVertex2f(Ax + 275, Ay + 200);
		glEnd();
	}
	void drawMouse() {
		GLint Ax, Ay;
		int id = mp["��"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		GLdouble R = 20; 
		glBegin(GL_POLYGON);
		for (GLdouble i = 0; i<100; ++i)
			glVertex2f(Ax + 250 + R*cos(2 * Pi / 100*i), Ay + 150 + R*sin(2 * Pi / 100*i));
		glEnd();
	}
	void drawLeftEar() {
		GLint Ax, Ay;
		int id = mp["���"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//���û������        
		glVertex2f(Ax + 125, Ay + 250);
		glVertex2f(Ax + 125, Ay + 300);
		glVertex2f(Ax + 100, Ay + 300);
		glVertex2f(Ax + 100, Ay + 250);
		glEnd(); //���û�����ν���
	}
	void drawRightEar() {
		GLint Ax, Ay;
		int id = mp["�Ҷ�"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//���û������        
		glVertex2f(Ax + 375, Ay + 250);
		glVertex2f(Ax + 375, Ay + 300);
		glVertex2f(Ax + 400, Ay + 300);
		glVertex2f(Ax + 400, Ay + 250);
		glEnd(); //���û�����ν���

	}
	void drawExtraFace() {
		GLint Ax, Ay;
		int id = mp["����"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		GLdouble R = 150;
		glBegin(GL_POLYGON);
		for (GLdouble i = 0; i<100; ++i)
			glVertex2f(Ax + 600 + R*cos(2 * Pi / 100 * i), Ay + 250 + R*sin(2 * Pi / 100 * i));
		glEnd();

	}
};
