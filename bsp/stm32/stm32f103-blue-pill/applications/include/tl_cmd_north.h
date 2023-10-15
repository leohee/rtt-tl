#ifndef __TL_CMD_NORTH_H__
#define __TL_CMD_NORTH_H__

// north commands for pc
// $ <cmd><;><optional parameterrs ;><cr 0x0D>
//   <cmd><;><return values ;><cr>
enum {
	// monitoring and configuration
	N_DeviceID					= (1),		// R string <24				Read device identifier written to the laser in the factory.
	N_DeviceSN					= (2),		// R string	<24				Read device serial number
	N_FWrevision				= (3),		// R string < 255			Read device firmware revision
	N_Vendor					= (99),		// R string < 255			Read device vendor written to the laser in the factory
	N_DeviceStatus				= (4),		// R int 32bits				Read device status, binary decoding is required
	N_DeviceTemperature			= (5),		// R float .1f				Read module temperature in degree Celsius
	N_DigitalInterfaceStatus	= (10),		// R int 32bits				Read digital interface status, decimal to binary decoding is required 
												// (available for the lasers equipped with HP-A interface)
	N_ExtendedStatus			= (11),		// R int 32bits				Read device extended status binary decoding is required
	N_BRCounter					= (12),		// R int 32bits				Read back reflection counter
	N_SessionBRCounter			= (13),		// R int 32bits				Read back reflection counter for current session.
												// Session starts with supplying voltage to the laser module.
	N_NominalAveragePower		= (14),		// R float .1f				Read nominal average power of the laser in [W]
	N_NominalPulseDuration		= (15),		// R int 32bits				Read nominal pulse duration of the laser [ns]
	N_NominalPulseEnergy		= (16),		// R float .2f				Read nominal pulse energy of the laser [mJ]
	N_NominalPeakPower			= (17),		// R float .1f				Read nominal peak power of the laser in [kW].
												// Value is calculated via nominal energy and nominal pulse duration.
	N_PRRRange					= (18),		// R floats <min>;<max> kHz Read pulse repetition rates range.
												// Return value is two floats separated by semicolon, correspondingly for minimum and maximum PRRs in [kHz].
	N_OperatingModeRead			= (23),		// R int 16bits				Read control interface operating mode, binary decoding is required.
	N_OperatingModeSet			= (24),		// W int 16bits				Set control interface operating mode, binary decoding is required.
	N_InstalledOptions			= (25),		// R int 16bits				Read list of installed options and operating modes, binary decoding is required
	N_StartOperatingModeSet		= (26),		// W int 16bits				Set initial control interface operating mode, binary encoding is required. 
	N_StartOperatingModeRead	= (27),		// R int 16bits				Read control interface operating mode, 
												// which activates after connecting the laser to the supply voltage. 
	N_OperatingPowerW			= (33),		// R float .1f	in W		Read back operating power in [W] set by command 32 (in RS-232 mode) 
												// or via digital pins (in HP-A mode), but recalculated into Watts using nominal laser parameters
	N_OperatingPowerPercent		= (34),		// R float .1f	in %		Read back operating power in [%] set by command 32 (in RS-232 mode) 
												// or via digital pins (in HP-A mode), but recalculated into [%] using nominal laser parameters
	N_OperatingPulseEnergy		= (36),		// R float .2f	in mJ		Read operating pulse energy in [mJ]. Value is calculated using 
												// nominal laser parameters and power settings.
	N_PRRMonitor				= (38),		// R float .1f	in kHz		Read back real laser PRR in [kHz]

	// rs232 control interface
	N_PRRSet					= (28),		// W float .1f				Set operating pulse repetition rate in [kHz]
												// Note: changing of PRR is recommended when the laser is in “Laser Emission OFF” state.
	N_PRRRead					= (29),		// R float .1f				Read back operating pulse repetition rate in [kHz] set by command 28
	N_LaserEmissionON			= (30),		// W						Command to switch ON laser emission
	N_LaserEmissionOFF			= (31),		// W						Switch OFF laser emission.
	N_OperatingPower			= (32),		// W float .1f %			Set operating power in [%]. Range 0…100, resolution 255 levels for full scale
	N_GuideLaserON				= (40),		// W						Switch ON guide laser
	N_GuideLaserOFF				= (41),		// W						Switch OFF guide laser
	N_ResetAlarms				= (50),		// W						Reset alarms, see alarms description for details

