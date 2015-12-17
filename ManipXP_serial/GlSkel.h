//---------------------------------------------------------------------------
#ifndef GlSkelH 
#define GlSkelH
//--------------------------------------------------------------------------- 
#include <Classes.hpp> 
#include <Controls.hpp> 
#include <StdCtrls.hpp> 
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#include <OleCtrls.hpp>
#include <vcfi.hpp>
#include <ToolWin.hpp>
#include <MPlayer.hpp>
#include <ImgList.hpp>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glut.h>
#include "Unit1.h"
#include "Unit2.h"

//--------------------------------------------------------------------------- 
class Tbase1 : public TForm
{ 
__published:
        TTimer *Timer1;
        TMainMenu *MainMenu1;
        TMenuItem *Arquivo1;
        TMenuItem *Sair1;
        TMenuItem *Reiniciar1;
        TStatusBar *StatusBar1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TTabSheet *TabSheet2;
        TTrackBar *TrackBar4;
        TTrackBar *TrackBar5;
        TToolBar *ToolBar1;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TTrackBar *TrackBar6;
        TTrackBar *TrackBar7;
        TLabel *Label10;
        TMenuItem *Ajuda1;
        TMenuItem *Sobre1;
        TTabSheet *TabSheet3;
        TEdit *Edit7;
        TEdit *Edit8;
        TEdit *Edit9;
        TLabel *Label11;
        TTrackBar *TrackBar8;
        TTrackBar *TrackBar9;
        TTrackBar *TrackBar10;
        TBitBtn *BitBtn3;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TTabSheet *TabSheet4;
        TStringGrid *StringGrid2;
        TButton *Button10;
        TTimer *Timer2;
        TLabel *Label18;
        TButton *Button15;
        TComboBox *ComboBox1;
        TButton *Button16;
        TMenuItem *Configurao1;
        TMenuItem *N3DOFSimples1;
        TMenuItem *N6DOFPulsoEsfrico1;
        TMenuItem *TpicosdaAjuda1;
        TMenuItem *Visualizao1;
        TMenuItem *XoY1;
        TMenuItem *XoZ1;
        TMenuItem *YoZ1;
        TMenuItem *GradenoplanoX1;
        TMenuItem *GradenoplanoY1;
        TMenuItem *GradenoplanoZ1;
        TMenuItem *Exibir1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton6;
        TTrackBar *TrackBar14;
        TTrackBar *TrackBar15;
        TTrackBar *TrackBar16;
        TLabel *Label22;
        TGroupBox *GroupBox3;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton8;
        TSpeedButton *SpeedButton9;
        TSpeedButton *SpeedButton10;
        TLabel *Label23;
        TSpeedButton *SpeedButton11;
        TToolButton *ToolButton2;
        TImageList *ImageList1;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton11;
        TToolButton *ToolButton12;
        TToolButton *ToolButton13;
        TToolButton *ToolButton14;
        TToolButton *ToolButton15;
        TToolButton *ToolButton16;
        TToolButton *ToolButton8;
        TToolButton *ToolButton17;
        TToolButton *ToolButton18;
        TToolButton *ToolButton19;
        TToolButton *ToolButton20;
        TMenuItem *Joystick1;
        TMenuItem *Eixos1;
        TToolButton *ToolButton1;
        TMenuItem *Polar1;
        TMenuItem *Cilindrico1;
        TMenuItem *Cartesiano1;
        TSpeedButton *SpeedButton12;
        TLabel *Label25;
        TLabel *Label26;
        TMenuItem *Opes1;
        TMenuItem *Movimentarcmeracommouse1;
        TMenuItem *Movimentarrobcommouse1;
        TToolButton *ToolButton7;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TGroupBox *GroupBox4;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *Edit10;
        TEdit *Edit11;
        TEdit *Edit12;
        TEdit *Edit13;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TMenuItem *SCARA1;
        TTrackBar *TrackBar1;
        TMenuItem *Ortogonal1;
        TMenuItem *Perspectiva1;
        TToolButton *ToolButton22;
        TToolButton *ToolButton23;
        TToolButton *ToolButton24;
        TLabel *Label24;
        TLabel *Label35;
        TLabel *Label36;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn4;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn6;
        TBitBtn *BitBtn7;
        TTabSheet *TabSheet5;
        TMenuItem *Cartesiano2;
        TComboBox *ComboBox2;
        TBitBtn *BitBtn8;
        TBitBtn *BitBtn9;
        TLabel *Label37;
        TLabel *Label38;
        TEdit *Edit14;
        TComboBox *ComboBox3;
        TLabel *Label39;
        TGroupBox *GroupBox5;
        TTrackBar *TrackBar2;
        TTrackBar *TrackBar3;
        TTrackBar *TrackBar11;
        TTrackBar *TrackBar12;
        TTrackBar *TrackBar13;
        TTrackBar *TrackBar17;
        TTrackBar *TrackBar18;
        TTrackBar *TrackBar19;
        TTrackBar *TrackBar20;
        TTrackBar *TrackBar21;
        TTrackBar *TrackBar22;
        TTrackBar *TrackBar23;
        TTrackBar *TrackBar24;
        TTrackBar *TrackBar25;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *Label43;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label60;
        TLabel *Label61;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TBitBtn *BitBtn10;
        TBitBtn *BitBtn11;
        TCheckBox *CheckBox1;
    TGroupBox *GroupBox6;
    TLabel *Label17;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label12;
    TLabel *Label13;
    TGroupBox *GroupBox7;
    TEdit *Edit22;
    TEdit *Edit23;
    TLabel *Label21;
    TLabel *Label49;
    TEdit *Edit24;
    TEdit *Edit25;
    TEdit *Edit26;
    TEdit *Edit27;
    TEdit *Edit28;
    TEdit *Edit29;
    TBitBtn *BitBtn12;
    TTrackBar *TrackBar26;
    TGroupBox *GroupBox8;
    TLabel *Label34;
    TLabel *Label69;
    TLabel *Label70;
    TSpeedButton *SpeedButton13;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    TCheckBox *CheckBox4;
    TCheckBox *CheckBox5;
    TBitBtn *BitBtn13;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Sair1Click(TObject *Sender);
        void __fastcall Reiniciar1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall TrackBar4Change(TObject *Sender);
        void __fastcall TrackBar5Change(TObject *Sender);
        void __fastcall TrackBar6Change(TObject *Sender);
        void __fastcall TrackBar7Change(TObject *Sender);
        void __fastcall Sobre1Click(TObject *Sender);
        void __fastcall TrackBar8Change(TObject *Sender);
        void __fastcall TrackBar9Change(TObject *Sender);
        void __fastcall TrackBar10Change(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
        void __fastcall PageControl1Change(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall MouseAcao(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall XoY1Click(TObject *Sender);
        void __fastcall XoZ1Click(TObject *Sender);
        void __fastcall YoZ1Click(TObject *Sender);
        void __fastcall N6DOFPulsoEsferico1Click(TObject *Sender);
        void __fastcall N3DOFSimples1Click(TObject *Sender);
        void __fastcall GradenoplanoX1Click(TObject *Sender);
        void __fastcall GradenoplanoY1Click(TObject *Sender);
        void __fastcall GradenoplanoZ1Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall SpeedButton4Click(TObject *Sender);
        void __fastcall SpeedButton5Click(TObject *Sender);
        void __fastcall SpeedButton6Click(TObject *Sender);
        void __fastcall TpicosdaAjuda1Click(TObject *Sender);
        void __fastcall TrackBar14Change(TObject *Sender);
        void __fastcall TrackBar15Change(TObject *Sender);
        void __fastcall TrackBar16Change(TObject *Sender);
        void __fastcall SpeedButton7Click(TObject *Sender);
        void __fastcall SpeedButton8Click(TObject *Sender);
        void __fastcall SpeedButton9Click(TObject *Sender);
        void __fastcall SpeedButton10Click(TObject *Sender);
        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall ToolButton14Click(TObject *Sender);
        void __fastcall ToolButton13Click(TObject *Sender);
        void __fastcall ToolButton12Click(TObject *Sender);
        void __fastcall ToolButton11Click(TObject *Sender);
        void __fastcall ToolButton6Click(TObject *Sender);
        void __fastcall ToolButton5Click(TObject *Sender);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall ToolButton16Click(TObject *Sender);
        void __fastcall ToolButton15Click(TObject *Sender);
        void __fastcall Joystick1Click(TObject *Sender);
        void __fastcall Eixos1Click(TObject *Sender);
        void __fastcall Polar1Click(TObject *Sender);
        void __fastcall Cilindrico1Click(TObject *Sender);
        void __fastcall Cartesiano1Click(TObject *Sender);
        void __fastcall SpeedButton12Click(TObject *Sender);
        void __fastcall Movimentarcmeracommouse1Click(TObject *Sender);
        void __fastcall Movimentarrobcommouse1Click(TObject *Sender);
        void __fastcall ToolButton7Click(TObject *Sender);
        void __fastcall ToolButton9Click(TObject *Sender);
        void __fastcall MouseClique(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall SCARA1Click(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall Ortogonal1Click(TObject *Sender);
        void __fastcall Perspectiva1Click(TObject *Sender);
        void __fastcall ToolButton23Click(TObject *Sender);
        void __fastcall ToolButton22Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall BitBtn6Click(TObject *Sender);
        void __fastcall BitBtn7Click(TObject *Sender);
        void __fastcall Cartesiano2Click(TObject *Sender);
        void __fastcall DetectaPortas(TObject *Sender);
        void __fastcall BitBtn8Click(TObject *Sender);
        void __fastcall BitBtn9Click(TObject *Sender);
        void __fastcall BitBtn10Click(TObject *Sender);
        void __fastcall BitBtn11Click(TObject *Sender);
        void __fastcall TrackBar2Change(TObject *Sender);
        void __fastcall TrackBar3Change(TObject *Sender);
        void __fastcall TrackBar11Change(TObject *Sender);
        void __fastcall TrackBar12Change(TObject *Sender);
        void __fastcall TrackBar13Change(TObject *Sender);
        void __fastcall TrackBar17Change(TObject *Sender);
        void __fastcall TrackBar18Change(TObject *Sender);
        void __fastcall TrackBar19Change(TObject *Sender);
        void __fastcall TrackBar20Change(TObject *Sender);
        void __fastcall TrackBar21Change(TObject *Sender);
        void __fastcall TrackBar22Change(TObject *Sender);
        void __fastcall TrackBar23Change(TObject *Sender);
        void __fastcall TrackBar24Change(TObject *Sender);
        void __fastcall TrackBar25Change(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
    void __fastcall TrackBar26Change(TObject *Sender);
    void __fastcall MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton13Click(TObject *Sender);
    void __fastcall CheckBox2Click(TObject *Sender);
    void __fastcall CheckBox3Click(TObject *Sender);
    void __fastcall CheckBox4Click(TObject *Sender);
    void __fastcall CheckBox5Click(TObject *Sender);
    void __fastcall BitBtn12Click(TObject *Sender);
    void __fastcall BitBtn13Click(TObject *Sender);
private:
        HDC hdc;
        HGLRC hrc;
        float w,h;
        int PixelFormat;

public:
        __fastcall Tbase1(TComponent* Owner);
        void __fastcall IdleLoop(TObject*, bool&);
        void __fastcall RenderGLScene();
        void __fastcall SetPixelFormatDescriptor();
        void __fastcall Articulado1();
        void __fastcall Articulado2();
        void __fastcall Cilindrico();
        void __fastcall Polar();
        void __fastcall Cartesiano();
        void __fastcall Cartesiano_2();
        void __fastcall SCARA();
        void __fastcall SetupLighting();
        void __fastcall CompilaCuboSolido();
        void __fastcall CompilaGrade(bool , bool , bool , int );
        void __fastcall Joystick();
        void __fastcall GetOGLPos(int, int, double &, double &, double &);

        float editbase, editombro, editcotovelo,
              baseinc, ombroinc, cotoveloinc,
              baseant, ombroant, cotoveloant,
              rollant, yawant, pitchant;
        bool threadloop;
        float ombro, cotovelo, base, roll, pitch, yaw, garra, x, y, z,
              basemin, basemax, ombromin, ombromax,
              cotovelomin, cotovelomax, rollmin, rollmax;
};
//---------------------------------------------------------------------------
extern PACKAGE Tbase1 *base1;
//---------------------------------------------------------------------------
#endif
