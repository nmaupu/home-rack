# My rack @ home
This repo contains code about my home made rack at home.
It is still a work in progress.
I use DIY circuit to control fans to cool down the servers and stuff using Arduino and 140mm PWM fans.

## Using Arduino serial from command line

Configure TTY (source : https://playground.arduino.cc/Interfacing/LinuxTTY) :

    stty -f /dev/cu.wchusbserial1410 cs8 9600 \
      ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten \
      -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

Display serial out :

    cat /dev/cu.wchusbserial1410

Send a value to serial :

    echo -n "45" > /dev/cu.wchusbserial1410
