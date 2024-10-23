# UsbMidiAudioControl
A personal project to provide basic MIDI controls over USB to control the Reaper DAW (Digital Audio Workstation). The code is all basically baremetal (But I wrote a small layer I wrote over some of the peripherals to make them easier to deal with. It's pretty transparent in how it works).

I bought some cheap RGB LEDs from AliExpress that were labeled as WS2812B-2020, so I figured why not make a project out of them that would be useful for myself. Unfortunately, there was no datasheet and the LEDs seem to be a bit glitchy with 3.3V (where a regular WS2812B is not). To solve this, you can use a level converter capable, or run the board output into a single WS2812B regular size LED since it corrects the expected signal and removes the glitching.

I decided to go with 4 channels, providing 1 fader (to control volume) and 2 buttons per channel. 

![MixerUIBoard](https://github.com/user-attachments/assets/2933306d-4589-4577-92bf-61d3b3fa3a81)


# Technical stuff
The project uses an ATSAM L21 Explained Pro Devboard. Each linear fader goes into an ADC channel on the devboard, based upon tests the actual ADC value conversion has a range of 32-4080. The buttons are handled by a shift register that reads in the data over a SPI bus. The LED data is converted to a binary stream to provide the correct data format for a WS2812B LED then output over SPI.


# Physical Connections

PA22: UART TX

PA23: UART RX


PA9: Shift Reg Clock (SPI Clock)

PA10: Shift Reg SS (SPI SS)

PA11: Shift Reg Data In (SPI MISO)



PA16: LED Data Output (SPI MOSI)


PA4: Fader 1 (ADC Channel 4)

PA5: Fader 2 (ADC Channel 5)

PA6: Fader 3 (ADC Channel 6)

PA7: Fader 4 (ADC Channel 7)

# Compiling
Download Microchip Studio (Atmel Studio) 7.0.2594
Open project file from repo

# PCB
<img width="605" alt="image" src="https://github.com/user-attachments/assets/0f8f3ac8-8002-4e06-8617-ccfd09e8ca8a">


4 layer pretty simple board. It was same price as 2 layer at JLCPCB, so why not? I decided to give the LEDs their own power plane because the reviews of the LEDs complained about: thermal issues, current draw, and issues running of 3.3V instead of 5V. It was easier to track power (3.3V for anything else connected to the devboard). 

It's has purple soldermask and is pretty. 

# Dumb things I did
I did a few dumb things doing this:

I used LEDs ordered off Aliexpress expecting no real problems...ha ha ha.

I put the faders on upside down so I had to correct it in software. 0% is actually the highest value of the fader, while 100% is the lowest value. 

I soldered all of the components on this by hand (56 LEDs and 56 LEDs + headers + other components) because I didn't order a stencil when I ordered the PCBs (don't submit an order late at night).

I ordered logarithmic faders without realizing it because I ordered stuff at night. I spent 1 week wondering why my fader was giving me bad values on my ADC before I tried the ADC on a different devobard...ugh.
