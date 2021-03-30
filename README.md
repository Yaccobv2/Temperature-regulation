# Temperature-regulation
Temperature regulation project using NUCLEO F746ZG.

In this project I used NUCLEO F746ZG,temperature sensor BMP280, small resistor with high power rating, computer fan 
, LCD display with i2c converter and two transistors IRL540NS.

Electrical schemes:
![image](https://user-images.githubusercontent.com/39679208/112981420-74a5b380-915b-11eb-8e4f-1686e54a5195.png)

BMP280 connection scheme:
![image](https://user-images.githubusercontent.com/39679208/112982073-53919280-915c-11eb-84cb-ad6856b7858c.png)

LCD connection scheme:
![image](https://user-images.githubusercontent.com/39679208/112982257-905d8980-915c-11eb-9da1-1c9f7b5c332f.png)

In this project I used CMSIS DSP Software Library for my PID contorlers implementation. I used two PID's for current contorl going through resistor and speed contorl of the fan.

Used pin list:
![image](https://user-images.githubusercontent.com/39679208/112985112-116a5000-9160-11eb-8084-f4f101eab2b2.png)

Results:
![image](https://user-images.githubusercontent.com/39679208/112985570-a0776800-9160-11eb-8114-48dd311de791.png)


