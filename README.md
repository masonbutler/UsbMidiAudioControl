# UsbMidiAudioControl
A personal project to provide basic MIDI controls over USB to control the Reaper DAW (Digital Audio Workstation).

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
