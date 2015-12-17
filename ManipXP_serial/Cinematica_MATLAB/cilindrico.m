function a = cilindrico(L1, L2, L3, t1, l2, l3)
%CILINDRICO   Retorna a matriz de transformação de um braço cilindrico
%             de três graus de liberdade.
%   cilindrico(L1, L2, L3, t1, l2, l3) tem que L1, L2 e L3 são as dimensões
%   dos elos, t1 é o ângulo de rotação das junta, e l2 e l3 são prismáticas.

%O primeiro parâmetro do último termo deve ser o ângulo de rotação da garra
a = dh(t1, L1, 0, 0)*dh(90, 0, 0, 90)*dh(0, l2, 0, -90)*dh(0, -L1+l3, 0, 0);