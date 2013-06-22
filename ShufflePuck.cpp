/*
*	Projet d'infographie
*	Auteur: Olivier Francillon
*
*
*
*/


#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

float incr = 0.1;
int modeSolide = 0;
int identMenu;
int animation = 0;
int started = 0;

/*************************
* Paramètres de la fenètre
*/
int Wwidth = 800;
int Wheight = 500;

/*************************
* Paramètre de la caméra
*/
float up = 2;
float left = 1;

/*************************
* Paramètres de la table
*/
float tableL = 1;
float tableP = 2;

/*************************
* Paramètres des palets
*/ 
float palet1x=0.0;
float palet1z=0.9;
float palet1DX=0.0;
float palet1DZ=0.0;
float palet2x=0.0;
float palet2z=-0.9;
float palet2DX=0.0;
float palet2DZ=0.0;
float paletL = 0.2;
float paletP = 0.1;
float deplacementX, deplacementPX = 0;
float deplacementZ, deplacementPZ = 0;
float maxDroite = tableL/2 - paletL/2;
float maxGauche = -tableL/2 + paletL/2;

/*************************
* Paramètres de la balle
*/
float ballSize = 0.02;
float balleX = 0.0, balleZ = 0.0;
float balleDX = 0.0 , balleDZ = 0.0;

/*************************
* Paramètres des lumières
*/
GLfloat lumiere_position[ ]= {3.0, 1.0, 0.0, 1.0};	
GLfloat lum_ambiante[ ]={0.2, 0.2, 0.2, 1.0};
GLfloat lum_diffuse[ ]={1, 1.0, 1.0, 1.0};

GLfloat lumiere_position1[ ]= {0.0, 2.0, -1.0, 1.0};	
GLfloat lum_diffuse1[ ]={1.0, 1, 1.0, 1.0};

GLfloat lumiere_position2[ ]= {2.0, 0.0, 2.0, 1.0};	
GLfloat lum_diffuse2[ ]={1.0, 1.0, 1, 1.0};


/*************************
* Paramètres du fog
*/

GLfloat fogColor[4] ={0.8,0.8,0.8,1.};

const GLfloat P[4][3]=
// tableau des sommets du plateau
{
{-1,0,1}, // P0
{1,0,1}, // P1
{1,0,-1}, // P2
{-1,0,-1}
};


const GLubyte facette[1][4] =
{
	{0,1,2,3}
};

void win(int player){
	printf("Player %d win\n",player);
}

void MouseMove(int x, int y){

	
	deplacementX = ((float)x - Wwidth) /((float)(Wwidth/2)-100);
	deplacementZ = ((float)y - Wheight) /((float)(Wheight/2)-75);
	
}

void deplacementPalet(){
	palet1DX = deplacementX - deplacementPX;
	palet1x += palet1DX;
	float limite = tableL/2 - paletL/2 -0.02;
	if (palet1x >= limite){
		palet1x = limite;
	} else if (palet1x <= -limite){
		palet1x = -limite;
	}
	deplacementPX = deplacementX;	
	float limiteSup, limiteInf;
	
	
	palet1DZ = deplacementZ - deplacementPZ;
	palet1z += palet1DZ;
	limiteSup = (tableP/6 + paletP/2);
	limiteInf = (tableP/2 + paletP/2);

	if (palet1z >= limiteInf){
		palet1z = limiteInf;
	} else if (palet1z <= limiteSup){
		palet1z = limiteSup;
	}
	deplacementPZ = deplacementZ;	
	
}

void initGame(){
	animation = 0;
	started=0;
	balleX=0.0;
	balleZ=0.0;
	balleDX=0.0;
	balleDZ=0.0;
}

