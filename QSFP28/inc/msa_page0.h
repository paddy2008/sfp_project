/*
 * msa_page0.h
 *
 *  Created on: 2017-10-18
 *      Author: cuilin
 */

#ifndef MSA_PAGE0_H_
#define MSA_PAGE0_H_

typedef enum
{
	SR4 = 0x0,
	AOC = 0x1,
}SFP_TYPE;


/***************************************PAGE 00h**********************************
Byte 	Size 	Name 	Description 	PC 	AC 	AO 	SM
128	1	Identifier 	"Identifier Type of free sidedevice (See SFF-8024Transceiver Management)"	R 	R 	R 	R
129	1	Ext. Identifier 	"Extended Identifier of freeside device. Includes powerclasses, CLEI codes, CDRcapability (See Table 6-16)"	R 	R 	R 	R
130	1	Connector Type 	"Code for media connector type(See SFF-8024 TransceiverManagement)"	R 	R 	R 	R
"131-138"	8	"SpecificationCompliance"	"Code for electronic or opticalcompatibility (See Table 6-17)"	R 	R 	R 	R
139	1	Encoding 	"Code for serial encodingalgorithm. (See SFF-8024Transceiver Management)"	R 	R 	R 	R
140	1	BR, nominal 	"Nominal bit rate, units of 100Mbps. For BR > 25.4G, set thisto FFh and use Byte 222."	R 	R 	R 	R
141	1	"Extended RateSelectCompliance"	"Tags for extended rate selectcompliance"	R 	R 	R 	R
142	1	Length (SMF) 	"Link length supported for SMFfiber in km *"	R 	R 	R 	R
143	1	"Length (OM3 50um)"	"Link length supported for EBW50/125 um fiber (OM3), units of2 m *"	R 	R 	R 	R
144	1	"Length (OM2 50um)"	"Link length supported for50/125 um fiber (OM2), units of1 m *"	R 	R 	R 	R
145	1	"Length (OM1 62.5um)"	"Link length supported for62.5/125 um fiber (OM1), unitsof 1 m *"	R 	R 	R 	R
146	1	"Length (passivecopper or activecable or OM4 50um)"	"Length of passive or activecable assembly (units of 1 m)or link length supported forOM4 50/125 um fiber (units of 2m) as indicated by Byte 147.See 6.3.12."	R 	R 	R 	R
147	1	"Devicetechnology"	"Device technology (Table 6-18and Table 6-19)"	R 	R 	R 	R
"148-163"	16	Vendor name 	"Free side device vendor name(ASCII)"	R 	R 	R 	R
164	1	Extended Module 	"Extended Module codes forInfiniBand (See Table 6-21 )"	R 	R 	R 	R
"165-167"	3	Vendor OUI 	"Free side device vendor IEEEcompany ID"	R 	R 	R 	R
"168-183"	16	Vendor PN 	"Part number provided by freeside device vendor(ASCII)"	R 	R 	R 	R
"184-185"	2	Vendor rev 	"Revision level for part numberprovided by vendor(ASCII)"	R 	R 	R 	R
"186-187"	2	"Wavelength orCopper CableAttenuation"	"Nominal laser wavelength(wavelength=value/20 in nm) orcopper cable attenuation in dBat 2.5 GHz (Byte 186) and 5.0GHz (Byte 187)"	R 	R 	R 	R
"188-189"	2	"Wavelengthtolerance orCopper CableAttenuation"	"Guaranteed range of laserwavelength(+/- value) fromnominal wavelength.(wavelengthTol.=value/200 in nm) or coppercable attenuation in dB at 7.0GHz (Byte 188) and 12GHz (Byte189)"	R 	R 	R 	R
190	1	Max case temp. 	"Maximum case temperature indegrees C"	R 	R 	R 	R
191	1	CC_BASE 	"Check code for base ID fields(Bytes 128-190)"	R 	R 	R 	R
192	1	Link codes 	"Extended SpecificationCompliance Codes (See SFF-8024Transceiver Management)"	R 	R 	R 	R
"193-195"	3	Options 	"Rate Select, TX Disable, TXFault, LOS, Warning indicatorsfor: Temperature, VCC, RXpower, TX Bias, TX EQ, AdaptiveTX EQ, RX EMPH, CDR Bypass, CDRLOL Flag. See Table 6-22."	R 	R 	R 	R
"196-211"	16	Vendor SN 	"Serial number provided byvendor (ASCII)"	R 	R 	R 	R
"212-219"	8	Date Code 	"Vendor's manufacturing datecode"	R 	R 	R 	R
220	1	"DiagnosticMonitoring Type"	"Indicates which type ofdiagnostic monitoring isimplemented (if any) in thefree side device. Bit 1,0Reserved. See Table 6-24."	R 	R 	R 	R
221	1	Enhanced Options 	"Indicates which optionalenhanced features areimplemented in the free sidedevice. See Table 6-25."	R 	R 	R 	R
222	1	BR, nominal 	"Nominal bit rate per channel,units of 250 Mbps. ComplementsByte 140. See Table 32A."	R 	R 	R 	R
223	1	CC_EXT 	"Check code for the Extended IDFields (Bytes 192-222)"	R 	R 	R 	R
"224-255"	32	Vendor Specific 	Vendor Specific EEPROM 	- 	- 	- 	-
*******************************************************************************/
#define MSA_OFFSET_PAGE_00H_START                       128
#define MSA_P0_CHECK_CODE_BASE_START					MSA_OFFSET_PAGE_00H_START

