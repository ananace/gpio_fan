GPIO Fan daemon
===============

A small daemon for controlling a fan on a GPIO pin, similar to the dtoverlay gpio-fan.

Default configuration is to handle a fan on pin 18, activating it at 55°C and deactivating it again at 45°C.

The `-t`/`--temp` option will set a range in the same way as the dtoverlay gpio-fan, though with floating point values. (so 75°C would be 75, whereas the gpio-fan dtoverlay would want the value 75000)

```
Usage: gpio_fan [OPTIONS...]

Args:
  -p --pin PIN      Set the GPIO pin to toggle
  -n --interval SEC Set the delay between temperature checks (in seconds)
  -t --temp TEMP    Set temperature range as in gpio-fan

  -m --min MIN      Set turn-off point
  -M --max MAX      Set turn-on point
```

A systemd unit is provided which is suitable for use with Raspberry Pi OS or Debian, for Alpine or other non-systemd dists you can run it with rc.local or similar methods.
