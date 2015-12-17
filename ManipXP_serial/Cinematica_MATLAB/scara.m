function a = scara(L1, L2, L3, t1, t2, l3)
%SCARA   Retorna a matriz de transforma��o de um bra�o scara
%        de tr�s graus de liberdade.
%   scara(L1, L2, L3, t1, t2, l3) tem que L1, L2 e L3 s�o as dimens�es
%   dos elos, t1 e t2 s�o os �ngulos de rota��o das juntas, e l3 � a altura.

%O primeiro par�metro do �ltimo termo deve ser o �ngulo de rota��o da garra
a = dh(t1, L1, L2, 0)*dh(t2, 40, L3, 0)*dh(0, -L1+l3-40, 0, 0)*dh(0, 0, 0, 0);