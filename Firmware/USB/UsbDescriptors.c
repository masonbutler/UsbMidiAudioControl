#include "UsbDescriptors.h"

UsbDeviceStandardDescriptor usbDeviceDescriptor = {
	.bLength = sizeof(UsbDeviceStandardDescriptor),
	.bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass = USB_SUBCLASS_NONE,
	.bDeviceProtocol = USB_PROTOCOL_NONE,
	.bMaxPacketSize0 = 64,
	.idVendor = VENDOR_ID,
	.idProduct = PRODUCT_ID,
	.bcdDevice = 0x0100,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

UsbCombinedDescriptor usbCombinedDescriptor = {
	.usbConfigurationDescriptor = {
		.bLength = sizeof(UsbConfigurationDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_CONFIGURATION,
		.wTotalLength = sizeof(UsbCombinedDescriptor),
		.bNumInterfaces = 2,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_BUS_POWERED,
		.bMaxPower = 50,
	},
	.usbInterfaceDescriptor = {
		.bLength = sizeof(UsbInterfaceDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = USB_CLASS_AUDIO,
		.bInterfaceSubClass = USB_SUBCLASS_AUDIO_CONTROL,
		.bInterfaceProtocol = USB_PROTOCOL_NONE,
		.iInterface = 0,
	},
	.audioControlInterfaceDescriptor = {
		.bLength = sizeof(UsbACInterfaceDescriptor),
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_CS_HEADER,
		.bcdUSB = 0x0100,
		.wTotalLength = sizeof(UsbInterfaceDescriptor),
		.bInCollection = 1,
		.baInterfaceNr = 1,
	},

	.midiStreamingInterfaceDescriptor = {
		.bLength = sizeof(UsbInterfaceDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 1,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_CLASS_AUDIO,
		.bInterfaceSubClass = USB_SUBCLASS_AUDIO_MIDI_STREAMING,
		.bInterfaceProtocol = USB_PROTOCOL_NONE,
		.iInterface = 0,
	},
	.combinedMidiDescriptor.midiStreamingHeaderDescriptor = {
		.bLength = sizeof(UsbACMidiStreamingHeaderDescriptor),
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_CS_HEADER,
		.bcdMSC = 0x0100,
		.wTotalLength = sizeof(UsbCombinedMidiDescriptor),
	},
	.combinedMidiDescriptor.midiInJackEmbeddedDescriptor = {
		.bLength = sizeof(UsbACMidiInJackDescriptor),
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_AC_MIDI_IN_JACK,
		.bJackType = USB_AC_MIDI_JACK_EMBEDDED,
		.bJackID = 0x01,
		.iJack = 0,
	},
	.combinedMidiDescriptor.midiInJackExternalDescriptor = {
		.bLength = sizeof(UsbACMidiInJackDescriptor), 
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_AC_MIDI_IN_JACK,
		.bJackType = USB_AC_MIDI_JACK_EXTERNAL,
		.bJackID = 0x02,
		.iJack = 0,
	},
	.combinedMidiDescriptor.midiOutJackEmbeddedDescriptor = {
		.bLength = sizeof(UsbACMidiOutJackDescriptor), 
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_AC_MIDI_OUT_JACK,
		.bJackType = USB_AC_MIDI_JACK_EMBEDDED,
		.bJackID = 0x03,
		.bNrInputPins = 1,
		.baSourceID = 0x02,
		.baSourcePin = 0x01,
		.iJack = 0,
	},
	.combinedMidiDescriptor.midiOutJackExternalDescriptor = {
		.bLength = sizeof(UsbACMidiOutJackDescriptor), 
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubtype = USB_AC_MIDI_OUT_JACK,
		.bJackType = USB_AC_MIDI_JACK_EXTERNAL,
		.bJackID = 0x04,
		.bNrInputPins = 1,
		.baSourceID = 0x01,
		.baSourcePin = 0x01,
		.iJack = 0,
	},
	.combinedMidiDescriptor.midiOutEndpointDescriptor = {
		.bLength = sizeof(UsbACMidiStreamingStandardDataEndpointDescriptor), 
		.bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = 0x01,
		.bmAttributes = USB_ENDPOINT_TYPE_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,
		.bRefresh = 0,
		.bSynchAddress = 0,
	},
	.combinedMidiDescriptor.midiClassOutDescriptor = {
		.bLength = sizeof(UsbACMidiStreamingClassSpecificDataEndpointDescriptor), 
		.bDescriptorType = USB_CS_ENDPOINT,
		.bDescriptorSubtype = USB_AC_MIDI_GENERAL,
		.bNumEmbMIDIJack = 1,
		.baAssocJackID = 0x01,
	},
	.combinedMidiDescriptor.midiInEndpointDescriptor = {
		.bLength = sizeof(UsbACMidiStreamingStandardDataEndpointDescriptor), 
		.bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = 0x81,
		.bmAttributes = USB_ENDPOINT_TYPE_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,
		.bRefresh = 0,
		.bSynchAddress = 0,
	},
	.combinedMidiDescriptor.midiClassInDescriptor = {
		.bLength = sizeof(UsbACMidiStreamingClassSpecificDataEndpointDescriptor),
		.bDescriptorType = USB_CS_ENDPOINT,
		.bDescriptorSubtype = USB_AC_MIDI_GENERAL,
		.bNumEmbMIDIJack = 1,
		.baAssocJackID = 0x03,
	},
};

uint8_t usbStringDescriptor0[] = {
	0x04,	// Size in bytes
	USB_STRING_DESCRIPTOR_TYPE,
	0x09, 0x04, // USB_STRING_LANID_ENGLISH
};

uint8_t usbStringDescManufacturer[] = {
	STR_DESC_SIZE(USB_STR_MANUFACTURER_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_MANUFACTURER,
};

uint8_t usbStringDescProduct[] = {
	STR_DESC_SIZE(USB_STR_PRODUCT_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_PRODUCT,
};

uint8_t usbStringDescSerialNumber[] = {
	STR_DESC_SIZE(USB_STR_SERIAL_NUM_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_SERIAL_NUM,
};