#ifndef USBAUDIOSTANDARD_H_
#define USBAUDIOSTANDARD_H_

#define USB_AC_INPUT_TERMINAL				0x02
#define USB_SUBCLASS_AUDIO_MIDI_STREAMING	0x03
#define USB_AC_MIDI_IN_JACK					0x02
#define USB_AC_MIDI_OUT_JACK				0x03

#define USB_AC_MIDI_JACK_EMBEDDED			0x01
#define USB_AC_MIDI_JACK_EXTERNAL			0x02

#define USB_AC_CHANNEL_CONFIG_LEFT_FRONT	0x01
#define USB_AC_CHANNEL_CONFIG_RIGHT_FRONT	0x02

#define USB_AC_MIDI_GENERAL					0x01


typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdUSB;
	uint16_t wTotalLength;
	uint8_t bInCollection;
	uint8_t baInterfaceNr;
} __attribute__((__packed__)) UsbACInterfaceDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bNrChannels;
	uint16_t wChannelConfig;
	uint8_t iChannelNames;
	uint8_t iTerminal;
} __attribute__((__packed__)) UsbACInputTerminalDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bSourceID;
	uint8_t iTerminal;
} __attribute__((__packed__)) UsbACOutputTerminalDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bTerminalLink;
	uint8_t bDelay;
	uint16_t wFormatTag;
} __attribute__((__packed__)) UsbACAudioStreamingInterfaceDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint8_t bLockDelayUnits;
	uint8_t wLockDelay;
} __attribute__((__packed__)) UsbACAudioStreamingIsocSynchEndpointDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
	uint8_t bRefresh;
	uint8_t bSynchAddress;
} __attribute__((__packed__)) UsbACAudioStreamingIsocDataEndpointDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint8_t bmaControls;
	uint8_t iFeature;
} __attribute__((__packed__)) UsbACFeatureUnitDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bFormatType;
	uint8_t bNrChannels;
	uint8_t bSubFrameSize;
	uint8_t bBitResolution;
	uint8_t bSamFreqType;
	uint8_t tSamFreq[3];
} __attribute__((__packed__)) UsbACFormatTypeDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdMSC;
	uint16_t wTotalLength;
} __attribute__((__packed__)) UsbACMidiStreamingHeaderDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bJackType;
	uint8_t bJackID;
	uint8_t iJack;
} __attribute__((__packed__)) UsbACMidiInJackDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bJackType;
	uint8_t bJackID;
	uint8_t bNrInputPins;
	uint8_t baSourceID;
	uint8_t baSourcePin;
	uint8_t iJack;
} __attribute__((__packed__)) UsbACMidiOutJackDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
	uint8_t bRefresh;
	uint8_t bSynchAddress;
} __attribute__((__packed__)) UsbACMidiStreamingStandardDataEndpointDescriptor;

typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bNumEmbMIDIJack;
	uint8_t baAssocJackID;
} __attribute__((__packed__)) UsbACMidiStreamingClassSpecificDataEndpointDescriptor;




#endif