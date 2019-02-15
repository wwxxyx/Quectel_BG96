Open Source Usage
=================

The ADK core (HAP Library) in general does not use any open source libraries. However, the POSIX sample SDK uses a
number of open source libraries and tools for its reference platform.

Raspberry Pi 3 Reference Platform (Raspbian)
--------------------------------------------

Our current Raspbian reference platform assumes the existence of Linux (GPL license) plus some suitable Linux services.
Currently we recommend a few libraries and tools (various licenses):

- Raspbian                      (variant of Debian Linux for Raspberry Pi)
- hostapd                       (implements a soft access point for Wi-Fi)
- dnsmasq                       (DHCP server)
- mDNSResponder                 (Apple's implementation of mDNS/Bonjour)
- Avahi                         (an alternative implementation of mDNS/Bonjour)
- libusb                        (USB-connected devices)
- libnfc                        (programmable NFC)
- GStreamer                     (audio/video framework)
- libssl                        (TLS library)
- libasound2-dev                (ALSA sound library)
- dbus                          (inter-process communication)
- WiringPi                      (GPIO library)
- Clang                         (compiler and assembler)
- Valgrind                      (memory analyzer)
- Vim                           (text editor)
- tmux                          (terminal multiplexer)
- Mercurial                     (version control system)
- Git                           (version control system)
- CMake                         (tool for controlling compilation of programs)
- GCC                           (compiler and assembler)
- hidapi                        (USB-connected devices)

Instead of mDNSResponder, which is currently the default for the Linux platform, Avahi could be used as well.
Accessory makers are free to use any compatible Bonjour service with any license that they deem acceptable.

HAP Libraries
-------------

Some toolchains require additional files to build.

- musl                          (a lightweight C-library)
