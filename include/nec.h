#import <cerror.h>

enum {
	NEC_ERR_PROTOCOL,
	NEC_ERR_CHECKSUM,
	NEC_ERR_BAD_RESPONSE
};
extern cerror_id_t CERROR_ID_NEC;

typedef enum {
	NEC_ERR_TYPE_NOT_SUPPORTED = 0x00,
	NEC_ERR_TYPE_PARAMETER_ERROR = 0x01
} nec_error_type_t;

typedef union {
	enum {
		NEC_ERR_UNKNOWN_COMMAND = 0x00,
		NEC_ERR_FUNCTION_NOT_SUPPORTED = 0x01
	} not_supported;
	enum {
		NEC_ERR_INVALID_VALUES = 0x00,
		NEC_ERR_TERMINAL_UNAVAILABLE = 0x01
	} parameter_error;
} nec_error_subtype_t;

typedef struct {
	cerror_t cerror;
	struct {
		nec_error_type_t code;
		const char *message;
	} type;
	struct {
		nec_error_subtype_t code;
		const char *message;
	} subtype;
} nec_error_t;

typedef struct __attribute__((packed)) {
	union {
		enum {
			NEC_COMMAND_RUNNING_SENSE = 0x0081
		} command;
		struct __attribute__((packed)) {
			uint8_t unused : 5;
			bool is_response : 1;
			uint8_t unused1 : 1;
			bool ack : 1;
			uint8_t unused2 : 8;
		} response;
	} id;
	enum {
		NEC_PROJECTOR_ID_BROADCAST_1 = 0x00,
		NEC_PROJECTOR_ID_BROADCAST_2 = 0xff
	} projector_id : 8;
	enum {
		NEC_MODEL_BROADCAST_1 = 0x00,
		NEC_MODEL_MT_SERIES = 0x01,
		NEC_MODEL_LT_SERIES = 0x02,
		NEC_MODEL_SX_SERIES = 0x03,
		NEC_MODEL_GT_SERIES = 0x05,
		NEC_MODEL_BROADCAST_2 = 0x0f
	} model : 4;
	uint16_t data_length : 12;
} nec_header_t;

typedef union {
	struct __attribute__((packed)) {
		bool powering_on_off : 1;
		bool selecting_signal : 1;
		bool cooling : 1;
		bool external_control : 1;
		enum {
			NEC_RUNNING_SENSE_POWER_OFF_POSSIBLE = 0x00,
			NEC_RUNNING_SENSE_POWER_OFF_IMPOSSIBLE = 0x01
		} power_off_period;
		bool reserved : 1;
		enum {
			NEC_RUNNING_SENSE_IDLING = 0x00,
			NEC_RUNNING_SENSE_ON = 0x01
		} status : 1;
		bool reserved2 : 1;
	} running_sense;
	struct __attribute__((packed)) {
		nec_error_type_t type;
		nec_error_subtype_t subtype;
	} error;
} nec_data_t;

nec_data_t *nec_read( cerror_t **err, FILE *stream );
bool nec_write( cerror_t **err, const nec_header_t *header, const void *data, FILE *stream );
bool nec_library_init();
void nec_library_destroy();
