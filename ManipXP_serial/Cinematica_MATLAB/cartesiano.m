function a = cartesiano(L1, L2, L3, t1, t2, t3)
%CARTESIANO   Retorna a matriz de transformação de um braço articulado
%             de seis graus de liberdade.
%   cartesiano(L1, L2, L3, t1, t2, t3) tem que L1, L2 e L3 são as dimensões
%   dos elos e t1, t2 e t3 são as posições das juntas.

%O primeiro parâmetro do último termo deve ser o ângulo de rotação da garra
a = dh(0, 0, 0, -90)*dh(0, t2, 0, 90)*dh(90, t3+140, 0, 90)*dh(0, t1, 0, 90)*dh(0, 140, 0, 0);