void boudingPalet(int player, float posBX, float posBZ){
	float impactPos, posPaletX,posPaletZ;
	float oldBDZ = balleDZ, oldBDX = balleDX;
	int impact = 0;
	int stick = 0;
	if(player == 1){
		posPaletX = palet1x;
		posPaletZ = palet1z - paletP/2;
	} else {
		posPaletX = palet2x;
		posPaletZ = palet2z + paletP/2;
	}
	
	if(((posBZ >= posPaletZ && 
		 posBZ <= posPaletZ + paletP &&
		player == 1)  &&
		posBX >= posPaletX - paletL/2 &&
		posBX <= posPaletX + paletL/2)){

		if(palet1DZ < 0 && palet1DZ <= balleDZ){
			balleDZ = palet1DZ;
			balleDX = palet1DX;
			stick = 1;
		}else if(palet1DZ == 0){
			balleDZ = palet1DZ;
			balleDX = palet1DX;
			stick = 1;
		}else if(palet1DZ >= 0 || palet1DZ > -oldBDZ){
			balleDZ=oldBDZ+palet1DZ;
			balleDX=oldBDX+palet1DX;
			stick = 0;
		}
		

		impact = 1;
	}

	if(impact && !stick ){
		
		if(player == 1){
			if(balleDZ > 0){
				balleDZ += 10*palet1DZ;
			}else{
				balleDZ -= 10*palet1DZ;
			}
			
				
				balleDX += palet1DX/9 ;
			
		}
	}
	printf("Stick: %d\n", stick);
	
	
}
void deplacementBalle(int start){
	if (start){
		srand ( time(NULL) );
		balleDX = (rand() % 10 )/1000.0;
		balleDZ = ((rand() % 10) + 1 )/1000.0;
	}

	if (balleX >= tableL/2 - 0.02 - ballSize/2){ balleDX *= -1 ;}
	if (balleX <= -(tableL/2 - 0.02 - ballSize/2)){ balleDX *= -1 ;}
	if (balleZ >= tableP/2){ win(2);initGame();};
	if (balleZ <= -tableP/2){ win(1);initGame();};
	boudingPalet(1,balleX,balleZ);
	
	balleX += balleDX;
	balleZ += balleDZ;
}

void material(float r, float b, float g){
	GLfloat mat_diffuse[ ]={r,b,g,1.0};
	GLfloat mat_specular[ ]={1,1,1,1};
	GLfloat surf_shininess[ ]={30};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);			//Matérieau Diffu
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, surf_shininess);

	
}
void plateau()
{
	glPushMatrix();
	glScalef(tableL,0.02,-tableP);
	glutSolidCube(1);
	glPopMatrix();

}

//Dessin de bordure position x,y,z et dimension longueur, hauteur, profondeur
void bordure(float x, float y, float z, float l, float h, float p)
{
	material(0,0,1);
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(l,h,p);
	glutSolidCube(1);
	glPopMatrix();
}

void tableJeu(){
	material(0,1,0);
	glPushMatrix();
	plateau();
	bordure(-0.49,0.06,0,0.02,0.1,-2);
	bordure(0.49,0.06,0,0.02,0.1,-2);
	glPopMatrix();
}

void paletJoueur1(float x, float y, float z){
	float largeur = 0.2;
	float hauteur = 0.05;
	float profondeur = 0.1;
	material(.5,.5,.5);
	glPushMatrix();
	glTranslatef(x,y + hauteur/2,z);
	glScalef(largeur,hauteur,profondeur);
	glutSolidCube(1);
	glPopMatrix();
}

void paletJoueur2(float x, float y, float z){
	float largeur = 0.2;
	float hauteur = 0.05;
	float profondeur = 0.1;
	material(.5,.5,.5);
	glPushMatrix();
	glTranslatef(x,y + hauteur/2,z);
	glScalef(largeur,hauteur,profondeur);
	glutSolidCube(1);
	glPopMatrix();
}

void balle(float x, float y, float z){
	material(1,1,1);
	glPushMatrix();
	glTranslatef(x,0.04,z);
	glutSolidSphere(ballSize,10,10);
	glPopMatrix();

}

void axes(){
	glLineWidth(5);
        glColor3ub(255,0,0);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(1.0,0.0,0.0); 
        glEnd();
        glColor3ub(0,255,0);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(0.0,1.0,0.0); 
        glEnd();
        glColor3ub(0,0,255);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(0.0,0.0,1.0); 
        glEnd();
        glLineWidth(1);
}


void light(){
	glLightfv(GL_LIGHT0, GL_POSITION, lumiere_position);  	//Position Lumière
	glLightfv(GL_LIGHT0, GL_AMBIENT, lum_ambiante);			//Lumière ambiante
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lum_diffuse);			//Lumière diffuse

	glLightfv(GL_LIGHT1, GL_POSITION, lumiere_position1);  	//Position Lumière
	//glLightfv(GL_LIGHT1, GL_AMBIENT, lum_ambiante1);			//Lumière ambiante
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lum_diffuse1);			//Lumière diffuse

	glLightfv(GL_LIGHT2, GL_POSITION, lumiere_position2);  	//Position Lumière
	//glLightfv(GL_LIGHT2, GL_AMBIENT, lum_ambiante2);			//Lumière ambiante
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lum_diffuse2);			//Lumière diffuse

}



void fog(){

	//enclenchereffetbrouillard
	glFogi(GL_FOG_MODE,GL_EXP);
	glFogfv(GL_FOG_COLOR,fogColor);
	//la couleur du brouillard
	glFogf(GL_FOG_DENSITY, 0.10);
	//la densité du bruillard
}

void lookAt(){
	gluLookAt(	left,up,3.0, 	//Où je suis
				0.0,0.0,0.0, 	//Oû je regarde
				0.0,1.0,0.0);	//Comment je regarde
}

