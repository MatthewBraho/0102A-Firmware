# 0102A-Firmware
# Branches
main: The final iteration of firmware which is used in the current prototype <br />
adam_dev: Adam’s branch which he uses to add, edit, and make changes to the code <br />
michael_dev: Michael’s branch which he uses to add, edit, and make changes to the code <br />
oliver_dev: Oliver’s branch which he uses to add, edit, and make changes to the code <br />

In adam_dev, michael_dev, and oliver_dev, when their firmware is verified to work with testing as per the firmware and electrical specifications (0102A-SP-Specification_Guide_V2), a pull request to main is created, which adds their changes to the main branch. The firmware and electrical specifications are as follows:

EF1. Moisture data must be transmitted from several probes to one central receiver. <br />
EF2. Central receivers must be able to request probe operational status. <br />
EF3. On the central receiver, an LCD screen must inform the user when there is no signal being received from a probe. <br />
EF4. On the central receiver, an LCD screen must change to display different moisture information from a different probe upon button press. <br />
EF5. The LCD screen must display how much volume of water the farmer needs to add to the probe location to reach the optimal soil moisture. <br />
EF6. An LED matrix must light up to identify when the moisture value of a probe is in the “dry” range. <br />
EF7. Wires should be long enough to accommodate specification S1. <br />
EF8. Prototype should be powered by batteries. <br />
EF9. The interior electronics can be easily accessed without permanently modifying the design. <br />
EF10. Wiring is not too long or short, and fits tightly above the breadboard. <br />

# File Organization
Files are organized such that each folder is a component of the prototyped design along with a systems integration folder, which is the code which combines all of the components together. Within each component folder, there are folders for each iteration taken within the component. The iteration folders contain the code written for that iteration. All the design iterations and processes explaining and discussing the effectiveness of the firmware are documented in the Firmware Design Report (0102A-Firmware-Design_Report) in the Design Dossier. 
