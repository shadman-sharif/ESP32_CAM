# Troubleshooting checklist

## A. `Failed to connect to ESP32`

Check in this order:
1. GPIO0 is connected to GND.
2. USB-TTL TX is connected to ESP32 U0R/RX.
3. USB-TTL RX is connected to ESP32 U0T/TX.
4. GND is shared.
5. Press RST just as upload begins.
6. Try 115200 upload speed.
7. Confirm the correct COM port.
8. Confirm **AI Thinker ESP32-CAM** is selected.

## B. `Camera init failed`

1. Power off.
2. Reseat the ribbon cable.
3. Check the ribbon orientation.
4. Confirm the board setting.
5. Test with the included camera web server.
6. If it still fails, test another camera module if available.

## C. Random reset / brownout

This is very commonly a power problem.

Try:
- stable 5V
- short power leads
- better USB cable/supply
- fewer peripherals
- lower camera frame size

## D. Web page does not open

- PC/phone must be on the same LAN as the ESP32-CAM.
- Use the IP printed in Serial Monitor.
- Check that Wi-Fi actually connected.
- Try `http://IP_ADDRESS/`, not HTTPS.

## E. SD card fails

- test another card
- format the card
- avoid hot-removing it
- remember that SD uses several GPIOs
- remember GPIO4 is commonly the flash LED

## F. It worked yesterday and now does not

Disconnect all extra sensors/modules and test the original camera sketch.

If the original works, reconnect peripherals one at a time. This quickly identifies GPIO conflicts or power problems.
