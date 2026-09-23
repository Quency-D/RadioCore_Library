/*
 * RadioCore DEPG1020BNS770F1 e-paper example
 *
 * Requires a heltec-eink-modules version that provides the
 * DEPG1020BNS770F1 display class.
 */
#include <Arduino.h>
#include <RadioCore_Kit.h>

#if !RADIOCORE_HAS_DEPG1020BNS770F1_DISPLAY
#error "DEPG1020BNS770F1_Display requires a supported RadioCore board configuration."
#else

#include <heltec-eink-modules.h>

namespace {

constexpr uint16_t kPageHeight = 32;
constexpr uint32_t kRefreshRestMs = 60000UL;

DEPG1020BNS770F1 display(
    RADIOCORE_DEPG1020BNS770F1_DC,
    RADIOCORE_DEPG1020BNS770F1_CS,
    RADIOCORE_DEPG1020BNS770F1_BUSY,
    RADIOCORE_DEPG1020BNS770F1_RST,
    RADIOCORE_DEPG1020BNS770F1_ENABLE,
    RADIOCORE_DEPG1020BNS770F1_ENABLE_ACTIVE,
    RADIOCORE_DEPG1020BNS770F1_MOSI,
    RADIOCORE_DEPG1020BNS770F1_SCK,
    kPageHeight);

uint32_t refreshCount = 0;

void drawTestPage()
{
  display.drawRect(0, 0, display.width(), display.height(), BLACK);
  display.drawRect(8, 8, display.width() - 16, display.height() - 16, BLACK);

  display.setTextColor(BLACK);
  display.setTextSize(4);
  display.setCursor(40, 48);
  display.println(F("RadioCore"));

  display.setTextSize(2);
  display.setCursor(40, 112);
  display.print(F("Board: "));
  display.println(F(RADIOCORE_DEPG1020BNS770F1_BOARD_NAME));
  display.setCursor(40, 148);
  display.println(F("DEPG1020BNS770F1 / SSD1677"));
  display.setCursor(40, 184);
  display.println(F("960 x 640 monochrome"));
  display.setCursor(40, 220);
  display.print(F("Refresh count: "));
  display.println(refreshCount);

  constexpr int16_t blockTop = 300;
  constexpr int16_t blockHeight = 180;
  constexpr int16_t blockWidth = 180;
  display.fillRect(60, blockTop, blockWidth, blockHeight, BLACK);
  display.drawRect(260, blockTop, blockWidth, blockHeight, BLACK);
  display.fillCircle(570, blockTop + blockHeight / 2, 90, BLACK);
  display.drawCircle(790, blockTop + blockHeight / 2, 90, BLACK);

  display.drawLine(40, 540, display.width() - 40, 540, BLACK);
  display.setCursor(40, 568);
  display.println(F("Full refresh; then 60 s rest"));
}

} // namespace

void setup()
{
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print(F("RadioCore DEPG1020BNS770F1 example: "));
  Serial.println(F(RADIOCORE_DEPG1020BNS770F1_BOARD_NAME));
}

void loop()
{
  ++refreshCount;
  Serial.print(F("Starting full refresh "));
  Serial.println(refreshCount);

  // Full-refresh mode also powers, resets, and initializes the driver board.
  display.fastmodeOff();
  if (!display.timedOut()) {
    DRAW(display) {
      drawTestPage();
    }
  }

  if (display.timedOut()) {
    Serial.println(F("E-paper BUSY timeout; the driver board was powered down."));
  } else {
    Serial.println(F("Refresh complete; the panel is asleep and power is off."));
  }

  delay(kRefreshRestMs);
}

#endif // RADIOCORE_HAS_DEPG1020BNS770F1_DISPLAY
