//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GlSkel.h"
#include "Unit3.h"
#include "funcmanip.h"
#include <math.h>
#include<mmsystem.h>                                           
#include "coldet.h"
#include <vcl\registry.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tbase1 *base1;

ThreadClass *MyThread; //thread que controla comunicação serial

unsigned short int tipo=1;
bool eixovisivel=true;
GLUquadricObj *quadratic;

/////////////////VARIAVEIS DE CINEMATICA/////////////////

float editx, edity, editz;
float maior;
GLfloat L1=1.0, L2=1.4, L3=1.6;

////////////////TABELA DE PROGRAMAÇÃO///////////////////
int i=0, j=0, k=0;
int n=5, nn=5;
unsigned int getmatrix=0;

////////////////// VARIÁVEL DO SOM /////////////////////
String v_som = NULL;

//////////////// VARIAVEIS DE CAMERA ///////////////////
GLfloat rothoricam=0.0;
GLfloat rotvertcam=20.0;
GLfloat zoomcam=12.0;

GLfloat xcam=0.0;
GLfloat ycam=0.0;
GLfloat zcam=0.0;

////////////////VARIAVEIS DE COLISAO//////////////////
CollisionModel3D* model = newCollisionModel3D();
CollisionModel3D* model1 = newCollisionModel3D();
CollisionModel3D* model2 = newCollisionModel3D();
CollisionModel3D* model3 = newCollisionModel3D();
CollisionModel3D* model4 = newCollisionModel3D();
CollisionModel3D* model5 = newCollisionModel3D();
CollisionModel3D* model6 = newCollisionModel3D();
float mat[16];
float mat1[16];
float mat2[16];
float mat3[16];
float mat4[16];
float mat5[16];
float mat6[16];
BOOL collision;

float viewmat[16];

GLuint listaobj;
//---------------------------------------------------------------------------
void __fastcall Tbase1::CompilaCuboSolido(){
    //listaobj = glGenLists(2);
    glNewList(listaobj, GL_COMPILE);
    glBegin(GL_QUADS);
      glColor4ub( 255.0, 255.0, 120.0, 150.0);
      glVertex3f(-0.5, 0.5, -0.5);                    //face de cima
      glVertex3f(0.5, 0.5, -0.5);
      glVertex3f(0.5, 0.5, 0.5);
      glVertex3f(-0.5, 0.5, 0.5);

      glVertex3f(-0.5, -0.5, -0.5);                   //face de baixo
      glVertex3f(0.5, -0.5, -0.5);
      glVertex3f(0.5, -0.5, 0.5);
      glVertex3f(-0.5, -0.5, 0.5);

      glColor4ub(205.0, 205.0, 120, 150.0);
      glVertex3f(-0.5, 0.5, 0.5);                     //face do fundo
      glVertex3f(0.5, 0.5, 0.5);
      glVertex3f(0.5, -0.5, 0.5);
      glVertex3f(-0.5, -0.5, 0.5);

      glVertex3f(-0.5, 0.5, -0.5);                    //face da frente
      glVertex3f(0.5, 0.5, -0.5);
      glVertex3f(0.5, -0.5, -0.5);
      glVertex3f(-0.5, -0.5, -0.5);

      glColor4ub(235.0, 235.0, 120, 150.0);
      glVertex3f(0.5, 0.5, -0.5);                     //face da esquerda
      glVertex3f(0.5, 0.5, 0.5);
      glVertex3f(0.5, -0.5, 0.5);
      glVertex3f(0.5, -0.5, -0.5);

      glVertex3f(-0.5, 0.5, -0.5);                    //face da direita
      glVertex3f(-0.5, 0.5, 0.5);
      glVertex3f(-0.5, -0.5, 0.5);
      glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
    glEndList();
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::CompilaGrade(bool gradex, bool gradey, bool gradez, int nn){
    //listaobj = glGenLists(2);
    glNewList(listaobj+1, GL_COMPILE);
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
    glEndList();
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Joystick()
{
   JOYINFO joystickInfo;
   if (joyGetPos(JOYSTICKID1,&joystickInfo) == JOYERR_NOERROR)
   {
      switch(joystickInfo.wButtons)
      {
        case(JOY_BUTTON1):
                pitch=pitch+3;
                break;
        case(JOY_BUTTON2):
                if(garra<100)
                    garra=(float)garra+5;
                break;
        case(JOY_BUTTON3):
                if(garra>20)
                    garra=(float)garra-5;
                break;
        case(JOY_BUTTON4):
                pitch=pitch-3;
                break;
        case(JOY_BUTTON5):
                ++rothoricam;
                break;
        case(JOY_BUTTON6):
                --rothoricam;
                break;
        case(JOY_BUTTON7):
                ++rotvertcam;
                break;
        case(JOY_BUTTON8):
                --rotvertcam;
                break;
        case(JOY_BUTTON9):
                base=0;
                ombro=0;
                cotovelo=0;
                break;
        case(JOY_BUTTON10):
                Timer1->Enabled=false;
                ++StringGrid2->RowCount;
                ++StringGrid2->RowCount;
                StringGrid2->Cells[0][j]="DIR";
                StringGrid2->Cells[1][j]=FloatToStr(base);
                StringGrid2->Cells[2][j]=FloatToStr(ombro);
                StringGrid2->Cells[3][j]=FloatToStr(cotovelo);
                ++j;
                StringGrid2->Cells[0][j]="GRIP";
                StringGrid2->Cells[1][j]=FloatToStr(Arredonda((garra-15)*1.1764706, 2));
                ++j;
                Label25->Caption=IntToStr(StringGrid2->RowCount);
                if(StringGrid2->RowCount>3000){
                    StatusBar1->Panels->Items[1]->Text="Número máximo de linhas (3000).";
                    j=0;
                }
                Sleep(1000);
                break;
        case(JOY_BUTTON11):
                ++zoomcam;
                break;
        case(JOY_BUTTON12):
                if(zoomcam>2)
                    --zoomcam;
                break;
        case(JOY_BUTTON13):
                roll=roll-3;
                break;
        case(JOY_BUTTON14):
                yaw=yaw-3;
                break;
        case(JOY_BUTTON15):
                roll=roll+3;
                break;
        case(JOY_BUTTON16):
                yaw=yaw+3;
                break;
        default:
                break;
      }
      base=Resto(base+3*((joystickInfo.wXpos)/32768.0-1));
      ombro=Resto(ombro+3*((joystickInfo.wYpos)/32768.0-1));
      cotovelo=Resto(cotovelo+3*((joystickInfo.wZpos)/32768.0-1));

      if(yaw<-45)
              yaw=yaw+3;
      else{
              if(yaw>45)
                      yaw=yaw-3;
      }
      if(cotovelo<-120)
              cotovelo=cotovelo+3;
      else{
              if(cotovelo>120)
                      cotovelo=cotovelo-3;
      }
      if(ombro<-20)
              ombro=ombro+3;
      else{
              if(ombro>200)
                      ombro=ombro-3;
      }
      editbase = base;
      editombro = ombro;
      editcotovelo = cotovelo;
   }
}
//---------------------------------------------------------------------------
GLdouble posX, posY, posZ;
void __fastcall Tbase1::GetOGLPos(int x, int y, double &posX, double &posY, double &posZ)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	winX = (float)x;
	winY = (float)viewport[3] + 97.0 - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
}
//---------------------------------------------------------------------------

__fastcall Tbase1::Tbase1(TComponent* Owner)
    : TForm(Owner)
{
    Application->OnIdle = IdleLoop;
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::IdleLoop(TObject*, bool& done)
{
    done = false;
    RenderGLScene();
    SwapBuffers(hdc);    // USADO PARA DOUBLE BUFFERING
    StatusBar1->Panels->Items[2]->Text= "(junta1, junta2, junta3) : "+
                                    FloatToStr(Arredonda(base,2))+" ; "+
                                    FloatToStr(Arredonda(ombro,2))+" ; "+
                                    FloatToStr(Arredonda(cotovelo,2));
    StatusBar1->Panels->Items[3]->Text= "(x,y,z) : "+
                                        FloatToStr(Arredonda(x,2))+" ; "+
                                        FloatToStr(Arredonda(y,2))+" ; "+
                                        FloatToStr(Arredonda(z,2));
}
//----------------------------------------------------------------------------
void __fastcall Tbase1::FormCreate(TObject *Sender)
{    
    hdc = GetDC(Handle);
    SetPixelFormatDescriptor();
    hrc = wglCreateContext(hdc);
    if(hrc == NULL)
        ShowMessage("Erro na leitura do hrc.");
    if(wglMakeCurrent(hdc, hrc) == false)
        ShowMessage("Erro em MakeCurrent.");
    w = ClientWidth;
    h = ClientHeight;

    DetectaPortas(Sender);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    SetupLighting();

    model->addTriangle(-4.0f,0.0f,-4.0f,4.0f,0.0f,-4.0f,4.0f,0.0f,4.0f);
    model->addTriangle(-4.0f,0.0f,-4.0f,-4.0f,0.0f,4.0f,4.0f,0.0f,4.0f);
    model->finalize ();

    model1->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model1->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model1->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model1->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model1->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model1->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model1->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model1->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model1->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model1->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model1->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model1->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model1->finalize ();

    model2->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model2->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model2->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model2->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model2->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model2->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model2->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model2->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model2->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model2->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model2->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model2->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model2->finalize ();

    model3->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model3->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model3->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model3->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model3->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model3->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model3->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model3->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model3->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model3->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model3->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model3->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model3->finalize ();

    model4->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model4->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model4->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model4->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model4->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model4->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model4->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model4->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model4->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model4->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model4->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model4->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model4->finalize ();

    model5->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model5->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model5->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model5->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model5->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model5->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model5->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model5->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model5->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model5->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model5->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model5->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model5->finalize ();

    model6->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model6->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f);
    model6->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f);
    model6->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f);

    model6->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,-0.5f,0.5f, 0.5f);
    model6->addTriangle(0.5f, 0.5f, -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,0.5f,0.5f);
    model6->addTriangle(-0.5f, 0.5f, -0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f);
    model6->addTriangle(-0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,0.5f, -0.5f);

    model6->addTriangle(0.5f, 0.5f, 0.5f,-0.5f,0.5f, -0.5f, -0.5f,0.5f,0.5f);
    model6->addTriangle(0.5f, 0.5f, -0.5f,-0.5f,0.5f, -0.5f, 0.5f,0.5f, 0.5f);
    model6->addTriangle(0.5f, -0.5f, -0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f);
    model6->addTriangle(-0.5f, -0.5f, 0.5f,0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f);
    model6->finalize ();

    ombro = 0; cotovelo = 0; base = 0;
    roll=0; pitch=0; yaw=0; garra=100;
    x = 300; y = 0; z = 100;

    Tbase1::N3DOFSimples1Click(Sender);

    listaobj = glGenLists(2);
    CompilaCuboSolido();
    CompilaGrade(true, false, false, 5);
}

