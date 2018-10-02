#ifndef SYSTRONIX_I2C_COMMON_h
#define SYSTRONIX_I2C_COMMON_h

//---------------------------< I N C L U D E S >--------------------------------------------------------------
//
// At this writing (13 May 2018), the code in this .h and its accompanying .ccp is not tested and known to work
// with wire.h.
//

#include <Arduino.h>

#if defined (__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || \
                           defined(__MK64FX512__) || defined(__MK66FX1M0__) // 3.0/3.1-3.2/LC/3.5/3.6
#include <i2c_t3.h>	// for Teensy 3 family use optimized and more robust library
#else
#include <Wire.h>	// for AVR I2C library
#endif


//---------------------------< D E F I N E S >----------------------------------------------------------------
//
// These defines are common to all Systronix i2c libraries
//

#define		SUCCESS				0
#define		FAIL				(~SUCCESS)
#define		ABSENT				0xFD

#define		WR_INCOMPLETE		11
#define		SILLY_PROGRAMMER	12


typedef	struct error_t
	{
	bool		exists;								// set false after an unsuccessful attempt to init the device
	uint8_t		error_val;							// the most recent error value, not just SUCCESS or FAIL
	uint32_t	incomplete_write_count;				// Wire.write failed to write all of the data to tx_buffer
	uint32_t	data_len_error_count;				// data too long
	uint32_t	timeout_count;						// slave response took too long
	uint32_t	rcv_addr_nack_count;				// slave did not ack address
	uint32_t	rcv_data_nack_count;				// slave did not ack data
	uint32_t	arbitration_lost_count;
	uint32_t	buffer_overflow_count;
	uint32_t	other_error_count;					// from endTransmission there is "other" error
	uint32_t	unknown_error_count;
	uint32_t	data_value_error_count;				// I2C message OK but value read was wrong; how can this be?
	uint32_t	silly_programmer_error;				// I2C address to big or something else that "should never happen"
	uint64_t	total_error_count;					// quick check to see if any have happened
	uint64_t	successful_count;					// successful access cycle
	} error_t;


class Systronix_i2c_common
	{
	public:
		void	tally_transaction (uint8_t value, error_t* error_ptr);

	protected:
	private:
	};

extern Systronix_i2c_common i2c_common;	

#endif		// SYSTRONIX_I2C_COMMON_h