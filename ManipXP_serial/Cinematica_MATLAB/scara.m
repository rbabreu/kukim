function a = scara(L1, L2, L3, t1, t2, l3)
%SCARA   Retorna a matriz de transformação de um braço scara
%        de três graus de liberdade.
%   scara(L1, L2, L3, t1, t2, l3) tem que L1, L2 e L3 são as dimensões
%   dos elos, t1 e t2 são os ângulos de rotação das juntas, e l3 é a altura.

%O primeiro parâmetro do último termo deve ser o ângulo de rotação da garra
a = dh(t1, L1, L2, 0)*dh(t2, 40, L3, 0)*dh(0, -L1+l3-40, 0, 0)*dh(0, 0, 0, 0);