//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "GlSkel.h"
#include "math.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ThreadClass::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ThreadClass::ThreadClass(bool CreateSuspended)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void ThreadClass::AbrirPorta()
{
    m_hCom = CreateFile(NomePorta,
                     GENERIC_READ | GENERIC_WRITE,
                     0,
                     NULL,
                     OPEN_EXISTING,
                     NULL,
                     NULL);
    if(m_hCom == INVALID_HANDLE_VALUE) {
        MessageBox(0, "Porta serial não foi aberta!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }
}
//---------------------------------------------------------------------------
void ThreadClass::ConfigPorta()
{
    ///////////////// DEFINE ESTADO DA PORTA /////////////////
    if (!GetCommState(m_hCom, &m_Dcb)){
        MessageBox(0, "Erro ao ler estado da porta!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }
    m_Dcb.BaudRate = CBR_19200;
    m_Dcb.ByteSize = 8;
    m_Dcb.Parity = NOPARITY;
    m_Dcb.StopBits = ONESTOPBIT;
    if (SetCommState(m_hCom, &m_Dcb) == 0){
        MessageBox(0, "Erro ao definir estado da porta!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }

    ///////////////// DEFINE TIMEOUTS DA PORTA ///////////////
    if ( GetCommTimeouts ( m_hCom , &m_CommTimeouts ) == 0){
        MessageBox(0, "Erro ao ler Timeouts", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }

    m_CommTimeouts.ReadIntervalTimeout = 2;
    m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    m_CommTimeouts.ReadTotalTimeoutConstant = 2;
    m_CommTimeouts.WriteTotalTimeoutMultiplier = 5;
    m_CommTimeouts.WriteTotalTimeoutConstant = 5;
    if (SetCommTimeouts(m_hCom, &m_CommTimeouts) == 0){
        MessageBox(0, "Erro ao definir Timeouts", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }
}
//---------------------------------------------------------------------------
void __fastcall ThreadClass::Execute()
{
    m_Tam_Buffer = 4; //Define o tamanho do buffer
    char* m_BufferRecebe = new char[m_Tam_Buffer]; //Para armazenar a string a ser lida.
    char* m_BufferEnvia = new char[m_Tam_Buffer]; //Para armazenar a string a ser enviada.
    int v_TamaString; //Para armazenar o tamanho da String.
    valor_enviado = 1;
    LS = 0;
    MS = 0;

    strcpy(NomePorta, base1->ComboBox2->Text.c_str());
    AbrirPorta();
    ConfigPorta();
    while(base1->threadloop){

        if(!base1->CheckBox1->Checked){ //Envia angulos por RS232
            angulos[0] = base1->base * base1->TrackBar2->Position/10.0 + base1->TrackBar19->Position;
            angulos[1] = base1->ombro * base1->TrackBar3->Position/10.0  + base1->TrackBar20->Position;
            angulos[2] = base1->cotovelo * base1->TrackBar11->Position/10.0  + base1->TrackBar21->Position;
            angulos[3] = base1->roll * base1->TrackBar12->Position/10.0  + base1->TrackBar22->Position;
            angulos[4] = base1->pitch * base1->TrackBar13->Position/10.0  + base1->TrackBar23->Position;
            angulos[5] = base1->yaw * base1->TrackBar17->Position/10.0  + base1->TrackBar24->Position;
            angulos[6] = base1->garra * base1->TrackBar18->Position/10.0  + base1->TrackBar25->Position;

            for(int i = 0; i < m_Tam_Buffer; ++i){ //Limpa buffer
                m_BufferRecebe[i] = 0;
                m_BufferEnvia[i] = 0;
            }
            DWORD v_nBytesLidos = 0;
            ReadFile(m_hCom, m_BufferRecebe, m_Tam_Buffer, &v_nBytesLidos, NULL);
            v_Data = (unsigned char)m_BufferRecebe[0];
            if(v_Data>0 && v_Data<8){
                DWORD v_nBytesEscritos = 0;
                //SOLUÇÂO 1: Transmite numero para PIC calcular tempo alto
                valor_calculo = (angulos[v_Data-1])+100;
                valor_enviado = (valor_calculo>=0&&valor_calculo<=255)?valor_calculo:100;
                *m_BufferEnvia = valor_enviado;
                int v_TamaString = strlen(m_BufferEnvia); //Calcula o tamanho da string a ser enviada.
                WriteFile(m_hCom, m_BufferEnvia, v_TamaString, &v_nBytesEscritos, NULL);
            }
        }
        else{ //Recebe angulos por RS232
            Synchronize(SerialExecute);

            for(int i = 0; i < m_Tam_Buffer; ++i){ //Limpa buffer
                m_BufferRecebe[i] = 0;
                m_BufferEnvia[i] = 0;
            }
            DWORD v_nBytesLidos = 0;
            ReadFile(m_hCom, m_BufferRecebe, m_Tam_Buffer, &v_nBytesLidos, NULL);
            valor_enviado = (unsigned char)m_BufferRecebe[0];
            v_Data = (unsigned char)m_BufferRecebe[1];
            if(v_Data!=0){
                angulos[valor_enviado-1] = (short)v_Data - 121;
            }
            DWORD v_nBytesEscritos = 0;
            valor_enviado = 100;
            *m_BufferEnvia = valor_enviado;
            int v_TamaString = strlen(m_BufferEnvia); //Calcula o tamanho da string a ser enviada.
            WriteFile(m_hCom, m_BufferEnvia, v_TamaString, &v_nBytesEscritos, NULL);
        }
    }
    CloseHandle(m_hCom);
    delete [] m_BufferRecebe;
    delete [] m_BufferEnvia;
}
//---------------------------------------------------------------------------
void __fastcall ThreadClass::SerialExecute()
{
    base1->base = angulos[0];
    base1->ombro = angulos[1];
    base1->cotovelo = angulos[2];
    base1->roll = angulos[3];
    base1->pitch = angulos[4];
    base1->yaw = angulos[5];
    base1->garra = angulos[6];
}
//---------------------------------------------------------------------------

