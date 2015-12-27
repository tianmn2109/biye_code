% x=[1;1.5;2;2.5;3];y=[0.9;1.7;2.2;2.6;3];
% EXPR = {'x','sin(x)','1'};
% p=fittype(EXPR)
% f=fit(x,y,p)
% plot(f,x,y);
% 
% x=[1;1.5;2;2.5;3];y=[0.9;1.7;2.2;2.6;3];
% p=fittype('a*x+b*sin(x)+c','independent','x')
% f=fit(x,y,p)
% plot(f,x,y);

% data = ...
% [
% 52 15
% 53 64494
% 54 34807
% 55 41
% 56 89
% 57 256
% 58 268
% 59 30
% ];

data=load('expData');

x=data(:,1);y=data(:,2);
plot(x,y,'.')
% p=fittype('poly9')
% f=fit(x,y,p)
% plot(f,x,y);