#ifndef USBDESCRIPTORS_H_
#define USBDESCRIPTORS_H_
#include "Standard/UsbStandard.h"
#include "Standard/UsbAudioStandard.h"

#define VENDOR_ID	(0x1208)
#define PRODUCT_ID	(0xEEE2)

#define STR_DESC_SIZE(LEN) (2U + (2U * (LEN)))

#define USB_STR_MANUFACTURER	'M', 0x00U, 'a', 0x00U, 's', 0x00U, 'o', 0x00U, 'n', 0x00U, ' ', 0x00U, 'B', 0x00U, 'u', 0x00U, 't', 0x00U, 'l', 0x00U, 'e', 0x00U, 'r', 0x00U
#define USB_STR_MANUFACTURER_LEN	12U

#define USB_STR_PRODUCT 'D', 0x00U, 'A', 0x00U, 'W', 0x00U, ' ', 0x00U, 'C', 0x00U, 'O', 0x00U, 'N', 0x00U, 'T', 0x00U, 'R', 0x00U, 'O', 0x00U, 'L', 0x00U
#define USB_STR_PRODUCT_LEN	11U

#define USB_STR_SERIAL_NUM	'4', 0x00U, '4', 0x00U, '7',0x00U, '3', 0x00U, '6', 0x00U, '3', 0x00U, '7', 0x00U, '5', 0x00U, '9', 0x00U
#define USB_STR_SERIAL_NUM_LEN	9U


typedef struct
{
	UsbACMidiStreamingHeaderDescriptor midiStreamingHeaderDescriptor;
	UsbACMidiInJackDescriptor midiInJackEmbeddedDescriptor;
	UsbACMidiInJackDescriptor midiInJackExternalDescriptor;
	UsbACMidiOutJackDescriptor midiOutJackEmbeddedDescriptor;
	UsbACMidiOutJackDescriptor midiOutJackExternalDescriptor;


	UsbACMidiStreamingStandardDataEndpointDescriptor midiOutEndpointDescriptor;
	UsbACMidiStreamingClassSpecificDataEndpointDescriptor midiClassOutDescriptor;
	
	UsbACMidiStreamingStandardDataEndpointDescriptor midiInEndpointDescriptor;
	UsbACMidiStreamingClassSpecificDataEndpointDescriptor midiClassInDescriptor;
}  __attribute__((__packed__))  UsbCombinedMidiDescriptor;

typedef struct
{
	UsbConfigurationDescriptor usbConfigurationDescriptor;
	UsbInterfaceDescriptor usbInterfaceDescriptor;
	UsbACInterfaceDescriptor audioControlInterfaceDescriptor;
	UsbInterfaceDescriptor midiStreamingInterfaceDescriptor;
	UsbCombinedMidiDescriptor combinedMidiDescriptor;
} __attribute__((__packed__))  UsbCombinedDescriptor;

extern UsbDeviceStandardDescriptor usbDeviceDescriptor;
extern UsbCombinedDescriptor usbCombinedDescriptor;
extern uint8_t usbStringDescriptor0[4];
extern uint8_t usbStringDescManufacturer[STR_DESC_SIZE(USB_STR_MANUFACTURER_LEN)];
extern uint8_t usbStringDescProduct[STR_DESC_SIZE(USB_STR_PRODUCT_LEN)];
extern uint8_t usbStringDescSerialNumber[STR_DESC_SIZE(USB_STR_SERIAL_NUM_LEN)];


#endif