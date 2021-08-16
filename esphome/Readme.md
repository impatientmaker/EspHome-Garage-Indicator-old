# ESPHome.io files

### Garage indicator
This tells me if my garage door is open or not, fetching the state from Home Assistant, and displaying on a 0.96" two-color OLED display.

### Swimming Pool (Newswim temporarily)

This is a solar powered pool thermometer.  Currently designed to support two waterproof Dallas thermocouples, one for pool and one for jacuzzi, and an internal DHT to measure internal 
temporature of the device.  Also measures the battery level (tbd).   Variable deep sleep times depending on time of day, as I tend to use the jacuzzi in the evening and want it sampled 
more frequently.  While I don't care about the temp while I'm sleeping, I still have the device measure every hour over night so I can see trends.
