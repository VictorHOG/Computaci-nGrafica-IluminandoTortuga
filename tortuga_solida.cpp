/**
* Carlos Felipe Montoya - 1535038
* Diana Melissa Millares - 1526062
* Victor Hugo Ortega - 1532342
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "light.h"
#include "primitivas.h"
#include "camera.h"
#include "vector_tools.h"

bool command = false; /* command mode */
char strCommand[256];
static camera *LOCAL_MyCamera;
static int old_x, old_y;
static light **LOCAL_MyLights;
static int current_mode = 0;
static int current_light = -1;

void drawSphereTurtle() {

    int slices = 40;
    int stacks = 40;

    glPushMatrix();
    glScalef(1.0f, .3f, 1.0f);
    glutSolidSphere(1.0, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.7f, 0.0f, .7f);
    glutSolidSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.7f, 0.0f, .7f);
    glutSolidSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.7f, 0.0f, -.7f);
    glutSolidSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.7f, 0.0f, -.7f);
    glutSolidSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glScalef(1.0f, .6f, 1.0f);
    glTranslatef(0.0f, 0.0f, -1.2f);
    glutSolidSphere(.4, slices, stacks);
    glPopMatrix();
}

void display(void) {
    float At[3];
    float Direction[3];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    SetGLCamera( LOCAL_MyCamera );
    
    SetLight( LOCAL_MyLights[0] );
    SetLight( LOCAL_MyLights[1] );
    SetLight( LOCAL_MyLights[2] );
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    drawSphereTurtle();

    switch( current_light ){
        case 0:
            At[0] = LOCAL_MyLights[current_light]->position[0];
            Direction[0] = - LOCAL_MyLights[current_light]->position[0];
            Draw_Parallel(At);
            break;
        case 1:
            At[1] = LOCAL_MyLights[current_light]->position[1];
            Direction[1] = - LOCAL_MyLights[current_light]->position[1];
            Draw_Meridian(At);
            break;
        case 2:               
            At[2] = LOCAL_MyLights[current_light]->position[2];
            Direction[2] = - LOCAL_MyLights[current_light]->position[2];
            Draw_Vector(At, Direction);
            break;
        default:
            break;
    }

    glPopMatrix();

    SetGLCamera( LOCAL_MyCamera );

    glutSwapBuffers();
}

