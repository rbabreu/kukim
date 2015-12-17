//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
          
void __fastcall TForm2::FormCreate(TObject *Sender)
{
  StringGrid1->Cells[0][0] = "D-H";
  StringGrid1->Cells[0][1] = "1";
  StringGrid1->Cells[0][2] = "2";
  StringGrid1->Cells[0][3] = "3";
  StringGrid1->Cells[1][0] = "teta i";
  StringGrid1->Cells[2][0] = "d i";
  StringGrid1->Cells[3][0] = "a i-1";
  StringGrid1->Cells[4][0] = "alfa i-1";
}
//---------------------------------------------------------------------------

