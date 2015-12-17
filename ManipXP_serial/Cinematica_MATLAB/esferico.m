function a = esferico(L1, L2, L3, t1, t2, l3)
%ESFERICO  Retorna a matriz de transformação de um braço esférico
%           de três graus de liberdade.
%   esferico(L1, L2, L3, t1, t2, l3) tem que L1, L2 e L3 são as dimensões
%   dos elos e t1, t2 são os ângulos de rotação das juntas, e l3 é o raio.

%O primeiro parâmetro do último termo deve ser o ângulo de rotação da garra
a = dh(t1, L1, 0, 90)*dh(t2, 0, L2+L3, -90)*dh(90, 0, 0, 90)*dh(0, l3, 0, 0);