data = ...
[


];

x=data(:,1);y=data(:,2);
p=fittype('gauss7')
f=fit(x,y,p)
plot(x,y, '.')
%plot(f,x,y);
xlabel('time(ms)','fontsize',26);
ylabel('deviation(10us)','fontsize',22);
set(gca,'FontName','Times New Roman','FontSize',26)