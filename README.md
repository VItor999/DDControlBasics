# Basic Data Driven Control Project

## Description
This repository was created to organize the final project developed for the ENG10019 - SISTEMAS DE CONTROLE DIGITAIS discipline (UFRGS - 2021/2). The project was proposed by the professor Alexandre Sanfelici Bazanela and solved by the students Eduardo Brezolin and Vitor Carvalho.

The goal of the project is to tune a controller using the Virtual Reference Feedback Tuning (VRFT) method. The system that will be controlled consist of three RC circuits in parallel. The output variable is the tension of the last capacitor and the input variable is the tension applied to the circuit. In order to implement the controller and collect the data from the circuit an Arduino was used. The Figure 1 shows the circuit that was considered as the system. The Figure 2 shows the physical circuit that was implemented to collect the data and act over the system.

![1. Process that will be controlled](/imgs/circuitoPlanta.png)
<p align = "center">
Fig.1 - Process to be controlled.
</p>

![2. Physical implementation of the circuit](/imgs/circuitoReal.png)
<p align = "center">
Fig.2 - Physical implementation of the circuit.
</p>

## Requirements:

- Matlab/Simulink 2018a +;
- Jupyter Notebook;
- Hardware Requirements (avaiable at *TRABALHO_CTRL_DIGITAL.pdf* and only needed in order to implement the experiment physically).

## Results 
In this scope a brief detail of the results will be presented. The Figure 3, bellow, show the system response in open loop. Using a sampling time of 500mS the settling time was 109 seconds or 218 samples. The major goal of the project was to make the closed loop system faster and maintain the stability. The theoretical settling time aimed was 120 samples.  As shown in Figure 4, the settling time that was possible to achieve was 141 samples. 

![3.System in Open loop](/imgs/MASALTOREAL.png)
<p align = "center">
Fig.3 - Open loop step response.
</p>

![4.System in closed loop](/imgs/MAMFcomAntiAjustado.png)
<p align = "center">
Fig.4 - Closed loop step response.
</p>

This was 15% highier than projected. This result can be explained by the fact that the system saturated and was needed to use a anti-windup loop for the integral action. In order to achieve better results a better hardware (power supply specially) should be used. Despite this diference of the results, the main goal of the project was achived. The controller developed made the system stable in 35% less time, which is a great result.

## Other Considerations 

In order to visualize the data, a simple GUI was developed in python. This implementation was based on the tutorial avaible at https://www.jcbrolabs.org/python-codes . There is a lot of room for improvement (implementing blit and so on). Despite this minor issues it is possible to visualize in real time the control signals and the evolution of the system. The Figure 5 shows a screenshot of the GUI developed.

![5.GUI for visualizing the signals](/imgs/Software%20Auxiliar.png)
<p align = "center">
Fig.5 GUI for visualizing the signals.
</p>
