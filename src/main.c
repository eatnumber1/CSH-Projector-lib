#import <stdio.h>
#import <stdlib.h>

#import "cerror.h"
#import "nec.h"

void die( cerror_t *err ) __attribute__((noreturn));
void die( cerror_t *err ) {
	fprintf(stderr, "%s", err->message);
	if( err->type == CERROR_ID_NEC && err->code == NEC_ERR_PROTOCOL ) {
		nec_error_t *nec_err = (nec_error_t *) err;
		fprintf(stderr, ": %s: %s", nec_err->type.message, nec_err->subtype.message);
	}
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

int main() {
	nec_library_init();

	nec_header_t header = {
		.id.command = NEC_COMMAND_RUNNING_SENSE,
		.projector_id = NEC_PROJECTOR_ID_BROADCAST_1,
		.model = NEC_MODEL_BROADCAST_1,
		.data_length = 0
	};
	
	cerror_t *err = NULL;
	if( !nec_write(&err, &header, NULL, stdout) ) die(err);

	nec_data_t *data = nec_read(&err, stdin);
	if( err != NULL ) die(err);
	printf("nec_data_t {\n");
	printf("\tpowering_on_off = %x\n", data->running_sense.powering_on_off);
	printf("\t...\n");
	printf("}\n");

	nec_library_destroy();
}
