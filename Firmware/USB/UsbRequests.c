#include "sam.h"
#include "UsbRequests.h"
#include "UsbDescriptors.h"
#include "../UART.h"
#include "../Utility.h"

static void HandleStandardRequest(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleStandardInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleClassInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request);

static void HandleGetDescriptor(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleStringDescriptors(sUsbDevice *usbDevice, uint8_t index);

void UsbHandleRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	// Check request type then call the correct function on class request type
	switch(request->bmRequestType & (USB_BM_REQUEST_TYPE_TYPE_MASK | USB_BM_REQUEST_TYPE_RECIPIENT_MASK))
	{
		case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE:
			HandleStandardRequest(usbDevice, request);
			break;
		case USB_BM_REQUEST_TYPE_TYPE_CLASS | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
			HandleClassInterfaceRequest(usbDevice, request);
			break;
		case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
			HandleStandardInterfaceRequest(usbDevice, request);
			break;
		default:
			break;
	}
}

static void HandleStandardRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	switch(request->bRequest)
	{
		case USB_STANDARD_DEVICE_REQUEST_GET_DESCRIPTOR:
			HandleGetDescriptor(usbDevice, request);
			break;
		case USB_STANDARD_DEVICE_REQUEST_SET_ADDRESS:
			UsbSendZLP(usbDevice, CONTROL_EP_NUM);
			UsbSetAddress(usbDevice, request->wValue);
			break;	
		case USB_STANDARD_DEVICE_REQUEST_SET_CONFIGURATION:
			UsbSendZLP(usbDevice, CONTROL_EP_NUM);
			
			UsbEndpointConfigure(usbDevice, (UsbEndpointDescriptor *) &usbCombinedDescriptor.combinedMidiDescriptor.midiInEndpointDescriptor);
			UsbEndpointConfigure(usbDevice, (UsbEndpointDescriptor *) &usbCombinedDescriptor.combinedMidiDescriptor.midiOutEndpointDescriptor);
			break;
		default:
			UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
			break;
	}
}

static void HandleClassInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	
}

static void HandleStandardInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request)
{

}

static void HandleGetDescriptor(sUsbDevice *usbDevice, UsbRequest *request)
{
	uint8_t descType = request->wValue >> 8;
	uint16_t length = request->wLength;
	
	switch(descType)
	{
		case USB_DESCRIPTOR_TYPE_DEVICE:
			// Length is whichever is shorter, the packet size or the max endpoint size
			length = MIN(length, usbDeviceDescriptor.bLength);
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbDeviceDescriptor, length);
			break;
		case USB_DESCRIPTOR_TYPE_CONFIGURATION:
			length = MIN(length, usbCombinedDescriptor.usbConfigurationDescriptor.wTotalLength);
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbCombinedDescriptor, length);
			break;
		case USB_DESCRIPTOR_TYPE_STRING:
		{
			uint8_t stringIndex = request->wValue & 0xFF;
			HandleStringDescriptors(usbDevice, stringIndex);
		}
			break;
		default:
			UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
			break;
	}
}

static void HandleStringDescriptors(sUsbDevice *usbDevice, uint8_t index)
{
	switch(index)
	{
		case USB_STRING_DESCRIPTOR_INDEX_0:
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescriptor0, sizeof(usbStringDescriptor0));
			break;
		case USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER:
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescManufacturer, sizeof(usbStringDescManufacturer));
			break;
		case USB_STRING_DESCRIPTOR_INDEX_PRODUCT:
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescProduct, sizeof(usbStringDescProduct));
			break;
		case USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUM:
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescSerialNumber, sizeof(usbStringDescSerialNumber));
			break;
		default:
			// Stall since we don't handle other requests
			UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
			break;
	}
}