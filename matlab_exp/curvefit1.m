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

data = ...
[
139 15
131 11
168 3
123 3
160 10
152 14
144 18
136 15
128 17
165 3
157 17
149 16
141 22
133 13
125 12
170 3
162 2
154 21
146 16
138 18
130 10
167 3
159 19
151 17
143 13
135 14
127 17
164 2
156 14
148 14
140 9
132 15
169 1
124 5
161 5
153 10
145 12
137 13
129 9
166 2
158 13
150 13
142 17
134 15
126 8
171 1
163 3
155 16
147 17
];

x=data(:,1);y=data(:,2);
p=fittype('gauss2')
f=fit(x,y,p)
figure('color',[1 1 1])
plot(f,x,y,'*');
box off;
xlabel('传输时间(us)','fontsize',26);
ylabel('次数','fontsize',22);
set(gca,'FontName','Times New Roman','FontSize',26)