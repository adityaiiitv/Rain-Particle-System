# Rain-Particle-System

I have created a particle system which imitates rain and snow. The system has 2000 particles and I have used terminal velocity for downward movement of the rain-drops.

![Resulting images augmented in the reality](https://github.com/adityaiiitv/Rain-Particle-System/blob/master/Pictures/Screenshot%20(22).png)
Sample

I started with initializing 2000 particles. Initial mass, radii, velocity, positions were set. However, if one goes to the implementations present, they would just see either rain-drops falling with a certain velocity or under constant acceleration.
It is the physical nature of a rain-drop to fall with a constant velocity. There are two forces working on the rain-drop.
F1 = FGravity = Mass * Acceleration
F2 = FFriction = 6 * Pi * Viscosity * Radius * Terminal Velocity
After a certain amount of time, F2 completely counteracts F1 and the net force on the droplet is 0. At that instant, the drop is falling with a constant velocity which is observed in nature as 10 m/s. I calculated the Viscosity as well as tried using the online variants of it but my calculations were working better and showing better results.
The dead-end I faced was when I implemented the program with physical values of the real world. With all the real values the implementation was not possible. This may be because all the reference links had different values for any particular variable. These values were significantly different.

I have successfully created a particle system which simulates rain. When a particle hits the ground, it makes the ground wet. For snow, when the particle hits the ground, it makes a slight jump.
There are 2 modes. If you press T or t, particles move with a terminal velocity. If it is not pressed, then the speed of the particles can be adjusted using > for increasing and < for decreasing speed.
After a particle lives out its life, it is re-initiated to the top.

I followed these steps:
(1) Set up OpenGL variables.
(2) Initialize Velocity variables and the ground.
(3) Initialize all the particles.
(4) Check for inputs and draw the system.
(5) Re-initialize the particles when they hit the ground.

I was able to incorporate terminal velocity in the motion of rain which is missing in other implementations of particle systems. I have calculated values from separate references and made a working system. All previous implementations do not have this embedded in the motion.

