#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h> //Header file for sleep(). man 3 sleep for details. 
#include<GL/glut.h>


#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

void drawPhilosopher(int,int,int,int);
void outerCircle(int,int,int);
void filledCircle(int,int, int);
void drawPlates();
void drawChopsticks();
void drawLegend();
void init();	
void display();
void idle();
void algo();
void mouseInit(int,int,int,int);

int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 
int final_state[100][5];
int iterate =0;
int chopSticks[5][4] = {{130,140,80,70}, {190,-60,100,-10},  {0,-300,0,100}, {-280,-90,100,30},{-130,140,-80,70}};
int leftSticks[5][4] = {{20,180,60,70}, {190,30,80,-60}, {80,-270,5,80}, {-220,-180,90,30}, {-180,80,-70,70}}; // left wrt philosopher
int rightSticks[5][4] = {{-20,180,-60,70}, {180,75,70,60}, {125,-145,100,-15}, {-90,-280,0,100}, {-290,-10,100,40}}; //right wrt philosopher

sem_t mutex; 
sem_t S[N]; 

int p,p1;

void test(int phnum) 
{ 
	if (state[phnum] == HUNGRY 
		&& state[LEFT] != EATING 
		&& state[RIGHT] != EATING) { 
		// state that eating 
		state[phnum] = EATING; 
		printf("Philosopher %d takes fork %d and %d\n", 
					phnum + 1, LEFT + 1, phnum + 1); 
		sleep(2);
		
		printf("Philosopher %d is Eating\n", phnum + 1); 
		// sem_post(&S[phnum]) has no effect 
		// during takefork 
		// used to wake up hungry philosophers 
		// during putfork 
		sem_post(&S[phnum]); 
	} 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that hungry 
	state[phnum] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", phnum + 1); 
	
	// eat if neighbours are not eating 
	test(phnum); 

	sem_post(&mutex); 

	// if unable to eat wait to be signalled 
	sem_wait(&S[phnum]); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that thinking 
	state[phnum] = THINKING; 

	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

	test(LEFT); 
	test(RIGHT); 

	sem_post(&mutex); 
} 

