%% Visualização Exata da FT
Ts = 0.5
s =tf('s');
g=((1)/(100*s^(3)+140*s^(2)+33*s+1));
zpk(g)
step(g)
gz = c2d(g,Ts,'method','matched')

%% Cálculos relevantes
fT =@(p1,p2) p1*p2/(s^2+ (p1+p2)*s+p1*p2);
Td = fT(0.05,0.5);
step(Td);
Tdz = c2d(Td,0.500,'method','matched')
Tdz = zpk(Tdz) % verificar se consigo achar isso na mão 
% 31 amostras para acomodar 
hold on 
grid on 
step(Tdz);

%% VRFT SIMULADO - Virtual Reference Feedback Tuning SIMULADO
EnsaioSimulado;  %chama o scrip que faz o ensaio simulado e etc
rho = zeros(3,1); %[kp; ki; kd] = vetor coluna do meu PID que quero descobrir
%% EXEMPLO DO CATA
%y = teste1.y';
%u = teste1.u';
t = (0:Ts:2430)';
tamanho = size(y);
t = t(1:tamanho(2));
z = zpk('z',Ts);
%Amostras para acomodação: 20 amostras  
%%
polo1 = 0.8752;
polo2 = 0.5134;
%polo3 = 0.9247;
%polo4 = 0.5;

zero1 = -0.5;
%zero2 = 0.65;
%zero3 = 0.45;

ganho = ((1-zero1)/((1-polo1)*(1-polo2)))^(-1);

Td = ganho*(z-zero1)/((z-polo1)*(z-polo2));
step(Td)
%%
Cbarra = [z/(z-1);1/(z-1);1/((z-1)*z)]; %PID

Filtro = minreal(Cbarra * (1-Td)/Td);

Filtroatrasado = minreal(Filtro/z);

Phi = lsim(Filtroatrasado,y,t);

u = u(1:end);
%t = t(2:end);

ro = (Phi'*Phi)^(-1)*(Phi'*u')

C = minreal(ro'*Cbarra);
[numg, deng] = tfdata(gz,'v');
[numC, denC] = tfdata(C,'v');
Kd = ro(3);
Ki = ro(1)+ro(2)+ro(3);
Kp= -(ro(2)+2*ro(3));
Kt = sqrt(Kd/Ki);
%%
aux = size(y);
ref = 5 * ones(1,aux(2));

%stem(y2);
%grid;
%Amostras para acomodação: 57

ye =lsim(Td,u);
%Amostras para acomodação: 52
J=0;
for c = 1:tamanho   
    J = J + (ye(c) - y(c))^2;
end



%% Cálculos relevantes
T =@(eps,omega) omega^2/(s^2+ 2*eps*omega*s+omega^2);
Td = T(0.7,0.076);
step(Td);
Tdz = c2d(Td,0.500,'method','matched')
Tdz = zpk(Tdz) % verificar se consigo achar isso na mão 
% 31 amostras para acomodar 
hold on 
grid on 
step(Tdz);

%% TDz 
z=tf('z',0.5)
polo1 = 0.9736858;
polo2 = 0.7659285;
%polo3 = 0.9247;
%polo4 = 0.5;

zero1 =  0.2;
%zero2 = 0.65;
%zero3 = 0.45;

ganho = ((1-zero1)/((1-polo1)*(1-polo2)))^(-1);

Tdz = ganho*(z-zero1)/((z-polo1)*(z-polo2));
step(Tdz)


Tdz 
[Tnum, Tden] = tfdata(Tdz,'v');
Tnum
Tden
%% L
L = tf(Tdz*(1-Tdz))
[lnum, lden] = tfdata(L,'v');
lnum
lden
zpk(L)
%%
C = (12.5*z^2 -26.12*z+13.64)/(z*(z-1))
[numC, denC] = tfdata(C,'v')
T = minreal(gz*C/(gz*C+1));
step(T)