#include <GL/glut.h>
#include <stdio.h> 

#define HUD_SHIFT 500
int HUD_w, HUD_h ;
//const int font=(int)GLUT_BITMAP_9_BY_15;
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;


static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;
	HUD_w = width;
	HUD_h = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);     glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
} 

void setOrthographicProjection() {
	glViewport(0, 0, HUD_w, HUD_h);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, HUD_w, 0, HUD_h);
	glScalef(1, -1, 1);
	glTranslatef(0, -HUD_h, 0);
	glMatrixMode(GL_MODELVIEW);
} 

void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
} 

void renderBitmapString(float x, float y, void *font,const char *string){
	const char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrengthBar(float x,float y,int length){
	glRasterPos2f(x,y);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+length,y);
	glVertex2f(x+length,y+10);
	glVertex2f(x,y+10);
	glEnd();
}

void HUD(int score,int time,int strength) {

	char a[30],b[30];
	sprintf(a, "%2d sec", time);
	sprintf(b, "%2d", score);
	
	glColor3d(1.0, 0.0, 0.0);
	setOrthographicProjection();
	glPushMatrix();
	
	glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);// disable depth-testing
	
	glLoadIdentity();

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	int windowWidth = w * 1 / 2;
	int windowHeight = h * 1 / 2;
	
	glColor4f(1.0f,0.0f,1.0f,1.0f);
	renderBitmapString((w-windowWidth)/2,50,(void *)font,"TOTAL TIME PASSED");
	renderBitmapString((w-windowWidth)/2+HUD_SHIFT,50, (void*)font, a);
	
	glColor4f(0.0f,1.0f,1.0f,1.0f);
	renderBitmapString((w-windowWidth)/2,80,(void *)font,"YOUR SCORE :- ");
	renderBitmapString((w-windowWidth)/2+HUD_SHIFT,80,(void *)font,b);
	
	glColor4f(1.0f,1.0f,0.0f,1.0f);
	renderBitmapString((w-windowWidth)/2,110,(void *)font,"TIME REMAINING ON THIS TILE :- ");
	renderStrengthBar((w-windowWidth)/2+HUD_SHIFT,100,strength*5);
	
	glPopMatrix();
	resetPerspectiveProjection();

}




static void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	HUD(10,10,10);
	glutSwapBuffers();
} 


void update(int value){
	glutTimerFunc(1000, update, 0);
	glutPostRedisplay();
} 

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);     glutCreateWindow("Font Rendering Using Bitmap Font - Programming Techniques0");     glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(25, update, 0);     glutMainLoop();
	return EXIT_SUCCESS;
}
