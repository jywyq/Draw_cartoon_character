#include <windows.h>//包括大部分的Win32 API函数，以及一些Win32宏 
#include <GL/glut.h> //包含GL头文件
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
	//黑色0  红色1  绿色2  蓝色3  青色4  紫色5  黄色6  白色7 
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
	//初始化从五官到数字的映射和逆映射
	void initMpAndRemap() 
	{
		mp["脸"] = 1;   remp[1] = "脸";
		mp["外脸"] = 2;  remp[2] = "外脸";
		mp["头发"] = 3;   remp[3] = "头发";
		mp["眉毛"] = 4;   remp[4] = "眉毛";
		mp["左眼"] = 5;   remp[5] = "左眼";
		mp["右眼"] = 6;   remp[6] = "右眼";
		mp["鼻子"] = 7;   remp[7] = "鼻子";
		mp["嘴"] = 8;   remp[8] = "嘴";
		mp["左耳"] = 9;   remp[9] = "左耳";
		mp["右耳"] = 10;   remp[10] = "右耳";
		memset(Mx, 0, sizeof Mx);
		memset(My, 0, sizeof My);
		memset(Bx, 0, sizeof Bx);
		memset(By, 0, sizeof By);

	}
	//拖动时的偏移量
	void ChangeMoveXY(int Choose, int x, int y) {
		Mx[Choose] = x;
		My[Choose] = y;
	}
	//松开后改变基础偏移量
	void AddBaseXY(int Choose, int x, int y) {
		Bx[Choose] += x;
		By[Choose] += y;
		Mx[Choose] = 0;
		My[Choose] = 0;
	}
	//封装：修改颜色
	void ChangeColor(int Choose, int colorid) {
		paint_color[Choose] = colorid;
		printf("Change_color Choose: %d  %s colorid: %d\n", Choose ,remp[Choose], colorid);
	}
	//画
	void draw() {
		/*
		int mod = GL_RENDER_MODE;
		if (mod = GL_SELECT)cout << "select" << endl;
		if (mod = GL_RENDER)cout << "render" << endl;
		*/

		//初始化namestack
		glInitNames();
		glPushName(0);

		//glPushMatrix();glLoadIdentity();
		glLoadName(mp["脸"]); drawFace();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["外脸"]); drawExtraFace();

		//glPushMatrix();glLoadIdentity();
		glLoadName(mp["头发"]);drawHair();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["眉毛"]); drawBrow();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["左眼"]); drawLeftEye();

		///glPushMatrix(); glLoadIdentity();
		glLoadName(mp["右眼"]); drawRightEye();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["鼻子"]); drawNose();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["嘴"]); drawMouse();

		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["左耳"]); drawLeftEar();
		
		//glPushMatrix(); glLoadIdentity();
		glLoadName(mp["右耳"]); drawRightEar();


		//还原颜色
		glColor3f(1, 1, 1);
	}
	void drawFace() {
		GLint Ax, Ay; 
		int id = mp["脸"], colorid=paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//设置画多边形        
		glVertex2f(Ax + 125, Ay + 150);
		glVertex2f(Ax + 125, Ay + 360);
		glVertex2f(Ax + 375, Ay + 360);
		glVertex2f(Ax + 375, Ay + 150);
		glVertex2f(Ax + 250, Ay +100);
		glEnd(); //设置画多边形结束
	}
	void drawHair() {
		GLint Ax, Ay;
		int id = mp["头发"], colorid = paint_color[id];
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
		int id = mp["眉毛"], colorid = paint_color[id];
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
		int id = mp["左眼"], colorid = paint_color[id];
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
		int id = mp["右眼"], colorid = paint_color[id];
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
		int id = mp["鼻子"], colorid = paint_color[id];
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
		int id = mp["嘴"], colorid = paint_color[id];
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
		int id = mp["左耳"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//设置画多边形        
		glVertex2f(Ax + 125, Ay + 250);
		glVertex2f(Ax + 125, Ay + 300);
		glVertex2f(Ax + 100, Ay + 300);
		glVertex2f(Ax + 100, Ay + 250);
		glEnd(); //设置画多边形结束
	}
	void drawRightEar() {
		GLint Ax, Ay;
		int id = mp["右耳"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		glBegin(GL_POLYGON);//设置画多边形        
		glVertex2f(Ax + 375, Ay + 250);
		glVertex2f(Ax + 375, Ay + 300);
		glVertex2f(Ax + 400, Ay + 300);
		glVertex2f(Ax + 400, Ay + 250);
		glEnd(); //设置画多边形结束

	}
	void drawExtraFace() {
		GLint Ax, Ay;
		int id = mp["外脸"], colorid = paint_color[id];
		GetDxDy(id, Ax, Ay);
		glColor3f(colors[colorid][0], colors[colorid][1], colors[colorid][2]);

		GLdouble R = 150;
		glBegin(GL_POLYGON);
		for (GLdouble i = 0; i<100; ++i)
			glVertex2f(Ax + 600 + R*cos(2 * Pi / 100 * i), Ay + 250 + R*sin(2 * Pi / 100 * i));
		glEnd();

	}
};