void Examinar(int x, int y){
	float rot_x, rot_y;
	rot_y = (float)(old_y - y);
	rot_x = (float)(x - old_x);
	Rotar_Latitud( LOCAL_MyCamera, rot_y * DEGREE_TO_RAD );
	Rotar_Longitud( LOCAL_MyCamera, rot_x * DEGREE_TO_RAD );
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void MouseMotion(int x, int y){
	old_y = y;
	old_x = x;
}

void Mouse_Luces(int x, int y){
    float rot_x, rot_y;
    rot_y = (float)(old_y - y);
    rot_x = (float)(x - old_x);
    Rotar_Luces_Latitud( LOCAL_MyLights[current_light],rot_y*DEGREE_TO_RAD );
    Rotar_Luces_Longitud( LOCAL_MyLights[current_light], rot_x*DEGREE_TO_RAD );
    LOCAL_MyLights[current_light]->pointAtInfinity[0] =
        LOCAL_MyLights[current_light]->position[0];
    LOCAL_MyLights[current_light]->pointAtInfinity[1] =
        LOCAL_MyLights[current_light]->position[1];
    LOCAL_MyLights[current_light]->pointAtInfinity[2] =
        LOCAL_MyLights[current_light]->position[2];
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Mouse_Luces_Acercar_Alejar(int x, int y){
    float step;
    step = (float) (y - old_y) / 20.0f;
    old_y = y;
    Acercar_Alejar_Luces( LOCAL_MyLights[current_light], step );
    glutPostRedisplay();
}

static void SpecialKey ( int key, int x, int y ){
	switch(key) {
		case GLUT_KEY_F1:
             current_mode = 0;
		     glutPassiveMotionFunc(MouseMotion);
		     LOCAL_MyCamera->camMovimiento = CAM_STOP;
             current_light = -1;
		     break;
		case GLUT_KEY_F2:
             if (current_mode != 0) break;
             current_mode = 1;
		     glutPassiveMotionFunc(Examinar);
		     LOCAL_MyCamera->camMovimiento = CAM_EXAMINAR;
		     break;
		case GLUT_KEY_F3:
             if (current_mode != 0) break;
             current_mode = 2;
		     glutPassiveMotionFunc(MouseMotion);
		     LOCAL_MyCamera->camMovimiento = CAM_PASEAR;
		     LOCAL_MyCamera->camAtY = 0;
		     LOCAL_MyCamera->camViewY = 0;
		     SetDependentParametersCamera( LOCAL_MyCamera );
		     break;
		case GLUT_KEY_F4:
		     if(LOCAL_MyCamera->camProjection == CAM_CONIC){
			printf("PARALLEL\n");
			LOCAL_MyCamera->x1=-3;
			LOCAL_MyCamera->x2=3;
			LOCAL_MyCamera->y1=-3;
			LOCAL_MyCamera->y2=3;
			LOCAL_MyCamera->z1=-5;
			LOCAL_MyCamera->z2=5;
		     	LOCAL_MyCamera->camProjection = CAM_PARALLEL;
		     } else{ LOCAL_MyCamera->camProjection = CAM_CONIC; printf("CONIC\n"); }
		     break;
		case GLUT_KEY_F10: //Reset Camera
		     LOCAL_MyCamera->camAtX =0;
		     LOCAL_MyCamera->camAtY =0;
		     LOCAL_MyCamera->camAtZ =0;
		     LOCAL_MyCamera->camViewX = 0;
		     LOCAL_MyCamera->camViewY = 1;
		     LOCAL_MyCamera->camViewZ = -3;
		     SetDependentParametersCamera( LOCAL_MyCamera );
		     break;
        case GLUT_KEY_F8:
             if (current_mode != 0 && current_mode != 7) break;
             current_mode = 7;
             if (current_light == -1) glutPassiveMotionFunc(Mouse_Luces);
             if (current_light != 2) current_light++;
             else current_light = 0;
             printf("Luz actual = %d\n",current_light);
             break;
        case GLUT_KEY_F9:
             if (current_light != -1) {
                if ( LOCAL_MyLights[current_light]->switched ) {
                    SwitchLight( LOCAL_MyLights[current_light], FALSE);
                }
                else SwitchLight( LOCAL_MyLights[current_light], TRUE);
             }
             break;
        case GLUT_KEY_HOME: //Reset Camera
             LOCAL_MyCamera->camAtX =0;
             LOCAL_MyCamera->camAtY =0;
             LOCAL_MyCamera->camAtZ =0;
             LOCAL_MyCamera->camViewX = 0;
             LOCAL_MyCamera->camViewY = 1;
             LOCAL_MyCamera->camViewZ = -3;
             SetDependentParametersCamera( LOCAL_MyCamera );
             break;
		default:
		     printf("key %d %c X %d Y %d\n", key, key, x, y );
	}
	glutPostRedisplay();
}

void parseCommand(char* strCommandParse) {
    char *strToken0;
    char *strToken1;
    double val;
    strToken0 = strtok(strCommandParse, " ");
    while ((strToken1 = strtok(NULL, " ")) != NULL) {
        val = atof(strToken1);
        if (!strcmp("fd", strToken0)) { // FORWARD
            glTranslatef(0.0, 0.0, val);
        } else if (!strcmp("bk", strToken0)) { // BACK
            glTranslatef(0.0, 0.0, -val);
        } else if (!strcmp("rt", strToken0)) { // RIGHT
            glRotatef(-val, 0., 1., 0.);
        } else if (!strcmp("lt", strToken0)) { // LEFT
            glRotatef(val, 0., 1., 0.);
        } else if (!strcmp("up", strToken0)) { // UP
            glRotatef(val, 1., 0., 0.);
        } else if (!strcmp("dn", strToken0)) { // DOWN
            glRotatef(-val, 1., 0., 0.);
        }
        strToken0 = strtok(NULL, " ");
        display();
    }glutSpecialFunc(SpecialKey);
    // EXIT COMMAND MODE
    if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
        command = false;
        // HOME
    } else if (strToken0 != NULL && !strcmp("home", strToken0)) {
        glLoadIdentity();
    }
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	SetGLAspectRatioCamera( LOCAL_MyCamera );
}

/* OLD RESHAPE
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}*/

void keyboard(unsigned char key, int x, int y) {
    if (command) {
        if (key == 13) {
            strcat(strCommand, " ");
            if (strlen(strCommand) == 1) command = false;
            parseCommand(strCommand);
            strcpy(strCommand, "");
        } else {
            char strKey[2] = " ";
            strKey[0] = key;
            printf("%s\n",strKey);
            strcat(strCommand, strKey);
        }
    } else { // not in command mode
        switch (key) {
            case 'h':glutSpecialFunc(SpecialKey);
                printf("help\n\n");
                printf("c - Toggle culling\n");
                printf("q/escape - Quit\n\n");
                break;
            case 'c':
                if (glIsEnabled(GL_CULL_FACE))
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);
                break;
            case '1':
                glRotatef(1.0, 1., 0., 0.);
                break;
            case '2':
                glRotatef(1.0, 0., 1., 0.);
                break;
            case 'i':
                command = true;
                break;
            case 27:
                DestroyCamera(&LOCAL_MyCamera);
                DestroyLight( LOCAL_MyLights[0] );
                DestroyLight( LOCAL_MyLights[1] );
                DestroyLight( LOCAL_MyLights[2] );
                free (LOCAL_MyLights);
                exit(0);
                break;
            case 'q':
                exit(0);
                break;
        }
    }
    glutPostRedisplay();
}