	// extended prr mode
	N_minmaxPRR					= (46),		// R 						Read back minimum and maximum operating PRRs.
												// Return value is two float in [kHz] values separated by semicolon.

	// adjustable pulse duration mode
	N_PulseDurationRead			= (48),		// R in 16bits				Read back pulse duration in [ns] set by command 49
	N_PulseDurationSet			= (49),		// W in 16bits				Set optical pulse duration in [ns]. The set value
												// should correspond to one from the list returned by the command 51
	N_ListPulseDurations		= (51),		// R 16bits <int1>;<int2>;...;<intN>  Read list of preset pulse durations in [ns]. 
												// List of 16 bit integers separated by semicolon
	N_SavePulseDuration			= (54)		// W						Permanently save the pulse duration to EEPROM.
												// Next start the device will be initialized by saved pulse duration.
};

/* $4<cr>	N_DeviceStatus		"Read device status"
b0		Back reflection Alarm active.
b1		Temperature Alarm active.
b2		Reserved.
b3		System Alarm active
b4		Reserved.
b5		Reserved.
b6		Laser is ready for emission
b7		A least one of the warning is active
b8		Process fiber alarm
b8		Feeding fiber alarm
b10		Laser Safety relay is closed.
*/

/* $10<cr>	N_DigitalInterfaceStatus	"Read YLP-HP-A interface DB-25 status"
b0		Latched D0
b1		Latched D1
b2		Latched D2
b3		Latched D3
b4		Latched D4
b5		Latched D5
b6		Latched D6
b7		Latched D7
b8		pin 1	D0 power setting
b9		pin 2	D1 power setting
b10		pin 3	D2 power setting
b11		pin 4	D3 power setting
b12		pin 5	D4 power setting
b13		pin 6	D5 power setting
b14		pin 7	D6 power setting
b15		pin 8	D7 power setting
b16		pin 9	Latch
b17		pin 23	Auxiliary stop
b18		pin 19	Emission Modulation
b19		pin 22	Guide laser control
b20		pin 20	External Synchronization
b21		pin18	Emission enable
b22		Reserved
b23		Reserved
b24		pin 16	Alarm0
b25		pin 21	Alarm1
b26		pin 11	Alarm2
b27		Reserved
b28		Reserved
b29		Reserved
b30		Reserved
b31		Reserved
*/

/* $11<cr> 	N_ExtendedStatus	"Read device extended status"
b0		Auxiliary stop was actived
b1		External synchronization frequency on pin 20 is above specification
b2		External synchronization frequency on pin 20 is below specification
b3		Laser ON time is lower than speccification
b4		Laser OFF time is lower than speccification
b5		Guide laser was activated
b6		Reserved
b7		Reserved
b8		Laser emission is 1=ON (laser is pumped) / 0=OFF (laser isn't pumped)
b9		Reserved
b10		Reserved
b11		Laser emission 1=ON / 0=OFF command was received by RS232C
b12		Guide laser 1=ON / 0=OFF command was received by RS232C
b13		Reserved
b14		Reserved
b15		Reserved
*/

/* $23<cr> $27<cr> $24<cr> $26<cr>	Operating modes
b0		HP-A control interface is 1=active(to activate) / 0=not active(to deactivate)
b1		RS-232C control interface is 1=active(to activate) / 0=not active(to deactivate)
b2		Reserved
b3		Reserved
b4		Adjustable pulse duration mode is 1=active(to activate) / 0=not active(to deactivate)
b5		Extended PRR mode is1=active(to activate) / 0=not active(to deactivate)
b6		Reserved
b7		Emission Modulation signal (pin 8 of HAN DDD part A) is 1=in use(to use) / 0=not use
b8		Jump PRR mode is 1=active(to activate) / 0=not active(to deactivate)
b9		Reserved
b10		Accelerated laser ON time(ACON) mode is 1=active(to activate) / 0=not active(to deactivate)
b11		Reserved
*/

/* $25<cr>	N_InstalledOptions	"Read installed options and operationg modes"
b0		HP-A digital control interface is installed
b1		RS-232 control interface is installed
b2		Extended PRR mode option is installed
b3		Adjustable Pulse Duration option is installed
b4		Reserved
b5		Reserved
b6		Reserved
b7		Guide laser is installed
b8		High Contrast (HC) option is installed
b9		Reserved
b10		Accelerated laser ON time (ACON) mode is installed
b11		Reserved
*/


#endif	// __TL_CMD_NORTH_H__

