#import <stdbool.h>
#import <stdlib.h>
#import <stdint.h>
#import <stddef.h>
#import <string.h>
#import <assert.h>

#import <libc.h>

#import "nec.h"

static const char * const CERROR_DOMAIN_NEC = "CERROR_DOMAIN_NEC";
cerror_id_t CERROR_ID_NEC;
static bool initialized = false;

static uint8_t nec_checksum( const void *data, size_t size ) {
	const uint8_t *d = data;
	uint8_t checksum = 0;
	for( size_t i = 0; i < size; i++ ) {
		checksum += d[i];
	}
	return checksum;
}

bool nec_library_init() {
	if( initialized ) return true;
	CERROR_ID_NEC = cerr_create_id(CERROR_DOMAIN_NEC);
	if( !cerr_library_init() ) return false;
	initialized = true;
	return true;
}

void nec_library_destroy() {
	cerr_library_destroy();
	initialized = false;
}

nec_data_t *nec_read( cerror_t **error, FILE *stream ) {
	cerror_t *err = NULL;
	// Read packet header
	nec_header_t header;
	fread_e(&err, &header, sizeof(header), 1, stream);
	if( err != NULL ) {
		cerr_propagate_error(error, err);
		return NULL;
	};
	
	// Read packet data
	uint8_t raw_data[header.data_length];
	fread_e(&err, raw_data, 1, header.data_length, stream);
	if( err != NULL ) {
		cerr_propagate_error(error, err);
		return NULL;
	};
	
	// Read checksum
	uint8_t expected_checksum;
	fread_e(&err, &expected_checksum, sizeof(uint8_t), 1, stream);
	if( err != NULL ) {
		cerr_propagate_error(error, err);
		return NULL;
	};
	
	// Check checksum
	uint8_t checksum = nec_checksum(&header, sizeof(header)) + nec_checksum(raw_data, header.data_length);
	if( checksum != expected_checksum ) {
		cerr_new_error(error, CERROR_ID_NEC, NEC_ERR_CHECKSUM, "Bad checksum", NULL);
		return NULL;
	}
	
	// Check for valid response
	if( !header.id.response.is_response ) {
		cerr_new_error(error, CERROR_ID_NEC, NEC_ERR_BAD_RESPONSE, "Bad response", NULL);
		return NULL;
	}

	// Check for ack
	if( !header.id.response.ack ) {
		cerror_t *proto_error;
		cerr_new_error(&proto_error, CERROR_ID_NEC, NEC_ERR_PROTOCOL, "Protocol error", NULL);
		nec_error_t *ret = realloc_e(&err, proto_error, sizeof(nec_error_t));
		if( ret != NULL ) {
			cerr_error_free(proto_error);
			cerr_propagate_error(error, err);
			return NULL;
		}
		nec_data_t *wire_error = (nec_data_t *) raw_data;
		const char *subtype_msg, *type_msg;
		switch( wire_error->error.type ) {
			case NEC_ERR_TYPE_NOT_SUPPORTED:
				type_msg = "Not supported";
				switch( wire_error->error.subtype.not_supported ) {
					case NEC_ERR_UNKNOWN_COMMAND:
						subtype_msg = "Unknown command";
						break;
					case NEC_ERR_FUNCTION_NOT_SUPPORTED:
						subtype_msg = "The current model does not support this function";
						break;
					default:
						subtype_msg = "Unknown error subtype";
				}
				break;
			case NEC_ERR_TYPE_PARAMETER_ERROR:
				type_msg = "Parameter error";
				switch( wire_error->error.subtype.parameter_error ) {
					case NEC_ERR_INVALID_VALUES:
						subtype_msg = "Invalid values specified";
						break;
					case NEC_ERR_TERMINAL_UNAVAILABLE:
						subtype_msg = "Specified terminal is unavailable or cannot be selected";
						break;
					default:
						subtype_msg = "Unknown error subtype";
				}
				break;
			default:
				type_msg = "Unknwon error type";
				subtype_msg = NULL;
		}
		ret->type.code = wire_error->error.type;
		ret->type.message = type_msg;
		ret->subtype.code = wire_error->error.subtype;
		ret->subtype.message = subtype_msg;
		cerr_propagate_error(error, (cerror_t *) ret);
		return NULL;
	}
	
	nec_data_t *data = malloc_e(&err, sizeof(nec_data_t));
	if( err != NULL ) {
		cerr_propagate_error(error, err);
		return NULL;
	}
	assert(header.data_length <= sizeof(nec_data_t));
	memcpy(data, raw_data, header.data_length);
	return data;
}

bool nec_write( cerror_t **err, const nec_header_t *header, const void *data, FILE *stream ) {
	// Write packet header
	cerror_t *error = NULL;
	fwrite_e(&error, header, sizeof(header), 1, stream);
	if( error != NULL ) {
		cerr_propagate_error(err, error);
		return false;
	}

	// Write data
	if( data != NULL ) {
		fwrite_e(&error, data, 1, header->data_length, stream);
		if( error != NULL ) {
			cerr_propagate_error(err, error);
			return false;
		}
	}

	// Write checksum
	uint8_t checksum = nec_checksum(header, sizeof(*header)) + nec_checksum(data, header->data_length);
	fwrite_e(&error, &checksum, sizeof(uint8_t), 1, stream);
	if( error != NULL ) {
		cerr_propagate_error(err, error);
		return false;
	}
	
	return true;
}
