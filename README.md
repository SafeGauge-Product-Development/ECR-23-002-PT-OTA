## ECR-23-002 PT-OTA
***All aspects of the PT OTA ECR***

#### Includes:
1. Delphi UI - used on the PC side to facilitate OTA updates (using the PT OTA Firmware)
   1. PT OTA Firmware **Hex files** - for production use, the new version of 'app code' to run on the PT's 
      1. This is what's tested and will be released to Production
      2. What can be changed to add new features to the OTA capable PT's
2. Dongle Software - the embedded firmware used to make the dongle run and connect to the PC via USB and OTA capable PT's via Bluetooth
3. PT OTA Source Code - 


#### Version History
1. ##### Initial version provided from Steve [v0.1]
   - not in this repo, split into PT-OTA-Uploader & OTA Firmware
   1. original dongle
   2. original Delphi UI
   3. PT OTA firmware - contained the bug where it needs to start from charge
2. ##### Next version from Steve [v0.2]
   1. new dongle with increased baud rate
   2. new Delphi UI software, only compatible with new dongle
   3. updated PT OTA firmware - 

#### Instructions
- **See https://safegauge.atlassian.net/l/cp/DEkYf747 for current instructions**
- Production Instructions with more detail, to come