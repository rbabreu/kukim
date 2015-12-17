function a = articulado(L1, L2, L3, t1, t2, t3)
%ARTICULADO   Retorna a matriz de transforma��o de um bra�o articulado
%             de tr�s graus de liberdade.
%   articulado(L1, L2, L3, t1, t2, t3) tem que L1, L2 e L3 s�o as dimens�es
%   dos elos e t1, t2 e t3 s�o os �ngulos de rota��o das juntas.

a = dh(t1, L1, 0, 90)*dh(t2, 0, L2, 0)*dh(t3, 0, L3, 0);