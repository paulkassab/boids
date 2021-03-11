# boids

This is an engine/simulation that applies basic physics, raycasting, and collision detection to entities in a 2D environment.
The goal is to code rules into the simulation with the aim of obtaining some sort of swarm movement patterns from individual entity rules.
Coded in C++ using SFML graphics library.

+This is the Visual Studio 2017 project. To run, double-click on Boids/Boids/Boids.exe.

-Right-click to create new entities

-Left-click on entity to switch control of entity from AI to user or from user to AI (rays == AI controlled, no rays == user controlled)

-Buttons on the screen allow to control some parameters (physics, friction, collision). 

 Newly spawned entities have no acceleration physics, no collision and no friction by default. Toggle buttons twice to ensure all entities have same parameters.
 
-Pausing allows for placement of entities in specific positions.

-AWSD keys to move user-controlled entities

-EntitiesAI will try to follow other entities detected through raycasting

