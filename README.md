# Bluetooth iDog

Video Link: https://youtu.be/caRnsafZBII

Description: This project uses the esp32 to convert the idog into a bluetooth speaker. Additioanlly, you can connect your Spotify account so that the display screen shows the title of the song you are listening to. 

Hardware Used:
  -esp32
  -LCD1602 display screen
  -Audio converter and amplifyer
  -headphone to headphone cable
  
  
Wiring: 
  Audio Converter:
    1. Connect GND to GND
    2. VCC to GPIO 27
    3. LCK to GPIO 25
    4. DIN to GPIO 22
    5. BCK to GPIO 26
    6. Headphone jack on the AC to headphone jack on the iDog
 
 LCD1602:
    1. GND to GND
    2. VCC to 5V
    3. SDA to GPIO 13
    4. SCL to GPIO 14
  
  
How To: 
  1. Wire as stated above
  2. Download the code provided and fill your wifi and spotify information where it is marked (wifi in cesfinal.ino, Spotify in cesfinal.py)
  3. Use the serial monitor to print the correct IP adress for the esp32 and fill this in wher marked in the code
  4. Be sure to install Spotipy (pip3 install spotipy)
  5. Upload the code to the esp32 and connect your phone via bluetooth to esp32
  6. Start up your favorite song on spotify and you should be all set!
  7. To have the song name display on the screen, run the python code provided
