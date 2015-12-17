function a = cilindrico(L1, L2, L3, t1, l2, l3)
%CILINDRICO   Retorna a matriz de transforma��o de um bra�o cilindrico
%             de tr�s graus de liberdade.
%   cilindrico(L1, L2, L3, t1, l2, l3) tem que L1, L2 e L3 s�o as dimens�es
%   dos elos, t1 � o �ngulo de rota��o das junta, e l2 e l3 s�o prism�ticas.

%O primeiro par�metro do �ltimo termo deve ser o �ngulo de rota��o da garra
a = dh(t1, L1, 0, 0)*dh(90, 0, 0, 90)*dh(0, l2, 0, -90)*dh(0, -L1+l3, 0, 0);