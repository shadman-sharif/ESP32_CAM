# 🛠️ ESP32-CAM Troubleshooting (Real Errors & Fixes)

These are the actual error messages you'll see in the Serial Monitor, and what they mean — most generic guides just say "check your wiring," this one tells you the specific fix.

## 1. "Brownout detector was triggered"

```text
Brownout detector was triggered
ets Jul 29 2019 ...rst:0xc (SW_CPU_RESET)
```

**Cause:** power supply voltage dipped below what the chip needs, almost always during Wi-Fi transmit or camera capture.

**Fix, in order of likelihood:**
1. Switch to a proper 5V/1A+ power supply on the `5V` pin (see `docs/hardware.md` §3) — this fixes it 90% of the time.
2. Use a shorter/thicker USB cable if powering through a USB-TTL adapter.
3. Add a 100–470 µF capacitor across 5V/GND close to the board (`docs/hardware.md §3`) — catches the millisecond-scale sag a multimeter can't show you.
4. As a last resort, you can silence (not fix) the detector in software — only do this after trying real power fixes, since it hides the symptom, not the cause:

```cpp
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector
  Serial.begin(115200);
  // ... rest of your setup
}
```

## 2. Camera init failed: specific error codes

```text
Camera init failed with error 0x105
```

| Code | Meaning | Fix |
|---|---|---|
| `0x105` (ESP_ERR_NOT_FOUND) | Camera not detected | Ribbon cable not seated, inserted backwards, or wrong board pin config |
| `0x20001` | Camera probe failed | Same as above — reseat the ribbon cable, gold contacts facing the PCB |
| `0x20002` / `0x20004` | Not enough memory | Enable PSRAM in Tools menu, or lower `frame_size` (e.g. use `FRAMESIZE_QVGA` instead of `FRAMESIZE_VGA`) |
| `0x103` (ESP_ERR_TIMEOUT) | SCCB (I2C-like) handshake with the sensor timed out | Almost always a power issue at the moment of init, not a wiring issue — check the 5V supply before reseating anything |
| `0xffffffff` / random large hex | Usually a wiring/power issue, not the camera itself | Check power supply first (see brownout section above) |

## 3. "Sketch too big" / upload fails at compile

**Cause:** default partition scheme reserves too little space for app code.

**Fix:** Tools → Partition Scheme → **Huge APP (3MB No OTA/1MB SPIFFS)**.

## 4. Upload works but board resets in a loop after

```text
rst:0x10 (RTCWDT_RTC_RESET)
```

- Almost always power-related (see §1), or
- IO0 jumper wasn't removed after uploading — the board is stuck trying to re-enter download mode.

## 5. "Failed to connect to ESP32: Timed out waiting for packet header"

- IO0 not connected to GND *before* pressing reset/power-cycling.
- TX/RX swapped (should be crossed: adapter TX → board RX).
- Wrong serial port selected, or the USB-TTL driver (CP2102/CH340) isn't installed on your PC.
- Try a slower upload speed (115200) in Tools menu.

## 6. Browser can't open the camera stream

- Confirm the IP address printed in Serial Monitor after "Wi-Fi connected."
- Phone/PC must be on the **same Wi-Fi network** as the ESP32-CAM (not mobile data).
- Some routers isolate devices from each other ("AP/client isolation") — try a different network or hotspot if this is on.
- ESP32-CAM only supports 2.4GHz Wi-Fi, not 5GHz — make sure your router/hotspot is broadcasting a 2.4GHz network.

## 7. Image looks flipped, mirrored, or discolored

Add this after `esp_camera_init()` succeeds:

```cpp
sensor_t *s = esp_camera_sensor_get();
s->set_vflip(s, 1);      // flip vertically: 0 = off, 1 = on
s->set_hmirror(s, 1);    // mirror horizontally
s->set_brightness(s, 1); // -2 to 2
s->set_saturation(s, 0); // -2 to 2
```

## 8. microSD card not detected

- Must be formatted as **FAT32** (not exFAT — this is a very common cause).
- Use a card 32GB or smaller for best compatibility.
- Camera and SD card share some GPIOs — if your camera is running in a mode that uses those lines, SD access can become unreliable. Test SD alone first, then add the camera.
- Use `SD_MMC.begin("/sdcard", true)` (the `true` selects **1-bit mode**) rather than the default 4-bit mode — 4-bit mode uses GPIOs the camera also needs, and mixing the two is a common source of intermittent SD failures once the camera is active.

## 9. Stream works for a while, then freezes or the board silently stops responding

This is usually one of two things, both under-documented:

- **Frame buffer starvation:** with `fb_count = 2` and PSRAM enabled but a client that reads frames slower than the camera produces them (e.g. a laptop browser tab in the background, throttled), buffers can back up. Reduce `jpeg_quality` (higher number = more compression = smaller/faster frames) or drop to `fb_count = 1` if this happens consistently.
- **A client that never closes its connection cleanly** (phone browser locking the screen mid-stream is the classic case) can leave the single MJPEG stream handler stuck waiting on a socket write that will never succeed, effectively hanging that endpoint. Restarting the board clears it; for a permanent fix, add a write timeout to the HTTP server config, or track connection state and force-close stale streams after a timeout.

## 10. Colors look washed out or greenish in low light

Not a fault — the OV2640's automatic white balance and exposure algorithms are tuned for daylight/typical indoor lighting and struggle under single-color LED lighting or very low light. Try:

```cpp
s->set_whitebal(s, 1);       // enable auto white balance (should already be on)
s->set_awb_gain(s, 1);       // enable AWB gain
s->set_gain_ctrl(s, 1);      // enable automatic gain control
s->set_aec2(s, 1);           // enable advanced auto-exposure
```
If it's still poor, the fix is almost always better/more consistent physical lighting, not more sensor tuning — the OV2640 is a budget sensor and doesn't have much dynamic range to recover from bad lighting in software.
