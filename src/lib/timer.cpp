#include <timer.hpp>
#include <isr.hpp>
#include <terminal.hpp>
#include <stdint.h>
#include <io.hpp>
#include <serial.hpp>

using namespace serial;

namespace timer
{
uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
   (void)regs; // `regs` is not used
   tick++;
}

void init(uint32_t frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(32, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
   log("pit", "Initialized");
}
} // namespace timer