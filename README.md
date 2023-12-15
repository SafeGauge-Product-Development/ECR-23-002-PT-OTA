## ECR-23-002 PT-OTA
***All aspects of the PT OTA ECR***

#### Includes:
1. **Bootloader** - Used on the PC side to modify the bootloader after compiling it in MPLAB
      1. source code to modify the bootloader.
      2. MPLAB source code for the bootloader
2. **Delphi UI** - used on the PC side to facilitate OTA updates (using the PT OTA Firmware)
   **1. Production App - Production Use**
      1. PT OTA Firmware **Hex files** - for production use, the new version of 'app code' to run on the PT's 
         1. This is what's tested and will be released to Production
         2. What can be changed to add new features to the OTA capable PT's
3. **Dongle Software** - the embedded firmware used to make the dongle run and connect to the PC via USB and OTA capable PT's via Bluetooth
   1. source code to modify things like baud rate
4. **PT OTA Source Code**

#### Version History
1. ##### Initial version provided from Steve [v0.1]
   - not in this repo, split into PT-OTA-Uploader & OTA Firmware
   1. original dongle
   2. original Delphi UI
   3. PT OTA firmware - contained the bug where it needs to start from charge
2. ##### Next version from Steve [v0.2]
   1. new dongle with increased baud rate
   2. new Delphi UI software, only compatible with new dongle
   3. updated PT OTA firmware
3. ##### Added booloader files [v0.3]
   1. bootloader Delphi source code.
   2. Booloader MPLAB source code.

#### Current Instructions
- **See https://safegauge.atlassian.net/l/cp/DEkYf747 for current instructions**
- Production Instructions with more detail, to come

#### Figma Flow outlining the OTA sequence.
 - https://www.figma.com/file/KI1gS9zljPgQ6eukQwufK3/PT-OTA---ECR-23-003?type=whiteboard&node-id=0-1&t=t2fqzATc3eH29OGi-0
 