#define MSA_OFFSET_PAGE_00H_IDENTIFIER                  128    
#define MSA_OFFSET_PAGE_00H_EXT_IDENTIFIER              129
#define MSA_OFFSET_PAGE_00H_CONNECTOR                   130
#define MSA_OFFSET_PAGE_00H_TRANSCEIVER                 131
#define MSA_OFFSET_PAGE_00H_SONET_COMP_CODES            132
#define MSA_OFFSET_PAGE_00H_SAS_COMP_CODES              133
#define MSA_OFFSET_PAGE_00H_GI_ETH_COMP_CODES           134
#define MSA_OFFSET_PAGE_00H_FIBER_CHN_LINK_LEN          135
#define MSA_OFFSET_PAGE_00H_FIBER_CHN_TX_TECH           136
#define MSA_OFFSET_PAGE_00H_FIBER_CHN_TX_MEDIA          137
#define MSA_OFFSET_PAGE_00H_FIBER_CHN_SPEED             138
#define MSA_OFFSET_PAGE_00H_ENCODING                    139
#define MSA_OFFSET_PAGE_00H_BR_NOMINAL_1                140
#define MSA_OFFSET_PAGE_00H_EXTENDED_RATESELECT         141
#define MSA_OFFSET_PAGE_00H_LEN_SMF                     142
#define MSA_OFFSET_PAGE_00H_LEN_OM3                  	143
#define MSA_OFFSET_PAGE_00H_LEN_OM2                    	144
#define MSA_OFFSET_PAGE_00H_LEN_OM1                  	145          
#define MSA_OFFSET_PAGE_00H_LEN_COPPER                  146
#define MSA_OFFSET_PAGE_00H_DEV_TECH                    147
#define MSA_OFFSET_PAGE_00H_VENDOR_NAME                 148
#define MSA_OFFSET_PAGE_00H_EXT_TRANS                   164
#define MSA_OFFSET_PAGE_00H_VENDOR_OUI                  165
#define MSA_OFFSET_PAGE_00H_VENDOR_PN                   168
#define MSA_OFFSET_PAGE_00H_VENDOR_REV                  184
#define MSA_OFFSET_PAGE_00H_WAVELENGTH                  186
#define MSA_OFFSET_PAGE_00H_WAVELENGTH_TOLERANCE        188
#define MSA_OFFSET_PAGE_00H_MAX_CASE_TEMP               190
#define MSA_P0_CHECK_CODE_BASE_END						MSA_OFFSET_PAGE_00H_MAX_CASE_TEMP
#define MSA_P0_CHECK_CODE_BASE                 			191

