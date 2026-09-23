# DEPG1020BNS770F1 E-Paper Display

## Purpose

This example drives a DEPG1020BNS770F1 960-by-640 monochrome e-paper panel
through an RD02E SSD1677 driver board. It uses 32-row paged drawing, performs one
full-screen refresh, powers the driver board down, rests for 60 seconds, and
then repeats with an incremented refresh counter.

## Requirements

- RadioCore RC32 or RC52 board configuration
- A DEPG1020BNS770F1 driver board that includes the panel power and high-voltage
  circuitry; do not connect the bare 24-pin panel directly to the MCU
- A `heltec-eink-modules` version that provides the `DEPG1020BNS770F1` class

Install the updated `heltec-eink-modules` library manually in the Arduino
sketchbook `libraries` directory. The currently published Library Manager
version does not contain this panel driver.

## RD02E wiring

RD02E P1 mates pin-for-pin with RC32 P3 or RC52 P1. The signal assignment comes
from the RD02E and RadioCore schematics; it is not the RadioCore TFT SPI mapping.

| Signal | RD02E P1 | Heltec RC32 P3 | Heltec RC52 P1 |
| --- | ---: | ---: | ---: |
| RST/RES | 1 | GPIO4 | P0.10 (10) |
| BUSY | 2 | GPIO6 | P1.13 (45) |
| DC | 3 | GPIO5 | P0.09 (9) |
| EN/VEINK_Ctrl | 4 | GPIO21 | P1.11 (43) |
| CS | 5 | GPIO2 | P1.15 (47) |
| SCK/SCL | 11 | GPIO48 | P0.05 (5) |
| MOSI/SDA | 13 | GPIO47 | P0.15 (15) |

`VEINK_Ctrl` is active low: pulling it low turns on the RD02E P-channel power
switch. BUSY is active high, while RST and CS are active low. BS1 is tied to
ground on RD02E, fixing the panel in 4-wire, 8-bit SPI mode. There is no separate
MISO signal; SDA is only used as MOSI by this write-only driver.

RC52 uses `SPI1`, explicitly remapped to SCK D5 and MOSI D15 by the display
constructor. Do not rely on the board variant's TFT SPI defaults.

The example owns these connector pins while it runs. Do not simultaneously use
the conflicting sensor reset/power, soil moisture, WS2812, buzzer, relay, TFT,
or other connector functions. Check the PCB and RD02E silkscreen pin-1 marks
before applying power so the board-to-board connector is not inserted mirrored.

## Behavior and limitations

The sketch draws a static identification page, test geometry, and an increasing
refresh counter. A complete image is rendered through a 3,840-byte page buffer.
The driver waits at most 60 seconds for BUSY to return low. After a successful
refresh it sends the supplied-demo deep-sleep sequence and disables driver-board
power; the next cycle performs a complete power-on and initialization sequence.

Only monochrome full-screen refresh is supported. Partial refresh, fast mode,
grayscale, controller readback, and hardware operation have not been verified.
RD02E has no isolation between the MCU control signals and the powered-down
SSD1677 domain. This implementation does not place the SPI and control pins in
high impedance after disabling `VDD_EINK`; hardware backfeed remains unverified.