//---------------------------------------------------------------------------
void __fastcall Tbase1::SetPixelFormatDescriptor()
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        16,   //Bit color; Mudar para 16 ou 32 dependendo da placa de vídeo
        0,0,0,0,0,0,
        0,0, 
        0,0,0,0,0,
        16,   //tamanho do depth buffer; Usar 32 em placas modernas
        0, 
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0 
    };
    PixelFormat = ChoosePixelFormat(hdc, &pfd); //retorna o resultado do pedido
                                        //sobre a avaliabilidade do pixel format
    SetPixelFormat(hdc, PixelFormat, &pfd);//determina que hdc use &pdf
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::FormResize(TObject *Sender)
{
    w = ClientWidth;
    h = ClientHeight;
    if(h == 0)
        h = 1;
    glViewport(120.0, -30.0, w, h);
    GLfloat aspect = (GLfloat)w/(GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(Perspectiva1->Checked==true)
        gluPerspective(46.0f, aspect, 1.0, 100.0);
    else
        glOrtho(-0.01*w,0.01*w,-0.01*h,0.01*h,0.5,50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::RenderGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(collision){
        base=baseant;
        ombro=ombroant;
        cotovelo=cotoveloant;
    }

    if(N3DOFSimples1->Checked==true)
        Tbase1::Articulado1();
    else if(N6DOFPulsoEsfrico1->Checked==true)
        Tbase1::Articulado2();
    else if(Polar1->Checked==true)
        Tbase1::Polar();
    else if(Cilindrico1->Checked==true)
        Tbase1::Cilindrico();
    else if(Cartesiano1->Checked==true)
        Tbase1::Cartesiano();
    else if(Cartesiano2->Checked==true)
        Tbase1::Cartesiano_2();
    else if(SCARA1->Checked==true)
        Tbase1::SCARA();
        
    if(!collision){
        baseant=base;
        ombroant=ombro;
        cotoveloant=cotovelo;
    }
    glFlush();
}

//---------------------------------------------------------------------------
float rasterpos[4];

void __fastcall Tbase1::Articulado1()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1.0,0.0,0.0);
        glRotatef(rothoricam,0.0,1.0,0.0);
        glPushMatrix();
                glPushMatrix();
                        glTranslatef (1.0, 0.5, -2.0);
                        glRotatef(-25.0,0.0,1.0,0.0);
                        glGetFloatv(GL_MODELVIEW_MATRIX, viewmat);
                glPopMatrix();

                glPushMatrix();
                        // origem posicionada no ombro no centro
                        glPushMatrix();
                                glRotatef(-90,1,0,0);
                                glColor4ub(150.0, 150.0, 100.0,200.0);
                                gluCylinder(quadratic,0.5f,0.5f,0.3f,32,32);
                                glTranslatef (0.0, 0.0, 0.3);
                                gluDisk(quadratic,0.0f,0.5f,32,32);
                        glPopMatrix();

                        Eixos(eixovisivel,1,0,0,0.02);
                        LabelXYZ(eixovisivel);//DESENHA XYZ
                        //////////////DESENHA CHÃO//////////////
                        //n=5;
                        glBegin(GL_QUADS);
                        glColor3ub(180.0, 180.0, 180.0);
                        glVertex3f(-n, 0.0, -n);
                        glVertex3f(n, 0.0, -n);
                        glVertex3f(n, 0.0, n);
                        glVertex3f(-n, 0.0, n);
                        glEnd();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                        model->setTransform (mat);
                        ///////////////////DESENHA GRADE/////////////////////
                        glCallList(listaobj+1);
                glPopMatrix();

                // origem voltou para o ombro no centro
                glTranslatef (0.0, L1, 0.0);
                glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
                glPushMatrix();
                        glPushMatrix();
                                glTranslatef (0.0, 0.0, -0.35);
                                glColor4ub(160.0, 160.0, 120.0,200.0);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                                gluCylinder(quadratic,0.35f,0.35f,0.7f,32,32);
                                glTranslatef (0.0, 0.0, 0.7);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                        glPopMatrix();

                        Eixos(eixovisivel,1,0.0,0.0,0.0);
                        glTranslatef (0.0, -L1/2, 0.0);
                        glScalef (0.6, L1, 0.12);
                        glPushMatrix();
                                glScalef (1.0, 1.0, 5.4);
                                //CuboSolidoTri();
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat4);
	                        model4->setTransform (mat4);
                        glPopMatrix();

                        glTranslatef (0.0, 0.0, 2.2);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                        glTranslatef (0.0, 0.0, -4.4);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                // origem posicionada no centro do braço
                glRotatef ((GLfloat) ombro, 0.0, 0.0, 1.0);
                glPushMatrix();
                        glRotatef(90.0,1.0,0.0,0.0);
                        Eixos(eixovisivel,1,0,0,0);
                glPopMatrix();

                glTranslatef (L2/2, 0.0, 0.0);
                glPushMatrix();
                        glPushMatrix();
                                glScalef (L2, 0.4, 0.4);
                                //CuboSolidoTri();
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
	                        model3->setTransform (mat3);
                        glPopMatrix();

                        glScalef (L2, 0.4, 0.05);
                        glTranslatef (0.0, 0.0, 3.5);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                        glTranslatef (0.0, 0.0, -7.0);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                // origem posicionada no cotovelo
                glTranslatef (L2/2, 0.0, 0.0);
                glRotatef ((GLfloat) cotovelo, 0.0, 0.0, 1.0);
                glPushMatrix();
                        glRotatef(90.0,1.0,0.0,0.0);
                        Eixos(eixovisivel,1,0,0,0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.0, 0.0, -0.22);
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                        gluCylinder(quadratic,0.22f,0.22f,0.44f,32,32);
                        glTranslatef (0.0, 0.0, 0.44);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                glPopMatrix();

                glTranslatef (L3/2-0.2, 0.0, 0.0);
                glPushMatrix();
                        glScalef (L3-0.2, 0.4, 0.4);
                        //CuboSolidoTri();
	                glGetFloatv(GL_MODELVIEW_MATRIX, mat2);
                        model2->setTransform (mat2);
                glPopMatrix();

                glPushMatrix();
                        glScalef (L3-0.4, 0.4, 0.05);
                        glTranslatef (0.0, 0.0, 2.5);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                        glTranslatef (0.0, 0.0, -5.0);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();
    
                //origem na ponta do braço (garra)
                glTranslatef (L3/2-0.2, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(-90,0,-1,0);
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                        gluCylinder(quadratic,0.2f,0.2f,0.1f,32,32);
                        glTranslatef (0.0, 0.0, 0.1);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                        glTranslatef (0.0, 0.0, -0.13);
                        glScalef (0.2, 0.4, 0.06);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
	                model5->setTransform (mat5);
                        //////////////Desenha garra  ///////////////
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
	                model6->setTransform (mat6);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glTranslatef (0.4, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(180.0,0.0,1.0,0.0);
                        glRotatef(90.0,0.0,0.0,1.0);
                        Eixos(eixovisivel,1,0,0,0);
                glPopMatrix();

        glPopMatrix();

        /*glPushMatrix();//DESENHA CUBO NA POSICAO DITA PELA MATRIZ viewmat
                glLoadMatrixf (viewmat);
                CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat1);
	        model1->setTransform (mat1);
        glPopMatrix();*/

    glPopMatrix();
    // origem volta para o sistema de coordenadas original

    collision=(model->collision(model6)|model->collision(model5)|
               model->collision(model2)|model->collision(model3));
               //|model1->collision(model6)|model1->collision(model5)|
               //model1->collision(model2)|model1->collision(model3));

}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Articulado2()
{
    glLoadIdentity();
    gluLookAt( 0, 0,zoomcam, 0,0,0, 0,1,0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1,0,0);
        glRotatef(rothoricam,0,1,0);
        glPushMatrix();
        // origem posicionada no ombro no centro
                glPushMatrix();
                        glTranslatef (1.0, 0.5, -2.0);
                        glRotatef(-25,0,1,0);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat1);
                        model1->setTransform (mat1);
                glPopMatrix();

                glPushMatrix();
                        glRotatef(-90,1,0,0);
                        glColor4ub(160.0, 160.0, 100.0,200.0);
                        gluCylinder(quadratic,0.5f,0.5f,0.3f,32,32);
                        glTranslatef (0.0, 0.0, 0.3);
                        gluDisk(quadratic,0.0f,0.5f,32,32);
                glPopMatrix();

                Eixos(eixovisivel,1,0,0,0.02);
                LabelXYZ(eixovisivel);
                //////////////DESENHA CHÃO//////////////
                //n=5;
                glBegin(GL_QUADS);
                glColor3ub(180.0, 180.0, 180.0);
                glVertex3f(-n, 0.0, -n);
                glVertex3f(n, 0.0, -n);
                glVertex3f(n, 0.0, n);
                glVertex3f(-n, 0.0, n);
                glEnd();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                model->setTransform (mat);
                ///////////////////DESENHA GRADE/////////////////////
                glCallList(listaobj+1);
        glPopMatrix();

        // origem voltou para o ombro no centro
        glTranslatef (0.0, L1, 0.0);
        glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
        glColor4ub(150.0, 150.0, 100.0,200.0);
        glPushMatrix();
                glPushMatrix();
                        glTranslatef (0.0, 0.0, -0.35);
                        gluDisk(quadratic,0.0f,0.35f,32,32);
                        gluCylinder(quadratic,0.35f,0.35f,0.7f,32,32);
                        glTranslatef (0.0, 0.0, 0.7);
                        gluDisk(quadratic,0.0f,0.35f,32,32);
                glPopMatrix();

                Eixos(eixovisivel,1,0,0,0.0);
                glTranslatef (0.0, -L1/2, 0.0);
                glScalef (0.6, L1, 0.6);
                //CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat4);
                model4->setTransform (mat4);
                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
        glPopMatrix();

        // origem posicionada no centro do braço
        glRotatef ((GLfloat) ombro, 0.0, 0.0, 1.0);
        glPushMatrix();
                glRotatef(90.0,1.0,0.0,0.0);
                Eixos(eixovisivel,1,0,0,0);
        glPopMatrix();

        glPushMatrix();
                glRotatef(-90,0,-1,0);
                gluCylinder(quadratic,0.21f,0.21f,L2,32,32);
        glPopMatrix();

        glTranslatef (L2/2, 0.0, 0.0);
        glPushMatrix();
                glScalef (L2, 0.4, 0.4);//para que o cubo fique com L2 de comprimento e 0.4 x 0.4
                //CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
                model3->setTransform (mat3);
        glPopMatrix();

        // origem posicionada no cotovelo
        glTranslatef (L2/2, 0.0, 0.0);
        glRotatef ((GLfloat) cotovelo, 0.0, 0.0, 1.0);
        glPushMatrix();
                glRotatef(90.0,1.0,0.0,0.0);
                Eixos(eixovisivel,1,0,0,0);
        glPopMatrix();

        glPushMatrix();
                glColor4ub(150.0, 150.0, 100.0,200.0);
                glTranslatef (0.0, 0.0, -0.22);
                gluDisk(quadratic,0.0f,0.23f,32,32);
                gluCylinder(quadratic,0.23f,0.23f,0.44f,32,32);
                glTranslatef (0.0, 0.0, 0.44);
                gluDisk(quadratic,0.0f,0.23f,32,32);
        glPopMatrix();

        glColor4ub(255.0, 255.0, 120.0, 150.0);
        glPushMatrix();
                glRotatef(-90,0,-1,0);
                gluCylinder(quadratic,0.21f,0.21f,L3-0.2,32,32);
                glTranslatef (0.0, 0.0, L3-0.2);
                gluDisk(quadratic,0.0f,0.21f,32,32);
        glPopMatrix();

        glTranslatef (L3/2, 0.0, 0.0);
        glPushMatrix();
                glScalef (L3+0.4, 0.4, 0.4);
                //CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat2);
                model2->setTransform(mat2);
        glPopMatrix();

        //origem na ponta do braço (garra)
        glTranslatef (L3/2-0.2, 0.0, 0.0);

        ////////////////DESENHA PUNHO///////////////
        glRotatef (-90.0, 1.0, 0.0, 0.0);
        glPushMatrix();
                glTranslatef (0.2, 0.0, 0.0);
                glRotatef ((GLfloat) roll+90, 1.0, 0.0, 0.0);
                glRotatef((GLfloat) -yaw, 0.0, 0.0, 1.0);
                glRotatef(90.0,1.0,0.0,0.0);
                glRotatef(90.0,0.0,1.0,0.0);
                Eixos(eixovisivel,1,0,0,0);
        glPopMatrix();

        glRotatef ((GLfloat) roll+90, 1.0, 0.0, 0.0);
        glRotatef(90,0,1,0);
        glTranslatef (0.0, 0.0, -0.12);
        glColor4ub(150.0, 150.0, 100.0,200.0);
        gluCylinder(quadratic,0.17f,0.17f,0.24f,32,32);
        glTranslatef (0.0, 0.0, 0.24);
        gluDisk(quadratic,0.0f,0.17f,32,32);
        glTranslatef (0.0, 0.0, 0.07);
        glPushMatrix();
                glRotatef(90,0,1,0);
                glTranslatef (0.0, 0.0, -0.12);
                glColor4ub(255.0, 255.0, 120.0, 150.0);
                gluDisk(quadratic,0.0f,0.13f,32,32);
                gluCylinder(quadratic,0.13f,0.13f,0.24f,32,32);
                glTranslatef (0.0, 0.0, 0.24);
                gluDisk(quadratic,0.0f,0.13f,32,32);
        glPopMatrix();
        glPushMatrix();
                glRotatef(180.0,0.0,1.0,0.0);
                glRotatef(90.0,0.0,0.0,1.0);
                glRotatef(-90.0,1.0,0.0,0.0);
                Eixos(eixovisivel,1,0,0,0);
        glPopMatrix();
        glRotatef ((GLfloat) yaw, 1.0, 0.0, 0.0);
        glPushMatrix();
                glTranslatef (0.0, 0.0, 0.12);
                glColor4ub(150.0, 150.0, 100.0,200.0);
                gluDisk(quadratic,0.0f,0.15f,32,32);
                gluCylinder(quadratic,0.15f,0.15f,0.07f,32,32);
                glTranslatef (0.0, 0.0, 0.07);
                gluDisk(quadratic,0.0f,0.15f,32,32);
        glPopMatrix();

        glRotatef(-90.0,0.0,1.0,0.0);
        //Eixos(eixovisivel,1,0,0,0);
        glRotatef ((GLfloat) pitch+90, 1.0, 0.0, 0.0);
        glPushMatrix();
                glRotatef(-90.0,0.0,0.0,1.0);
                glRotatef(90.0,0.0,1.0,0.0);
                Eixos(eixovisivel,1,0,0,0);
        glPopMatrix();
        glTranslatef (0.1, 0.0, 0.0);

        ////////////Desenha garra////////////
        glPushMatrix();
                glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                glScalef (0.25, 0.15, 0.03);
                //CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
                model5->setTransform (mat5);
                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
        glPopMatrix();

        glPushMatrix();
                glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                glScalef (0.25, 0.15, 0.03);
                //CuboSolidoTri();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
                model6->setTransform (mat6);
                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
        glPopMatrix();

    glPopMatrix();
    // origem volta para o sistema de coordenadas original

    collision=(model->collision(model6)|model->collision(model5)|
               model->collision(model2)|model->collision(model3));
              // |model1->collision(model6)|model1->collision(model5)|
              // model1->collision(model2)|model1->collision(model3));

}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Cilindrico()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1,0,0);
        glRotatef(rothoricam,0,1,0);
        glPushMatrix();
                // origem posicionada no ombro no centro
                glPushMatrix();
                        glRotatef(-90,1,0,0);
                        glColor4ub(255.0, 255.0, 120.0, 150.0);//cor da base
                        gluCylinder(quadratic,0.3f,0.3f,L1,32,32);
                        glColor4ub(160.0, 160.0, 120.0, 150.0);
                        gluCylinder(quadratic,0.45f,0.45f,0.3,32,32);
                        glTranslatef (0.0, 0.0, 0.3);
                        gluDisk(quadratic,0.0f,0.45f,32,32);
                glPopMatrix();

                Eixos(eixovisivel,1,0,0,0.02);
                LabelXYZ(eixovisivel);
                //////////////DESENHA CHÃO//////////////
                //n=5;
                glBegin(GL_QUADS);
                glColor3ub(180.0, 180.0, 180.0);
                glVertex3f(-n, 0.0, -n);
                glVertex3f(n, 0.0, -n);
                glVertex3f(n, 0.0, n);
                glVertex3f(-n, 0.0, n);
                glEnd();
                glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                model->setTransform (mat);
                ///////////////////DESENHA GRADE/////////////////////
                glCallList(listaobj+1);
        glPopMatrix();

        glTranslatef (0.0, L1, 0.0);
        glPushMatrix();
                //Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                glRotatef (90.0, 0.0, 1.0, 0.0);
                //glTranslatef ( 0.0, (GLfloat)ombro/100-0.6, 0.0);
                glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
                glPushMatrix();
                        glRotatef (-90.0, 1.0, 0.0, 0.0);
                        glTranslatef ( 0.0, 0.0, -0.25);
                        glColor4ub(160.0, 160.0, 120.0, 150.0);
                        gluDisk(quadratic,0.0f,0.35f,32,32);
                        gluCylinder(quadratic,0.35f,0.35f,0.5,32,32);
                        glTranslatef (0.0, 0.0, 0.5);
                        gluDisk(quadratic,0.0f,0.35f,32,32);
                glPopMatrix();
                glPushMatrix();
                        glRotatef (-90.0, 0.0, 1.0, 0.0);
                        Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                glPopMatrix();
                glPushMatrix();
                        glColor4ub(255.0, 255.0, 120.0, 150.0);//cor da base
                        glTranslatef (0.0, 0.0, L2/2);
                        glScalef(0.05,0.36,L2);
                        glTranslatef (4.0, 0.0, 0.0);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                        glTranslatef (-8.0, 0.0, 0.0);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();
                
                glTranslatef ( 0.0, 0.0, (GLfloat)cotovelo/100) ;
                glPushMatrix();
                        glRotatef (90.0, 1.0, 0.0, 0.0);
                        Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                glPopMatrix();
                CuboSolido(0.5, 170.0, 170.0, 120.0);    
                glTranslatef ( 0.0, (GLfloat)ombro/100, 0.0);
                glRotatef (90.0, 1.0, 0.0, 0.0);
                glColor4ub(255.0, 255.0, 120.0,200.0);
                gluDisk(quadratic,0.0f,0.15f,32,32);
                gluCylinder(quadratic,0.15f,0.15f,L1-0.4,32,32);
                glTranslatef (0.0, 0.0, L1-0.4);
                ///////////////Desenha garra  ///////////////
                glPushMatrix();
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                        gluCylinder(quadratic,0.2f,0.2f,0.1f,32,32);
                        glTranslatef (0.0, 0.0, 0.1);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                glPopMatrix();

                glRotatef(90.0,0.0,-1.0,0.0);
                glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
                        model5->setTransform (mat5);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
                        model6->setTransform (mat6);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glRotatef(90.0,0.0,1.0,0.0);
                        glRotatef(90.0,1.0,0.0,0.0);
                        Eixos(eixovisivel,1.0,0.0,0.0,0.4); //0.4 desloca eixo para ponta
                glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