#define MSA_OFFSET_PAGE_00H_EXT_SPEC_COMP_CODE          192
#define MSA_P0_CHECK_CODE_EXT_START						MSA_OFFSET_PAGE_00H_EXT_SPEC_COMP_CODE

#define MSA_OFFSET_PAGE_00H_OPTIONS_193                 193
#define MSA_OFFSET_PAGE_00H_OPTIONS_193_TX_EQ_AUTO      0x8
#define MSA_OFFSET_PAGE_00H_OPTIONS_193_TX_EQ_FIXED     0x4
#define MSA_OFFSET_PAGE_00H_OPTIONS_193_RX_EMPHASIS     0x2
#define MSA_OFFSET_PAGE_00H_OPTIONS_193_RX_AMPLITUDE    0x1

#define MSA_OFFSET_PAGE_00H_OPTIONS_194                 194
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_TX_ONOFF        0x80
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_ONOFF        0x40
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_TX_LOL        	0x20
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_LOL        	0x10
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_SQUELCH      0x08
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_LOL        	0x10
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_LOL        	0x10
#define MSA_OFFSET_PAGE_00H_OPTIONS_194_RX_LOL        	0x10


#define MSA_OFFSET_PAGE_00H_OPTIONS_195                 195



#define MSA_OFFSET_PAGE_00H_VENDOR_SN                   196
#define MSA_OFFSET_PAGE_00H_DATE_CODE                   212
#define MSA_OFFSET_PAGE_00H_DIAG_MONITOR                220
#define MSA_OFFSET_PAGE_00H_ENHANCE_OPT                 221
#define MSA_OFFSET_PAGE_00H_BR_NOMINAL_2                222
#define MSA_P0_CHECK_CODE_EXT_END						MSA_OFFSET_PAGE_00H_BR_NOMINAL_2

#define MSA_P0_CHECK_CODE_EXT                 			223
#define MSA_OFFSET_PAGE_00H_VENDOR_SPEC                 224
#define MSA_OFFSET_PAGE_00H_MAX                         255

