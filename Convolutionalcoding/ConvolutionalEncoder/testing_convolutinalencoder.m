clc
clear all
close all
a=0;
b=1;

%uniform distribution between 0 and 1
data= a + (b-a).*rand(498000/2,1);
data=data';
for i=1:length(data)
     if(data(i)>0.5)
         data(i)=1;
     else
         data(i)=0;
     end
end
%% Encoder
% Define a convolutional coding trellis and use it
% to encode the binary data.
t = poly2trellis(3,[7 5]); % Trellis
code = convenc(data,t); % Encode.



encsig=zeros(1,2*length(data)); 
encsig=convo_encoder(data);

diff=encsig-code;
plot(diff)