//---------------------------------------------------------------------------
void __fastcall Tbase1::Polar()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1.0,0.0,0.0);
        glRotatef(rothoricam,0.0,1.0,0.0);
        glPushMatrix();
                glPushMatrix();
                        //origem posicionada no ombro no centro
                        glPushMatrix();
                                glRotatef(-90,1,0,0);
                                glColor4ub(150.0, 150.0, 100.0,200.0);
                                gluCylinder(quadratic,0.5f,0.5f,0.3f,32,32);
                                glTranslatef (0.0, 0.0, 0.3);
                                gluDisk(quadratic,0.0f,0.5f,32,32);
                        glPopMatrix();

                        Eixos(eixovisivel,1,0,0,0.02);
                        LabelXYZ(eixovisivel);

                        //////////////DESENHA CHÃO//////////////
                        //n=5;
                        glBegin(GL_QUADS);
                        glColor3ub(180.0, 180.0, 180.0);
                        glVertex3f(-n, 0.0, -n);
                        glVertex3f(n, 0.0, -n);
                        glVertex3f(n, 0.0, n);
                        glVertex3f(-n, 0.0, n);
                        glEnd();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                        model->setTransform (mat);
                        ///////////////////DESENHA GRADE/////////////////////
                        glCallList(listaobj+1);
                glPopMatrix();

                // origem voltou para o ombro no centro
                glTranslatef (0.0, L1, 0.0);
                glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
                glPushMatrix();
                        glPushMatrix();
                                glTranslatef (0.0, 0.0, -0.35);
                                glColor4ub(160.0, 160.0, 120.0,200.0);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                                gluCylinder(quadratic,0.35f,0.35f,0.7f,32,32);
                                glTranslatef (0.0, 0.0, 0.7);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                        glPopMatrix();

                        Eixos(eixovisivel,1,0.0,0.0,0.0);
                        glTranslatef (0.0, -L1/2, 0.0);
                        glScalef (0.6, L1, 0.12);
                        glPushMatrix();
                                glScalef (1.0, 1.0, 5.4);
                                //CuboSolidoTri();
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat4);
	                        model4->setTransform (mat4);
                        glPopMatrix();

                        glTranslatef (0.0, 0.0, 2.2);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                        glTranslatef (0.0, 0.0, -4.4);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glRotatef ((GLfloat) ombro, 0.0, 0.0, 1.0);
                glPushMatrix();
                        glRotatef(90.0,1.0,0.0,0.0);
                        Eixos(eixovisivel,1,0,0,0);/////////////////////////////////////
                glPopMatrix();

                // origem posicionada no centro do braço
                glTranslatef (L2/2, 0.0, 0.0);
                glPushMatrix();
                        glScalef (L2, 0.4, 0.4);//para que o cubo fique com L2 de comprimento e 0.4 x 0.4
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
	                model3->setTransform (mat3);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glTranslatef (L2/2, 0.0, 0.0);
                //glTranslatef (L3/2-0.2, 0.0, 0.0);
                glTranslatef ((GLfloat)cotovelo/100, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(-90,0,-1,0);
                        glColor4ub(240.0, 240.0, 120.0,200.0);
                        gluCylinder(quadratic,0.19f,0.19f,L3-0.4,32,32);
                glPopMatrix();

                //////////Desenha garra//////////
                glTranslatef (L3-0.4, 0.0, 0.0);
                glRotatef(-90.0,0.0,-1.0,0.0);
                glPushMatrix();
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluCylinder(quadratic,0.18f,0.18f,0.1f,32,32);
                        glTranslatef (0.0, 0.0, 0.1);
                        gluDisk(quadratic,0.0f,0.18f,32,32);
                glPopMatrix();

                glRotatef(90.0,0.0,-1.0,0.0);
                //glPushMatrix();
                //        glTranslatef (0.4, 0.0, 0.0);
                //        glRotatef(90.0,0.0,0.0,1.0);
                //        glRotatef(180.0,1.0,0.0,0.0);
                //        Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                //glPopMatrix();
                glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
                        model5->setTransform (mat5);
                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
                        model6->setTransform (mat6);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glTranslatef (0.4, 0.0, 0.0);
                glRotatef(90.0,0.0,0.0,1.0);
                glRotatef(180.0,1.0,0.0,0.0);
                Eixos(eixovisivel,1.0,0.0,0.0,0.0);

        glPopMatrix();

    glPopMatrix();
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Cartesiano()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1,0,0);
        glRotatef(rothoricam,0,1,0);
        glPushMatrix();
                glPushMatrix();
                    //origem posicionada no ombro no centro
                    glPushMatrix();
                            glTranslatef (0.0, 0.125, 0.0);
                            glScalef(0.7,0.25,L3);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                            glScalef(0.3,1.4,1.0);
                            glTranslatef (0.0, 0.15, 0.0);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                    glPopMatrix();

                    Eixos(eixovisivel,1,0,0,0.02);
                    LabelXYZ(eixovisivel);
                    //////////////DESENHA CHÃO//////////////
                    //n=5;
                    glBegin(GL_QUADS);
                    glColor3ub(180.0, 180.0, 180.0);
                    glVertex3f(-n, 0.0, -n);
                    glVertex3f(n, 0.0, -n);
                    glVertex3f(n, 0.0, n);
                    glVertex3f(-n, 0.0, n);
                    glEnd();
                    ///////////////////DESENHA GRADE/////////////////////
                    glCallList(listaobj+1);
                glPopMatrix();

                glTranslatef (0.0, L1, 0.0);
                glRotatef (90.0, 0.0, 1.0, 0.0);
                glTranslatef ((GLfloat)ombro/100, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.0, -L1/2, 0.0);
                        glScalef(0.49,L1,0.49);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glRotatef (-90.0, 0.0, 1.0, 0.0);
                        glRotatef (-90.0, 1.0, 0.0, 0.0);
                        Eixos(eixovisivel,1.0,0.0,L1-0.03,0.0);
                glPopMatrix();

                glTranslatef ( 0.0, (GLfloat)cotovelo/100-L1+1.4, 0.0) ;
                CuboSolido(0.6, 170.0, 170.0, 120.0);
                Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                glRotatef (45.0, 0.0, 0.0, 1.0);
                glColor4ub(255.0, 255.0, 120.0, 150.0);//cor da base
                gluCylinder(quadratic,0.20f,0.20f,L2,4,4);
                glTranslatef (0.0, 0.0, L2);
                gluDisk(quadratic,0.0f,0.20f,4,4);
                glRotatef (-45.0, 0.0, 0.0, 1.0);
                glTranslatef ( 0.0, 0.0, (GLfloat) base/100-L2) ;
                CuboSolido(0.35, 170.0, 170.0, 120.0);
                glRotatef (90.0, 1.0, 0.0, 0.0);
                Eixos(eixovisivel,1.0,0.0,0.0,0.0);
                glColor4ub(255.0, 255.0, 120.0,200.0);
                gluCylinder(quadratic,0.15f,0.15f,1.0,32,32);
                glTranslatef (0.0, 0.0, 1.0);
                ////////////////Desenha garra////////////////
                glPushMatrix();
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                        gluCylinder(quadratic,0.2f,0.2f,0.1f,32,32);
                        glTranslatef (0.0, 0.0, 0.1);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                glPopMatrix();

                glRotatef(90.0,0.0,-1.0,0.0);
                glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glTranslatef (0.4, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(90.0,0.0,1.0,0.0);
                        glRotatef(90.0,1.0,0.0,0.0);
                        Eixos(eixovisivel,1,0,0,0);
                glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Cartesiano_2()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1,0,0);
        glRotatef(rothoricam,0,1,0);
        glPushMatrix();
            glPushMatrix();
                    Eixos(eixovisivel,1,0,0,0.02);
                    LabelXYZ(eixovisivel);
                    //////////////DESENHA CHÃO//////////////
                    //n=5;
                    glBegin(GL_QUADS);
                    glColor3ub(180.0, 180.0, 180.0);
                    glVertex3f(-n, 0.0, -n);
                    glVertex3f(n, 0.0, -n);
                    glVertex3f(n, 0.0, n);
                    glVertex3f(-n, 0.0, n);
                    glEnd();
                    ///////////////////DESENHA GRADE/////////////////////
                    glCallList(listaobj+1);

                    //origem posicionada no ombro no centro
                    glPushMatrix();
                        glPushMatrix();
                            glTranslatef (L2/2.0, L1/2.0, L3/2.0);
                            glScalef(0.3,L1,0.3);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                        glPopMatrix();
                        glPushMatrix();
                            glTranslatef (-L2/2.0, L1/2.0, L3/2.0);
                            glScalef(0.3,L1,0.3);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                        glPopMatrix();
                        glPushMatrix();
                            glTranslatef (L2/2.0, L1/2.0, -L3/2.0);
                            glScalef(0.3,L1,0.3);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                        glPopMatrix();
                        glPushMatrix();
                            glTranslatef (-L2/2.0, L1/2.0, -L3/2.0);
                            glScalef(0.3,L1,0.3);
                            CuboSolido(1.0, 170.0, 170.0, 120.0);
                        glPopMatrix();
                        glPushMatrix();  //Desenha as duas barras de deslize
                            glTranslatef (L2/2.0, L1-0.2, -L3/2.0);
                            glColor4ub(255.0, 255.0, 120.0, 150.0);
                            gluCylinder(quadratic,0.10f,0.10f,L3,16,16);
                            glTranslatef (-L2, 0, 0.0);
                            gluCylinder(quadratic,0.10f,0.10f,L3,16,16);
                        glPopMatrix();
                        glPushMatrix();
                            glRotatef(90,0,1,0);
                            glTranslatef (L3/2.0, L1-0.2, -L2/2.0);
                            glColor4ub(160.0, 160.0, 120.0, 200.0);
                            gluCylinder(quadratic,0.10f,0.10f,L2,16,16);
                            glTranslatef (-L3, 0.0, 0.0);
                            gluCylinder(quadratic,0.10f,0.10f,L2,16,16);
                        glPopMatrix();
                    glPopMatrix();
                    glPushMatrix();
                        glPushMatrix();
                            glTranslatef (0.0, L1-0.2, -ombro/100.0);
                            glPushMatrix();
                                glTranslatef (L2/2.0, 0.0, 0.0);
                                glPushMatrix();
                                    glScalef(0.3,0.3,0.6);
                                    CuboSolido(1.0, 255.0, 255.0, 120.0);
                                glPopMatrix();
                                glTranslatef (-L2, 0.0, 0);
                                glPushMatrix();
                                    glScalef(0.3,0.3,0.6);
                                    CuboSolido(1.0, 255.0, 255.0, 120.0);
                                glPopMatrix();
                                glPushMatrix();
                                    glRotatef(90,0,1,0);
                                    glTranslatef (0.2, 0.0, 0);
                                    gluCylinder(quadratic,0.05f,0.05f,L2,16,16);
                                    glTranslatef (-0.4, 0.0, 0);
                                    gluCylinder(quadratic,0.05f,0.05f,L2,16,16);
                                glPopMatrix();
                            glPopMatrix();
                            glTranslatef (base/100.0, 0.0, 0.0);
                            glPushMatrix();
                                glScalef(0.6,0.3,0.6);
                                CuboSolido(1.0, 255.0, 255.0, 120.0);
                            glPopMatrix();
                            glTranslatef (0.0, cotovelo/100.0+0.3, 0.0);
                            glPushMatrix();
                                glRotatef(90,1,0,0);
                                gluDisk(quadratic,0.0f,0.14f,16,16);
                                gluCylinder(quadratic,0.14f,0.14f,L1-0.3,16,16);
                            glPopMatrix();
                            ///////////////Desenha garra  ///////////////
                            glRotatef(90.0,1.0,0.0,0.0);
                            glTranslatef (0.0, 0.0, L1-0.3);
                            glPushMatrix();
                                glColor4ub(160.0, 160.0, 120.0,200.0);
                                gluDisk(quadratic,0.0f,0.2f,32,32);
                                gluCylinder(quadratic,0.2f,0.2f,0.1f,32,32);
                                glTranslatef (0.0, 0.0, 0.1);
                                gluDisk(quadratic,0.0f,0.2f,32,32);
                            glPopMatrix();
                            glRotatef(90.0,0.0,-1.0,0.0);
                            glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                            glPushMatrix();
                                glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                                glScalef (0.4, 0.2, 0.03);
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
                                model5->setTransform (mat5);
                                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                            glPopMatrix();
                            glPushMatrix();
                                glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                                glScalef (0.4, 0.2, 0.03);
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
                                model6->setTransform (mat6);
                                glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                            glPopMatrix();
                            glPushMatrix();
                                glRotatef(90.0,0.0,1.0,0.0);
                                glRotatef(90.0,1.0,0.0,0.0);
                                Eixos(eixovisivel,1.0,0.0,0.0,0.4); //0.4 desloca eixo para ponta
                            glPopMatrix();
                            //termina desenho da garra
                        glPopMatrix();
                    glPopMatrix();

            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

