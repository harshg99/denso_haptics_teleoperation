#   Introduction
This repositiory provides the necessary drivers to collect end effector data from a 7-DOF Force Dimension Haptic Device for remote teleoperation of a Denso robotic arm. 
The necessary end effector torques on the robotic arm are converted to the reactive forces on the haptic device. There is a forward and inverse kinematics tool transcribed
to do this conversion in real-time.

The programming of the robot was done on Matlab Simulink using simple Inverse Kinematics and Dynamics.

Link to video of the teleoperation - (https://youtu.be/OW81gdqCgDk)

# Usage

Setup Haptic device and activate drivers. Compile and execute the Jointangles.cpp and set the input port and output port over local host at 34398 to communicate with the Denso robotic arm 
