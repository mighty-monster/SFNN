# Momentum

Momentum aims to provides cross-platform API for training and inference of neural networks using Vulkane compute shaders


Currently, at the early stages of development, the main focus is on "Make It Work", the documentation is not available, but the source code contains enough comments. 

<br>

### Why develop a new framework, why C++, why Vulkan?
* I am enthusiastic about the internals of ML libraries like TensorFlow. Project momentum is my starting point for facing the challenges necessary to deepen my understanding about them. I hope it would be helpful for others with the same taste ;)

* I like python and understand why someone would prefer to model his neural networks using it. Also, TensorFlow(and other advanced high-level libraries) provide anything an ML Engineer/Scientist might need. Despite all of that, I like to have a relatively simple and lightweight library with C++ being a first-class citizen at my disposal.

* By using Vulkan it would be possible to leverage a broader range of hardware, mobile devices included. Also: 
    * Cuda is limited to Nvidia 
    * After years of waiting for OpenCL to become a viable option for heterogeneous computing, I lost hope 
    * There is no open-source implementation of SYSCL for mobile devices.  

<br>


### How to intall, use, contribute ?

The project is at its early stages and it is not functional yet, so it is not possible to use it.
<br>
If you have any suggestions, or are willing to contribute, you can contact me via email: fatehi.arash@gmail.com
