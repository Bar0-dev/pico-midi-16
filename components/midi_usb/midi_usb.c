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

void midi_send_note(uint8_t key_down, uint8_t note){
  uint8_t note_msg[3];
  if(key_down){
    note_msg[0] = 0x90 | CHANNEL;
    note_msg[2] = 127;
  } else {
    note_msg[0] = 0x80 | CHANNEL;
    note_msg[2] = 0;
  }
  note_msg[1] = note;
  tud_midi_stream_write(CABLE_NUM, note_msg, 3);

}

void midi_send_cc(uint8_t value, int8_t cc){
  uint8_t cc_msg[3] = { 0xB0 | CHANNEL, cc, value};
  tud_midi_stream_write(CABLE_NUM, cc_msg, 3);
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
uint32_t led_interval(void)
{
  return blink_interval_ms;
}