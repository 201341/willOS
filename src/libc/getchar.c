#include <stdio.h>

#ifdef __is_libc

#include <proc/descriptor.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/syscall.h>

/// The scancode for the <kbd>arrow up</kbd> key.
#define KB_ARROW_UP 72
/// The scancode for the <kbd>backspace</kbd> key.
#define KB_BACKSPACE 14
/// The scancode for the <kbd>caps lock</kbd> key.
#define KB_CAPS_LOCK 58
/// The scancode for the <kbd>enter</kbd> key.
#define KB_ENTER 28
/// The scancode for the left <kbd>ctrl</kbd> key.
#define KB_LCTRL 29
/// The scancode for the left <kbd>shift</kbd> key.
#define KB_LSHIFT 42
/// The scancode for the right <kbd>shift</kbd> key.
#define KB_RSHIFT 54
/// The scancode for the <kbd>tab</kbd> key.
#define KB_TAB 15

static unsigned char keymap[][128] = {
  { 0 },         { 27 },       { '1', '!' }, { '2', '@' },  { '3', '#' },
  { '4', '$' },  { '5', '%' }, { '6', '^' }, { '7', '&' },  { '8', '*' },
  { '9', '(' },  { '0', ')' }, { '-', '_' }, { '=', '+' },  { '\b' },
  { '\t' },      { 'q', 'Q' }, { 'w', 'W' }, { 'e', 'E' },  { 'r', 'R' },
  { 't', 'T' },  { 'y', 'Y' }, { 'u', 'U' }, { 'i', 'I' },  { 'o', 'O' },
  { 'p', 'P' },  { '[', '{' }, { ']', '}' }, { '\n' },      { 0 },
  { 'a', 'A' },  { 's', 'S' }, { 'd', 'D' }, { 'f', 'F' },  { 'g', 'G' },
  { 'h', 'H' },  { 'j', 'J' }, { 'k', 'K' }, { 'l', 'L' },  { ';', ':' },
  { '\'', '"' }, { '`', '~' }, { 0 },        { '\\', '|' }, { 'z', 'Z' },
  { 'x', 'X' },  { 'c', 'C' }, { 'v', 'V' }, { 'b', 'B' },  { 'n', 'N' },
  { 'm', 'M' },  { ',', '<' }, { '.', '>' }, { '/', '?' },  { 0 },
  { '*' },       { 0 },        { ' ' },      { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { '-' },
  { 0 },         { 0 },        { 0 },        { '+' },       { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },        { 0 },         { 0 },
  { 0 },         { 0 },        { 0 },
};

static bool caps_lock_mode = false;
static bool shift_mode = false;

char getchar()
{
  char c = -1;

  while (c == -1) {
    uint8_t scancode;
    read(STDIN, &scancode, 1);
    bool key_was_released = false;

    if (scancode > 128) {
      key_was_released = true;
      scancode -= 128;
    }

    switch (scancode) {
      case 0:
        // no input
        break;

      case KB_CAPS_LOCK:
        if (key_was_released) {
          caps_lock_mode = !caps_lock_mode;
        }

        break;

      case KB_LSHIFT:
      case KB_RSHIFT:
        if (key_was_released) {
          shift_mode = false;
        } else {
          shift_mode = true;
        }

        break;

      case KB_BACKSPACE:
        if (key_was_released) {
          c = '\b';
        }

        break;

      case KB_ENTER:
        if (key_was_released) {
          c = '\n';
        }

        break;

      case KB_TAB:
        if (key_was_released) {
          c = '\t';
        }

        break;

      default:
        if (key_was_released) {
          if ((caps_lock_mode || shift_mode) && keymap[scancode][1]) {
            c = keymap[scancode][1];
          } else {
            c = keymap[scancode][0];
          }
        }
    }
  }

  return c;
}

#endif
