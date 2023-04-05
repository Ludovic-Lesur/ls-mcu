/*
 * string.c
 *
 *  Created on: 05 dec. 2021
 *      Author: Ludo
 */

#include "string.h"

#include "math.h"
#include "mode.h"
#include "stdint.h"

#ifdef DEBUG

/*** STRING local macros ***/

#define STRING_DIGIT_DECIMAL_MAX			9
#define STRING_DIGIT_HEXADECIMAL_MAX		0x0F

#define STRING_FORMAT_BINARY_MAX_BITS		32
#define STRING_FORMAT_HEXADECIMAL_MAX_BYTES	4
#define STRING_FORMAT_DECIMAL_MAX_DIGITS	10
#define STRING_FORMAT_ASCII_MAX_VALUE		0xFF

/*** STRING functions ***/

/* CONVERTS THE ASCII CODE OF AN HEXADECIMAL CHARACTER TO THE CORRESPONDING A 4-BIT WORD.
 * @param ascii_code:	Hexadecimal ASCII code to convert.
 * @return value:		Corresponding value.
 */
uint8_t STRING_charo_value(char ascii_code) {
	uint8_t value = 0;
	if ((ascii_code >= 'A') && (ascii_code <= 'F')) {
		value = ascii_code - 'A' + 10;
	}
	if ((ascii_code >= '0') && (ascii_code <= '9')) {
		value = ascii_code - '0';
	}
	return value;
}

/* RETURN CORRESPONDING ASCII CHARACTER OF A GIVEN DECIMAL VALUE.
 * @param value:		Decimal digit.
 * @return ascii_code:	Corresponding ASCII code.
 */
char STRING_decimal_to_ascii(uint8_t decimal_digit) {
	char ascii_code = 0;
	if (decimal_digit <= STRING_DIGIT_DECIMAL_MAX) {
		ascii_code = decimal_digit + '0';
	}
	return ascii_code;
}

/* CONVERTS A 4-BITS VARIABLE TO THE ASCII CODE OF THE CORRESPONDING HEXADECIMAL CHARACTER IN ASCII.
 * @param n:			Hexadecimal digit.
 * @return ascii_code:	Corresponding ASCII code.
 */
char STRING_hexa_to_ascii(uint8_t hexa_digit) {
	char ascii_code = 0;
	if (hexa_digit <= STRING_DIGIT_HEXADECIMAL_MAX) {
		ascii_code = (hexa_digit <= 9 ? (char) (hexa_digit + '0') : (char) (hexa_digit + ('A' - 10)));
	}
	return ascii_code;
}

/* CHECK IF A GIVEN ASCII CODE CORRESPONDS TO AN HEXADECIMAL CHARACTER.
 * @param ascii_code:	The byte to analyse.
 * @return:				1 if the byte is the ASCII code of an hexadecimal character, 0 otherwise.
 */
uint8_t STRING_is_hexadecimal_char(char ascii_code) {
	return (((ascii_code >= '0') && (ascii_code <= '9')) || ((ascii_code >= 'A') && (ascii_code <= 'F')));
}

/* CHECK IF A GIVEN ASCII CODE CORRESPONDS TO A DECIMAL CHARACTER.
 * @param ascii_code:	The byte to analyse.
 * @return:				1 if the byte is the ASCII code of a decimal character, 0 otherwise.
 */
uint8_t STRING_is_decimal_char(char ascii_code) {
	return ((ascii_code >= '0') && (ascii_code <= '9'));
}

/* CONVERT A VALUE TO A STRING ACCORDING TO INUT FORMAT.
 * @param value:        Value to print.
 * @param format:       Printing format.
 * @param print_prefix: Print base prefix is non zero.
 * @param string:       Output string.
 */
void STRING_value_to_string(int32_t value, STRING_format_t format, uint8_t print_prefix, char* string) {
    // Local variables.
	uint32_t value_abs;
	uint8_t first_non_zero_found = 0;
	uint32_t idx;
    uint32_t string_idx = 0;
	uint8_t generic_byte = 0;
	uint32_t current_power = 0;
	uint32_t previous_decade = 0;
	// Manage negative numbers.
	if (value < 0) {
		string[string_idx++] = STRING_CHAR_MINUS;
		value_abs = (uint32_t) ((-1) * value);
	}
	else {
		value_abs = (uint32_t) (value);
	}
	// Build string according to format.
	switch (format) {
	case STRING_FORMAT_BOOLEAN:
		if (print_prefix != 0) {
			// Print "0b" prefix.
            string[string_idx++] = '0';
            string[string_idx++] = 'b';
		}
		for (idx=(STRING_FORMAT_BINARY_MAX_BITS - 1) ; idx>=0 ; idx--) {
			if (value_abs & (0b1 << idx)) {
				string[string_idx++] = '1';
				first_non_zero_found = 1;
			}
			else {
				if ((first_non_zero_found != 0) || (idx == 0)) {
					string[string_idx++] = '0';
				}
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case STRING_FORMAT_HEXADECIMAL:
		if (print_prefix != 0) {
			// Print "0x" prefix.
			string[string_idx++] = '0';
			string[string_idx++] = 'x';
		}
		for (idx=(STRING_FORMAT_HEXADECIMAL_MAX_BYTES - 1) ; idx>=0 ; idx--) {
			generic_byte = (value_abs >> (8 * idx)) & 0xFF;
			if (generic_byte != 0) {
				first_non_zero_found = 1;
			}
			if ((first_non_zero_found != 0) || (idx == 0)) {
				string[string_idx++] = STRING_hexa_to_ascii((generic_byte & 0xF0) >> 4);
				string[string_idx++] = STRING_hexa_to_ascii(generic_byte & 0x0F);
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case STRING_FORMAT_DECIMAL:
		if (print_prefix != 0) {
			// Print "0d" prefix.
			string[string_idx++] = '0';
			string[string_idx++] = 'd';
		}
		for (idx=(STRING_FORMAT_DECIMAL_MAX_DIGITS - 1) ; idx>=0 ; idx--) {
			current_power = MATH_pow_10(idx);
			generic_byte = (value_abs - previous_decade) / current_power;
			previous_decade += generic_byte * current_power;
			if (generic_byte != 0) {
				first_non_zero_found = 1;
			}
			if ((first_non_zero_found != 0) || (idx == 0)) {
				string[string_idx++] = generic_byte + '0';
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case STRING_FORMAT_ASCII:
		// Raw byte.
		if (value_abs <= STRING_FORMAT_ASCII_MAX_VALUE) {
			string[string_idx++] = value_abs;
		}
		break;
	}
    // End string.
    string[string_idx++] = STRING_CHAR_NULL;
}

#endif /* DEBUG */
