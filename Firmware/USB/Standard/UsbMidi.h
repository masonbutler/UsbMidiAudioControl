#ifndef USBMIDI_H_
#define USBMIDI_H_

#define USB_MIDI_NOTE_OFF	0x8
#define USB_MIDI_NOTE_ON	0x9


typedef struct {
	uint8_t codeIndexNum : 4;
	uint8_t cableNum : 4;
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
} __attribute__((__packed__)) UsbMidiMessage;



#endif