# Ultrasonic Sensor

This project was made using a PIC16F1455. It is to help cause a trigger when ultrasonic sensors detect an object nearby. For my specific use case, it is for triggering the display of my Magic Mirror when I step in front of it. I am using a PIC because the small SBC I am using for the mirror is not able to poll fast enough to get accurate readings from the sensor.

One thing to note is that I am using 2 sensors, but only one is activating the trigger. This is because I have my mirror close to my bedroom door. If the door is blocking both sensors (one on top and one below mirror), nothing happens. I am not tall enough to trigger the top mirror above, so the mirror only turns on when the bottom sensor detects something within range.
