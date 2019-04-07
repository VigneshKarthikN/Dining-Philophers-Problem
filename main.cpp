#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

void init() {
	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluOrtho2D(-600, 750, -500, 700);
    	glMatrixMode(GL_MODELVIEW);
    
}
void outerCircle(int r,int x1,int y1){
	glColor3f(0,0,0);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	for (int i=1;i<360;i++)
	{
	    int x = r*cos(i)+x1;
	    int y = r*sin(i)+y1;
	    glVertex2f(x,y);
	}
	glEnd();
	
}
void filledCircle(int r,int x1,int y1){
	glBegin(GL_TRIANGLE_FAN);
	for (int i=1;i<360;i++)
	{
	    int x = r*cos(i)+x1;
	    int y = r*sin(i)+y1;
	    glVertex2f(x,y);
	}
	glEnd();
	glFlush();
}
void drawFood(int x,int y){
	int r=10;
	outerCircle(r,x,y+35);
	outerCircle(r,x+35,y);
	outerCircle(r,x-35,y);
	outerCircle(r,x,y-35);
	glColor3ub(245,222,179);
	filledCircle(r,x,y+35);
	filledCircle(r,x+35,y);
	filledCircle(r,x-35,y);
	filledCircle(r,x,y-35);
}
void drawChopstick(int x,int y,int t1,int t2){
	glColor3f(0,0,0);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x,y);
	glVertex2f(x+t1,y+t2);
	glEnd();
	glFlush();
}
void drawPlates(){
	outerCircle(55,0,220); //philopher 1 (top) (clock-wise)
	glColor3f(1,1,1);//plate1
	filledCircle(55,0,220);
	//drawFood(0,220);

	outerCircle(55,220,60); //philospher 2
	glColor3f(1,1,1);//plate2
	filledCircle(55,220,60);
	//drawFood(220,60);
		
	outerCircle(55,-220,60);//philopher 3
	glColor3f(1,1,1);//plate3
	filledCircle(55,-220,60);	
	drawFood(-220,60);
	
	outerCircle(55,-130,-190);//philosopher 4
	glColor3f(1,1,1);//plate 4
	filledCircle(55,-130,-190);
	//drawFood(-130,-190);
	
	outerCircle(55,130,-190);//philosopher 5
	glColor3f(1,1,1);//plate 5
	filledCircle(55,130,-190);
	drawFood(130,-190);

	outerCircle(85,0, 0);//centre common plate
	glColor3f(1,0.9568,0.8980);
	filledCircle(85, 0, 0);
	drawFood(0,0);
	drawFood(35, 34);
	drawFood(-35, 34);
	drawFood(35, -34);
	drawFood(-35, -34);	
}
void drawChopsticks(){
	drawChopstick(0+130,220-80,80,70);
	drawChopstick(220-30,60-120,100,-10);
	drawChopstick(-220-60,60-150,100,30);
	drawChopstick(-130,220-80,-80,70);
	drawChopstick(0,-300,0,100);
}				
void drawPhilosopher(){
	outerCircle(55,0,440);//philosopher 1
	glColor3ub(255,255,102);
	filledCircle(55,0,440);
	outerCircle(100,0,280);
	glColor3ub(255,255,102);
	filledCircle(100,0,280);
	
	outerCircle(55,440,60);//philosopher 2
	glColor3ub(203,31,31);
	filledCircle(55,440,60);
	outerCircle(100,280,60);
	glColor3ub(203,31,31);
	filledCircle(100,280,60);
		
	outerCircle(55,-440,60);//philosopher 3
	glColor3ub(51,255,51);
	filledCircle(55,-440,60);
	outerCircle(100,-280,60);
	glColor3ub(51,255,51);
	filledCircle(100,-280,60);
	
	outerCircle(55,250,-380);//philosopher 4
	glColor3ub(51,255,51);
	filledCircle(55,250,-380);
	outerCircle(100,150,-250);
	glColor3ub(51,255,51);
	filledCircle(100,150,-250);
	
	outerCircle(55,-250,-380);//philosopher 5
	glColor3ub(255,255,102);
	filledCircle(55,-250,-380);
	outerCircle(100,-150,-250);
	glColor3ub(255,255,102);
	filledCircle(100,-150,-250);
	
}
void drawLegend(){
    glPointSize(15.0);
    glBegin(GL_POINTS);
    glColor3ub(203,31,31);
    glVertex2f(450, 600);
    glColor3ub(255,255,102);
    glVertex2f(450, 520);
    glColor3ub(51,255,51);
    glVertex2f(450, 440);
    glEnd();
    char str[20] = "Waiting";
    glColor3f(0,0,0);
    glRasterPos2i(480, 590);
	for (int j = 0; j < 20; j++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
	//str[20] = "Thinking";
	strcpy(str, "Thinking");
    glRasterPos2i(480, 510);
	for (int j = 0; j < 20; j++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
	//str[20] = "Eating";
	strcpy(str, "Eating");
    glRasterPos2i(480, 430);
	for (int j = 0; j < 6; j++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
	glFlush();
}
void display() {
	glClearColor(1,1,1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPhilosopher();
	outerCircle(300,0,0);
	glColor3ub(246,217,243);//table
	filledCircle(300,0,0);
	drawPlates();
	drawChopsticks();
	drawLegend();
}
void reshape(int w, int h){
    int t1,t2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    t1 = (float)w/(float)h;
    t2 = (float)h/(float)w;
    if(w>h) 
    	gluOrtho2D(-600*t1, 750*t1, -500, 700);
    else 
    	gluOrtho2D( -600, 750, -500*t2, 700*t2);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3d");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
