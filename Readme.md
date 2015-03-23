# Mask calculator
Generate masks which cover a set of numbers

## Algorithms
### qmm.cpp
* Implements the [Quine–McCluskey algorithm](http://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm) and [Petrick's method](http://en.wikipedia.org/wiki/Petrick%27s_method)
* Gives optimal solution but slow

### greedy_merge.cpp
* A simple & recurisve greedy algorithm which tries to generate a new mask from two numbers whenever possible
* Very fast but often gives non-optimal solution

## Other features
* Number of variables supported: 32
* Accept don't care terms

## Compiler Requirements
* Must support [C++11](http://en.wikipedia.org/wiki/C++11)
* For Windows users: [tdm-gcc](http://tdm-gcc.tdragon.net/)

## License
This program is licensed under [GPLv3](http://www.gnu.org/licenses/gpl.txt)

## Notes
* [Mask enumerator](https://bitbucket.org/27049215/mask-enumerator) was created to verify the solution of this program
* This program was created to help me to generate the port mask when writing traffic shaping rules. The device which runs traffic shaping only supports [u32 filter](http://lartc.org/howto/lartc.adv-filter.html), and the filter could only match ports by specifying a mask. At the time, I didn't find any tool which is suitable for this [task](http://serverfault.com/questions/231880/how-to-match-port-range-using-u32-filter), so I started out my own.
