#include <glew.h> // glew.h must be included before gl.h/glu.h/freeglut.h
#include <freeglut.h>
#include <texture_loader.h>
#include <cstdio>
#include <cstdlib>

#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3
#define MENU_WALK_START 4
#define MENU_WALK_STOP 5
#define MENU_BODY_OPEN 6
#define MENU_BODY_CLOSE 7
#define MENU_RST 9

int old_rot_x = 0;                       //剛按下滑鼠時的視窗座標
int old_rot_y = 0;

int rot_x = 0;                           //拖曳後的相對座標，用這決定要旋轉幾度
int rot_y = 0;

int record_x = 0;                        //紀錄上一次旋轉的角度
int record_y = 0;

float d_head[3] = {0.0f,12.0f,0.0f}, d_right_upper_arm[3] = {7.0f, 9.0f, 0.0f}, d_left_upper_arm[3] = {-7.0f,9.0f, 0.0f};
float d_right_upper_leg[3] = {3.5f,-12.5f,0.0f}, d_left_upper_leg[3] = {-3.5f,-12.5f,0.0f}, d_back[3] = {0.0f,6.0f,-5.0f};

GLfloat diffuse0[] = {1,0,0,0}, ambient0[] = {1,1,1,1}, specular0[] = {1,1,1,1}, light0_pos[] = {100, -50, 100, 1}, a0 = 0.8f;
float distance = 0.0f;
GLfloat back_angle = 0, backRotateSpeed = 1, upper_leg_angle = 0, body_angle = 0.0, body_updown = 0, body_open = 0, cannon_out = 0; 
GLfloat lower_rleg_angle = 0.0f, lower_lleg_angle = 0.0f, upper_arm_angle = 0.0f, lower_rarm_angle = 0.0f, lower_larm_angle = 0.0f;
GLfloat getInDis = 0.0f, getDownDis = 0.0f, getOutDis = 0.0f;
GLubyte timer_cnt = 0;
bool timer_enabled = true, backRotate = false, upperLegRotate = false, upperArmRotate = false, canChange = true, bodyOpen = false, bodyOpenWay = true;
bool upperLegWay = false, upperArmWay = false, transform = false, transform2 = false, transform3 = false, transformWay = false, midCannon = false;
unsigned int timer_speed = 16;

GLuint texture[4];
// Print OpenGL context related information.
void dumpInfo(void)
{
    printf("Vendor: %s\n", glGetString (GL_VENDOR));
    printf("Renderer: %s\n", glGetString (GL_RENDERER));
    printf("Version: %s\n", glGetString (GL_VERSION));
    printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}