/*
#define MSA_VENDOR_NAME_SIZE			16
#define MSA_PAGE_00H_VENDOR_NAME	'C', 'I', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '

#define MSA_VENDOR_OUI_SIZE				3
#define MSA_PAGE_00H_VENDOR_OUI		0, 0, 0

#define MSA_VENDOR_PN_SIZE				16
#define MSA_PAGE_00H_VENDOR_PN		'C', 'I', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '

#define MSA_VENDOR_REV_SIZE				2
#define MSA_PAGE_00H_VENDOR_REV		' ', ' '

#define MSA_VENDOR_SN_SIZE				16
#define MSA_PAGE_00H_VENDOR_SN		'C', 'I', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '

#define MSA_DATE_CODE_SIZE				8
#define MSA_PAGE_00H_DATE_CODE		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '

#define MSA_PAGE_00H_OPTIONS_193		0x0f	// 193
#define MSA_PAGE_00H_OPTIONS_194		0xff	// 194
#define MSA_PAGE_00H_OPTIONS_195		0x1a	// 195, page 1/2 not provided


#if defined(EFM8LB12E64_100G_SFP_AOC_101)
#define MSA_PAGE_00H_EXT_ID_VAL			0xcc	// 129, power class, max 3.5W
#define MSA_PAGE_00H_CONNECTOR			0xc		// 130, MPO (Multifiber Parallel Optic)
#define MSA_PAGE_00H_LEN_SMF			0x0		// 142, do not support single mode
#define MSA_PAGE_00H_LEN_OM3			0x25	// 143, 75m.   2000 MHz*km (850 nm) 50 micron multi-mode fiber
#define MSA_PAGE_00H_LEN_OM4			0x32	// 146, 100m.  4700 MHz*km (850 nm) 50 micron multi-mode fiber
#define MSA_PAGE_00H_DEV_TECH			0x0		// 147, Transmitter not tunable, Pin detector, Uncooled transmitter, No wavelength control, 850 nm VCSEL
#define MSA_PAGE_00H_WAVELEN_MSB		0x42	// 186, 850nm
#define MSA_PAGE_00H_WAVELEN_LSB		0x68	// 187, 850nm
#define MSA_PAGE_00H_WAVELEN_TOLRN_MSB	0x07	// 188, 1.05nm tolerance
#define MSA_PAGE_00H_WAVELEN_TOLRN_LSB	0xd0	// 189, 1.05nm tolerance
#define MSA_PAGE_00H_EXT_SPEC_COMP_CODE	0x01	// 192, aoc

#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
#define MSA_PAGE_00H_EXT_ID_VAL			0xcc	// 129, power class, max 3.5W
#define MSA_PAGE_00H_CONNECTOR			0x7		// 130, LC (Lucent Connector)
#define MSA_PAGE_00H_LEN_SMF			0x2		// 142, 2km single mode
#define MSA_PAGE_00H_LEN_OM3			0x0		// 143, do not support multi-mode
#define MSA_PAGE_00H_LEN_OM4			0x0		// 146, do not support multi-mode
#define MSA_PAGE_00H_DEV_TECH			0x40	// 147, Transmitter not tunable, Pin detector, Uncooled transmitter, No wavelength control, 1310 nm DFB
#define MSA_PAGE_00H_WAVELEN_MSB		0x66	// 186, 1310nm
#define MSA_PAGE_00H_WAVELEN_LSB		0x58	// 187, 1310nm
#define MSA_PAGE_00H_WAVELEN_TOLRN_MSB	0x00	// 188, 1.05nm tolerance
#define MSA_PAGE_00H_WAVELEN_TOLRN_LSB	0xd2	// 189, 1.05nm tolerance
#define MSA_PAGE_00H_EXT_SPEC_COMP_CODE	0x09	// 192, cwdm4
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
#define MSA_PAGE_00H_EXT_ID_VAL			0xce	// 129, high power class, max 4.5W
#define MSA_PAGE_00H_CONNECTOR			0x7		// 130, LC (Lucent Connector)
#define MSA_PAGE_00H_LEN_SMF			0xa		// 142, 10km single mode
#define MSA_PAGE_00H_LEN_OM3			0x0		// 143, do not support multi-mode
#define MSA_PAGE_00H_LEN_OM4			0x0		// 146, do not support multi-mode
#define MSA_PAGE_00H_DEV_TECH			0x44	// 147, Transmitter not tunable, Pin detector, Cooled transmitter, No wavelength control, 1310 nm DFB 
#define MSA_PAGE_00H_WAVELEN_MSB		0x66	// 186, 1310nm
#define MSA_PAGE_00H_WAVELEN_LSB		0x58	// 187, 1310nm
#define MSA_PAGE_00H_WAVELEN_TOLRN_MSB	0x00	// 188, 1.05nm tolerance
#define MSA_PAGE_00H_WAVELEN_TOLRN_LSB	0xd2	// 189, 1.05nm tolerance
#define MSA_PAGE_00H_EXT_SPEC_COMP_CODE	0x03	// 192, lr4
#endif

*/

void	MSA_p0_init(QSFP_MSA_PAGE_T *page);

void 	MSA_p0_eeprom_init(void);

uint8_t MSA_p0_read_by_isr(uint8_t offset);

uint8_t MSA_p0_write_by_isr(uint8_t offset, uint8_t value);


#endif /* MSA_PAGE0_H_ */
