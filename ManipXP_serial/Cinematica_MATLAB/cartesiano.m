function a = cartesiano(L1, L2, L3, t1, t2, t3)
%CARTESIANO   Retorna a matriz de transforma��o de um bra�o articulado
%             de seis graus de liberdade.
%   cartesiano(L1, L2, L3, t1, t2, t3) tem que L1, L2 e L3 s�o as dimens�es
%   dos elos e t1, t2 e t3 s�o as posi��es das juntas.

%O primeiro par�metro do �ltimo termo deve ser o �ngulo de rota��o da garra
a = dh(0, 0, 0, -90)*dh(0, t2, 0, 90)*dh(90, t3+140, 0, 90)*dh(0, t1, 0, 90)*dh(0, 140, 0, 0);