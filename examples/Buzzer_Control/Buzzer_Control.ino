/*
 * Plays the Meshtastic default RTTTL notification melody through the
 * board-designated buzzer output: RC32 GPIO48 or RC52 P0.05. RCC6 has no
 * supported buzzer output.
 *
 * Playback is non-blocking. The melody repeats five seconds after it finishes.
 * Define BUZZER_RTTTL or BUZZER_REPLAY_INTERVAL_MS before this sketch content
 * to use a different melody or replay interval.
 */
#ifndef BUZZER_RTTTL
#define BUZZER_RTTTL                                                          \
  "24:d=32,o=5,b=565:f6,p,f6,4p,p,f6,p,f6,2p,p,b6,p,b6,p,b6,p,b6,p,b,p," \
  "b,p,b,p,b,p,b,p,b,p,b,p,b,1p.,2p.,p"
#endif

#ifndef BUZZER_REPLAY_INTERVAL_MS
#define BUZZER_REPLAY_INTERVAL_MS 5000UL
#endif

#include <Arduino.h>
#include <RadioCore_Kit.h>

#if !RADIOCORE_HAS_BUZZER
#error "Buzzer_Control requires a RadioCore board with a supported buzzer output."
#else

#include <NonBlockingRtttl.h>

namespace {

constexpr char kRingtone[] = BUZZER_RTTTL;

uint32_t playbackFinishedAt = 0;
bool replayPending = false;

void startPlayback(bool replay)
{
  rtttl::begin(RADIOCORE_BUZZER_PIN, kRingtone);
  replayPending = false;
  Serial.println(
      replay ? F("Buzzer replay started") : F("Buzzer playback started"));
}

} // namespace

void setup()
{
  Serial.begin(115200);

  pinMode(RADIOCORE_BUZZER_PIN, OUTPUT);
  digitalWrite(RADIOCORE_BUZZER_PIN, LOW);

#if RADIOCORE_HAS_SENSOR_POWER_CTRL
  pinMode(RADIOCORE_SENSOR_POWER_CTRL_PIN, OUTPUT);
  digitalWrite(
      RADIOCORE_SENSOR_POWER_CTRL_PIN,
      RADIOCORE_SENSOR_POWER_ON);
  delay(RADIOCORE_SENSOR_POWER_WARMUP_MS);
#endif

  Serial.println();
  Serial.println(F("RadioCore buzzer control example"));
  startPlayback(false);
}

void loop()
{
  if (rtttl::isPlaying()) {
    rtttl::play();

    if (!rtttl::isPlaying()) {
      playbackFinishedAt = millis();
      replayPending = true;
      Serial.println(F("Buzzer playback finished"));
    }

    return;
  }

  if (replayPending &&
      millis() - playbackFinishedAt >= BUZZER_REPLAY_INTERVAL_MS) {
    startPlayback(true);
  }
}

#endif // RADIOCORE_HAS_BUZZER
