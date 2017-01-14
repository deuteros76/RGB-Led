# RGB-Led

This is a quick example of how to use websockets in an ESP8266 device. 

The websocket server is run in the NodeMCU and its code is based on one of the examples provided in the arduinoWebsocket project https://github.com/Links2004/arduinoWebSockets
The client is a QML application that can be executed in a desktop PC or a smartphone.

## Requirements

- This source uses Arduino IDE and its standard libraries
- Additional libraries: arduinoWebsocket
- QT creator IDE

## Hardware components

- NodeMCU 0.9/1.0
- An RGB Led
- A computer or smartphone to run the client Qt application

## Description

I wanted to experiment with websockets and this was a simple way of do it. The arduino code is very similar to what you can find in this [example](https://github.com/Links2004/arduinoWebSockets/blob/master/examples/WebSocketServer_LEDcontrol/WebSocketServer_LEDcontrol.ino).
On the other side, the client application is a QML application. I found QML an easy way for developing smartphone applications and the same code can be executed by a desktop PC.

The client application shows a color picker that sends the current RGB color value through websocket to the NodeMC device so you can see how the colors change as you move your finger on the screen.

The circuit is very simple and can be found in other project on the web.

![Breadboard](pics/RGB led_bb.png) 

I use three 220 ohm resistors but it is preferable to use different values according to the information RGB led specifications, you can read about this for instance [here](http://forums.netduino.com/index.php?/topic/8060-resistor-value-for-rgb-led/#entry44769)


Because of that, the colors I get are not very accurate, It works well with red, green, blue amd magenta colors but for instance, I couldn't get yellow.

<iframe width="560" height="315" src="https://www.youtube.com/embed/ierMWuUzd34" frameborder="0" allowfullscreen></iframe>

When the code is executed the led shows a red color while is connecting to the WiFi access point. When the network is up, the led turns into green, This second color will change after the smartphone application starts. The RGB code selected in the smartphone is transmitted through the websocket to the NodeMCU, then the code is splitted and each of the red, green and blue values is converted to an integer and sent to the corresponding PWM pin.
