/// @file Watchdog.h provides the interface to the Watchdog module
///
/// This provides basic Watchdog service for the mbed. You can configure
/// various timeout intervals that meet your system needs. Additionally,
/// it is possible to identify if the Watchdog was the cause of any
/// system restart, permitting the application code to take appropriate
/// behavior.
///
/// Adapted from Simon's Watchdog code from
/// http://mbed.org/forum/mbed/topic/508/
///
/// @note Copyright &copy; 2011 by Smartware Computing, all rights reserved.
///     This software may be used to derive new software, as long as
///     this copyright statement remains in the source file.
/// @author David Smart
///
/// History
/// \li v1.00 - 20110616: initial release with some documentation improvements
///
#pragma once
#include <mbed.h>

/// The Watchdog class provides the interface to the Watchdog feature
///
/// Embedded programs, by their nature, are usually unattended. If things
/// go wrong, it is usually important that the system attempts to recover.
/// Aside from robust software, a hardware watchdog can monitor the
/// system and initiate a system reset when appropriate.
///
/// This Watchdog is patterned after one found elsewhere on the mbed site,
/// however this one also provides a method for the application software
/// to determine the cause of the reset - watchdog or otherwise.
///
/// example:
/// @code
/// Watchdog wd;
///
/// ...
/// main() {
///    if (wd.watchdogCausedReset())
///        pc.printf("Watchdog caused reset.\r\n");
///
///    wd.configure(3.0);       // sets the timeout interval
///    for (;;) {
///         wd.service();       // kick the dog before the timeout
///         // do other work
///    }
/// }
/// @endcode
///
class Watchdog {
public:
  /// Create a Watchdog object
  ///
  /// example:
  /// @code
  /// Watchdog wd;    // placed before main
  /// @endcode
  Watchdog();

  /// configure the timeout for the Watchdog
  ///
  /// This configures the Watchdog service and starts it. It must
  /// be serviced before the timeout, or the system will be restarted.
  ///
  /// example:
  /// @code
  ///     ...
  ///     wd.configure(1.4);  // configure for a 1.4 second timeout
  ///     ...
  /// @endcode
  ///
  /// @param[in] timeout in seconds, as a floating point number
  /// @returns none
  ///
  void configure(float timeout);

  /// service the Watchdog so it does not cause a system reset
  ///
  /// example:
  /// @code
  ///    wd.service();
  /// @endcode
  /// @returns none
  void service();

  /// watchdogCausedReset identifies if the cause of the system
  /// reset was the Watchdog
  ///
  /// example:
  /// @code
  ///    if (wd.watchdogCausedReset())) {
  /// @endcode
  ///
  /// @returns true if the Watchdog was the cause of the reset
  bool watchdogCausedReset();

private:
  bool wdreset;
};
