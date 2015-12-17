function a = esferico(L1, L2, L3, t1, t2, l3)
%ESFERICO  Retorna a matriz de transforma��o de um bra�o esf�rico
%           de tr�s graus de liberdade.
%   esferico(L1, L2, L3, t1, t2, l3) tem que L1, L2 e L3 s�o as dimens�es
%   dos elos e t1, t2 s�o os �ngulos de rota��o das juntas, e l3 � o raio.

%O primeiro par�metro do �ltimo termo deve ser o �ngulo de rota��o da garra
a = dh(t1, L1, 0, 90)*dh(t2, 0, L2+L3, -90)*dh(90, 0, 0, 90)*dh(0, l3, 0, 0);