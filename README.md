# burger_express
A little script that enables the NIBO Burger to autonomously track a designated path of a certain color.

<br>

## Table of contents
* [Technologies](#technologies)
* [Setup](#setup)
* [Operation](#operation)
* [License](#license)

<br>

## Technologies

* [Ardruino IDE 1.6.6](https://www.arduino.cc/en/software/OldSoftwareReleases)
* [NiboRoboLib 3.4.1](https://www.roboter.cc/index.php?view=article&id=28&option=com_content&Itemid=1)
* [NIBO burger](http://www.nicai-systems.com/en/nibo-burger)

<br>

## Setup

Please refer to this guide, explaining how to use the NIBO burger with
the Ardruino IDE.

[Ardruino Tutorial.pdf](http://download.nicai-systems.com/nibo/Tutorial_Arduino_NIBOburger_eng_20151205.pdf)

### Line

Recommendations for the line:

- continues colors
- a clear color diffenrence between floor and line (i.e. white-black)
- curves >= 90°
- 3cm - 4cm wide

<br>

## Operation

For information on how to set up the line, please refer to [The Line](#line)

1. After turning on the NIBO burger, all 4 LED's should turn on
2. Then place the burger next to the line on the floor (Make sure the color sensors **only "see" the floor**)
3. Now press the *top button* (●) to measure the color of the floor
4. Place it on the center of the line that is intended to be followed
5. Press the *center button* (●●) now
6. Now move the NIBO burger on the line, until both blue LED's constantly shine
7. Lastly press the *bottom button* (●●●) to start the programm

<br>

## License

