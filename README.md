# [Combo Breaker](http://samy.pl/combobreaker/)

**[Combo Breaker](http://samy.pl/combobreaker/)** is a motorized, battery powered, 3D printed, Arduino-based combination lock cracking device. It is portable, open source, 3D models provided, and exploits a new technique I've discovered for [cracking combination locks in 8 attempts or less](https://www.youtube.com/watch?v=09UgmwtL12c), but in an even more exciting, automated fashion.

**Live demonstration** and full details available in the video:
<a href="https://www.youtube.com/watch?v=YcpSvHpbHQ4" target="_blank"><img src="http://img.youtube.com/vi/YcpSvHpbHQ4/0.jpg" alt="KeySweeper" width="640" height="480" border="10" /></a>

-----

**Point of Contact:** [@SamyKamkar](https://twitter.com/samykamkar) // <http://samy.pl>

**Video demonstration / explanation:** [https://youtu.be/YcpSvHpbHQ4](https://youtu.be/YcpSvHpbHQ4)

**Released:** May 14, 2015

**Source code / 3D models:** [https://github.com/samyk/combobreaker](https://github.com/samyk/combobreaker)

**Unit Cost:** < $100

**Status:** Operational, open source, open hardware, declassified.

-----

# (U) Hardware

[![Combo Breaker](http://samy.pl/combobreaker/sIMG_2916.JPG)](http://samy.pl/combobreaker/sIMG_2916.JPG)

### [Arduino Nano](http://amzn.to/1QLlf23)
**$15**: A [5V Arduino Nano](http://amzn.to/1QLlf23) microcontroller is used as the brains of the project.

### [Allegro A3967 / EasyDriver Stepper Driver](http://amzn.to/1A1Ovxl)
**$8**: I originally used an A3967-based [EasyDriver](http://amzn.to/1A1Ovxl) which can drive a stepper motor up to 30V at 750mA. This is what precisely controls the dial.

### or [Allegro A4988](http://amzn.to/1L3q7fK)
**$13**: Instead of the EasyDriver, I upgraded to a smaller, yet more powerful stepper driver that can go up to 1 amp (1000mA).

### [Stepper Motor](http://amzn.to/1FfLUQG)
**$18**: You can use a normal stepper motor for the "simpler" version of the Combo Breaker build, however if you're working on creating the more advanced version, you'll need a double-shaft stepper motor like I liste below.

### or [Double Shaft Stepper Motor](http://www.phidgets.com/products.php?product_id=3320)
**$16**: This double shaft stepper motor allows you to build the more advanced Combo Breaker while employing an optical rotary encoder on the back to detect the position of the stepper when the dial stops turning (which is part of the exploit employed).

### [HKT22 Optical Rotary Encoder](http://www.phidgets.com/products.php?product_id=3531)
**$25**: For use **only** with the double shaft stepper motor, this optical rotary encoder lets us know what position the shaft of the stepper motor is at and allows us to employ an exploit on Master combination locks where the stepper will get locked into certain grooves within the internal combination disc of the lock.

### [Analog Feedback Servo](https://www.adafruit.com/products/1404)
**$15**: This servo not only provides the torque we need to lift the shackle (90.26 oz*in), but has an additional wire providing analog feedback that we use to detect the state of the shackle and whether it has opened or not.

### [L7805 Voltage Regulators](http://amzn.to/1A28XOt)
**$5**: If you're using the analog feedback servo, you'll want to use an external voltage regulator to provide enough juice to the servo. While the Arduino is 5V and can drive the servo normally, it won't be able to pump out enough current for the servo when it starts hitting the shackle and could damage your Arduino.

### [500mAh 3S Lipo Battery](http://amzn.to/1PkRVlz)
**$14**: This 500mAh 3S (3-cell, 11.1V) battery gives enough juice to the stepper motor to be very effective, and at 500mAh can last between 30-120minutes straight before having to be recharged.

### [Lipo Charger](http://amzn.to/1A1Q2n7)
**$45**: This lipo charger is overkill if only used for this project, but is a good one that I use at home for a wide range of batteries and cells. You don't need this one, but if you want to recharge the battery, you'll need a charger that supports 3-cell LiPos.

### [Arctic Alumina Thermal Adehsive](http://amzn.to/1PkTakG)
**$8**: Not required, but this is the thermal **adhesive** I use to **permanently** attach the heatsink to the stepper motor driver. Normal thermal paste has very little adhesion and kept falling off, so I switched from thermal paste to this thermal adhesive.

### [Breadboard and Wires](http://amzn.to/1FfMb6e)
**$9**: If you don't already have a breadboard and wires, they will kind of help.

[![Combo Breaker](http://samy.pl/combobreaker/sIMG_2875.JPG)](http://samy.pl/combobreaker/sIMG_2875.JPG)

[![Combo Breaker](http://samy.pl/combobreaker/sIMG_2920.JPG)](http://samy.pl/combobreaker/sIMG_2920.JPG)

-----

# (U) Software

### Combo Breaker
Combo Breaker's source code can be obtained in entirety from my github: <https://github.com/samyk/combobreaker>

[![Combo Breaker 3D Print](http://samy.pl/combobreaker/model.jpg)](http://samy.pl/combobreaker/model.jpg)

-----

# (U) 3D Models

I've included the 3D models on my github: <https://github.com/samyk/combobreaker>
 
[![Combo Breaker 3D Print](http://samy.pl/combobreaker/smodel.png)](http://samy.pl/combobreaker/smodel.png)

[![Combo Breaker](http://samy.pl/combobreaker/sIMG_2919.JPG)](http://samy.pl/combobreaker/sIMG_2919.JPG)

-----

# (U) Contact

**Point of Contact:** [@SamyKamkar](https://twitter.com/samykamkar)

You can see more of my projects at <http://samy.pl>.

Thanks!

------