void CreateCube(float x, float y, float z)
{
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBegin(GL_QUADS);
    //glColor3f(1,0,0);  
	//正面
    glNormal3f(0,0,1);
        //設定法向量
		glVertex3f(-x, y, z); glTexCoord2f(0,0); 
		glVertex3f(-x,-y, z); glTexCoord2f(0,1);
		glVertex3f( x,-y, z); glTexCoord2f(1,1);
		glVertex3f( x, y, z); glTexCoord2f(1,0);
	//背面
	
    glNormal3f(0,0,-1);
        glVertex3f(-x, y,-z); glTexCoord2f(0,0);
        glVertex3f( x, y,-z); glTexCoord2f(0,1);
        glVertex3f( x,-y,-z); glTexCoord2f(1,1);
        glVertex3f(-x,-y,-z); glTexCoord2f(1,0);
      //右側面
	//glColor3f(0,1,0);
    glNormal3f(1,0,0);
        glVertex3f( x, y, z); glTexCoord2f(0,0);
        glVertex3f( x,-y, z); glTexCoord2f(0,1);
        glVertex3f( x,-y,-z); glTexCoord2f(1,1);
        glVertex3f( x, y,-z); glTexCoord2f(1,0);
    
		//左側面
    glNormal3f(-1,0,0);
        glVertex3f(-x, y,-z); glTexCoord2f(0,0);
        glVertex3f(-x,-y,-z); glTexCoord2f(0,1);
        glVertex3f(-x,-y, z); glTexCoord2f(1,1);
        glVertex3f(-x, y, z); glTexCoord2f(1,0);
    //上面
	//glColor3f(0,0,1);
    glNormal3f(0,1,0);
        glVertex3f(-x, y,-z); glTexCoord2f(0,0);
        glVertex3f(-x, y, z); glTexCoord2f(0,1);
        glVertex3f( x, y, z); glTexCoord2f(1,1);
        glVertex3f( x, y,-z); glTexCoord2f(1,0);
	glNormal3f(0,-1,0);
		glVertex3f(-x,-y,-z); glTexCoord2f(0,0);
		glVertex3f(-x,-y, z); glTexCoord2f(0,1);
		glVertex3f( x,-y, z); glTexCoord2f(1,1);
		glVertex3f( x,-y,-z); glTexCoord2f(1,0);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D,0);
}
// GLUT callback. Called to draw the scene.
void drawBody()
{
	CreateCube(7.0f,12.0f,5.0f);
	glPushMatrix();
	glTranslatef(0.0f,0.0f,5.2f);
	glColor3f(0,0,0);

	glBindTexture(GL_TEXTURE_2D,texture[3]);
	
	glPushMatrix();
	glTranslatef(body_open,0.0f,0.0f);
	glBegin(GL_QUADS);

	glNormal3f(0,0,1);
	glVertex3f(0.0f,-15.0f,0.0f); glTexCoord2f(0, 0);
	glVertex3f(7.3f,-12.0f,0.0f); glTexCoord2f(0,1);
	glVertex3f(7.3f,13.0f,0.0f);  glTexCoord2f(1, 1);
	glVertex3f(0.0f,10.0f,0.0f); glTexCoord2f(1,0);
	glEnd();
	
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D,0);

	glPushMatrix();
	glTranslatef(-body_open,0.0f,0.0f);
	glBegin(GL_QUADS);

	glColor3f(1,1,1);
	glNormal3f(0,0,1);
	glVertex3f(0.0f,-15.0f,0.0f); glTexCoord2f(0, 0.5);
	glVertex3f(0.0f,10.0f,0.0f); glTexCoord2f(0.75, 0.5);
	glVertex3f(-7.3f,13.0f,0.0f); glTexCoord2f(1,0);
	glVertex3f(-7.3f,-12.0f,0.0f); glTexCoord2f(0.25, 0);

	glEnd();
	glPopMatrix();
	
	
	if(midCannon || !bodyOpenWay){
		for(float i = 0; i < cannon_out; i += 0.1f){
			glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(0.0f,0.0f,i);
			glRotatef(0,0.0f,0.0f,0.0f);
			glScalef(1.0f,1.0f,5.0f);
		
			GLUquadricObj *quadric = gluNewQuadric();
			gluQuadricDrawStyle( quadric, GLU_FILL); //FILL also can be line(wire)
			gluQuadricNormals( quadric, GLU_SMOOTH); // For if lighting is to be used.
			gluQuadricOrientation( quadric,GLU_OUTSIDE);
			gluQuadricTexture( quadric, GL_TRUE);// if you want to map a texture to it.
			gluDisk(quadric, 2,3,100,100);

			glPopMatrix();
		}
	}

	glPopMatrix();
}
void drawBack()
{
	glTranslatef(d_back[0], d_back[1], d_back[2]);
	
	glColor3f(0,0,1);
	glutSolidSphere(2,100,100);

	glTranslatef(0.0f,0.0f,-1.5f);
	glRotatef(back_angle,0.0f,0.0f,1.0f);

	for(int i = 0; i < 24; i++)
	{
		glPushMatrix();
		glColor3f(1,0,1);
		glRotatef(15.0f * i,0.0f,0.0f,1.0f);
		
		glBindTexture(GL_TEXTURE_2D,texture[2]);
		glBegin(GL_QUADS);
		glNormal3f(0.0,0.0,-1.0);
		
		glVertex3f((GLfloat)0.3,(GLfloat)0.0,(GLfloat)0.0); glTexCoord2f(0,0);
		glVertex3f((GLfloat)-0.3,(GLfloat)0.0,(GLfloat)0.0); glTexCoord2f(0,1);
		glVertex3f((GLfloat)0.7,(GLfloat)15.0,(GLfloat)0.0); glTexCoord2f(1,1);
		glVertex3f((GLfloat)-0.7,(GLfloat)15.0,(GLfloat)0.0); glTexCoord2f(1,0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,0);
		glPopMatrix();
	}
}
void drawHead()
{
	glColor3f(1,0,0);
	glTranslatef(d_head[0], d_head[1], d_head[2]);
	CreateCube(4.0f,4.0f,4.0f);

	glPushMatrix();
	glColor3f(0,1,0);
	glTranslatef(3.0f,3.4f,-1.0f);
	glRotatef(-135.0f,1.0f,0.0f,0.0f);
	glRotatef(45.0f,0.0f,1.0f,0.0f);
	//glRotatef(20.0f,0.0f,0.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glutSolidCone(1,5,100,100);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0,1,0);
	glTranslatef(-3.0f,3.4f,-1.0f);
	glRotatef(-135.0f,1.0f,0.0f,0.0f);
	glRotatef(-45.0f,0.0f,1.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glutSolidCone(1,5,100,100);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
void drawRightUpperArm()
{
	glTranslatef(d_right_upper_arm[0],d_right_upper_arm[1], d_right_upper_arm[2]);
	glColor3f(0,0,1);
	
	glPushMatrix();
	glutSolidSphere(2.2,100,100);
	glPopMatrix();

	glRotatef(-upper_arm_angle, 1.0f,0.0f,0.0f);
	glColor3f(1,0,0);
	glTranslatef(3.0f, -3.0f, 0.0f);
	glPushMatrix();
		glPushMatrix();
		CreateCube(2.0f,6.0f,2.0f);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0,1,0);
		glTranslatef(0.7f,4.0f,0.0f);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(45.0f,0.0f,1.0f,0.0f);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glutSolidCone(2,5,100,100);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

	glPopMatrix();
}
void drawRightLowerArm()
{
	glTranslatef(0.0f, -6.5f,0.0f);
	glColor3f(0,0,1);

	glPushMatrix();
	glutSolidSphere(1.8f,100,100);
	glPopMatrix();

	glRotatef(lower_rarm_angle,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,-3.0f,0.0f);
	glColor3f(1,0,0);

	glPushMatrix();
	CreateCube(2.0f, 2.0f, 2.0f);
	glPopMatrix();
}
void drawHand()
{
	glColor3f(0,0,0);
	glTranslatef(0.0f,-2.0f,0.0f);
	glutSolidSphere(2,100,100);
	glTranslatef(0.0f,getInDis,0.0f);
	
	glPushMatrix();
	glTranslatef(0.0f,-2.0f,0.0f);
	glColor3f(1,1,0);

	if(getInDis < 2.0f){
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		glBindTexture(GL_TEXTURE_2D, texture[2]);	
		glutSolidCone(0.5f,2.6f,100,100);
			glPushMatrix();
			glTranslatef(1.0f,-0.3f,0.0f);
			glRotatef(30,0.0f,1.0f,0.0f);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glutSolidCone(0.5f,2.6f,100,100);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-1.0f,-0.3f,0.0f);
			glRotatef(-30.0f,0.0f,1.0f,0.0f);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glutSolidCone(0.5f,2.6f,100,100);
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
	}else{
		
		glTranslatef(0.0,getDownDis,0.0);
		glColor3f(0.5,0.5,0.5);

		glPushMatrix();
		glScalef(getDownDis, getDownDis,getDownDis);
		CreateCube(0.5f,0.5f,2.7f);
		glPopMatrix();
		
		if(getDownDis < -1.0){
			glTranslatef(0.0f,0.0f,2.5f);
			//glRotatef(180, 1.0f,0.0f,0.0f);
			glScalef(getOutDis, getOutDis, getOutDis);
			glutSolidCone(2.5,15,100,100);
		}
	}
	glPopMatrix();
	
}
void drawLeftUpperArm()
{
	glTranslatef(d_left_upper_arm[0],d_left_upper_arm[1], d_left_upper_arm[2]);
	glColor3f(0,0,1);
	
	glPushMatrix();
	glutSolidSphere(2.2,100,100);
	glPopMatrix();

	glRotatef(upper_arm_angle, 1.0f,0.0f,0.0f);
	glColor3f(1,0,0);
	glTranslatef(-3.0f, -3.0f, 0.0f);

	glPushMatrix();
		glPushMatrix();
		CreateCube(2.0f,6.0f,2.0f);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0,1,0);
		glTranslatef(-0.7f,4.0f,0.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		glRotatef(225.0f,0.0f,1.0f,0.0f);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glutSolidCone(2,5,100,100);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

	glPopMatrix();
}
void drawLeftLowerArm()
{
	glTranslatef(0.0f, -6.5f,0.0f);
	glColor3f(0,0,1);

	glPushMatrix();
	glutSolidSphere(1.8f,100,100);
	glPopMatrix();

	glRotatef(lower_larm_angle, 1.0f,0.0f,0.0f);
	glTranslatef(0.0f,-3.0f,0.0f);
	glColor3f(1,0,0);

	glPushMatrix();
	CreateCube(2.0f, 2.0f, 2.0f);
	glPopMatrix();
}
void drawRightUpperLeg()
{
	glTranslatef(d_right_upper_leg[0],d_right_upper_leg[1], d_right_upper_leg[2]);
	glColor3f(0,0,1);
	
	glPushMatrix();
	glutSolidSphere(2.2,100,100);
	glPopMatrix();

	glRotatef(upper_leg_angle,1.0f,0.0f,0.0f);
	glColor3f(1,0,0);
	glTranslatef(0.0f, -4.5f, 0.0f);

	glPushMatrix();
	CreateCube(2.0f,3.5f,2.0f);
	glPopMatrix();
}
void drawRightLowerLeg()
{
	glTranslatef(0.0f, -4.0f,0.0f);
	glColor3f(0,0,1);

	glPushMatrix();
	glutSolidSphere(1.6f,100,100);
	glPopMatrix();

	glRotatef(lower_rleg_angle,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,-3.0f,0.0f);
	glColor3f(1,0,0);

	glPushMatrix();
	CreateCube(2.0f, 2.0f, 2.0f);
	glPopMatrix();
}
void drawRightFoot()
{
	glTranslatef(0.0f,-4.0f,0.0f);
	glColor3f(0,1,0);

	glPushMatrix();
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	GLUquadricObj *Cylinder;
	
	Cylinder = gluNewQuadric();
	gluQuadricDrawStyle(Cylinder,GLU_FILL);
	gluQuadricNormals(Cylinder,GLU_SMOOTH);
	gluQuadricOrientation(Cylinder,GLU_OUTSIDE);
	gluQuadricTexture(Cylinder, GL_TRUE);
	gluCylinder(Cylinder,3,2,2,100,100);
	glPopMatrix();

	glColor3f(1,0,0);
	glTranslatef(0.0f,0.8f,-2.5f);
	glRotatef(225.0,1.0f,0.0f,0.0f);
	glutSolidCone(1,4,100,100);
}
void drawLeftUpperLeg()
{
	glTranslatef(d_left_upper_leg[0],d_left_upper_leg[1], d_left_upper_leg[2]);
	glColor3f(0,0,1);
	
	glPushMatrix();
	glutSolidSphere(2.2,100,100);
	glPopMatrix();

	glRotatef(-upper_leg_angle,1.0f,0.0f,0.0f);
	glColor3f(1,0,0);
	glTranslatef(0.0f, -4.5f, 0.0f);

	glPushMatrix();
	CreateCube(2.0f,3.5f,2.0f);
	glPopMatrix();
}
void drawLeftLowerLeg()
{
	glTranslatef(0.0f, -4.0f,0.0f);
	glColor3f(0,0,1);

	glPushMatrix();
	glutSolidSphere(1.6f,100,100);
	glPopMatrix();

	glRotatef(lower_lleg_angle,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,-3.0f,0.0f);
	glColor3f(1,0,0);

	glPushMatrix();
	CreateCube(2.0f, 2.0f, 2.0f);
	glPopMatrix();
}
void drawLeftFoot()
{
	glTranslatef(0.0f,-4.0f,0.0f);
	glColor3f(0,1,0);

	glPushMatrix();
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	GLUquadricObj *Cylinder;
	
	Cylinder = gluNewQuadric();
	gluQuadricDrawStyle(Cylinder,GLU_FILL);
	gluQuadricNormals(Cylinder,GLU_SMOOTH);
	gluQuadricOrientation(Cylinder,GLU_OUTSIDE);
	gluQuadricTexture(Cylinder, GL_TRUE);
	gluCylinder(Cylinder,3,2,2,100,100);
	glPopMatrix();

	glColor3f(1,0,0);
	glTranslatef(0.0f,0.8f,-2.5f);
	glRotatef(225.0,1.0f,0.0f,0.0f);
	glutSolidCone(1,4,100,100);
}