void display(){		
	int i;		

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);		// efface contenu de la fenêtre (couleur et profondeur)
	glCullFace(GL_BACK);						//Enclenche le back face culling
	glEnable(GL_DEPTH_TEST);					//Enclenche la estion de la profondeur
	axes();
	
	glColor3ub(0,255,0);		//Vert
	tableJeu();					//Dessin du plateau
	//calculDeplacement();
	
	paletJoueur1(palet1x,0,palet1z);
	paletJoueur2(palet2x,0,palet2z);
	balle(balleX,0,balleZ);
	glPopMatrix();				//Dépile la matrice des coordonnées
	light();
	fog();

	glutSwapBuffers(); 			//Echange les buffers
 
}

void reshape(int w, int h) // fenétre pour l'affichage de la scène 3D
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	//Projection perspective
	gluPerspective	(	50.0,		// angle d'ouverture
						4.0/3.0,	// rapport
						0.1,		// distance avant-plan
						1000.0);	// distance arrière-plan

	glMatrixMode (GL_MODELVIEW);

}

void init(void){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(255.0, 255.0, 255.0, 0.0);		// Fond Blanc
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_FOG);
	glMatrixMode(GL_MODELVIEW);					// matrice de modélisation
	glLoadIdentity();							// matrice d'identité
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 	//Mode Polygone
	lookAt();
	
}


void gestionMenu(int value)
{
  switch(value)		//Gestion des menus
  {
	case 1: 
		modeSolide = 0;  
		break;
	case 2: 
		modeSolide = 1;  
		break;
	case 3: 
		modeSolide = 2;  
		break;
	case 4: 
		animation = 1;
		break;
	case 5: 
		animation = 0;
		break;
	case 6: 
		exit(0); 
		break;
  }

  glutPostRedisplay();
}

void menu(){
	identMenu=glutCreateMenu(gestionMenu); //Ajout des menus
	glutAddMenuEntry("mode plein (p)", 1);
	glutAddMenuEntry("mode filaire (f)", 2);
	glutAddMenuEntry("mode point (t)", 3);
	glutAddMenuEntry("animation on (i)", 4);
	glutAddMenuEntry("animation off (o)", 5);
	glutAddMenuEntry("quitter (q)", 6);
	glutAttachMenu(GLUT_LEFT_BUTTON); //On attache le menu au bouton gauche de la souris
}

void  keyboard (unsigned char key, int x, int y)
{
    switch (key) { 	  //Traitement des touches
            
        case 'p':    //  Change le mode en solide
        case 'P':
			modeSolide = 0;
            break;
        case 'f':    // change le mode en filaire
        case 'F':
            modeSolide = 1;
            break;
        case 't':    //  change le mode en point
        case 'T':
			modeSolide = 3;
            break;
        case 'i':
        case 'I':
        	animation = 1;
        	break;
        case 'o':
        case 'O':
        	animation = 0;
        	break;
        case 'q':	 // quitte le programme
        case 'Q':
        case 27 :	// quitte le programme avec la touche ESC
            exit(0);
            break;
            		//Il y a surement un moyen plus propre
        case 'w':  
        case 'W':
        	up+=0.1;
        	init();
        	break;
        case 's':
        case 'S':
        	up-=0.1;
        	init();
        	break;
        case 'a':
        case 'A':
        	left+=0.1;
        	init();
        	break;
        case 'd':
        case 'D':
        	left-=0.1;
        	init();
        	break;
    }

    glutPostRedisplay(); //Relance l'affichage
}


void anime(void)
{
    
    sleep(.01);
    if(animation){
    	if (!started){
    		started = 1;
    		deplacementBalle(started);	
    	}
    	deplacementPalet();
    	deplacementBalle(!started);

	    
	}
	
	
    glutPostRedisplay();
}
int main(int argc, char** argv)			
{
	glutInit(&argc, argv);				// initialise GLUT et traite les éventuels
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);	// double tampon, z-buffer
	glutInitWindowSize(Wwidth, Wheight);					// largeur et hauteur de la fenêtre
	glutInitWindowPosition(300, 100);				// position de la fenêtre
	glutCreateWindow("Shuffle Puck");			// Création d'une fenêtre

	glutPassiveMotionFunc(MouseMove); 
	reshape(1, 1);						// Apelle la fonction de reshape

	init();								// Apelle la fonction d'initialisation
	glutDisplayFunc(display);			// Apelle la fonction d'affichage
	menu();								// Apelle la fonction du menu
	glutKeyboardFunc(keyboard);			// Apelle la fonction de gestion du clavier
	glutIdleFunc(anime);				// Apelle la fonction d'animation

	glutMainLoop();						// Lance la boucle
}