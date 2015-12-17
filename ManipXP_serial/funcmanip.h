#ifndef __funcmanip_h
#define __funcmanip_h

float Resto(float);
float Arredonda(float, int);
float Distancia(float&, float&, float&, float&, float&, float&);
void Incremento(float, float, float, float &, float &, float &);

int Cinversa(float, float, float , float &, float &, float &, float, float , float, bool, bool, bool, float, float, float, float, float, float);
int C_inversa(float, float, float , float &, float &, float &, float, float , float, bool, bool, bool, float, float, float, float, float, float);
int Cdireta(float, float, float, float &, float &, float &, float, float , float, bool, bool, bool, float, float, float, float, float, float);
int CinversaPolar(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int C_inversaPolar(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int CdiretaPolar(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int CinversaCilindrico(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int CdiretaCilindrico(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int CinversaSCARA(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int C_inversaSCARA(float, float, float, float &, float &, float &, float, float, float, bool, bool, bool, float, float, float, float, float, float);
int CdiretaSCARA(float, float, float, float &, float &, float &, float,float, float, bool, bool, bool, float, float, float, float, float, float);
int CCartesiano(float, float, float, bool, bool, bool, float, float, float, float, float, float);

void Eixos(bool, float, float, float, float);
//void Texto(char *, float, float);
void CuboSolido(float, float, float, float);
void CuboSolidoTri();
void LabelXYZ(bool);
void DesenhaGrade(bool, bool, bool, int);

#endif

