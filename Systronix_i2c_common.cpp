//Systronix_i2c_common.cpp
//
// This file holds functions that are common to all Systronix i2c libraries
//

#include <Systronix_i2c_common.h>

//---------------------------< T A L L Y _ T R A N S A C T I O N >--------------------------------------------
//
// This function is an attempt to centralize transaction tallying so that there is only one function but multiple
// error structs (each declared in the appropriate library)
//

void Systronix_i2c_common::tally_transaction (uint8_t value, error_t* error_ptr)
	{
	if (value && (error_ptr->total_error_count < UINT64_MAX))
		error_ptr->total_error_count++; 			// every time here incr total error count

	error_ptr->error_val = value;

	switch (value)
		{
		case SUCCESS:
			if (error_ptr->successful_count < UINT64_MAX)
				error_ptr->successful_count++;
			break;
		case 1:								// i2c_t3 and Wire: data too long from endTransmission() (rx/tx buffers are 259 bytes - slave addr + 2 cmd bytes + 256 data)
			error_ptr->data_len_error_count++;
			break;
#if defined I2C_T3_H
		case I2C_TIMEOUT:
			error_ptr->timeout_count++;			// 4 from i2c_t3; timeout from call to status() (read)
#else
		case 4:
			error_ptr->other_error_count++;		// i2c_t3 and Wire: from endTransmission() "other error"
#endif
			break;
		case 2:								// i2c_t3 and Wire: from endTransmission()
		case I2C_ADDR_NAK:					// 5 from i2c_t3
			error_ptr->rcv_addr_nack_count++;
			break;
		case 3:								// i2c_t3 and Wire: from endTransmission()
		case I2C_DATA_NAK:					// 6 from i2c_t3
			error_ptr->rcv_data_nack_count++;
			break;
		case I2C_ARB_LOST:					// 7 from i2c_t3; arbitration lost from call to status() (read)
			error_ptr->arbitration_lost_count++;
			break;
		case I2C_BUF_OVF:
			error_ptr->buffer_overflow_count++;
			break;
		case I2C_SLAVE_TX:
		case I2C_SLAVE_RX:
			error_ptr->other_error_count++;		// 9 & 10 from i2c_t3; these are not errors, I think
			break;
		case WR_INCOMPLETE:					// 11; Wire.write failed to write all of the data to tx_buffer
			error_ptr->incomplete_write_count++;
			break;
		case SILLY_PROGRAMMER:				// 12
			error_ptr->silly_programmer_error++;
			break;
		default:
			error_ptr->unknown_error_count++;
			break;
		}
	}


