<h1 align="center"> GPU-VPM </h1>
<p align="center">
A GPU-based aerodynamics solver
</p>

<p align="center">
<img src="images/particle_field.png" width=500>
</p>

## Description
GPU-VPM is a GPU-based implementation of the Vortex Particle Method (VPM) which simulates airflow in aerodynamics. Positioned between low-fidelity, fast simulations and high-fidelity, resource-intensive CFD methods, this medium-fidelity approach enables realistic aerodynamic modeling without requiring heavy computational resources. 
GPU-VPM is implemented with CUDA, a parallel computing platform and API created by NVIDIA which allows software to use GPUs for faster processing. Our implementation focuses on high-performance, parallelized vortex particle calculations which make it suitable for early-stage design of aerial vehicles.

## Build Instructions
1. Clone this repository to your computer.
2. Build the project using CMake. We recommend using CMake version 3.30.3. 

    <img src="images/cmake.png" width=700>

3. Open the project using Visual Studio. We recommend **Visual Studio 2019**. 
4. Set the build mode to "Release" to run the project optimally.

    <img src="images/release.png" width=700>

## Methodology
<img src="images/[INSERT MODULAR DIAGRAM HERE].png" width=700>



## Implementation
We decided to implement this as a CUDA C++ project.
The VPM solver (which is used to simulate dynamic particles) runs on the GPU.
    
## Performance Analysis
Validation was performed via a comparison of the runtime of the vortex ring simulation using our GPU-based implementation versus the original CPU-based FLOWVPM implementation.

Our GPU based implementation was run on a laptop with the following specs:
* **MACHINE 1:** ASUS ROG Zephyrus M16
    * **OS:** Windows 11 
    * **Processor:** 12th Gen Intel(R) Core(TM) i9-12900H, 2500 Mhz, 14 Core(s), 20 Logical Processor(s)     
    * **GPU:** NVIDIA GeForce RTX 3070 Ti Laptop GPU 

The original, CPU-based FLOWVPM implementation was run on a desktop PC with the following specs:
* **MACHINE 2:** CETS lab computer
    * **OS:** Windows 10
    * **Processor:** i7-12700 @ 2.10 GHz, 32 GB, 
    * **GPU:** T1000 4096 MB

## GPU-VPM Vortex Ring Simulation Performance
The following graph shows the performance of the vortex ring simulation using our GPU-based implementation. The simulation was run with the following parameters:

+--------------+----------------------+
| Parameter    | Value                |
+==============+======================+
| maxParticles | 2147483647 (INT_MAX) |
+--------------+----------------------+
| numTimeSteps | 2000                 |
+--------------+----------------------+
| blockSize    | 128                  |
+--------------+----------------------+

<img src="images/graph1.png" width=1000>

## Bloopers
<center>
<img src="images/particle_field.png" width=500>

<i>Randomly initialized points in a particle field. Our first sign of success! :)
</i>
</center>
<center>
<h2> 
Meet the Team
</h2>
</center>

+-----------------------------------------------------+--------------------------------------------------+-----------------------------------------------------------+
| <img src="images/dominik.png" width=200>            | <img src="images/nadine.png" width=200>          | <img src="images/shreyas.png" width=200>                  |
+=====================================================+==================================================+===========================================================+
| Dominik Kau                                         | Nadine Adnane                                    | Shreyas Singh                                             |
+-----------------------------------------------------+--------------------------------------------------+-----------------------------------------------------------+
| [dominikk@seas.upenn.edu](dominikk@seas.upenn.edu)  | [nadnane@seas.upenn.edu](nadnane@seas.upenn.edu) | [shreyas4@seas.upenn.edu](shreyas4@seas.upenn.edu)        |
+-----------------------------------------------------+--------------------------------------------------+-----------------------------------------------------------+
| [LinkedIn](https://www.linkedin.com/in/dominikkau/) | [LinkedIn](https://www.linkedin.com/in/nadnane/) | [LinkedIn](https://www.linkedin.com/in/shreyassinghiitr/) |
+-----------------------------------------------------+--------------------------------------------------+-----------------------------------------------------------+

<h2 align="center">
Special Thanks
</h2>

<p align="center">
<img src="images/ed.png" width = 200>
</p>

<p align="center">
<i> Special thanks to Ed Alvarez, without whom this project would not exist! :) </i>
</p>

## References
* [FLOWUnsteady](https://github.com/byuflowlab/FLOWUnsteady) (GitHub Repository of CPU implementation)
* [VTK Output](https://github.com/mmorse1217/lean-vtk)
* [FLOWVLM (Vortex Lattice Method)](https://github.com/byuflowlab/FLOWVLM)
* [Stable Vortex Particle Method Formulation for Meshless Large-Eddy Simulation](https://www.nas.nasa.gov/assets/nas/pdf/ams/2022/AMS_20220809_Alvarez.pdf) (Alvarez & Ning, 2023, research paper on reformulated Vortex Particle Method) 
* [NASA presentation slides for introductory concepts ](https://www.nas.nasa.gov/assets/nas/pdf/ams/2022/AMS_20220809_Alvarez.pdf)
* [Reformulated Vortex Particle Method and Meshless Large Eddy Simulation of Multirotor Aircraft](https://scholarsarchive.byu.edu/etd/9589/?utm_source=scholarsarchive.byu.edu%2Fetd%2F9589&utm_medium=PDF&utm_campaign=PDFCoverPages) (Alvarez PhD thesis on reformulated VPM) 
* [Scalable Fast Multipole Accelerated Vortex Methods](https://doi.org/10.1109/IPDPSW.2014.110) (Parallelized VPM) 
[Treecode and fast multipole method for N-body simulation with CUDA](https://arxiv.org/abs/1010.1482) (FMM implementation in CUDA)

<p align="center">
  <img src="images/penn.png" alt="University of Pennsylvania Logo" width=200>
</p>
<p align="center">
University of Pennsylvania, CIS 5650: GPU Programming and Architecture, Final Project 
</p>