void* philospher(void* num) 
{ 
        int* i = (int*)num; 

	sleep(1); 

	take_fork(*i); 
	
	for(int i=0;i<N;i++)
	        final_state[iterate][i]=state[i];
	iterate += 1;
	
	sleep(0); 

	put_fork(*i); 
		
	pthread_exit(NULL);
 
} 
void mouseInit(int b,int s,int x,int y){
	if(b==GLUT_LEFT_BUTTON && s==GLUT_DOWN){
			algo();
	}
	if(b==GLUT_RIGHT_BUTTON && s==GLUT_DOWN)
		exit(0);

}
void display_text(char text[],int x,int y){
	glColor3f(0,0,0);
	glRasterPos2i(x,y);
	for (int j = 0; j < strlen(text); j++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[j]);
	glFlush();
	
}
void algo() 
{ 
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT); 
	char str[20] = "Initializing.....";
	display_text(str,-100, 100);
	
	int i; 
	pthread_t thread_id[N]; 

	// initialize the semaphores 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) 
		sem_init(&S[i], 0, 0); 

	for (i = 0; i < N; i++) 
		state[i]=THINKING;

	for (i = 0; i < N; i++) { 
		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, 
					philospher, &phil[i]); 
		printf("Philosopher %d is thinking\n", i + 1); 
	} 
		
	for (i = 0; i < N; i++) 
		pthread_join(thread_id[i], NULL); 

}
void start_screen(){
	//Start screen
    	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3ub(160,160,160);
	glBegin(GL_POLYGON);
	glVertex2f(-150,50);
	glVertex2f(-150,150);
	glVertex2f(250,150);
	glVertex2f(250,50);
	glEnd();
	char str[20] = "Click to Start";
	display_text(str,-120, 80);
	//end of start screen
	
}
void init() {
	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluOrtho2D(-600, 750, -500, 700);
    	glMatrixMode(GL_MODELVIEW);
    	start_screen();
    	glutMouseFunc(mouseInit);
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
	outerCircle(55,0,220); //philopher 1 
	glColor3f(1,1,1);//plate1
	filledCircle(55,0,220);
	
	outerCircle(55,220,60); //philospher 2
	glColor3f(1,1,1);//plate2
	filledCircle(55,220,60);
			
	outerCircle(55,-220,60);//philopher 5
	glColor3f(1,1,1);//plate3
	filledCircle(55,-220,60);	
		
	outerCircle(55,-130,-190);//philosopher 3
	glColor3f(1,1,1);//plate 4
	filledCircle(55,-130,-190);
		
	outerCircle(55,130,-190);//philosopher 4
	glColor3f(1,1,1);//plate 5
	filledCircle(55,130,-190);
	
	outerCircle(85,0, 0);//centre common plate
	glColor3f(1,0.9568,0.8980);
	filledCircle(85, 0, 0);
	drawFood(0,0);
	drawFood(35, 34);
	drawFood(-35, 34);
	drawFood(35, -34);
	drawFood(-35, -34);	
}
void drawPhilosopher(int ch,int a,int b,int c){
	if(ch==1){
		outerCircle(55,0,440);//philosopher 1
		glColor3ub(a,b,c);
		filledCircle(55,0,440);
		outerCircle(100,0,280);
		glColor3ub(a,b,c);
		filledCircle(100,0,280);
	}
	if(ch==2){
		outerCircle(55,440,60);//philosopher 2
		glColor3ub(a,b,c);
		filledCircle(55,440,60);
		outerCircle(100,280,60);
		glColor3ub(a,b,c);
		filledCircle(100,280,60);
	}
	else if(ch==5){
		outerCircle(55,-440,60);//philosopher 3
		glColor3ub(a,b,c);
		filledCircle(55,-440,60);
		outerCircle(100,-280,60);
		glColor3ub(a,b,c);
		filledCircle(100,-280,60);
	}
	else if(ch==3){
		outerCircle(55,250,-380);//philosopher 4
		glColor3ub(a,b,c);
		filledCircle(55,250,-380);
		outerCircle(100,150,-250);
		glColor3ub(a,b,c);
		filledCircle(100,150,-250);
	}
	else if(ch==4){
		outerCircle(55,-250,-380);//philosopher 5
		glColor3ub(a,b,c);
		filledCircle(55,-250,-380);
		outerCircle(100,-150,-250);
		glColor3ub(a,b,c);
		filledCircle(100,-150,-250);
	}
	glFlush();
	glutPostRedisplay();
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
    	display_text(str,480, 590);
	
	strcpy(str, "Thinking");
    	display_text(str,480, 510);
	
	strcpy(str, "Eating");
    	display_text(str,480, 430);
}
void display() {
	for(int x = 0; x<iterate; x++){
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT); 
	        for(int j=0;j<N;j++){
			if(final_state[x][j]==EATING)
		        	drawPhilosopher(j+1,51,255,51);
		    	else if(final_state[x][j]==HUNGRY)
		        	drawPhilosopher(j+1,203,31,31);
		    	else
		        	drawPhilosopher(j+1,255,255,102);
		}
		outerCircle(300,0,0);
		glColor3ub(246,217,243);//table
		filledCircle(300,0,0);
		drawPlates();
		drawLegend();
		int arrJ[N+N],count=0,drawn=0;
		for(int j=0;j<N;j++){
			if(final_state[x][j]==EATING){
				if((j+1) == 1) drawFood(0,220);
                		if((j+1) == 2) drawFood(220,60);
                		if((j+1) == 5) drawFood(-220,60);
                		if((j+1) == 3) drawFood(130,-190);
                		if((j+1) == 4) drawFood(-130,-190);
                		drawChopstick(leftSticks[j][0], leftSticks[j][1], leftSticks[j][2], leftSticks[j][3]);
                		drawChopstick(rightSticks[j][0], rightSticks[j][1], rightSticks[j][2], rightSticks[j][3]);  
                		arrJ[count++]=j;
                		arrJ[count++]=(j+1)%N;
                	}
		}
		for(int k=0;k<N;k++){
			drawn=0;
			for(int l=0;l<count;l++){
				if(k == arrJ[l]){
					drawn=1;
					break;
				}	
			}
			if(drawn==0){
				int index = k-1;
				if(index==-1) index=4;
				drawChopstick(chopSticks[index][0], chopSticks[index][1], chopSticks[index][2], chopSticks[index][3]);  
			}
		}
		glFlush();               
		sleep(5);   
    	}
	glutMouseFunc(mouseInit);        
}
void reshape(int w, int h){
    	float t1,t2;
    	glViewport(0,0,w,h);
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
void idle(){
	glutPostRedisplay();
}
	
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	p1 = glutCreateWindow("Dining Philosophers Problem");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
}
