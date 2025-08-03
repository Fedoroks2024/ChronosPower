#include "ChronosPowerLite.h"

namespace ChronosPowerLite {

// Global instance definition
PowerManager Power;

#if defined(__AVR__)
// Watchdog interrupt handler for AVR
ISR(WDT_vect) {
    // Just wake up from sleep
}
#endif

} // namespace ChronosPowerLite