//---------------------------------------------------------------------------
void __fastcall Tbase1::SCARA()
{
    glLoadIdentity();
    gluLookAt( 0.0, 0.0,zoomcam, 0.0,0.0,0.0, 0.0,1.0,0.0);
    quadratic=gluNewQuadric();

    glPushMatrix();
        glTranslatef (xcam, ycam, zcam);
        glRotatef(rotvertcam,1.0,0.0,0.0);
        glRotatef(rothoricam,0.0,1.0,0.0);
        glPushMatrix();
                glPushMatrix();
                // origem posicionada no ombro no centro
                        glPushMatrix();
                                glRotatef(-90,1,0,0);
                                glColor4ub(150.0, 150.0, 100.0,200.0);
                                gluCylinder(quadratic,0.5f,0.5f,0.3f,32,32);
                                glColor4ub(255.0, 255.0, 120.0, 150.0);
                                gluCylinder(quadratic,0.3f,0.3f,L1,32,32);
                                glTranslatef (0.0, 0.0, 0.3);
                                glColor4ub(150.0, 150.0, 100.0,200.0);
                                gluDisk(quadratic,0.0f,0.5f,32,32);
                        glPopMatrix();

                        Eixos(eixovisivel,1,0,0,0.02);////////////////////////////////////
                        LabelXYZ(eixovisivel);
                        //////////////DESENHA CHÃO//////////////
                        //n=5;
                        glBegin(GL_QUADS);
                        glColor3ub(180.0, 180.0, 180.0);
                        glVertex3f(-n, 0.0, -n);
                        glVertex3f(n, 0.0, -n);
                        glVertex3f(n, 0.0, n);
                        glVertex3f(-n, 0.0, n);
                        glEnd();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                        model->setTransform (mat);
                        ///////////////////DESENHA GRADE/////////////////////
                        glCallList(listaobj+1);
                glPopMatrix();

                // origem voltou para o ombro no centro
                glTranslatef (0.0, L1, 0.0);
                glPushMatrix();
                        glPushMatrix();
                                glRotatef (90.0, 1.0, 0.0, 0.0);
                                glTranslatef (0.0, 0.0, -0.35);
                                glColor4ub(160.0, 160.0, 120.0,200.0);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                                gluCylinder(quadratic,0.35f,0.35f,0.7f,32,32);
                                glTranslatef (0.0, 0.0, 0.7);
                                gluDisk(quadratic,0.0f,0.35f,32,32);
                        glPopMatrix();

                        glTranslatef (0.0, -L1/2, 0.0);
                        glScalef (0.6, L1, 0.12);
                        glPushMatrix();
                                glScalef (1.0, 1.0, 5.4);
                                //CuboSolidoTri();
                                glGetFloatv(GL_MODELVIEW_MATRIX, mat4);
	                        model4->setTransform (mat4);
                        glPopMatrix();

                glPopMatrix();

                // origem posicionada no centro do braço
                glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
                Eixos(eixovisivel,1,0,0,0);
                glTranslatef (L2/2, 0.0, 0.0);
                glPushMatrix();
                        glScalef (L2, 0.4, 0.4);//para que o cubo fique com L2 de comprimento e 0.4 x 0.4
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
	                model3->setTransform (mat3);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                // origem posicionada no cotovelo
                glTranslatef (L2/2, 0.0, 0.0);
                glRotatef ((GLfloat) cotovelo, 0.0, 1.0, 0.0);
                Eixos(eixovisivel,1.0,0.0,0.0,0.4);
                glPushMatrix();
                        glRotatef(-90.0,1.0,0.0,0.0);
                        glTranslatef (0.0, 0.0, -0.22);
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                        gluCylinder(quadratic,0.22f,0.22f,0.84f,32,32);
                        glTranslatef (0.0, 0.0, 0.84);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                glPopMatrix();

                glTranslatef (L3/2, 0.4, 0.0);
                glPushMatrix();
                        glScalef (L3, 0.4, 0.4);
                        //CuboSolidoTri();
	                glGetFloatv(GL_MODELVIEW_MATRIX, mat2);
                        model2->setTransform (mat2);
                glPopMatrix();

                glPushMatrix();
                        glScalef (L3, 0.4, 0.4);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();
    
                //origem na ponta do braço (garra)
                glTranslatef (L3/2, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(-90.0,1.0,0.0,0.0);
                        glTranslatef (0.0, 0.0, -0.22);
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                        gluCylinder(quadratic,0.22f,0.22f,0.44f,32,32);
                        glTranslatef (0.0, 0.0, 0.44);
                        gluDisk(quadratic,0.0f,0.22f,32,32);
                glPopMatrix();

                ////////Desenha garra  ///////////////

                glTranslatef (0.0, ombro/100-0.3, 0.0);
                glRotatef(90,0,0,-1);
                glPushMatrix();
                        glRotatef(-90,0,-1,0);
                        glTranslatef (0.0, 0.0, -0.3);
                        glColor4ub(250.0, 250.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.15f,32,32);
                        gluCylinder(quadratic,0.15f,0.15f,L1,32,32);
                        glTranslatef (0.0, 0.0, L1);
                        glColor4ub(160.0, 160.0, 120.0,200.0);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                        gluCylinder(quadratic,0.2f,0.2f,0.1f,32,32);
                        glTranslatef (0.0, 0.0, 0.1);
                        gluDisk(quadratic,0.0f,0.2f,32,32);
                glPopMatrix();

                glRotatef ((GLfloat) roll, 1.0, 0.0, 0.0);
                glTranslatef (L1-0.3, 0.0, 0.0);
                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)-garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat5);
                        model5->setTransform (mat5);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glPushMatrix();
                        glTranslatef (0.2, 0.0, (GLfloat)garra/1000);
                        glScalef (0.4, 0.2, 0.03);
                        //CuboSolidoTri();
                        glGetFloatv(GL_MODELVIEW_MATRIX, mat6);
                        model6->setTransform (mat6);
                        glCallList(listaobj);//CuboSolido(1.0, 255.0, 255.0, 120.0);
                glPopMatrix();

                glTranslatef (0.4, 0.0, 0.0);
                glPushMatrix();
                        glRotatef(90.0,0.0,0.0,1.0);
                        Eixos(eixovisivel,1,0,0,0);
                glPopMatrix();

        glPopMatrix();
        
    // origem volta para o sistema de coordenadas original
    glPopMatrix();
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::FormPaint(TObject *Sender)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Articulado1();
    glFlush();
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::FormDestroy(TObject *Sender)
{
    threadloop = false;
    gluDeleteQuadric(quadratic);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
}

