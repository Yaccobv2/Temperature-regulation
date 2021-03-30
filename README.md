# Temperature-regulation
Temperature regulation project using NUCLEO F746ZG.

In this project I used NUCLEO F746ZG,temperature sensor BMP280, small resistor with high power rating, computer fan 
, LCD display with i2c converter and two transistors IRL540NS.

Contorl system was created with CMSIS DSP Software Library. I used two PID's for current contorl going through resistor and speed contorl of the fan.

Project picture:  
<img src="https://user-images.githubusercontent.com/39679208/112987773-5c399700-9163-11eb-9eed-c5470a601dc4.png"  width="80%" height="80%">

Electrical schemes:  
<img src="https://user-images.githubusercontent.com/39679208/112981420-74a5b380-915b-11eb-8e4f-1686e54a5195.png"  width="80%" height="80%">


BMP280 connection scheme:  
<img src="https://user-images.githubusercontent.com/39679208/112982073-53919280-915c-11eb-84cb-ad6856b7858c.png"  width="40%" height="40%">


LCD connection scheme:  
<img src="https://user-images.githubusercontent.com/39679208/112982257-905d8980-915c-11eb-9da1-1c9f7b5c332f.png"  width="80%" height="80%">


Used pin list:  
<img src="https://user-images.githubusercontent.com/39679208/112985112-116a5000-9160-11eb-8084-f4f101eab2b2.png"  width="80%" height="80%">


Results:  
<img src="https://user-images.githubusercontent.com/39679208/112987484-1250b100-9163-11eb-9a6e-f5bc151e252d.png"  width="80%" height="80%">




