#include "midi_usb.h"

enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void midi_init(void)
{
  board_init();
  // init device stack on configured roothub port
  tusb_init();
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

uint8_t notes[] = {72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87};
uint8_t cc_setup[] = {7, 3, 9, 14, 15, 20, 21, 22};

void midi_send_note(uint8_t btn, uint8_t key_down){
  uint8_t note[3];
  if(key_down){
    note[0] = (0x90);
    note[2] = 127;
  } else {
    note[0] = (0x80);
    note[2] = 0;
  }
  note[1] = CHANNEL;
  tud_midi_stream_write(CABLE_NUM, note, 3);

}

void midi_send_cc(uint8_t cc_num, uint8_t value){
  uint8_t cc_msg[3] = { 0xB0 | CHANNEL, cc_setup[cc_num], value};
  tud_midi_stream_write(CABLE_NUM, cc_msg, 3);
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
uint32_t led_interval(void)
{
  return blink_interval_ms;
}