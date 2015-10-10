// From: https://github.com/inspirit/PS3EYEDriver/


#pragma once


#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <chrono>
#include <memory>
#include "libusb.h"

#define DEBUG 1


#if defined(DEBUG)
#define debug(x...) fprintf(stdout,x)
#else
#define debug(x...)
#endif


class USBMgr;
class URBDesc;


namespace ps3eye {

class PS3EYECam
{
public:
	typedef std::shared_ptr<PS3EYECam> PS3EYERef;

	static const uint16_t VENDOR_ID;
	static const uint16_t PRODUCT_ID;

	PS3EYECam(libusb_device *device);
	~PS3EYECam();

	bool init(uint32_t width = 0,
			  uint32_t height = 0,
			  uint8_t desiredFrameRate = 30);

	void start();
	void stop();

	// Controls
	bool getAutogain() const;
	void setAutogain(bool val);
	bool getAutoWhiteBalance() const;
	void setAutoWhiteBalance(bool val);
	uint8_t getGain() const;


	void setGain(uint8_t val);
	uint8_t getExposure() const;
	void setExposure(uint8_t val);
	uint8_t getSharpness() const;
	void setSharpness(uint8_t val);
	uint8_t getContrast() const;
	void setContrast(uint8_t val);
	uint8_t getBrightness() const;
	void setBrightness(uint8_t val);
	uint8_t getHue() const;
	void setHue(uint8_t val);
	uint8_t getRedBalance() const;
	void setRedBalance(uint8_t val);
	uint8_t getBlueBalance() const;
	void setBlueBalance(uint8_t val);
	uint8_t getGreenBalance() const;
	void setGreenBalance(uint8_t val);

	void setFlip(bool horizontal = false, bool vertical = false);


	bool isStreaming() const;
	bool isNewFrame() const;
	const uint8_t* getLastFramePointer();

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	uint8_t getFrameRate() const;
	uint32_t getRowBytes() const;

	void setLED(bool enable);

	//
	static const std::vector<PS3EYERef>& getDevices( bool forceRefresh = false );
	static bool updateDevices();

	
private:
	PS3EYECam(const PS3EYECam&);
	void operator=(const PS3EYECam&);

	void release();

	// usb ops
	uint8_t ov534_set_frame_rate(uint8_t frame_rate, bool dry_run = false);

	// Two bits control LED: 0x21 bit 7 and 0x23 bit 7.
	// (direction and output)?
	void ov534_set_led(bool status);
	void ov534_reg_write(uint16_t reg, uint8_t val);
	uint8_t ov534_reg_read(uint16_t reg);
	int sccb_check_status();
	void sccb_reg_write(uint8_t reg, uint8_t val);
	uint8_t sccb_reg_read(uint16_t reg);

	// output a bridge sequence (reg - val)
	void reg_w_array(const uint8_t (*data)[2], int len);

	// output a sensor sequence (reg - val)
	void sccb_w_array(const uint8_t (*data)[2], int len);
	
	// controls
	bool autogain;
	uint8_t gain; // 0 <-> 63
	uint8_t exposure; // 0 <-> 255
	uint8_t sharpness; // 0 <-> 63
	uint8_t hue; // 0 <-> 255

	bool awb;

	uint8_t brightness; // 0 <-> 255
	uint8_t contrast; // 0 <-> 255
	uint8_t blueblc; // 0 <-> 255
	uint8_t redblc; // 0 <-> 255
	uint8_t greenblc; // 0 <-> 255

	bool flip_h;
	bool flip_v;

	bool is_streaming;
	
	std::shared_ptr<USBMgr> mgrPtr;
	
	static bool devicesEnumerated;
	static std::vector<PS3EYERef> devices;
	
	uint32_t frame_width;
	uint32_t frame_height;
	uint32_t frame_stride;
	uint8_t frame_rate;
	
	std::chrono::time_point<std::chrono::high_resolution_clock> last_qued_frame_time;
	
	//usb stuff
	libusb_device *device_;
	libusb_device_handle *handle_;
	uint8_t *usb_buf;
	
	std::shared_ptr<URBDesc> urb;
	
	bool open_usb();
	void close_usb();
	
};

} // namespace