void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode (GL_NONE, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef( 0, 0, distance);

	glRotatef( (float)rot_y + (float)record_y, 1.0, 0.0, 0.0);   //以x軸當旋轉軸
    glRotatef( (float)rot_x + (float)record_x, 0.0, 1.0, 0.0);   //以y軸當旋轉軸
	glColor3f(0,1,0);

	glPushMatrix();
	glTranslatef(0.0f,body_updown,0.0f);
	glRotatef(body_angle,0.0f,0.0f,1.0f);
	drawBody();
		glPushMatrix();
			drawBack();
		glPopMatrix();
		glPushMatrix();
			drawHead();
		glPopMatrix();

		glPushMatrix();
			drawRightUpperArm();
			drawRightLowerArm();
			drawHand();
		glPopMatrix();

		glPushMatrix();
			drawLeftUpperArm();
			drawLeftLowerArm();
			drawHand();
		glPopMatrix();

		glPushMatrix();
			drawRightUpperLeg();
			drawRightLowerLeg();
			drawRightFoot();
		glPopMatrix();

		glPushMatrix();
			drawLeftUpperLeg();
			drawLeftLowerLeg();
			drawLeftFoot();
		glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	if(height ==0) height = 1;
	float viewportAspect = (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, viewportAspect, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void My_Timer(int val)
{
	if(backRotate){
		back_angle += backRotateSpeed;
		if(back_angle > 360 || back_angle < -360)
			back_angle = 0;
	}
	if(upperLegRotate){
		if(upperLegWay == false){
			body_angle += 0.01f;
			upper_leg_angle += 1;
			if(upper_leg_angle > 0)
				body_updown -= 0.01f;
			else
				body_updown += 0.01f;

			if(lower_rleg_angle < 25)
				lower_rleg_angle += 0.5;
			if(lower_lleg_angle > 0)
				lower_lleg_angle -= 0.5;
			if(upper_leg_angle > 30){
				upperLegWay = true;
			}
		}else{
			if(upper_leg_angle < 0)
				body_updown -= 0.01f;
			else
				body_updown += 0.01f;
			upper_leg_angle -= 1;
			body_angle -= 0.01f;
			if(lower_rleg_angle > 0)
				lower_rleg_angle -= 0.5;
			if(lower_lleg_angle < 25)
				lower_lleg_angle += 0.5;
			if(upper_leg_angle < -30){
				upperLegWay = false;
			}
		}
	}
	if(upperArmRotate){
		if(upperArmWay){
			upper_arm_angle -= 0.5f;
			if(lower_rarm_angle < 0)
				lower_rarm_angle += 0.3f;
			if(lower_larm_angle > -25)
				lower_larm_angle -= 0.3f;

			if(upper_arm_angle < -15)
				upperArmWay = false;
		}else{
			upper_arm_angle += 0.5f;
			if(lower_rarm_angle > -25)
				lower_rarm_angle -= (GLfloat)0.3;
			if(lower_larm_angle < 0)
				lower_larm_angle += 0.3f;

			if(upper_arm_angle > 15)
				upperArmWay = true;
		}
	}
	if(transform && !transformWay){
		getInDis += (GLfloat)0.03;
		if(getInDis > 2.0f){
			transform = false;	
			transform2 = true;
		}
	}else if(transform){
		getInDis -= (GLfloat)0.03;
		if(getInDis < 0.0){
			transformWay = false;
			transform = false;
			canChange = true;
		}
	}
	if(transform2 && !transformWay){
		getDownDis -= (GLfloat)0.03;
		if(getDownDis < -1.0f){
			transform2 = false;
			transform3 = true;
		}
	}else if(transform2){
		getDownDis += (GLfloat)0.03;
		if(getDownDis > 0.0){
			transform2 = false;
			transform = true;
		}
	}
	if(transform3 && !transformWay){
		getOutDis += (GLfloat)0.01;
		if(getOutDis > 1.0f)
			transform3 = false;
	}else if(transform3){
		getOutDis -= (GLfloat)0.01;
		if(getOutDis < 0.0f){
			transform3 = false;
			transform2 = true;
		}
	}

	if(bodyOpen){
		if(bodyOpenWay){
			body_open += 0.01f;
			
			if(body_open > 3.0f){
				bodyOpen = false;
				bodyOpenWay = false;
				midCannon = true;
			}
		}else{
			body_open -= 0.01f;
			midCannon = false;
			if(body_open < 0.0f){
				bodyOpenWay = true;
				bodyOpen = false;
				canChange = true;
			}
		}
	}
	if(midCannon){
		if(cannon_out < 5) cannon_out += 0.1f;
	}else{
		if(cannon_out > 0) cannon_out -= 0.1f;
	}

	glutPostRedisplay();
	if(timer_enabled)
	{
		glutTimerFunc(timer_speed, My_Timer, val);
	}
}
void My_Mouse(int button, int state, int x, int y)
{
	if(state)
	{
		record_x += x - old_rot_x;
        record_y += y - old_rot_y;
     
        rot_x = 0;   //沒有歸零會有不理想的結果
        rot_y = 0;
		printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
	}
	else 
	{
		old_rot_x = x;
        old_rot_y = y;
		printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}
void My_Keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'T':
	case 't':
		if(canChange){
			canChange = false;
			transform = true;
		}break;
	case 'B':
	case 'b':
		if(!canChange && (getOutDis > 1.0f)){
			transform3 = true;
			transformWay = true;
		}break;
	case 'G':
	case 'g':
		if(backRotate == false)
			backRotate = true;
		else backRotate = false;
		break;
	case 'F':
	case 'f':
		backRotateSpeed++;
		break;
	case 'S':
	case 's':
		backRotateSpeed--;
		break;
	case 'Q':
	case 'q':
		distance += 1;
		break;
	case 'E':
	case 'e':
		distance -= 1;
		break;
	case '`':
		exit(0);
		break;
	}
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
}
void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		printf("Left arrow is pressed at (%d, %d)\n", x, y);
		break;
	default:
		printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
}
void MotionMouse(int x, int y)
{
   rot_x = (x - old_rot_x);
   rot_y = (y - old_rot_y);
   glutPostRedisplay();
}
void My_Menu(int id)
{
	switch(id)
	{
	case MENU_TIMER_START:
		if(!timer_enabled)
		{
			timer_enabled = true;
			glutTimerFunc(timer_speed, My_Timer, 0);
		}
		break;
	case MENU_TIMER_STOP:
		timer_enabled = false;
		break;
	case MENU_EXIT:
		exit(0);
		break;
	case MENU_WALK_START:
		upperLegRotate = true;
		upperArmRotate = true;
		break;
	case MENU_WALK_STOP:
			upperLegRotate = false;
			upperArmRotate = false;
			break;
	case MENU_RST:
		back_angle = 0; backRotateSpeed = 1; upper_leg_angle = 0, getInDis = 0.0f; body_updown = 0; body_angle = 0;
		distance = 0.0f; lower_rleg_angle = 0.0f; lower_lleg_angle = 0.0f; upper_arm_angle = 0.0f; lower_rarm_angle = 0.0f; lower_larm_angle = 0.0f;
		backRotate = false; upperLegRotate = false; upperArmRotate = false, transform = false;
		getInDis = 0.0f; getDownDis = 0.0f; getOutDis = 0.0f;
		transformWay = false; transform3 = false; transform2 = false; transform = false;
		body_open = 0; cannon_out = 0;
		canChange = true; bodyOpen = false; bodyOpenWay = true; midCannon = false;
		break;
	case MENU_BODY_OPEN:
		if(canChange){
			bodyOpen = true;
			canChange = false;
		}
		break;
	case MENU_BODY_CLOSE:
		if(!canChange && !bodyOpenWay){
			bodyOpen = true;
		}
		break;
	default:
		break;
	}
}

