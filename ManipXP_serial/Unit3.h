//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class ThreadClass : public TThread
{
private:
protected:
        void __fastcall Execute();
        void AbrirPorta();
        void ConfigPorta();
public:
        __fastcall ThreadClass(bool CreateSuspended);
        void __fastcall SerialExecute();

        DCB m_Dcb;
        HANDLE m_hCom;
        COMSTAT m_ComState;
        COMMTIMEOUTS m_CommTimeouts;
        int m_Tam_Buffer;// = 100; //Define o tamanho do buffer
        //char* m_BufferRecebe;// = new char[m_Tam_Buffer]; //Para armazenar a string a ser lida.
        //char* m_BufferEnvia;// = new char[m_Tam_Buffer]; //Para armazenar a string a ser enviada.
        char NomePorta[4];
        int v_Data;

        String v_DataStr;
        String v_Edit;

        float angulos[7];
        int ajuste[7];
        int valor_enviado;
        unsigned short valor_calculo;
        unsigned char ShortToBytes[2];
        int LS;
        int MS;
};
//---------------------------------------------------------------------------
#endif
