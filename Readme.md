# Mask calculator
Generate masks for a set of numbers

<!---
Uncomment when travis supports C++11
![Travis CI status](https://travis-ci.org/Chocobo1/mask-calculator.svg?branch=master)](https://travis-ci.org/Chocobo1/mask-calculator)
-->

## Algorithms
### qmm.cpp
* Implements [Quine–McCluskey algorithm](http://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm) and [Petrick's method](http://en.wikipedia.org/wiki/Petrick%27s_method)
* Gives optimal solution but the algorithm itself is slow

### greedy_merge.cpp
* A simple & recurisve greedy algorithm which aggressively try to generate a new mask by combining two numbers whenever possible
* Very fast but sometimes give non-optimal solution

## Features
* Maximum number of variables supported: 32
* Accept don't care terms

## Example
Suppose we want to calculate the port mask when filtering Stream UDP [ports](https://support.steampowered.com/kb_article.php?ref=8571-GLVN-8711) ranging from [27000, 27015]

Run the program as follow:
```
$ ./mask_calc 27000-27015
```
The program then output the results, first pair is value `27000` masked by `0xfffffff8`, second pair is value `27008` masked by `0xfffffff8`.
```
value(dec) / mask(hex)
1: 27000 / fffffff8
2: 27008 / fffffff8
```
Additionaly, you can check the result by another program I wrote, see [Notes](#notes).


## Compiler Requirements
* [C++11](http://en.wikipedia.org/wiki/C++11)
* For Windows users: [tdm-gcc](http://tdm-gcc.tdragon.net/)

## License
This program is licensed under [GPLv3](http://www.gnu.org/licenses/gpl.txt)

## Notes
* There is another program [Mask Enumerator](https://github.com/Chocobo1/mask-enumerator), which was created at the same time to verify the solution of this program.
* This program was created to help me generate the port mask when writing traffic shaping rules. When doing traffic shaping by utilizing [u32 filter](http://lartc.org/howto/lartc.adv-filter.html), the filter could only match multiple ports by specifying a mask.

  At the time writing the filtering rules, I couldn't find any suitable tool for this [task](http://serverfault.com/questions/231880/how-to-match-port-range-using-u32-filter), so I started my own.
