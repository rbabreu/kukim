#include "funcmanip.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math>

#define PI 3.14159265358979323846

float  xc=0.0, yc=0.0, zc=0.0, L=1.0; //, xg=0.0, yg=0.0, zg=0.0,

//---------------------------------------------------------------------------
float Resto(float num){
	while(num>360.0 || num<-360.0){
		if (num>360.0)
			num-=360.0;
		else
			num+=360.0;
	}
	return num;
}
//---------------------------------------------------------------------------
float Arredonda(float num, int ncasas){
	num = floor(num*pow(10,ncasas)+0.5);
	return num/pow(10,ncasas);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//******************CINEMATICA DO MANIPULADOR ARTICULADO*********************
int Cinversa(float x, float y, float z,
             float &a, float &b, float &c,
             float L1, float L2, float L3,
             bool j1checked, bool j2checked, bool j3checked,
             float j1max, float j1min,
             float j2max, float j2min,
             float j3max, float j3min){

	z=z-L1;

	L=sqrt((x*x)+(y*y)+(z*z));                     //distancia do ombro ate a garra


	if (L>(L2+L3)|| L<abs(L2-L3)){                 //limitações do espaço de trabalho
        return 1;
    }

	float cosc=(((L*L)-(L2*L2)-(L3*L3))/(2.0*L2*L3)); //cosseno do angulo c

	c=-atan2(sqrt(1.0-cosc*cosc),cosc);              //angulo de elevaçao do cotovelo

    float d=PI/2;                                  //assim funciona corretamente
    if (x!=0.0 || y!=0.0){
        d=atan(z/sqrt((x*x)+(y*y)));                //angulo de elevaçao da distancia (auxiliar)
    }

	b=d-atan2((L3*sin(c)),(L2+L3*cos(c)));          //angulo de elevaçao do ombro (braço)
    if (x!=0.0 || y!=0.0)
        a=atan2(y,x);                               //angulo de rotaçao da base
    else
        a=d;

	z=z+L1;
        
	a=Resto((a/PI)*180.0);
	b=Resto((b/PI)*180.0);
	c=Resto((c/PI)*180.0);

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (b>j2max||b<j2min)
            return 1;
    }
    if(j3checked){
        if (c>j3max||c<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int C_inversa(float x, float y, float z,
              float &a, float &b, float &c,
              float L1, float L2, float L3,
              bool j1checked, bool j2checked, bool j3checked,
              float j1max, float j1min,
              float j2max, float j2min,
              float j3max, float j3min){
	z=z-L1;

	L=sqrt((x*x)+(y*y)+(z*z));                      //distancia do ombro ate a garra


	if (L>(L2+L3)|| L<abs(L2-L3)){                  //Limitações do espaço de trabalho
        return 1;
    }

	float cosc=(((L*L)-(L2*L2)-(L3*L3))/(2.0*L2*L3)); //cosseno do angulo c

	c=atan2(sqrt(1.0-cosc*cosc),cosc);               //angulo de elevaçao do cotovelo

    float d=PI/2;                               //assim funciona corretamente
    if (x!=0.0 || y!=0.0){
        d=atan(z/sqrt((x*x)+(y*y)));         //angulo de elevaçao da distancia (auxiliar)
    }

	b=d-atan2((L3*sin(c)),(L2+L3*cos(c)));           //angulo de elevaçao do ombro (braço)
    if (x!=0.0 || y!=0.0)
        a=atan2(y,x);                        //angulo de rotaçao da base
    else
        a=d;

	z=z+L1;

	a=Resto((a/PI)*180.0);
	b=Resto((b/PI)*180.0);
	c=Resto((c/PI)*180.0);

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (b>j2max||b<j2min)
            return 1;
    }
    if(j3checked){
        if (c>j3max||c<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int Cdireta(float aa, float bb, float cc,
            float &xg, float &yg, float &zg,
            float L1, float L2, float L3,
            bool j1checked, bool j2checked, bool j3checked,
            float j1max, float j1min,
            float j2max, float j2min,
            float j3max, float j3min){
    aa=Resto(aa);
	bb=Resto(bb);
	cc=Resto(cc);

	cc=cc+bb;                               //muda para angulo pitch
	float a=(PI/180.0)*aa;
	float b=(PI/180.0)*bb;
	float c=(PI/180.0)*cc;
	xg=(cos(a))*(L2*(cos(b))+L3*(cos(c)));
	yg=(sin(a))*(L2*(cos(b))+L3*(cos(c)));
	zg=L1+L2*sin(b)+L3*sin(c);
	xc=xg-(cos(a))*(L3*cos(c));
	yc=yg-(sin(a))*(L3*cos(c));
	zc=zg-L3*sin(c);
    cc=cc-bb;

    if(j1checked){
        if (aa>j1max||aa<j1min)
            return 1;
    }
    if(j2checked){
        if (bb>j2max||bb<j2min)
            return 1;
    }
    if(j3checked){
        if (cc>j3max||cc<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------
//******************CINEMATICA DO MANIPULADOR POLAR**************************
int CinversaPolar(float x, float y, float z,
                  float &a, float &b, float &l,
                  float L1 , float L2, float L3,
                  bool j1checked, bool j2checked, bool j3checked,
                  float j1max, float j1min,
                  float j2max, float j2min,
                  float j3max, float j3min){

	z=z-L1;
	l=sqrt((x*x)+(y*y)+(z*z))-L3-L2;      //distancia do ombro ate a garra
    b=PI/2;                              //assim funciona certo
    if (x!=0.0 || y!=0.0){
        b=atan(z/sqrt((x*x)+(y*y)));      //angulo de elevaçao
    }
    if (x!=0.0 || y!=0.0)
        a=atan2(y,x);                     //angulo de rotaçao da base
    else
        a=b;

	a=Resto((a/PI)*180.0);
	b=Resto((b/PI)*180.0);

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (b>j2max||b<j2min)
            return 1;
    }
    if(j3checked){
        if (l>j3max||l<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------
int C_inversaPolar(float x, float y, float z,
                  float &a, float &b, float &l,
                  float L1 , float L2, float L3,
                  bool j1checked, bool j2checked, bool j3checked,
                  float j1max, float j1min,
                  float j2max, float j2min,
                  float j3max, float j3min){

	z=z-L1;
	l=sqrt((x*x)+(y*y)+(z*z))-L3-L2;              //distancia do ombro ate a garra
    b=PI/2;                                  //assim funciona certo
    if (x!=0.0 || y!=0.0){
        b=atan(z/sqrt((x*x)+(y*y)));      //angulo de elevaçao
    }
    if (x!=0.0 || y!=0.0)
        a=atan2(y,x);                     //angulo de rotaçao da base
    else
        a=b;

	a=Resto((a/PI)*180.0)+180.0;
	b=Resto((b/PI)*180.0);
    b=180.0-b;

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (b>j2max||b<j2min)
            return 1;
    }
    if(j3checked){
        if (l>j3max||l<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int CdiretaPolar(float aa, float bb, float ll,
                 float &xg, float &yg, float &zg,
                 float L1 , float L2, float L3,
                 bool j1checked, bool j2checked, bool j3checked,
                 float j1max, float j1min,
                 float j2max, float j2min,
                 float j3max, float j3min){
	aa=Resto(aa);
	bb=Resto(bb);

	float a=(PI/180.0)*aa;
	float b=(PI/180.0)*bb;

	xg=(ll+L2+L3)*cos(a)*cos(b);
	yg=(ll+L2+L3)*sin(a)*cos(b);
	zg=L1+(ll+L2+L3)*sin(b);

    if(j1checked){
        if (aa>j1max||aa<j1min)
            return 1;
    }
    if(j2checked){
        if (bb>j2max||bb<j2min)
            return 1;
    }
    if(j3checked){
        if (ll>j3max||ll<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------
//******************CINEMATICA DO MANIPULADOR CILINDRICO*********************
int CinversaCilindrico(float x, float y, float z,
                       float &a, float &l, float &zz,
                       float L1, float L2, float L3,
                       bool j1checked, bool j2checked, bool j3checked,
                       float j1max, float j1min,
                       float j2max, float j2min,
                       float j3max, float j3min){

	zz=z;

	l=sqrt((x*x)+(y*y));                //distancia do ombro ate a garra

    a=atan2(y,x);                    //angulo de rotaçao da base
	a=Resto((a/PI)*180.0);

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (z>j2max||z<j2min)
            return 1;
    }
    if(j3checked){
        if (l>j3max||l<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int CdiretaCilindrico(float aa, float ll, float zz,
                      float &xg, float &yg, float &zg,
                      float L1, float L2, float L3,
                      bool j1checked, bool j2checked, bool j3checked,
                      float j1max, float j1min,
                      float j2max, float j2min,
                      float j3max, float j3min){

	aa=Resto(aa);

    float a=(PI/180.0)*aa;

	xg=ll*cos(a);
	yg=ll*sin(a);
	zg=zz;

    if(j1checked){
        if (aa>j1max||aa<j1min)
            return 1;
    }
    if(j2checked){
        if (zz>j2max||zz<j2min)
            return 1;
    }
    if(j3checked){
        if (ll>j3max||ll<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------
//******************CINEMATICA DO MANIPULADOR SCARA**************************
int CinversaSCARA(float x, float y, float z,
                  float &a, float &b, float &zz,
                  float L0, float L1, float L2,
                  bool j1checked, bool j2checked, bool j3checked,
                  float j1max, float j1min,
                  float j2max, float j2min,
                  float j3max, float j3min){

	zz=z;

	L=sqrt((x*x)+(y*y));                      //+(z*z)distancia do ombro ate a garra

    if (L>(L1+L2)|| L<abs(L1-L2)){                  //Limitações do espaço de trabalho
        return 1;
    }
	b = (((L*L)-(L1*L1)-(L2*L2))/(2.0*L1*L2));  //angulo de rotação do cotovelo
    b = atan2(sqrt(1-b*b),b);
    a = atan2((y*(L1+L2*cos(b))-x*L2*sin(b)),(x*(L1+L2*cos(b))+y*L2*sin(b)));                           //angulo de rotaçao do braço

	a=Resto((a/PI)*180.0);
	b=Resto((b/PI)*180.0);

    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (z>j2max||z<j2min)
            return 1;
    }
    if(j3checked){
        if (b>j3max||b<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------
int C_inversaSCARA(float x, float y, float z,
                  float &a, float &b, float &zz,
                  float L0, float L1, float L2,
                  bool j1checked, bool j2checked, bool j3checked,
                  float j1max, float j1min,
                  float j2max, float j2min,
                  float j3max, float j3min){

	zz=z;

	L=sqrt((x*x)+(y*y));                      //+(z*z)distancia do ombro ate a garra

    if (L>(L1+L2)|| L<abs(L1-L2)){                  //Limitações do espaço de trabalho
        return 1;
    }
	b = (((L*L)-(L1*L1)-(L2*L2))/(2.0*L1*L2));  //angulo de rotação do cotovelo
    b = -atan2(sqrt(1-b*b),b);
    a = atan2((y*(L1+L2*cos(b))-x*L2*sin(b)),(x*(L1+L2*cos(b))+y*L2*sin(b)));                           //angulo de rotaçao do braço


	a=Resto((a/PI)*180.0);
	b=Resto((b/PI)*180.0);


    if(j1checked){
        if (a>j1max||a<j1min)
            return 1;
    }
    if(j2checked){
        if (z>j2max||z<j2min)
            return 1;
    }
    if(j3checked){
        if (b>j3max||b<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int CdiretaSCARA(float aa, float zz, float bb,
                 float &xg, float &yg, float &zg,
                 float L0, float L1, float L2,
                 bool j1checked, bool j2checked, bool j3checked,
                 float j1max, float j1min,
                 float j2max, float j2min,
                 float j3max, float j3min){
	aa=Resto(aa);
	bb=Resto(bb);

    if(j3checked){
        if (bb>j3max||bb<j3min)
            return 1;
    }

    bb=aa+bb;                       //muda para angulo pitch
    float a=(PI/180.0)*aa;
    float b=(PI/180.0)*bb;

	xg=L1*(cos(a))+L2*(cos(b));
	yg=L1*(sin(a))+L2*(sin(b));
	zg=zz;

    if(j1checked){
        if (aa>j1max||aa<j1min)
            return 1;
    }
    if(j2checked){
        if (zz>j2max||zz<j2min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

int CCartesiano(float x, float y, float z,
                 bool j1checked, bool j2checked, bool j3checked,
                 float j1max, float j1min,
                 float j2max, float j2min,
                 float j3max, float j3min){

    if(j1checked){
        if (x>j1max||x<j1min)
            return 1;
    }
    if(j2checked){
        if (y>j2max||y<j2min)
            return 1;
    }
    if(j3checked){
        if (z>j3max||z<j3min)
            return 1;
    }

	return 0;
}
//---------------------------------------------------------------------------

//******************Desenha eixos de tamanho d na posicao x, y, z************
void Eixos(bool visivel,float d, float x, float y, float z){
	glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
	glLineWidth(2.0);
    if(visivel==true){
        glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(x,y,z);
        glVertex3f(x+d,y,z);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(x,y,z);
        glVertex3f(x,y+d,z);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(x,y,z);
        glVertex3f(x,y,z+d);
        glEnd();
    }
	glLineWidth(1.0);                       //volta a espessura corrente
	glColor4ub(255.0, 255.0, 100.0, 150.0); //volta a cor corrente
	glPopMatrix();
}
//---------------------------------------------------------------------------

void Incremento(float a, float b, float c, float &baseinc, float &ombroinc, float &cotoveloinc){
    float maior=0;
    if (abs(Resto(a))>=abs(Resto(b)) && abs(Resto(a))>=abs(Resto(c))){
            maior=abs(Resto(a));
    }
    else{
        if(abs(Resto(b))>=abs(Resto(a)) && abs(Resto(b))>=abs(Resto(c))){
                maior=abs(Resto(b));
        }
        else{
                maior=abs(Resto(c));
        }
    }
    if(maior==0){
        baseinc=0;
        ombroinc=0;
        cotoveloinc=0;
    }else{
        if(a>=0)
                baseinc=abs(a)/maior;
        else{
                baseinc=-abs(a)/maior;
        }
        if(b>=0)
                ombroinc=abs(b)/maior;
        else{
                ombroinc=-abs(b)/maior;
        }
        if(c>=0)
                cotoveloinc=abs(c)/maior;
        else{
                cotoveloinc=-abs(c)/maior;
        }
    }
}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

/*void Texto(char *caracter, float x, float y){
        glColor3ub(255.0, 255.0, 100.0);
        glRasterPos2f(x, y);
        while(*caracter)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*caracter++);
} */
//---------------------------------------------------------------------------

float Distancia(float &x1, float &y1, float &z1,
                float &x2, float &y2, float &z2){
        return sqrt(pow((x1-x2),2.0)+pow((y1-y2),2.0)+pow((z1-z2),2.0));
}
//---------------------------------------------------------------------------
//******************Desenha Cubo*********************************************
void CuboSolido(float a, float r, float g, float b){
    a=a/2;
    glBegin(GL_QUADS);
      glColor4ub( r, g, b, 150.0);
      glVertex3f(-a, a, -a);                    //face de cima
      glVertex3f(a, a, -a);
      glVertex3f(a, a, a);
      glVertex3f(-a, a, a);

      glVertex3f(-a, -a, -a);                   //face de baixo
      glVertex3f(a, -a, -a);
      glVertex3f(a, -a, a);
      glVertex3f(-a, -a, a);

      glColor4ub(r-50.0, g-50.0, b, 150.0);
      glVertex3f(-a, a, a);                     //face do fundo
      glVertex3f(a, a, a);
      glVertex3f(a, -a, a);
      glVertex3f(-a, -a, a);

      glVertex3f(-a, a, -a);                    //face da frente
      glVertex3f(a, a, -a);
      glVertex3f(a, -a, -a);
      glVertex3f(-a, -a, -a);

      glColor4ub(r-20.0, g-20.0, b, 150.0);
      glVertex3f(a, a, -a);                     //face da esquerda
      glVertex3f(a, a, a);
      glVertex3f(a, -a, a);
      glVertex3f(a, -a, -a);

      glVertex3f(-a, a, -a);                    //face da direita
      glVertex3f(-a, a, a);
      glVertex3f(-a, -a, a);
      glVertex3f(-a, -a, -a);
    glEnd();
}
//---------------------------------------------------------------------------

void CuboSolidoTri(){
	glBegin(GL_TRIANGLES);  		// Start Drawing A Triangle
		glColor3f(0.5f,0.0f,0.0f);	// Vermelho
		glVertex3f( 0.5f, 0.5f, 0.5f);	// Top Of Triangle (Front)
		glVertex3f(-0.5f,-0.5f, 0.5f);	// Left Of Triangle (Front)
		glVertex3f( 0.5f,-0.5f, 0.5f);	// Right Of Triangle (Front)

		glVertex3f( 0.5f, 0.5f, -0.5f);	// Top Of Triangle (Right)
		glVertex3f( 0.5f,-0.5f, 0.5f);	// Left Of Triangle (Right)
		glVertex3f( 0.5f,-0.5f, -0.5f);	// Right Of Triangle (Right)

		glVertex3f( -0.5f, 0.5f, -0.5f);// Top Of Triangle (Back)
		glVertex3f( 0.5f,-0.5f, -0.5f);	// Left Of Triangle (Back)
		glVertex3f(-0.5f,-0.5f, -0.5f);	// Right Of Triangle (Back)

		glVertex3f( -0.5f, 0.5f, 0.5f);	// Top Of Triangle (Left)
		glVertex3f(-0.5f,-0.5f,-0.5f);	// Left Of Triangle (Left)
		glVertex3f(-0.5f,-0.5f, 0.5f);	// Right Of Triangle (Left)

                glColor3f(0.0f,1.0f,0.0f);	// Verde
		glVertex3f( 0.5f, 0.5f, 0.5f);	// Top Of Triangle (Front)
		glVertex3f(-0.5f,-0.5f, 0.5f);	// Left Of Triangle (Front)
		glVertex3f( -0.5f,0.5f, 0.5f);

                glVertex3f( 0.5f, 0.5f, -0.5f);	// Top Of Triangle (Right)
		glVertex3f( 0.5f,-0.5f, 0.5f);	// Left Of Triangle (Right)
		glVertex3f( 0.5f,0.5f, 0.5f);

                glVertex3f( -0.5f, 0.5f, -0.5f);// Top Of Triangle (Back)
		glVertex3f( 0.5f,-0.5f, -0.5f);	// Left Of Triangle (Back)
		glVertex3f(0.5f,0.5f, -0.5f);

                glVertex3f( -0.5f, 0.5f, 0.5f);	// Top Of Triangle (Left)
		glVertex3f(-0.5f,-0.5f,-0.5f);	// Left Of Triangle (Left)
		glVertex3f(-0.5f,0.5f, -0.5f);

                glColor3f(0.0f,0.0f,1.0f);	// Azul
	      	glVertex3f( 0.5f, 0.5f, 0.5f);	// Top Of Triangle (Front)
		glVertex3f(-0.5f,0.5f, -0.5f);	// Left Of Triangle (Front)
		glVertex3f( -0.5f,0.5f, 0.5f);

                glColor3f(1.0f,0.0f,1.0f);	// Rosa
                glVertex3f( 0.5f, 0.5f, -0.5f);	// Top Of Triangle (Right)
		glVertex3f( -0.5f,0.5f, -0.5f);	// Left Of Triangle (Right)
		glVertex3f( 0.5f,0.5f, 0.5f);

                glColor3f(1.0f,1.0f,0.0f);	// Amarelo
		glVertex3f( 0.5f, -0.5f, -0.5f);// Top Of Triangle (Front)
		glVertex3f(-0.5f,-0.5f, 0.5f);	// Left Of Triangle (Front)
		glVertex3f( 0.5f,-0.5f, 0.5f);

                glColor3f(0.0f,1.0f,1.0f);	// Cyan
		glVertex3f( -0.5f, -0.5f, 0.5f);// Top Of Triangle (Back)
		glVertex3f( 0.5f,-0.5f, -0.5f);	// Left Of Triangle (Back)
		glVertex3f(-0.5f,-0.5f, -0.5f);
	glEnd();
}
//---------------------------------------------------------------------------
//******************Desenha Label XYZ****************************************
void LabelXYZ(bool eixovisivel){

    if(eixovisivel==true){
        glPushMatrix();
        glScalef (0.15, 0.15, 0.15);
        glBegin(GL_LINES);
        glLineWidth(1.0);
        glColor3ub(255.0, 255.0, 0.0);
        glVertex3f(8.5, 0.5, 1.0);
        glVertex3f(7.5, 0.5, 0.0);
        glVertex3f(7.5, 0.5, 1.0);
        glVertex3f(8.5, 0.5, 0.0);

        glVertex3f(0.0,0.5,-8.0);
        glVertex3f(0.5,0.5,-7.5);
        glVertex3f(1.0,0.5,-8.0);
        glVertex3f(0.5,0.5,-7.5);
        glVertex3f(0.5,0.5,-7.5);
        glVertex3f(0.5,0.5,-7.0);

        glVertex3f(-0.5,14.0,0.0);
        glVertex3f(0.5,14.0,0.0);
        glVertex3f(0.5,15.0,0.0);
        glVertex3f(-0.5,14.0,0.0);
        glVertex3f(-0.5,15.0,0.0);
        glVertex3f(0.5,15.0,0.0);
        glEnd();
        glPopMatrix();
    }
}
//---------------------------------------------------------------------------
//******************DESENHA GRADE********************************************
void DesenhaGrade(bool gradex, bool gradey, bool gradez, int nn){
    int n;

    glLineWidth(1);
    if(gradex==true){
        glPushMatrix();
        glTranslatef (0.0, 0.02, 0.0);
        glBegin(GL_LINES);
        glColor3ub(255, 0, 0);
        for(n=0; n<=nn; ++n){
            glVertex3f(-nn, 0.0, -n);//paralelas a x
            glVertex3f(nn, 0.0, -n);
            glVertex3f(nn, 0.0, n);
            glVertex3f(-nn, 0.0, n);
            glVertex3f(-n, 0.0, -nn);//paralelas a y
            glVertex3f(-n, 0.0, nn);
            glVertex3f(n, 0.0, nn);
            glVertex3f(n, 0.0, -nn);
        }
        glEnd();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef (0.0, nn, 0.0);
    if(gradey==true){
        glBegin(GL_LINES);
        glColor3ub(255.0, 0.0, 0.0);
        for(n=0; n<=nn; ++n){
            glVertex3f(nn, n, 0);   //paralelas a x
            glVertex3f(-nn, n, 0);
            glVertex3f(nn, -n, 0);
            glVertex3f(-nn, -n, 0);
            glVertex3f(-n, -nn, 0); //paralelas a z
            glVertex3f(-n, nn, 0);
            glVertex3f(n, -nn, 0);
            glVertex3f(n, nn, 0);
        }
        glEnd();
    }
    if(gradez==true){
        glBegin(GL_LINES);
        glColor3ub(255.0, 0.0, 0.0);
        for(n=0; n<=nn; ++n){
            glVertex3f(0, nn, -n);//paralelas a x
            glVertex3f(0, -nn, -n);
            glVertex3f(0, nn, n);
            glVertex3f(0, -nn, n);
            glVertex3f(0, -n, -nn);//paralelas a y
            glVertex3f(0, -n, nn);
            glVertex3f(0, n, nn);
            glVertex3f(0, n, -nn);
        }
        glEnd();
    }
    glPopMatrix();
    n=nn;
}

//---------------------------------------------------------------------------

