#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define USB_STORAGE_VID 0x0951 // Vendor ID for Kingston 
#define USB_STORAGE_PID 0x1665 // Product ID for DataTraveler G3

#define ENDPOINT_OUT 0x01
#define MAX_TRANSFER_SIZE 65536

int write_file_to_device(libusb_device_handle *handle, const char *filepath) {
    int r;
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("Error opening file: %s\n", filepath);
        return 1;
    }

    unsigned char buffer[MAX_TRANSFER_SIZE];
    int bytes_transferred;
    while ((bytes_transferred = fread(buffer, 1, MAX_TRANSFER_SIZE, file)) > 0) {
        r = libusb_bulk_transfer(handle, ENDPOINT_OUT, buffer, bytes_transferred, &bytes_transferred, 0);
        if (r < 0) {
            printf("Error writing data: %s\n", libusb_error_name(r));
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    int r;

    r = libusb_init(&ctx);
    if (r < 0) {
        printf("Error initializing libusb: %s\n", libusb_error_name(r));
        return 1;
    }

    handle = libusb_open_device_with_vid_pid(ctx, USB_STORAGE_VID, USB_STORAGE_PID);
    if (!handle) {
        printf("Error opening device: %s\n", libusb_error_name(r));
        goto exit;
    }

    if (libusb_kernel_driver_active(handle, 0)) {
        r = libusb_detach_kernel_driver(handle, 0);

