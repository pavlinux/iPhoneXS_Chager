/*
 * Original by Max Korenkov (c) 2011. https://github.com/mkorenkov/ipad_charge 
 */

#include <libusb-1.0/libusb.h>

#define CTRL_OUT  (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define ADDITIONAL_VALUE_DEFAULT 1600

int main(void)
{
	int ret;
	struct libusb_device_handle *handle;
	int iface = 0;
	int active_cfg = -5;
	int configId = 1;

	uint16_t VID = 0x05ac;
	uint16_t PID = 0x12a8;

	libusb_init(NULL);

	handle = libusb_open_device_with_vid_pid(NULL, VID, PID);
	if (handle == NULL) {
		ret = -1;
		goto out_exit;
	}

	libusb_get_configuration(handle, &active_cfg);
	if (active_cfg != configId)
		libusb_set_configuration(handle, configId);

	ret = libusb_claim_interface(handle, iface);
	if (ret != LIBUSB_SUCCESS)
		goto out_close;

	/* The 3rd and 4th numbers are the extra current in mA that the Apple device 
	 * may draw in suspend state.  Originally, the 4th was 0x6400, or 25600mA. 
	 * I believe this was a bug and they meant 0x640, or 1600 mA which would be the max
	 * for the MFi spec. Also the typical values for the 3nd listed in the MFi spec are:
	 * 0, 100, 500 so I chose 500 for that. And changed it to decimal to be clearer.
	 */	
	ret = libusb_control_transfer(handle, CTRL_OUT, 0x40, 500, ADDITIONAL_VALUE_DEFAULT, NULL, 0, 2000);
	if (ret != LIBUSB_SUCCESS)
		goto out_release;

 out_release:
	libusb_release_interface(handle, 0);
 out_close:
	libusb_close(handle);
 out_exit:
	libusb_exit(NULL);
	return ret;
}
