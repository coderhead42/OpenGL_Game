#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <time.h>
#include <stdio.h>
#include "glm.cpp"
#define INF (GLfloat)100

using namespace std;

// Global Declarations
float pi  = acos(-1.0);

int myTimer=0; // Timer for the Game
int currLevel=0; //Level at which the user is at

float _g = 0.04f;

bool _highShininess = false; //Whether the shininess parameter is high
bool _lowSpecularity = false; //Whether the specularity parameter is high
bool _emission = false; //Whether the emission parameter is turned on

bool upkey=0,downkey=0,leftkey=0,rightkey=0; // ? handleKeypress shoud take care of all this
int ckey=0; // detects the camera mode
int oldk=0,oldj=0; //I don't know why I declared them last time. But, I will find out. Might be for remembering the last tile.


/*****************************************************/

#define HUD_SHIFT 500
int HUD_w, HUD_h ;
//const int font=(int)GLUT_BITMAP_9_BY_15;
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;

/****************************************************/




typedef struct {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;

class Location{
public:
	float x,y,z;
	Location(){
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	Location( float a, float b, float c){
		this->x = a;
		this->y = b;
		this->z = c;
	}
};


void addTexture( vector<GLuint> &arr, string filename ){
	arr.push_back(SOIL_load_OGL_texture(
			(char *)filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			));
}


vector< GLuint > tileTex;

class Tiles
{
	public:
	bool trampoline, vertical, horizontal, box, activated, dead;
	int strength, texIndex;
	Location At;
	Location Vel;
	// GLMmodel* mymodel;

	void setLocation( float a, float b, float c ){
		this->At.x = a;
		this->At.y = b;
		this->At.z = c;
	}

	void setTex( int x ){
		this->texIndex = x;
	}

	void setTrampoline(){
		this->trampoline = true;
	}
	
	void setVertical(){
		this->Vel = Location( 0.0f, 0.1f, 0.0f );
		if( rand()%2 )
			this->At.y += rand()%9 * 0.1;
		else
			this->At.y -= rand()%9 * 0.1;
		this->vertical = true;
	}
	
	void setHorizontal(){
		this->Vel = Location( 0.1f, 0.0f, 0.0f );
		if( rand()%2 )
			this->At.x += rand()%9 * 0.1;
		else
			this->At.x -= rand()%9 * 0.1;
		this->horizontal = true;
	}

	void setBox(){
		this->box = true;
	}
	
	void setDead(){
		this->dead = true;
	}

	void unsetTrampoline(){
		this->trampoline = false;
	}
	
	void unsetVertical(){
		this->vertical = false;
	}
	
	void unsetHorizontal(){
		this->horizontal = false;
	}

	void unsetBox(){
		this->box = false;
	}
	
	void setStrength( int X ){
		this->strength = X;
	}

	void activate(){
		setTex(4);
		this->activated = true;
	}

	void update(){
		if( this->activated ){
			this->strength -= 1;
			if( this->strength < 0 ){
				setDead();
				return;		
			}
		}

		if( this->vertical ){
			if( abs(this->At.y + 1.0 + this->Vel.y)>2.0f ){
				this->Vel.y *= -1;
			}
			else{
				this->At.y += this->Vel.y;
			}
		}

		if( this->horizontal ){
			if( abs(this->At.x + 1.0 + this->Vel.x)>2.0f ){
				this->Vel.x *= -1;
			}
			else{
				this->At.x += this->Vel.x;
			}
		}
	}
	// void setModel( GLMmodel *X ){
	// 	this->mymodel = X;
	// }

	void draw(){

		if( this->dead )
			return;
		// Binding

		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		// //Bottom
		glBindTexture(GL_TEXTURE_2D, tileTex[this->texIndex]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



		if( this->box ){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.y + 0.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.y + 0.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.y + 2.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.y + 2.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.y + 0.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.y + 0.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.y + 2.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.y + 2.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.x + 0.0f, this->At.y + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 0.0f, this->At.y + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.y + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.y + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.x + 0.0f, this->At.y + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 0.0f, this->At.y + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.y + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 2.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.y + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			glBegin(GL_QUADS);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glTexCoord2f(this->At.x + 0.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 0.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 2.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 2.0f);
			glTexCoord2f(this->At.x + 2.0f, this->At.z + 0.0f);
			glVertex3f( this->At.x + 2.0f, this->At.y + 0.0f, this->At.z + 0.0f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		}
	}

	Tiles( float a, float b, float c ){
		this->At.x = a;
		this->At.y = b;
		this->At.z = c;
	
		this->activated = false;
		this->dead = false;

		if ( (rand() % 5)==1 ){
			setTex(3);
			setVertical();
			unsetHorizontal();
		}
		else if( (rand() % 5)==1 ){
			setTex(3);
			setHorizontal();
			unsetVertical();
		}
		else{
			setTex(2);
			unsetHorizontal();
			unsetVertical();
		}

		if ( (rand() % 5)==1 ){
			setBox();
			setTex(1);
		}
		else{
			unsetBox();
		}

		if ( (rand() % 5)==1 ){
			setTrampoline();
			setTex(0);
		}
		else{
			unsetTrampoline();
		}

		setStrength(100/(currLevel+1)); //This will depend on the difficulty level or the game level
	}

	Tiles(){
	}
};

vector<Tiles> myTiles;
class Vader{
		public:
		Location At; // Location of the model
		Location Vel;

		GLMmodel* mymodel; //Contains the character model
		float viewangle; //The angle in which the character is facing
		float walk_angle; //The angle with which the legs of the character will rotate while walking
		bool stable;

		bool jump;
		bool trampoline_jump;

		/*************************************************************/


		/***************************************************************/

		Vader(){
			this->mymodel = NULL;
			walk_angle = 20.0f;
			this->stable = false;
			this->viewangle = 180.0f;
			this->Vel = Location( 0.0f, 0.0f, 0.0f);
			this->jump=false;
			this->trampoline_jump=false;
		}

		void setLocation( float a, float b, float c ){
			this->At.x = a;
			this->At.y = b;
			this->At.z = c;
		}

		void setVelocity( float a, float b, float c ){
			this->Vel.x = a;
			this->Vel.y = b;
			this->Vel.z = c;
		}

		void setAngle( float x ){
			this->viewangle = x;
		}

		void setModel( string filename ){
			char *temp = new char[20];
			strcpy(temp, (char *)filename.c_str());
			this->mymodel = glmReadOBJ(temp);
			if (!this->mymodel) exit(0);
			glmUnitize(this->mymodel);
			glmFacetNormals(this->mymodel);        
			glmVertexNormals(this->mymodel, 90.0);
	   		glmLinearTexture(this->mymodel);
	   		glmSpheremapTexture(this->mymodel);
		}

		void update(){
			unsigned int i;
			for( i=0;i<myTiles.size();++i ){
				if( myTiles[i].dead )
					continue;
				if( (this->At.x-myTiles[i].At.x)>=0.0 && ((myTiles[i].At.x+2.0)-this->At.x)>=0.0 ){
					if((this->At.z-myTiles[i].At.z)>=0.0 && ((myTiles[i].At.z+2.0)-this->At.z)>=0.0 )
						break;
				}
			}
			if( i==myTiles.size() )
				this->stable = false;

			if( this->stable ){
				if( i<myTiles.size() && myTiles[i].vertical ){
					this->At.y += myTiles[i].Vel.y;
				}
				if( i<myTiles.size() && myTiles[i].horizontal ){
					this->At.x += myTiles[i].Vel.x;
				}
				this->Vel = Location( 0.0f, 0.0f, 0.0f);
				return;
			}

			if( (i<myTiles.size()) ){
				if((this->At.y-1.0)>myTiles[i].At.y && (this->At.y-1.0-this->Vel.y)<myTiles[i].At.y ){
					if( myTiles[i].trampoline ){
						this->Vel.y *= -1.0;
						this->trampoline_jump=true;
						myTiles[i].activate();
						return;
					}
					else
						this->trampoline_jump=false;
					this->At.y = myTiles[i].At.y + 1.0;
					this->stable = true;
					this->Vel = Location( 0.0f, 0.0f, 0.0f);
					myTiles[i].activate();
				}
				else if (myTiles[i].box && (this->At.y-1.0)>=(myTiles[i].At.y+2.0) && (this->At.y-1.0-this->Vel.y)<(myTiles[i].At.y+2.0))
				{
					if( myTiles[i].trampoline ){
						this->Vel.y *= -1.0;
						this->trampoline_jump=true;
						myTiles[i].activate();
						return;
					}
					else
						this->trampoline_jump=false;
					this->At.y = myTiles[i].At.y + 2.0 + 1.0;
					this->stable = true;
					this->Vel = Location( 0.0f, 0.0f, 0.0f);
					myTiles[i].activate();
				}
				else{
					this->At.y -= this->Vel.y;
				}
			}
			else{
				this->At.y -= this->Vel.y;
			}
		}

		void draw(){
				GLMmodel* model = this->mymodel;
		
				GLuint mode = GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL;

			static GLuint i;
			static GLMgroup* group;
			static GLMtriangle* triangle;
			static GLMmaterial* material;

			assert(model);
			assert(model->vertices);

			if (mode & GLM_COLOR)
				glEnable(GL_COLOR_MATERIAL);
			else if (mode & GLM_MATERIAL)
				glDisable(GL_COLOR_MATERIAL);

			glDisable(GL_TEXTURE_2D);

			group = model->groups;
			while (group) {
				if (mode & GLM_MATERIAL) {
					material = &model->materials[group->material];
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
					glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
				}

				if (mode & GLM_COLOR) {
					glColor3fv(material->diffuse);
				}


				if( upkey==1 || downkey==1 ){
					if( myTimer%10>=5 ){
						if( strncmp( group->name, " Rightx", 5 )==0 ){
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
							glRotatef(this->walk_angle,1.0f, 0.0f,0.0f);
						}
					}
					else{
						if( strncmp( group->name, " Leftx", 4 )==0 ){
							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
							glRotatef(this->walk_angle,1.0f, 0.0f,0.0f);
						}
					}
				}	
				glBegin(GL_TRIANGLES);
				for (i = 0; i < group->numtriangles; i++) {
					triangle = &T(group->triangles[i]);

					if (mode & GLM_FLAT)
						glNormal3fv(&model->facetnorms[3 * triangle->findex]);

					if (mode & GLM_SMOOTH)
						glNormal3fv(&model->normals[3 * triangle->nindices[0]]);
					if (mode & GLM_TEXTURE)
						glTexCoord2fv(&model->texcoords[2 * triangle->tindices[0]]);
					glVertex3fv(&model->vertices[3 * triangle->vindices[0]]);

					if (mode & GLM_SMOOTH)
						glNormal3fv(&model->normals[3 * triangle->nindices[1]]);
					if (mode & GLM_TEXTURE)
						glTexCoord2fv(&model->texcoords[2 * triangle->tindices[1]]);
					glVertex3fv(&model->vertices[3 * triangle->vindices[1]]);

					if (mode & GLM_SMOOTH)
						glNormal3fv(&model->normals[3 * triangle->nindices[2]]);
					if (mode & GLM_TEXTURE)
						glTexCoord2fv(&model->texcoords[2 * triangle->tindices[2]]);
					glVertex3fv(&model->vertices[3 * triangle->vindices[2]]);

				}
				glEnd();
				if( upkey==1 || downkey==1 ){
					glPopMatrix();
				}
				group = group->next;
			}
		}
};

Vader myChar;

class Cam{
public:
	Location At;
	Location To;
	Location Normal;
	float viewangle;

	Cam(){
		this->At = Location(0, 0, 0);
		this->To = Location(0, 0, 1);
		this->Normal = Location(0, 1, 0);
		this->viewangle = 0.0f;
	}

	Cam( Location A, Location B, Location C){
		this->At = A;
		this->To = B;
		this->Normal = C;
		this->viewangle = 0.0f;
	}

	void changeAngle( float X ){
		this->viewangle += X;
	}
};

Cam myCam;

/**************************************************************************************************/



class Level{
public:
	vector<Location> Position;
	Level(){
	}
	void load(void) {
		myTiles.clear();
		for(int i=0;i<(int)this->Position.size()-1;i++)
			myTiles.push_back(Tiles(this->Position[i].x, this->Position[i].y, Position[i].z));
	}
};

Level Level_Index[3];


void initLevels(void) {

	//Level 1
	
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-3.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-6.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-9.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-12.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-15.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-18.0f));
	Level_Index[0].Position.push_back(Location(-1.0f,-1.0f,-21.0f));
	Level_Index[0].Position.push_back(Location(2.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(5.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(8.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(11.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(14.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(17.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(20.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(23.0f,-1.0f,-24.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-21.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-18.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-15.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-12.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-9.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-6.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,-3.0f));
	Level_Index[0].Position.push_back(Location(26.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(29.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(32.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(35.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(38.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(41.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(44.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(47.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(50.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,0.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-3.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-6.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-9.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-12.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-15.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-18.0f));
	Level_Index[0].Position.push_back(Location(53.0f,-1.0f,-22.0f));


	//Level 2



	int n=20;
	int r=15;
	
	for(int i=(n/2);i<n;i++)
		Level_Index[1].Position.push_back(Location(r*cos(i*((double)2*3.14159265359/n))+15,-1.0,r*sin(i*((double)2*3.14159265359/n))));
	for(int i=0;i<(n/2);i++)
		Level_Index[1].Position.push_back(Location(r*cos(i*((double)2*3.14159265359/n))+15,-1.0,r*sin(i*((double)2*3.14159265359/n))));


	//Level 3

	
	Level_Index[2].Position.push_back(Location(0,-1.0,0));
	Level_Index[2].Position.push_back(Location(-3,-1.0,0));
	Level_Index[2].Position.push_back(Location(-6,-1.0,0));
	Level_Index[2].Position.push_back(Location(-6,-1.0,-3));
	Level_Index[2].Position.push_back(Location(-6,-1.0,-6));
	Level_Index[2].Position.push_back(Location(-6,-1.0,-9));
	Level_Index[2].Position.push_back(Location(-3,-1.0,-9));
	Level_Index[2].Position.push_back(Location(0,-1.0,-9));
	Level_Index[2].Position.push_back(Location(3,-1.0,-9));
	Level_Index[2].Position.push_back(Location(6,-1.0,-9));
	Level_Index[2].Position.push_back(Location(6,-1.0,-6));
	Level_Index[2].Position.push_back(Location(6,-1.0,-3));
	Level_Index[2].Position.push_back(Location(6,-1.0,0));
	Level_Index[2].Position.push_back(Location(6,-1.0,3));
	Level_Index[2].Position.push_back(Location(6,-1.0,6));
	Level_Index[2].Position.push_back(Location(3,-1.0,6));
	Level_Index[2].Position.push_back(Location(0,-1.0,6));
	Level_Index[2].Position.push_back(Location(-3,-1.0,6));
	Level_Index[2].Position.push_back(Location(-6,-1.0,6));
	Level_Index[2].Position.push_back(Location(-9,-1.0,6));
	Level_Index[2].Position.push_back(Location(-12,-1.0,6));
	Level_Index[2].Position.push_back(Location(-12,-1.0,3));
	Level_Index[2].Position.push_back(Location(-12,-1.0,0));
	Level_Index[2].Position.push_back(Location(-12,-1.0,-3));
	Level_Index[2].Position.push_back(Location(-12,-1.0,-6));
	Level_Index[2].Position.push_back(Location(-12,-1.0,-9));
	Level_Index[2].Position.push_back(Location(-12,-1.0,-12));
	Level_Index[2].Position.push_back(Location(-12,-1.0,-15));
	Level_Index[2].Position.push_back(Location(-9,-1.0,-15));
	Level_Index[2].Position.push_back(Location(-6,-1.0,-15));
	Level_Index[2].Position.push_back(Location(-3,-1.0,-15));
	Level_Index[2].Position.push_back(Location(0,-1.0,-15));
	Level_Index[2].Position.push_back(Location(3,-1.0,-15));
	Level_Index[2].Position.push_back(Location(6,-1.0,-15));
	Level_Index[2].Position.push_back(Location(9,-1.0,-15));
	Level_Index[2].Position.push_back(Location(12,-1.0,-15));
	Level_Index[2].Position.push_back(Location(12,-1.0,-12));
	Level_Index[2].Position.push_back(Location(12,-1.0,-9));
	Level_Index[2].Position.push_back(Location(12,-1.0,-6));
	Level_Index[2].Position.push_back(Location(12,-1.0,-3));
	Level_Index[2].Position.push_back(Location(12,-1.0,0));
	Level_Index[2].Position.push_back(Location(12,-1.0,3));
	Level_Index[2].Position.push_back(Location(12,-1.0,6));

}


void DrawGateway(void){

	Location temp=Level_Index[0].Position[(int)Level_Index[0].Position.size()-1];
	glBegin(GL_QUADS);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(temp.x,temp.y,temp.z);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(temp.x+3,temp.y,temp.z);
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(temp.x+3,temp.y+5,temp.z);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(temp.x,temp.y+5,temp.z);
	glEnd();

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
	
	setOrthographicProjection();
	glPushMatrix();
    	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);// disable depth-testing
	
	glLoadIdentity();

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	int windowWidth = w * 1 / 2;
	int windowHeight = h * 1 / 2;
	
	renderBitmapString((w-windowWidth)/2,50,(void *)font,"TOTAL TIME PASSED");
	renderBitmapString((w-windowWidth)/2+HUD_SHIFT,50, (void*)font, a);
	
	renderBitmapString((w-windowWidth)/2,80,(void *)font,"YOUR SCORE :- ");
	renderBitmapString((w-windowWidth)/2+HUD_SHIFT,80,(void *)font,b);
	
	renderBitmapString((w-windowWidth)/2,110,(void *)font,"TIME REMAINING ON THIS TILE :- ");
	renderStrengthBar((w-windowWidth)/2+HUD_SHIFT,100,strength*5);
	
    	glPopAttrib();
	glPopMatrix();
	resetPerspectiveProjection();

}


/********************************************************************************************/







static unsigned int getint( FILE *fp )
{
  int c, c1, c2, c3;

  // get 4 bytes
  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort( FILE *fp )
{
  int c, c1;
  
  //get 2 bytes
  c = getc(fp);  
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

void initTileTextures(){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	addTexture( tileTex, "tile2.jpg");
	addTexture( tileTex, "tile3.jpg");
	addTexture( tileTex, "tile4.jpg");
	addTexture( tileTex, "Tile2.jpg");
	addTexture( tileTex, "Tile.jpg");
}

vector<vector<GLuint> > skyboxTex(3);

void initSkybox(void){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	addTexture(skyboxTex[0], "fronthot.jpg");
	addTexture(skyboxTex[0], "lefthot.jpg");
	addTexture(skyboxTex[0], "backhot.jpg");
	addTexture(skyboxTex[0], "righthot.jpg");
	addTexture(skyboxTex[0], "tophot.jpg");
	addTexture(skyboxTex[0], "bothot.jpg");
}

void loadCamera(){
	gluLookAt( myCam.At.x, myCam.At.y, myCam.At.z, myCam.To.x, myCam.To.y, myCam.To.z,
	 			myCam.Normal.x, myCam.Normal.y, myCam.Normal.z);
}

void skyboxdraw(int level) {
	// Store the current matrix
    glPushMatrix();
 
    // Reset and transform the matrix.
    glLoadIdentity();

    loadCamera();

 

    // When you implement the camera modes, add the lookats here, bro. alright?

	// float posLight0[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	// float ambLight0[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	// glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight0);
    

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
 
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  INF, -INF, -INF );
        glTexCoord2f(1, 0); glVertex3f( -INF, -INF, -INF );
        glTexCoord2f(1, 1); glVertex3f( -INF,  INF, -INF );
        glTexCoord2f(0, 1); glVertex3f(  INF,  INF, -INF );
    glEnd();
 
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  INF, -INF,  INF );
        glTexCoord2f(1, 0); glVertex3f(  INF, -INF, -INF );
        glTexCoord2f(1, 1); glVertex3f(  INF,  INF, -INF );
        glTexCoord2f(0, 1); glVertex3f(  INF,  INF,  INF );
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -INF, -INF,  INF );
        glTexCoord2f(1, 0); glVertex3f(  INF, -INF,  INF );
        glTexCoord2f(1, 1); glVertex3f(  INF,  INF,  INF );
        glTexCoord2f(0, 1); glVertex3f( -INF,  INF,  INF );
 
    glEnd();
 
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -INF, -INF, -INF );
        glTexCoord2f(1, 0); glVertex3f( -INF, -INF,  INF );
        glTexCoord2f(1, 1); glVertex3f( -INF,  INF,  INF );
        glTexCoord2f(0, 1); glVertex3f( -INF,  INF, -INF );
    glEnd();
 
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -INF,  INF, -INF );
        glTexCoord2f(0, 0); glVertex3f( -INF,  INF,  INF );
        glTexCoord2f(1, 0); glVertex3f(  INF,  INF,  INF );
        glTexCoord2f(1, 1); glVertex3f(  INF,  INF, -INF );
    glEnd();
 
    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, skyboxTex[level][5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -INF, -INF, -INF );
        glTexCoord2f(0, 1); glVertex3f( -INF, -INF,  INF );
        glTexCoord2f(1, 1); glVertex3f(  INF, -INF,  INF );
        glTexCoord2f(1, 0); glVertex3f(  INF, -INF, -INF );
    glEnd();
 
    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}

void writeOnScreen(string a, int posx, int posy ){

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,400,0,400);
	glRasterPos2i(posx, posy);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glScalef(3.0f,3.0f,3.0f);
	for(int i=0;i<(int)a.size();i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, a[i]);
        glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void completeReset(int level){
	myTiles.clear();
	//tileTex.clear();
	//skyboxTex.clear();

	myChar = Vader();
	myChar.setLocation(0.3f, 3.5f, 0.3f);
	// this is the call that actualy reads the OBJ and creates the model object
	
	myChar.setModel("big_man2.obj");


	/*****************************************************************************/
	
	//float myz=0.0f;
	//myTiles.push_back(Tiles(0.0f, -1.0f, 0.0f));
		// cout<<"Tile Co-ordinates"<<endl;
	// cout<<myTiles[0].At.x<<" "<<myTiles[0].At.y<<" "<<myTiles[0].At.z<<endl;

	// cout<<"CharacterCo-ordinates"<<endl;
	// cout<<myChar.At.x<<" "<<myChar.At.y<<" "<<myChar.At.z<<endl;
	//for( int i=0;i<10;++i ){
	//	myTiles.push_back(Tiles(-1.0f, -1.0f, myz));
	//	myz -= 3.0f;
	//}


	Level_Index[level].load();

	/**********************************************************************************/

	myTiles[0].unsetBox();
	myTiles[0].unsetVertical();
	myTiles[0].unsetHorizontal();
	myTiles[0].unsetTrampoline();
	//This function should reset the game completely.
	//Idealy, the game should start again from the first level.
	//Vader should get back to the initial position.

	//All Tiles should be re-established, but we can also keep them same! (Can be used in implementing 'Try Again')
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
			break;
		case 'a':
			myCam.At.x += 0.1 ;
			break;
		case 'w':
			myCam.At.z += 0.1 ;
			break;
		case 's':
			myCam.At.z += -0.1 ;
			break;
		case 'd':
			myCam.At.x += -0.1 ;
			break;
		case ' ':
			if( myChar.stable ){
				myChar.Vel.y = -0.60;
				myChar.stable = false;
				myChar.jump = true;
			}
			if( myChar.trampoline_jump ){
				myChar.jump=true;
				myChar.trampoline_jump = false;
			}
			break;
		case 'r':
			completeReset(currLevel);
			break;
		case 'c':
			 ckey++;
			 if( ckey%6==4 ){
			 	int In = rand()%10;
			 	myCam.At = Location(myTiles[In].At.x,myTiles[In].At.y,myTiles[In].At.z);
			 }
			 else if( ckey%6==5 ){
			 	myCam.At.x = 0.0f;
			 	myCam.At.y = 10.0f;
			 	myCam.At.z = -10.0f;
			 }
			break;
	}
}

void initRendering() {

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)1,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	//glShadeModel(GL_SMOOTH);
	//GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	float posLight0[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float ambLight0[4] = {5.0f, 5.0f, 5.0f, 5.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight0);
	glEnable(GL_COLOR_MATERIAL);
	
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	initSkybox();
	initTileTextures();

	/*************************************************/
	
	initLevels();
	
	completeReset(currLevel);

	/***************************************************/
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	/**********************************************/	
	HUD_w=w;
	HUD_h=h;
	/*********************************************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void fixCamera(){
	if( ckey%6==1 ){
		myCam = Cam( Location( myChar.At.x, myChar.At.y+1.0, myChar.At.z-10.0), 
			Location(myChar.At.x + 0.1f * sin(myChar.viewangle * pi/180.0), myChar.At.y + 0.97, myChar.At.z -10.0f + 0.1f * cos(myChar.viewangle * pi/180.0)),
			Location(0, 1, 0));
	}
	else if( ckey%6==2 ){
		myCam = Cam( Location(0.0f, 10.0f, myChar.At.z-10.0f), 
			Location( 0.0f, 0.0f, myChar.At.z-10.0f),
			Location( 1.0f, 0.0f, 0.0f ));
	}
	else if( ckey%6==3 ){
		myCam = Cam( Location( myChar.At.x - 4.0f * sin(myChar.viewangle * pi/180.0), myChar.At.y + 4.0f, myChar.At.z -9.0f - 4.0f * cos(myChar.viewangle * pi/180.0)), 
			Location( myChar.At.x, myChar.At.y+0.95, myChar.At.z-10.0),
			Location(0, 1, 0));
	}
	else if( ckey%6==4 ){
		myCam = Cam( Location(myCam.At.x+1.0f, myCam.At.y, myCam.At.y+1.0f),
		 Location(myChar.At.x, myChar.At.y + 1.0f, myChar.At.z -10.f),
		 Location(0, 1, 0) );
	}
	else if( ckey%6==5 ){
		myCam = Cam( Location(myCam.At.x, myCam.At.y, myCam.At.z),
		 Location(myChar.At.x, myChar.At.y + 1.0f, myChar.At.z -10.f),
	     Location(-myCam.At.x+myChar.At.x, 0, -myChar.At.z-10.0f+myCam.At.z) );
	}
	else{
		myCam = Cam( Location(0, 0, 0),
		Location(0, 0, -11),
	    Location(0, 1, 0) );
	}
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fixCamera();

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	loadCamera();

	skyboxdraw(currLevel);

	/********************************************************/
	HUD(10,10,10);
	/*********************************************************/

	glLoadIdentity();
	loadCamera();

	glTranslatef( 0.0f, 0.0f, -10.0f);

	for( size_t i=0;i<myTiles.size(); ++i ){
		myTiles[i].draw();
	}
	/*******************************************************/
	DrawGateway();
	/******************************************************/

	glLoadIdentity();
	loadCamera();

	glTranslatef( 0.0f, 0.0f, -10.0f);
	// glScalef( 0.2f, 0.2f, 0.2f );

	glTranslatef(myChar.At.x, myChar.At.y, myChar.At.z);
	//cout<<myChar.At.x<<" "<<myChar.At.y<<" "<<myChar.At.z<<endl;
	glRotatef(myChar.viewangle,0.0f, 1.0f,0.0f);	
	//glScalef( 0.5f, 0.5f, 0.5f );

	myChar.draw();

	glLoadIdentity();
	loadCamera();

	glDisable(GL_TEXTURE_2D);

	
	if( upkey==1 )
		upkey=0;
	else if( downkey==1 )
		downkey=0;
	else if( rightkey==1 )
		rightkey=0;
	else if( leftkey==1 )
		leftkey=0;
		
	myTimer++;
	glutSwapBuffers();
}

//Called every 25 milliseconds
void update(int value) {
	
	glutPostRedisplay();
	for (size_t i = 0; i < myTiles.size(); ++i)
	{
		myTiles[i].update();
	}

	if(myChar.jump&&!myChar.trampoline_jump){
		myChar.At.x += 0.08f * sin(myChar.viewangle * pi/180.0);
		myChar.At.z += 0.08f * cos(myChar.viewangle * pi/180.0);
		if(myChar.stable)
			myChar.jump=false;
	}

	myChar.Vel.y += _g;
	myChar.update();
	//yextra -= 0.1f;
	glutTimerFunc(50, update, 0);
}

void handleKeypress2( int key, int x, int y ){
	if(key == GLUT_KEY_LEFT){
		leftkey=1;
		myChar.viewangle += 5.0f;
	}
	if(key == GLUT_KEY_RIGHT){
		rightkey=1;
		myChar.viewangle -= 5.0f;
	}
	if(key == GLUT_KEY_DOWN){
		downkey=1;
		myChar.At.x += -0.15f * sin(myChar.viewangle * pi/180.0);
		myChar.At.z += -0.15f * cos(myChar.viewangle * pi/180.0);
		
		// int k,j;
		// for( k=0;k<8;++k ){
		// 	for( j=0;j<7;++j ){
		// 		if( rextra >= myTiles[k][j].a && rextra <= myTiles[k][j].a+1.0f ){
		// 			if( (zextra-10.0f) >= myTiles[k][j].c && (zextra-10.0f) <= myTiles[k][j].c+1.0f )
		// 				break;
		// 		}
		// 	}
		// 	if( j<7 )
		// 		break;
		// }
		
		// if( myTiles[k][j].box ){
		// 	if( yextra < 1.0f){
		// 		rextra -= -0.1f * sin(angle * pi/180.0);
		// 		zextra -= -0.1f * cos(angle * pi/180.0);
		// 	}
		// }
	}
	if(key == GLUT_KEY_UP){
		upkey=1;
		myChar.At.x += 0.15f * sin(myChar.viewangle * pi/180.0);
		myChar.At.z += 0.15f * cos(myChar.viewangle * pi/180.0);
		// 		int k,j;
		// for( k=0;k<8;++k ){
		// 	for( j=0;j<7;++j ){
		// 		if( rextra >= myTiles[k][j].a && rextra <= myTiles[k][j].a+1.0f ){
		// 			if( (zextra-10.0f) >= myTiles[k][j].c && (zextra-10.0f) <= myTiles[k][j].c+1.0f )
		// 				break;
		// 		}
		// 	}
		// 	if( j<7 )
		// 		break;
		// }
		
		// if( myTiles[k][j].box ){
		// 	if( yextra < 1.0f){
		// 		rextra -= 0.1f * sin(angle * pi/180.0);
		// 		zextra -= 0.1f * cos(angle * pi/180.0);
		// 	}
		// }
	}

}

int main(int argc, char** argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("Vader on the Moon");
	initRendering();
	
	glutDisplayFunc(drawScene);
//	glutIdleFunc(&drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleKeypress2);
	glutReshapeFunc(handleResize);
	glutFullScreen();
	glutTimerFunc(50, update, 0);
	
	glutMainLoop();
	return 0;
}