//--------------------------------------------------------------------------- 
void __fastcall Tbase1::SetupLighting()
{
    GLfloat LightAmb[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat LightDif[] = {0.08, 0.08, 0.08, 1.0};
    GLfloat LightSpe[] = {0.3, 0.3, 0.3, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpe);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpe);

    GLfloat MaterialAmbient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat MaterialDiffuse[] = {0.43, 0.47, 0.70, 1.0};//{1.0, 1.0, 1.0, 1.0};
    GLfloat MaterialSpecular[] = {0.33, 0.33, 0.47, 1.0};//{1.0, 1.0, 1.0, 1.0};
    GLfloat MaterialEmission[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat MaterialShininess[] = {60.0};
    GLfloat AmbientLightPosition0[] = {0.5, 1.0, 1.0, 0.0};
    GLfloat AmbientLightPosition1[] = {-0.5, -1.0, -1.0, 0.0};
    GLfloat LightAmbient[] = {0.6, 0.6, 0.6, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, MaterialEmission);
    glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPosition0);
    glLightfv(GL_LIGHT1, GL_POSITION, AmbientLightPosition1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbient);

    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_FLAT);//(GL_SMOOTH);
}

//---------------------------------------------------------------------------

void __fastcall Tbase1::Timer1Timer(TObject *Sender)
{
    base=(abs(base-editbase)>=1)?(base+baseinc):editbase;
    ombro=(abs(ombro-editombro)>=1)?(ombro+ombroinc):editombro;
    cotovelo=(abs(cotovelo-editcotovelo)>=1)?(cotovelo+cotoveloinc):editcotovelo;

    if(base==editbase && ombro==editombro && cotovelo==editcotovelo){
        //PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        if(PageControl1->ActivePage==TabSheet3)
            Tbase1::SpeedButton13Click(Sender);
    }
    else{
        //PlaySound("robotarm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Sair1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Reiniciar1Click(TObject *Sender)
{
    base=0.0;
    ombro=0.0;
    cotovelo=0.0;
    editbase = 0.0;
    editombro = 0.0;
    editcotovelo = 0.0;
    rothoricam=0.0;
    rotvertcam=20.0;
    zoomcam=12.0;
    xcam=0.0;
    ycam=0.0;
    zcam=0.0;
    roll=0;
    pitch=0;
    yaw=0;
    Edit1->Text = "0";
    Edit2->Text = "0";
    Edit3->Text = "0";
    Edit4->Text = "300";
    Edit5->Text = "0";
    Edit6->Text = "100";
    Tbase1::N3DOFSimples1Click(Sender);
    StatusBar1->Panels->Items[1]->Text="Posição válida";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button1Click(TObject *Sender)
{

    //PlaySound("robotarm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    switch(tipo)
    {
    case(1):
        //Case dos manipuladores Articulados
        if(!Cdireta(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text),
                    x, y, z, L1*100, L2*100, L3*100,
                    CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                    basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = Resto(StrToFloat(Edit2->Text));
                editcotovelo = Resto(StrToFloat(Edit3->Text));
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit5->Text = FloatToStr(Arredonda(y,2));
        Edit6->Text = FloatToStr(Arredonda(z,2));
        break;
    case(2):
        //TODO: Case do articulado de 6 GDF
        break;
    case(3):
        //Case do manipulador Polar
        if(!CdiretaPolar(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text),
                         x, y, z, L1*100, L2*100, L3*100,
                         CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                         basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = Resto(StrToFloat(Edit2->Text));
                editcotovelo = StrToFloat(Edit3->Text);
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit5->Text = FloatToStr(Arredonda(y,2));
        Edit6->Text = FloatToStr(Arredonda(z,2));
        break;
    case(4):
        //Case do manipulador Cilindrico
        if(!CdiretaCilindrico(StrToFloat(Edit1->Text), StrToFloat(Edit3->Text), StrToFloat(Edit2->Text),
                              x, y, z, L1*100, L2*100, L3*100,
                              CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                              basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = Resto(StrToFloat(Edit2->Text));
                editcotovelo = Resto(StrToFloat(Edit3->Text));
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit5->Text = FloatToStr(Arredonda(y,2));
        Edit6->Text = FloatToStr(Arredonda(z,2));
        break;
    case(5):
        //Case do manipulador Cartesiano
        if(!CCartesiano(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text),
                        CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                        basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = StrToFloat(Edit2->Text);
                editcotovelo = StrToFloat(Edit3->Text);
                x=editbase;
                y=editcotovelo;
                z=editombro;
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit5->Text = FloatToStr(Arredonda(z,2));
        Edit6->Text = FloatToStr(Arredonda(y,2));
        break;
    case(6):
        //Case do manipulador SCARA
        if(!CdiretaSCARA(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text),
                         x, y, z, L1*100, L2*100, L3*100,
                         CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                         basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = Resto(StrToFloat(Edit2->Text));
                editcotovelo = Resto(StrToFloat(Edit3->Text));
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit5->Text = FloatToStr(Arredonda(y,2));
        Edit6->Text = FloatToStr(Arredonda(z,2));
        break;
    case(7):
        //Case do manipulador Cartesiano 2
        if(!CCartesiano(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text),
                        CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                        basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                editbase = StrToFloat(Edit1->Text);
                editombro = StrToFloat(Edit2->Text);
                editcotovelo = StrToFloat(Edit3->Text);
                x=editbase;
                y=editcotovelo;
                z=editombro;
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
        }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
        }
        Edit4->Text = FloatToStr(Arredonda(x,2));
        Edit6->Text = FloatToStr(Arredonda(y,2));
        Edit5->Text = FloatToStr(Arredonda(z,2));
        break;
    default:
        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button2Click(TObject *Sender)
{
      switch(tipo)
      {
        case(1):
            if (!C_inversa(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                           editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                           CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                           basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                x = StrToFloat(Edit4->Text);
                y = StrToFloat(Edit5->Text);
                z = StrToFloat(Edit6->Text);
                StatusBar1->Panels->Items[1]->Text="Posição válida";
                Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                Timer1->Enabled=true;
            }else{
                Timer1->Enabled=false;
                StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
            break;
        case(2):
            break;
        case(3):
            if (!C_inversaPolar(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                                editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                                CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                                basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));

            break;
        case(4):
            break;
        case(5):
            break;
        case(6):
            if (!C_inversaSCARA(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                                editbase, editcotovelo, editombro, L1*100, L2*100, L3*100,
                                CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                                basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
            break;
        case(7):
            break;
        default:
		break;
      }


}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button3Click(TObject *Sender)
{
      switch(tipo)
      {
        case(1):
            if (!Cinversa(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                          editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                          CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                          basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    editbase=base;
                    editombro=ombro;
                    editcotovelo=cotovelo;
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
            break;
        case(2):
            break;
        case(3):
            if (!CinversaPolar(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                               editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                               CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                               basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));

            break;
        case(4):
            if (!CinversaCilindrico(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                                    editbase, editcotovelo, editombro, L1*100, L2*100, L3*100,
                                    CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                                    basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
            break;
        case(5):
            if(StrToFloat(Edit4->Text)<=L2*100-20.0 && StrToFloat(Edit4->Text)>=50.0 &&
               StrToFloat(Edit5->Text)<=L3*50-30.0 && StrToFloat(Edit5->Text)>=-L3*50+30.0 &&
               StrToFloat(Edit6->Text)<=(L1*100-175.0) && StrToFloat(Edit6->Text)>=0.0){
                    editbase = StrToFloat(Edit4->Text);
                    editombro = StrToFloat(Edit5->Text);
                    editcotovelo = StrToFloat(Edit6->Text);
                    x=editbase;
                    y=editcotovelo;
                    z=editombro;
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(x,2));
            Edit2->Text = FloatToStr(Arredonda(z,2));
            Edit3->Text = FloatToStr(Arredonda(y,2));
            break;
        case(6):
            if (!CinversaSCARA(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                               editbase, editcotovelo, editombro, L1*100, L2*100, L3*100,
                               CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                               basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
                    x = StrToFloat(Edit4->Text);
                    y = StrToFloat(Edit5->Text);
                    z = StrToFloat(Edit6->Text);
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(editbase,2));
            Edit2->Text = FloatToStr(Arredonda(editombro,2));
            Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
            break;
        case(7):
            if(StrToFloat(Edit4->Text)<=L2*50.0-50.0 && StrToFloat(Edit4->Text)>=-L2*50.0+50.0 &&
               StrToFloat(Edit5->Text)<=L3*50-50.0 && StrToFloat(Edit5->Text)>=-L3*50+50.0 &&
               StrToFloat(Edit6->Text)<=(L1*100-100.0) && StrToFloat(Edit6->Text)>=0.0){
                    editbase = StrToFloat(Edit4->Text);
                    editombro = StrToFloat(Edit5->Text);
                    editcotovelo = StrToFloat(Edit6->Text);
                    x=editbase;
                    y=editcotovelo;
                    z=editombro;
                    StatusBar1->Panels->Items[1]->Text="Posição válida";
                    Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
                    Timer1->Enabled=true;
            }else{
                    Timer1->Enabled=false;
                    StatusBar1->Panels->Items[1]->Text="Posição inválida";
            }
            Edit1->Text = FloatToStr(Arredonda(x,2));
            Edit2->Text = FloatToStr(Arredonda(y,2));
            Edit3->Text = FloatToStr(Arredonda(z,2));
            break;
        default:
		break;
      }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button4Click(TObject *Sender)
{
    if (!Cinversa(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                  editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                  CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                  basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
        editbase+=180;
        editcotovelo=-editcotovelo;
        editombro=180-editombro;
        x = StrToFloat(Edit4->Text);
        y = StrToFloat(Edit5->Text);
        z = StrToFloat(Edit6->Text);
        StatusBar1->Panels->Items[1]->Text="Posição válida";
        Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
        Timer1->Enabled=true;
    }else{
        Timer1->Enabled=false;
        StatusBar1->Panels->Items[1]->Text="Posição inválida";
    }
    Edit1->Text = FloatToStr(Arredonda(editbase,2));
    Edit2->Text = FloatToStr(Arredonda(editombro,2));
    Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button5Click(TObject *Sender)
{
    if (!C_inversa(StrToFloat(Edit4->Text), StrToFloat(Edit5->Text), StrToFloat(Edit6->Text),
                   editbase, editombro, editcotovelo, L1*100, L2*100, L3*100,
                   CheckBox2->Checked, CheckBox3->Checked, CheckBox4->Checked,
                   basemax, basemin, ombromax, ombromin, cotovelomax, cotovelomin)){
        editbase+=180;
        editcotovelo=-editcotovelo;
        editombro=180-editombro;
        x = StrToFloat(Edit4->Text);
        y = StrToFloat(Edit5->Text);
        z = StrToFloat(Edit6->Text);
        StatusBar1->Panels->Items[1]->Text="Posição válida";
        Incremento(editbase-base, editombro-ombro, editcotovelo-cotovelo, baseinc, ombroinc, cotoveloinc);
        Timer1->Enabled=true;
    }else{
        Timer1->Enabled=false;
        StatusBar1->Panels->Items[1]->Text="Posição inválida";
    }
    Edit1->Text = FloatToStr(Arredonda(editbase,2));
    Edit2->Text = FloatToStr(Arredonda(editombro,2));
    Edit3->Text = FloatToStr(Arredonda(editcotovelo,2));

}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar4Change(TObject *Sender)
{
    rothoricam=TrackBar4->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar5Change(TObject *Sender)
{
    rotvertcam=TrackBar5->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar6Change(TObject *Sender)
{
    zoomcam=TrackBar6->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar7Change(TObject *Sender)
{
    garra=(float)(TrackBar7->Position);
    Edit10->Text=IntToStr(TrackBar7->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Sobre1Click(TObject *Sender)
{
    Form1 = new TForm1(Application);
    Form1->ShowModal();
    delete Form1;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar8Change(TObject *Sender)
{
    Edit7->Text=IntToStr(TrackBar8->Position);
    L1=TrackBar8->Position/100.0;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar9Change(TObject *Sender)
{
    Edit8->Text=IntToStr(TrackBar9->Position);
    L2=(TrackBar9->Position)/100.0;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar10Change(TObject *Sender)
{
    Edit9->Text=IntToStr(TrackBar10->Position);
    L3=(TrackBar10->Position)/100.0;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn3Click(TObject *Sender)
{
    Form2 = new TForm2(Application);
    Form2->ShowModal();
    delete Form2;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button10Click(TObject *Sender)
{
    ++StringGrid2->RowCount;
    Label25->Caption=IntToStr(StringGrid2->RowCount);
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Timer2Timer(TObject *Sender)
{
    Timer1->Enabled=false;
    Tbase1::Joystick();
    Edit1->Text = FloatToStr(Arredonda(base,2));
    Edit2->Text = FloatToStr(Arredonda(ombro,2));
    Edit3->Text = FloatToStr(Arredonda(cotovelo,2));
    /*
    Cdireta(base, ombro, cotovelo, x, y, z, L1*100, L2*100, L3*100);
    Edit4->Text = FloatToStr(Arredonda(x,2));
    Edit5->Text = FloatToStr(Arredonda(y,2));
    Edit6->Text = FloatToStr(Arredonda(z,2));            */
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button15Click(TObject *Sender)
{
    --StringGrid2->RowCount;
    Label25->Caption=IntToStr(StringGrid2->RowCount);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ComboBox1Change(TObject *Sender)
{
    StringGrid2->Cells[0][j]=ComboBox1->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SelectCell(TObject *Sender, int ACol, int ARow,
      bool &CanSelect)
{
    j=ARow;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::PageControl1Change(TObject *Sender)
{
    Timer1->Interval=1000-TrackBar26->Position;
    Tbase1::SpeedButton1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Button16Click(TObject *Sender)
{
   for(j=0;(int)j<StringGrid2->RowCount;++j){
      StringGrid2->Cells[0][j] = "";
      StringGrid2->Cells[1][j] = "";
      StringGrid2->Cells[2][j] = "";
      StringGrid2->Cells[3][j] = "";
   }
   StringGrid2->RowCount=13;
   Label25->Caption=IntToStr(StringGrid2->RowCount);
   j=0;
}
//---------------------------------------------------------------------------

float Yant,Xant;
void __fastcall Tbase1::MouseAcao(TObject *Sender, TShiftState Shift, int X, int Y)
{
    if(Movimentarcmeracommouse1->Checked==true){
        if (Shift.Contains(ssLeft)){
            rotvertcam=rotvertcam-(Yant-Y)/3.0;
            rothoricam=rothoricam-(Xant-X)/3.0;
            Yant=Y;
            Xant=X;
        }
        if (Shift.Contains(ssRight)){
            zoomcam=zoomcam-(Yant-Y)/20.0;
            if(zoomcam<2.0)
                    zoomcam=2.0;
            Yant=Y;
        }

        if (Shift.Contains(ssMiddle)){
            zcam=zcam-(Yant-Y)/50.0;
            xcam=xcam-(Xant-X)/50.0;
            Yant=Y;
            Xant=X;
        }
    }else{
        if (Shift.Contains(ssLeft)){
            //PlaySound("robotarm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            ombro+=(Yant-Y)/4;
            base+=(X-Xant)/4;

            Yant=Y;
            Xant=X;

            Edit1->Text = FloatToStr(Arredonda(base,2));
            Edit2->Text = FloatToStr(Arredonda(ombro,2));
            Tbase1::Button1Click(Sender);
            if(Timer1->Enabled==false){
                    base = baseant;
                    ombro = ombroant;
                    editbase = baseant;
                    editombro = ombroant;
                    Edit1->Text = FloatToStr(Arredonda(baseant,2));
                    Edit2->Text = FloatToStr(Arredonda(ombroant,2));
            }
            Timer1->Enabled=false;
        }
        if (Shift.Contains(ssRight)){
            //PlaySound("robotarm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            cotovelo+=(Yant-Y)/4;

            if(Xant>X&&garra>30){
                   garra-=10;
            }
            else if(Xant<X&&garra<100){
                   garra+=10;
            }
            Yant=Y;
            Xant=X;

            Edit3->Text = FloatToStr(Arredonda(cotovelo,2));
            Tbase1::Button1Click(Sender);
            if(Timer1->Enabled==false){
                cotovelo=cotoveloant;
                editcotovelo = cotoveloant;
                Edit3->Text = FloatToStr(Arredonda(cotoveloant,2));
            }
            Timer1->Enabled=false;
        }
        if (Shift.Contains(ssMiddle)){
            zcam=zcam-(Yant-Y)/50.0;
            xcam=xcam-(Xant-X)/50.0;
            Yant=Y;
            Xant=X;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::MouseClique(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    Xant=X;
    Yant=Y;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::XoY1Click(TObject *Sender)
{
    rothoricam=0;
    rotvertcam=90;
    xcam=0;
    zcam=0;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::XoZ1Click(TObject *Sender)
{
    rothoricam=0;
    rotvertcam=0;
    xcam=0;
    zcam=0;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::YoZ1Click(TObject *Sender)
{
    rothoricam=270;
    rotvertcam=0;
    xcam=0;
    zcam=0;
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::N6DOFPulsoEsferico1Click(TObject *Sender)
{
    N6DOFPulsoEsfrico1->Checked=true;
    N3DOFSimples1->Checked=false;
    Polar1->Checked=false;
    Cilindrico1->Checked=false;
    Cartesiano1->Checked=false;
    Cartesiano2->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=true;
    TrackBar16->Enabled=true;
    Edit11->Enabled=true;
    Edit12->Enabled=true;
    Edit13->Enabled=true;
    Button2->Enabled=true;
    Button3->Enabled=true;
    Button4->Enabled=true;
    Button5->Enabled=true;
    Label4->Caption="Âng.Base";
    Label5->Caption="Âng.Ombro";
    Label6->Caption="Âng.Cotovelo";
    Label24->Caption="o";
    Label35->Caption="o";
    Label36->Caption="o";

    L1=1.0;
    L2=1.4;
    L3=1.6;    
    TrackBar8->Min=70;
    TrackBar9->Min=70;
    TrackBar10->Min=70;
    TrackBar8->Position=100;
    TrackBar9->Position=140;
    TrackBar10->Position=160;
    TrackBar10->Enabled=true;

    base=0.0;
    Edit1->Text = "0";
    ombro=0.0;
    Edit2->Text = "0";
    cotovelo=0.0;
    Edit3->Text = "0";

    ComboBox1->Items->Add("WRIST");

    tipo=1;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::N3DOFSimples1Click(TObject *Sender)
{
    N3DOFSimples1->Checked=true;
    N6DOFPulsoEsfrico1->Checked=false;
    Polar1->Checked=false;
    Cilindrico1->Checked=false;
    Cartesiano1->Checked=false;
    Cartesiano2->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;
    Button2->Enabled=true;
    Button3->Enabled=true;
    Button4->Enabled=true;
    Button5->Enabled=true;
    Label4->Caption="Âng.Base";
    Label5->Caption="Âng.Ombro";
    Label6->Caption="Âng.Cotovelo";
    Label24->Caption="o";
    Label35->Caption="o";
    Label36->Caption="o";

    L1=1.0;
    L2=1.4;
    L3=1.6;
    TrackBar8->Min=70;
    TrackBar9->Min=70;
    TrackBar10->Min=70;
    TrackBar8->Position=100;
    TrackBar9->Position=140;
    TrackBar10->Position=160;
    TrackBar10->Enabled=true; 

    base=0.0;
    Edit1->Text = "0";
    ombro=0.0;
    Edit2->Text = "0";
    cotovelo=0.0;
    Edit3->Text = "0";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=1;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::GradenoplanoX1Click(TObject *Sender)
{
    if(GradenoplanoX1->Checked==true){
        GradenoplanoX1->Checked=false;
        ToolButton4->Hint="Exibir grade no plano XoY";
        ToolButton4->Down=false;
    }else{
        GradenoplanoX1->Checked=true;
        ToolButton4->Hint="Ocultar grade no plano XoY";
        ToolButton4->Down=true;
    }
    CompilaGrade(GradenoplanoX1->Checked, GradenoplanoY1->Checked, GradenoplanoZ1->Checked, n);

}
//---------------------------------------------------------------------------

void __fastcall Tbase1::GradenoplanoY1Click(TObject *Sender)
{
    if(GradenoplanoY1->Checked==true){
        GradenoplanoY1->Checked=false;
        ToolButton5->Hint="Exibir grade no plano XoZ";
        ToolButton5->Down=false;
    }else{
        GradenoplanoY1->Checked=true;
        ToolButton5->Hint="Ocultar grade no plano XoZ";
        ToolButton5->Down=true;
    }
    CompilaGrade(GradenoplanoX1->Checked, GradenoplanoY1->Checked, GradenoplanoZ1->Checked, n);

}
//---------------------------------------------------------------------------

void __fastcall Tbase1::GradenoplanoZ1Click(TObject *Sender)
{
    if(GradenoplanoZ1->Checked==true){
        GradenoplanoZ1->Checked=false;
        ToolButton6->Hint="Exibir grade no plano YoZ";
        ToolButton6->Down=false;
    }else{
        GradenoplanoZ1->Checked=true;
        ToolButton6->Hint="Ocultar grade no plano YoZ";
        ToolButton6->Down=true;
    }
    CompilaGrade(GradenoplanoX1->Checked, GradenoplanoY1->Checked, GradenoplanoZ1->Checked, n);

}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton1Click(TObject *Sender)
{
    i=0;
    j=0;
    Timer1->Enabled=false;
    SpeedButton2->Enabled=true;
    SpeedButton3->Enabled=true;
    SpeedButton4->Enabled=true;
    SpeedButton5->Enabled=true;
    SpeedButton6->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton2Click(TObject *Sender)
{
    try
    {
        Tbase1::ToolButton7Click(Sender);
        Timer2->Enabled=false;
        ToolButton2->Hint="Ativar joystick";
        ToolButton2->Down=false;
        Joystick1->Checked=false;
        Timer1->Interval = 1000-TrackBar26->Position;

        if(StringGrid2->Cells[i][j] == "DIR"){
            Edit1->Text=StringGrid2->Cells[i+1][j];
            Edit2->Text=StringGrid2->Cells[i+2][j];
            Edit3->Text=StringGrid2->Cells[i+3][j];
            Tbase1::Button1Click(Sender);
            Timer1->Enabled=true;
            i=0;
            j++;
        }
        else{
            if(StringGrid2->Cells[i][j] == "INV"){
                Edit4->Text=StringGrid2->Cells[i+1][j];
                Edit5->Text=StringGrid2->Cells[i+2][j];
                Edit6->Text=StringGrid2->Cells[i+3][j];
                Tbase1::Button3Click(Sender);
                Timer1->Enabled=true;
                i=0;
                j++;
            }
            else{
                if(StringGrid2->Cells[i][j] == "WAIT"){
                    Timer1->Interval=(StrToInt((StringGrid2->Cells[i+1][j])*1000));
                    Timer1->Enabled=true;
                    i=0;
                    j++;
                }
                else{
                    if(StringGrid2->Cells[i][j] == "GRIP"){
                        garra=(StrToFloat(StringGrid2->Cells[i+1][j])<=100&&StrToFloat(StringGrid2->Cells[i+1][j])>=0)?
                                ((StrToFloat(StringGrid2->Cells[i+1][j]))/1.1764706+15):garra;
                        Timer1->Enabled=true;
                        i=0;
                        j++;
                    }
                    else{
                        if(StringGrid2->Cells[i][j] == "LOOP"){
                            Timer1->Enabled=true;
                            i=0;
                            j=0;
                        }
                        else{
                            if(StringGrid2->Cells[i][j] == "WRIST"){
                                roll = StrToFloat(StringGrid2->Cells[i+1][j]);
                                yaw = (StrToFloat(StringGrid2->Cells[i+2][j])<=45&&StrToFloat(StringGrid2->Cells[i+2][j])>=-45)?
                                        StrToFloat(StringGrid2->Cells[i+2][j]):yaw;
                                pitch = StrToFloat(StringGrid2->Cells[i+3][j]);
                                Timer1->Enabled=true;
                                i=0;
                                j++;
                            }
                        }
                    }
                }
            }
        }
    }
    catch (Exception &exception)
    {
            SpeedButton1Click(Sender);
            Application->ShowException(&exception);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton3Click(TObject *Sender)
{
    if(Timer1->Enabled==true)
        Timer1->Enabled=false;
    else
        Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton4Click(TObject *Sender)
{
    if (j>0){
        --j;
        Tbase1::SpeedButton2Click(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton5Click(TObject *Sender)
{
    if ((int)j<StringGrid2->RowCount){
        ++j;
        Tbase1::SpeedButton2Click(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton6Click(TObject *Sender)
{
    i=0;
    j=0;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TpicosdaAjuda1Click(TObject *Sender)
{
    ShellExecuteA(NULL, "open", "AJUDA.CHM", NULL, NULL, SW_SHOWNORMAL );
    //Application->HelpFile = "AJUDA.CHM";
    //Application->HelpCommand(HELP_CONTENTS, 0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar14Change(TObject *Sender)
{
    roll = TrackBar14->Position;
    Edit11->Text=IntToStr(TrackBar14->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar15Change(TObject *Sender)
{
    yaw = -TrackBar15->Position;
    Edit12->Text=IntToStr(TrackBar15->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar16Change(TObject *Sender)
{
    pitch = TrackBar16->Position;
    Edit13->Text=IntToStr(TrackBar16->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton7Click(TObject *Sender)
{
    zcam=zcam+0.3;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton8Click(TObject *Sender)
{
    zcam=zcam-0.3;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton9Click(TObject *Sender)
{
    xcam=xcam-0.3;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton10Click(TObject *Sender)
{
    xcam=xcam+0.3;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton11Click(TObject *Sender)
{
    xcam=0;
    ycam=0;
    zcam=0;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton14Click(TObject *Sender)
{
    Tbase1::TpicosdaAjuda1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton13Click(TObject *Sender)
{
    Tbase1::XoY1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton12Click(TObject *Sender)
{
    Tbase1::XoZ1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton11Click(TObject *Sender)
{
    Tbase1::YoZ1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton6Click(TObject *Sender)
{
    Tbase1::GradenoplanoZ1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton5Click(TObject *Sender)
{
    Tbase1::GradenoplanoY1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton4Click(TObject *Sender)
{
    Tbase1::GradenoplanoX1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton3Click(TObject *Sender)
{
    Tbase1::Eixos1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton2Click(TObject *Sender)
{
    Tbase1::Joystick1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton16Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton15Click(TObject *Sender)
{
    Tbase1::Reiniciar1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Joystick1Click(TObject *Sender)
{
    if(Timer2->Enabled==false){
        Timer1->Enabled=false;
        Timer2->Enabled=true;
        ToolButton2->Hint="Desativar joystick";
        ToolButton2->Down=true;
        Joystick1->Checked=true;
    }else{
        ToolButton2->Hint="Ativar joystick";
        Timer1->Enabled=true;
        Timer2->Enabled=false;
        ToolButton2->Down=false;
        Joystick1->Checked=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Eixos1Click(TObject *Sender)
{
    if(eixovisivel==false){
        eixovisivel=true;
        ToolButton3->Hint="Ocultar eixos";
        ToolButton3->Down=true;
        Eixos1->Checked=true;
    }else{
        eixovisivel=false;
        ToolButton3->Hint="Exibir eixos";
        ToolButton3->Down=false;
        Eixos1->Checked=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Polar1Click(TObject *Sender)
{
    Polar1->Checked=true;
    Cilindrico1->Checked=false;
    N3DOFSimples1->Checked=false;
    N6DOFPulsoEsfrico1->Checked=false;
    Cartesiano1->Checked=false;
    Cartesiano2->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;    
    Button2->Enabled=true;
    Button3->Enabled=true;
    Button4->Enabled=false;
    Button5->Enabled=false;
    Label4->Caption="Âng.Base";
    Label5->Caption="Âng.Ombro";
    Label6->Caption="    Radial";
    Label24->Caption="o";
    Label35->Caption="o";
    Label36->Caption="";

    L1=1.0;
    L2=1.5;
    L3=1.5;
    TrackBar8->Min=70;
    TrackBar9->Min=70;
    TrackBar10->Min=70;    
    TrackBar8->Position=100;
    TrackBar9->Position=150;
    TrackBar10->Position=150;
    TrackBar10->Enabled=true;

    base=0.0;
    Edit1->Text = "0";
    ombro=0.0;
    Edit2->Text = "0";
    cotovelo=0.0;
    Edit3->Text = "0";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=3;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Cilindrico1Click(TObject *Sender)
{
    Cilindrico1->Checked=true;
    Polar1->Checked=false;
    N3DOFSimples1->Checked=false;
    N6DOFPulsoEsfrico1->Checked=false;
    Cartesiano1->Checked=false;
    Cartesiano2->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;    
    Button2->Enabled=false;
    Button3->Enabled=true;
    Button4->Enabled=false;
    Button5->Enabled=false;
    Label4->Caption="Âng.Base";
    Label5->Caption="   Vertical";
    Label6->Caption="    Radial";
    Label24->Caption="o";
    Label35->Caption="";
    Label36->Caption="";

    L1=2.0;
    L2=2.0;
    L3=1.0;
    TrackBar8->Min=110;
    TrackBar9->Min=100;
    TrackBar10->Min=60;    
    TrackBar8->Position=200;
    TrackBar9->Position=200;
    TrackBar10->Position=100;
    TrackBar10->Enabled=false;

    base=0.0;
    Edit1->Text = "0";
    ombro=30.0;
    Edit2->Text = "30";
    cotovelo=60.0;
    Edit3->Text = "60";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=4;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Cartesiano1Click(TObject *Sender)
{
    Cartesiano1->Checked=true;
    Cartesiano2->Checked=false;
    Cilindrico1->Checked=false;
    Polar1->Checked=false;
    N3DOFSimples1->Checked=false;
    N6DOFPulsoEsfrico1->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;    
    Button2->Enabled=false;
    Button3->Enabled=true;
    Button4->Enabled=false;
    Button5->Enabled=false;
    Label4->Caption="       X";
    Label5->Caption="       Y";
    Label6->Caption="       Z";
    Label24->Caption="";
    Label35->Caption="";
    Label36->Caption="";

    L1=3.0;
    L2=2.0;
    L3=4.0;
    TrackBar8->Min=185;
    TrackBar9->Min=100;
    TrackBar10->Min=70;
    TrackBar8->Position=300;
    TrackBar9->Position=200;
    TrackBar10->Position=400;
    TrackBar10->Enabled=true;

    base=50.0;
    Edit1->Text = "50";
    ombro=0.0;
    Edit2->Text = "0";
    cotovelo=5.0;
    Edit3->Text = "5";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=5;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::Cartesiano2Click(TObject *Sender)
{
    Cartesiano2->Checked=true;
    Cartesiano1->Checked=false;
    Cilindrico1->Checked=false;
    Polar1->Checked=false;
    N3DOFSimples1->Checked=false;
    N6DOFPulsoEsfrico1->Checked=false;
    SCARA1->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;    
    Button2->Enabled=false;
    Button3->Enabled=true;
    Button4->Enabled=false;
    Button5->Enabled=false;
    Label4->Caption="       X";
    Label5->Caption="       Y";
    Label6->Caption="       Z";
    Label24->Caption="";
    Label35->Caption="";
    Label36->Caption="";

    L1=3.0;
    L2=2.0;
    L3=4.0;
    TrackBar8->Min=185;
    TrackBar9->Min=100;
    TrackBar10->Min=70;
    TrackBar8->Position=300;
    TrackBar9->Position=200;
    TrackBar10->Position=400;
    TrackBar10->Enabled=true;

    base=0.0;
    Edit1->Text = "0";
    ombro=0.0;
    Edit2->Text = "0";
    cotovelo=0.0;
    Edit3->Text = "0";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=7;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton12Click(TObject *Sender)
{
    Timer1->Enabled=false;
    ++StringGrid2->RowCount;
    ++StringGrid2->RowCount;
    StringGrid2->Cells[0][j]="DIR";
    StringGrid2->Cells[1][j]=FloatToStr(base);
    StringGrid2->Cells[2][j]=FloatToStr(ombro);
    StringGrid2->Cells[3][j]=FloatToStr(cotovelo);
    ++j;
    StringGrid2->Cells[0][j]="GRIP";
    StringGrid2->Cells[1][j]=FloatToStr(Arredonda((garra-15)*1.1764706, 2));
    ++j;
    Label25->Caption=IntToStr(StringGrid2->RowCount);
    if(StringGrid2->RowCount>3000){
        StatusBar1->Panels->Items[1]->Text="Número máximo de linhas de comando (3000).";
        SpeedButton1Click(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Movimentarcmeracommouse1Click(TObject *Sender)
{
    Movimentarrobcommouse1->Checked=false;
    Movimentarcmeracommouse1->Checked=true;
    ToolButton7->Down=true;
    ToolButton9->Down=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Movimentarrobcommouse1Click(TObject *Sender)
{
    Timer1->Enabled=false;
    Movimentarcmeracommouse1->Checked=false;
    Movimentarrobcommouse1->Checked=true;
    ToolButton7->Down=false;
    ToolButton9->Down=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton7Click(TObject *Sender)
{
    Tbase1::Movimentarcmeracommouse1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton9Click(TObject *Sender)
{
    Tbase1::Movimentarrobcommouse1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::SCARA1Click(TObject *Sender)
{
    SCARA1->Checked=true;
    Cilindrico1->Checked=false;
    Polar1->Checked=false;
    N3DOFSimples1->Checked=false;
    N6DOFPulsoEsfrico1->Checked=false;
    Cartesiano1->Checked=false;
    Cartesiano2->Checked=false;
    TrackBar14->Enabled=true;
    TrackBar15->Enabled=false;
    TrackBar16->Enabled=false;
    Edit11->Enabled=true;
    Edit12->Enabled=false;
    Edit13->Enabled=false;
    Button2->Enabled=true;
    Button3->Enabled=true;
    Button4->Enabled=false;
    Button5->Enabled=false;
    Label4->Caption="Âng.Base";
    Label5->Caption="   Vertical";
    Label6->Caption="Âng.Cotovelo";
    Label24->Caption="o";
    Label35->Caption="";
    Label36->Caption="o";

    L1=1.5;
    L2=1.5;
    L3=1.5;
    TrackBar8->Min=70;
    TrackBar9->Min=70;
    TrackBar10->Min=70;
    TrackBar8->Position=150;
    TrackBar9->Position=150;
    TrackBar10->Position=150;
    TrackBar10->Enabled=true;

    base=0.0;
    Edit1->Text = "0";
    ombro=40.0;
    Edit2->Text = "40";
    cotovelo=0.0;
    Edit3->Text = "0";

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add("DIR");
    ComboBox1->Items->Add("INV");
    ComboBox1->Items->Add("WAIT");
    ComboBox1->Items->Add("GRIP");
    ComboBox1->Items->Add("LOOP");

    tipo=6;
    Tbase1::BitBtn13Click(Sender);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::TrackBar1Change(TObject *Sender)
{
    n = TrackBar1->Position;
    nn=n;
    CompilaGrade(GradenoplanoX1->Checked, GradenoplanoY1->Checked, GradenoplanoZ1->Checked, n);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Ortogonal1Click(TObject *Sender)
{
    Ortogonal1->Checked=true;
    Perspectiva1->Checked=false;
    ToolButton22->Down=false;
    ToolButton23->Down=true;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.01*w,0.01*w,-0.01*h,0.01*h,0.5,50.0);
    glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::Perspectiva1Click(TObject *Sender)
{
    Ortogonal1->Checked=false;
    Perspectiva1->Checked=true;
    ToolButton22->Down=true;
    ToolButton23->Down=false;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = (GLfloat)w/(GLfloat)h;
    gluPerspective(46.0f, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton23Click(TObject *Sender)
{
    Tbase1::Ortogonal1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::ToolButton22Click(TObject *Sender)
{
    Tbase1::Perspectiva1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::BitBtn1Click(TObject *Sender)
{
    Edit1->Text = FloatToStr(StrToFloat(Edit1->Text)-1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn2Click(TObject *Sender)
{
    Edit1->Text = FloatToStr(StrToFloat(Edit1->Text)+1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn4Click(TObject *Sender)
{
    Edit2->Text = FloatToStr(StrToFloat(Edit2->Text)-1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn5Click(TObject *Sender)
{
    Edit2->Text = FloatToStr(StrToFloat(Edit2->Text)+1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn6Click(TObject *Sender)
{
    Edit3->Text = FloatToStr(StrToFloat(Edit3->Text)-1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn7Click(TObject *Sender)
{
    Edit3->Text = FloatToStr(StrToFloat(Edit3->Text)+1.0);
    Tbase1::Button1Click(Sender);
    Timer1->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::DetectaPortas(TObject *Sender)
{
        /////////// COLOCA PORTAS SERIAIS DISPONIVEIS NA COMBOBOX///////////
        TRegistry *Registro = new TRegistry;
        TStringList *Lista = new TStringList;
        Registro->RootKey = HKEY_LOCAL_MACHINE ;
        Registro->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);
        //Obtém uma string contendo todos os nomes de valores associados com a chave atual.
        Registro->GetValueNames(Lista);
        //Count é a quantidade de portas instaladas no sistema.
        for (int indice=0; indice <= Lista->Count-1; indice++)  //adiciona-as no ComboBox1.
                base1->ComboBox2->Items->Add(Registro->ReadString(Lista->Strings[indice]));
        if (base1->ComboBox2->Items->Count > 0)  //Se há ítens.
                base1->ComboBox2->ItemIndex = 1; //Exibir o primeiro nome da porta no ComboBox.
        Lista->Free(); //Libera memória.
        Registro->CloseKey(); //Fecha a chave.
        Registro->Free(); //Libera memória.
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::BitBtn8Click(TObject *Sender)
{
        threadloop = true;
        MyThread=new ThreadClass(False);
        //MyThread->Priority = tpTimeCritical;
        //SetPriorityClass(GetCurrentProcess, HIGH_PRIORITY_CLASS);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn9Click(TObject *Sender)
{
        threadloop = false;
}
//---------------------------------------------------------------------------


void __fastcall Tbase1::BitBtn10Click(TObject *Sender)
{
        TrackBar2->Position = 10;
        TrackBar3->Position = 10;
        TrackBar11->Position = 10;
        TrackBar12->Position = 10;
        TrackBar13->Position = 10;
        TrackBar17->Position = 10;
        TrackBar18->Position = 10;
        Label51->Caption = "1,0";
        Label52->Caption = "1,0";
        Label53->Caption = "1,0";
        Label54->Caption = "1,0";
        Label55->Caption = "1,0";
        Label56->Caption = "1,0";
        Label57->Caption = "1,0";
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn11Click(TObject *Sender)
{
        TrackBar19->Position = 0;
        TrackBar20->Position = 0;
        TrackBar21->Position = 0;
        TrackBar22->Position = 0;
        TrackBar23->Position = 0;
        TrackBar24->Position = 0;
        TrackBar25->Position = 0;
        Label58->Caption = "0";
        Label59->Caption = "0";
        Label60->Caption = "0";
        Label61->Caption = "0";
        Label62->Caption = "0";
        Label63->Caption = "0";
        Label64->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar2Change(TObject *Sender)
{
        Label51->Caption = FloatToStr(TrackBar2->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar3Change(TObject *Sender)
{
        Label52->Caption = FloatToStr(TrackBar3->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar11Change(TObject *Sender)
{
        Label53->Caption = FloatToStr(TrackBar11->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar12Change(TObject *Sender)
{
        Label54->Caption = FloatToStr(TrackBar12->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar13Change(TObject *Sender)
{
        Label55->Caption = FloatToStr(TrackBar13->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar17Change(TObject *Sender)
{
        Label56->Caption = FloatToStr(TrackBar17->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar18Change(TObject *Sender)
{
        Label57->Caption = FloatToStr(TrackBar18->Position/10.0);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar19Change(TObject *Sender)
{
        Label58->Caption = IntToStr(TrackBar19->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar20Change(TObject *Sender)
{
        Label59->Caption = IntToStr(TrackBar20->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar21Change(TObject *Sender)
{
        Label60->Caption = IntToStr(TrackBar21->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar22Change(TObject *Sender)
{
        Label61->Caption = IntToStr(TrackBar22->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar23Change(TObject *Sender)
{
        Label62->Caption = IntToStr(TrackBar23->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar24Change(TObject *Sender)
{
        Label63->Caption = IntToStr(TrackBar24->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::TrackBar25Change(TObject *Sender)
{
        Label64->Caption = IntToStr(TrackBar25->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::CheckBox1Click(TObject *Sender)
{
        Tbase1::Movimentarcmeracommouse1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall Tbase1::TrackBar26Change(TObject *Sender)
{
        Timer1->Interval = 1000-TrackBar26->Position;
        Label70->Caption = IntToStr((TrackBar26->Position-499)/5)+"%";
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    //PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::SpeedButton13Click(TObject *Sender)
{
    try
    {
        Timer1->Interval = 1000-TrackBar26->Position;
        if(StringGrid2->Cells[i][j] == "DIR"){
            Edit1->Text=StringGrid2->Cells[i+1][j];
            Edit2->Text=StringGrid2->Cells[i+2][j];
            Edit3->Text=StringGrid2->Cells[i+3][j];
            Tbase1::Button1Click(Sender);
            Timer1->Enabled=true;
            i=0;
            j++;
        }
        else{
            if(StringGrid2->Cells[i][j] == "INV"){
                Edit4->Text=StringGrid2->Cells[i+1][j];
                Edit5->Text=StringGrid2->Cells[i+2][j];
                Edit6->Text=StringGrid2->Cells[i+3][j];
                Tbase1::Button3Click(Sender);
                Timer1->Enabled=true;
                i=0;
                j++;
            }
            else{
                if(StringGrid2->Cells[i][j] == "WAIT"){
                    Timer1->Interval=(StrToInt((StringGrid2->Cells[i+1][j])*1000));
                    Timer1->Enabled=true;
                    i=0;
                    j++;
                }
                else{
                    if(StringGrid2->Cells[i][j] == "GRIP"){
                        garra=(StrToFloat(StringGrid2->Cells[i+1][j])<=100&&StrToFloat(StringGrid2->Cells[i+1][j])>=0)?
                                ((StrToFloat(StringGrid2->Cells[i+1][j]))/1.1764706+15):garra;
                        Timer1->Enabled=true;
                        i=0;
                        j++;
                    }
                    else{
                        if(StringGrid2->Cells[i][j] == "LOOP"){
                            Timer1->Enabled=true;
                            i=0;
                            j=0;
                        }
                        else{
                            if(StringGrid2->Cells[i][j] == "WRIST"){
                                roll = StrToFloat(StringGrid2->Cells[i+1][j]);
                                yaw = (StrToFloat(StringGrid2->Cells[i+2][j])<=45&&StrToFloat(StringGrid2->Cells[i+2][j])>=-45)?
                                        StrToFloat(StringGrid2->Cells[i+2][j]):yaw;
                                pitch = StrToFloat(StringGrid2->Cells[i+3][j]);
                                Timer1->Enabled=true;
                                i=0;
                                j++;
                            }
                        }
                    }
                }
            }
        }
    }
    catch (Exception &exception)
    {
        SpeedButton1Click(Sender);
        Application->ShowException(&exception);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::CheckBox2Click(TObject *Sender)
{
    Edit22->Enabled = CheckBox2->Checked;
    Edit23->Enabled = CheckBox2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::CheckBox3Click(TObject *Sender)
{
    Edit24->Enabled = CheckBox3->Checked;
    Edit25->Enabled = CheckBox3->Checked;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::CheckBox4Click(TObject *Sender)
{
    Edit26->Enabled = CheckBox4->Checked;
    Edit27->Enabled = CheckBox4->Checked;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::CheckBox5Click(TObject *Sender)
{
    Edit28->Enabled = CheckBox5->Checked;
    Edit29->Enabled = CheckBox5->Checked;
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn12Click(TObject *Sender)
{
    basemax = StrToFloat(Edit22->Text);
    basemin = StrToFloat(Edit23->Text);
    ombromax = StrToFloat(Edit24->Text);
    ombromin = StrToFloat(Edit25->Text);
    cotovelomax = StrToFloat(Edit26->Text);
    cotovelomin = StrToFloat(Edit27->Text);
    rollmax = StrToFloat(Edit28->Text);
    rollmin = StrToFloat(Edit29->Text);
}
//---------------------------------------------------------------------------

void __fastcall Tbase1::BitBtn13Click(TObject *Sender)
{
    switch(tipo)
    {
    case(1):
        //Case dos manipuladores Articulados
        basemax = 0; basemin = 0;
        ombromax = 200; ombromin = -20;
        cotovelomax = 120; cotovelomin = -120;
        CheckBox2->Checked = false;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(2):
        //TODO: Case do articulado de 6 GDF
        basemax = 0; basemin = 0;
        ombromax = 200; ombromin = -20;
        cotovelomax = 120; cotovelomin = -120;
        CheckBox2->Checked = false;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(3):
        //Case do manipulador Polar
        basemax = 0; basemin = 0;
        ombromax = 200; ombromin = -20;
        cotovelomax = 0; cotovelomin = -L3*100+40;
        CheckBox2->Checked = false;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(4):
        //Case do manipulador Cilindrico
        basemax = 0; basemin = 0;
        ombromax = L1*100-70; ombromin = 30;
        cotovelomax = L2*100-30; cotovelomin = 60;
        CheckBox2->Checked = false;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(5):
        //Case do manipulador Cartesiano
        basemax = L2*100-20; basemin = 50;
        ombromax = L3*50-30; ombromin = -L3*50+30;
        cotovelomax = L1*100-175; cotovelomin = 0;
        CheckBox2->Checked = true;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(6):
        //Case do manipulador SCARA
        basemax = 0; basemin = 0;
        ombromax = L1*100-30; ombromin = 30;
        cotovelomax = 120; cotovelomin = -120;
        CheckBox2->Checked = false;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    case(7):
        //Case do manipulador Cartesiano 2
        basemax = L2*50-50; basemin = -L2*50+50;
        ombromax = L3*50-50; ombromin = -L3*50+50;
        cotovelomax = L1*100-100; cotovelomin = 0;
        CheckBox2->Checked = true;
        CheckBox3->Checked = true;
        CheckBox4->Checked = true;
        CheckBox5->Checked = false;
        break;
    default:
        break;
    }
    Edit22->Text = FloatToStr(basemax);
    Edit23->Text = FloatToStr(basemin);
    Edit24->Text = FloatToStr(ombromax);
    Edit25->Text = FloatToStr(ombromin);
    Edit26->Text = FloatToStr(cotovelomax);
    Edit27->Text = FloatToStr(cotovelomin);
    Edit28->Text = FloatToStr(rollmax);
    Edit29->Text = FloatToStr(rollmin);
    Edit22->Enabled = CheckBox2->Checked;
    Edit23->Enabled = CheckBox2->Checked;
    Edit24->Enabled = CheckBox3->Checked;
    Edit25->Enabled = CheckBox3->Checked;
    Edit26->Enabled = CheckBox4->Checked;
    Edit27->Enabled = CheckBox4->Checked;
    Edit28->Enabled = CheckBox5->Checked;
    Edit29->Enabled = CheckBox5->Checked;
}
//---------------------------------------------------------------------------

