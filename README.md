# diffusion_limited_aggregation_cxx

This is a simple c++ program which uses [diffusion limited aggregation](https://en.wikipedia.org/wiki/Diffusion-limited_aggregation), or DLA for short, to generate random patterns which represents patterns often found in nature.
The inspiration for this project came from [The Coding Train's implementation](https://www.youtube.com/watch?v=Cl_Gjj80gPE) of this Algorithm in JavaScript.

OpenFrameworks was used to easily render the result. Also, [this](https://github.com/ika-musuko/openFrameworks-clion) repository was used to generate the Cmake and Make files for my IDE of choice, clion.

This program features an option to configure the aspects of the simulation before running it.

You can also use keys to interact with it:
|Key  |Function |
|:---:|:--------:|
| d | enable debug view|
| r | return to setup|
| q | stop program|

## Examples
![example1]
*The most basic example*

![example2]
*The radius of the nodes was decreased significantly while the number of nodes was upped*

![example3]
*A big tree not quite done yet*

![example4]
*A tree with 3000 nodes*

![example5]
*A tree with 6000 nodes. This ran for 71 minutes but since then, the program got a lot more efficient*

![example6]
*Another tree with 6000 nodes but with their radius decreased significantly. It did not finish (see right lower corner)*



[example1]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example1.png
[example2]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example2.png
[example3]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example3.png
[example4]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example4_n3000.png
[example5]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example5_n6000_4-29721e6.png
[example6]:https://github.com/Benimautner/diffusion_limited_aggregation_cxx/raw/master/media/example6_n6000.png