void InitTexture(){
	
	texture_data Img[4];
	Img[0] = load_jpg("metal.jpg");
	Img[1] = load_jpg("bat.jpg");
	Img[2] = load_jpg("gold.jpg");
	Img[3] = load_jpg("star.jpg");
	glEnable(GL_TEXTURE_2D);
	glGenTextures(4, texture);
	for(int i = 0; i < 4; i++){
		glBindTexture(GL_TEXTURE_2D,texture[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,0,3,Img[i].width,Img[i].height,0,GL_RGB,GL_UNSIGNED_BYTE,Img[i].data);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		/// Filter parameters
		free_texture_data(Img[i]);
		glBindTexture(GL_TEXTURE_2D,0);
	}
}
void setLight(){
	
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0,	 GL_POSITION,	light0_pos);
	glLightfv(GL_LIGHT0,	 GL_AMBIENT,	ambient0);
	glLightfv(GL_LIGHT0,	 GL_DIFFUSE,	diffuse0);
	glLightfv(GL_LIGHT0,	 GL_SPECULAR,	specular0);
	glLightf(GL_LIGHT0,	GL_CONSTANT_ATTENUATION,	a0);
	
	GLfloat spot_dir[] = {1,-1,-1};
	GLint spot_ex = 30, spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_ex);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
	
	GLfloat Kc = 1.00, Kl = 0.00, Kq = 0.00;
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
}

