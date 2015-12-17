function x = dh(teta, d, a, alfa)
%DH   Matriz de transforma��o cinem�tica de Denavit-Hartenberg
%   dh(teta, d, a, alfa) cria uma matriz de transforma��o, sendo
%   que teta � o �ngulo de rota��o em torno do eixo zi, d � a
%   dist�ncia ao longo do eixo zi, a � a dist�ncia ao longo do
%   eixo x, e alfa � o �ngulo de rota��o em torno do eixo x.

alfa=alfa*pi/180; %passa de graus para radianos
teta=teta*pi/180; %passa de graus para radianos

x=[cos(teta) -cos(alfa)*sin(teta) sin(alfa)*sin(teta) a*cos(teta);
   sin(teta) cos(alfa)*cos(teta) -sin(alfa)*cos(teta) a*sin(teta);
   0 sin(alfa) cos(alfa) d;
   0 0 0 1];