void Zoom(int x, int y){
	float zoom;
	zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
	old_y = y;
	switch(LOCAL_MyCamera->camMovimiento){
		case CAM_EXAMINAR:
			if (LOCAL_MyCamera->camAperture + zoom > (5 * DEGREE_TO_RAD) &&
			LOCAL_MyCamera->camAperture + zoom < 175 * DEGREE_TO_RAD)
			LOCAL_MyCamera->camAperture=LOCAL_MyCamera->camAperture + zoom;
		break;
	}
	glutPostRedisplay();
}

void Andar(int x, int y){
	float rotacion_x, avance_y;
	avance_y = (float)(y - old_y) / 10;
	rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
	YawCamera( LOCAL_MyCamera, rotacion_x );
	AvanceFreeCamera( LOCAL_MyCamera, avance_y);
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	old_x = x;
	old_y = y;
	switch(button){
		case GLUT_LEFT_BUTTON:
            if(current_light > 0){
                if (state == GLUT_DOWN)
                    glutMotionFunc(Mouse_Luces_Acercar_Alejar);
                if (state == GLUT_UP){
                    glutPassiveMotionFunc(Mouse_Luces);
                    glutMotionFunc(NULL);
                }
            } else {
                switch(LOCAL_MyCamera->camMovimiento){
                    case CAM_EXAMINAR:
                        if (state == GLUT_DOWN) glutMotionFunc(Zoom);
                        if (state == GLUT_UP){
                            glutPassiveMotionFunc(Examinar);
                            glutMotionFunc(NULL);
                        }
                        break;
                    case CAM_PASEAR:
                        if (state == GLUT_DOWN) glutMotionFunc(Andar);
                        if (state == GLUT_UP) glutMotionFunc(NULL);
                    break;
                }
            }
		    break;
		case GLUT_RIGHT_BUTTON:
		    if (state == GLUT_DOWN) ;
		    break;
		default:
            break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("tortuga");

    LOCAL_MyCamera = CreatePositionCamera(0.0, 1.0, -3.0);

    //Reservamos memoria para tres interfaces de luces
    LOCAL_MyLights = (light **) malloc(3 * sizeof(light *));
    //Creamos las luces y damos a cada una sus características
    for(int i=0;i<3;i++) {
        LOCAL_MyLights[i] = CreateDefaultLight();
        LOCAL_MyLights[i]->type = AGA_DIRECTIONAL;
        LOCAL_MyLights[i]->id = GL_LIGHT0 + i;
        LOCAL_MyLights[i]->position[0] = 1.0f;
        LOCAL_MyLights[i]->position[1] = 1.0f;
        LOCAL_MyLights[i]->position[2] = 1.0f;
        LOCAL_MyLights[i]->position[3] = 0.0f;
        LOCAL_MyLights[i]->pointAtInfinity[0] = LOCAL_MyLights[0]->position[0];
        LOCAL_MyLights[i]->pointAtInfinity[1] = LOCAL_MyLights[0]->position[1];
        LOCAL_MyLights[i]->pointAtInfinity[2] = LOCAL_MyLights[0]->position[2];
    }

    glEnable(GL_COLOR_MATERIAL);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKey);
    glutPassiveMotionFunc(MouseMotion);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