int main(int argc, char *argv[])
{
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
	glewInit();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("My Project1 Robot"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
	dumpInfo();
	////////////////////
	
	// Initialize OpenGL states.
	////////////////////////
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	InitTexture();
	////////////////////////

	// Create a menu and bind it to mouse right button.
	////////////////////////////
	int menu_main = glutCreateMenu(My_Menu);
	int menu_timer = glutCreateMenu(My_Menu);
	int menu_walk = glutCreateMenu(My_Menu);
	int menu_cannon = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddSubMenu("Timer", menu_timer);
	glutAddSubMenu("Walk", menu_walk);
	glutAddSubMenu("Cannon", menu_cannon);
	glutAddMenuEntry("Reset", MENU_RST);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_timer);
	glutAddMenuEntry("Start", MENU_TIMER_START);
	glutAddMenuEntry("Stop", MENU_TIMER_STOP);
	
	glutSetMenu(menu_walk);
	glutAddMenuEntry("Start", MENU_WALK_START);
	glutAddMenuEntry("Stop", MENU_WALK_STOP);
	
	glutSetMenu(menu_cannon);
	glutAddMenuEntry("Open", MENU_BODY_OPEN);
	glutAddMenuEntry("Close", MENU_BODY_CLOSE);

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	////////////////////////////

	// Register GLUT callback functions.
	///////////////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutMotionFunc(MotionMouse);
	glutTimerFunc(timer_speed, My_Timer, 0); 
	///////////////////////////////
	setLight();
	// Enter main event loop.
	//////////////
	glutMainLoop();
	//////////////
	return 0;
}