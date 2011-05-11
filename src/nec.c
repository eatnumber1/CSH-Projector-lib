#include "include/nec.h"

#include <stdio.h>
#include <string.h>

static void init_err() {
    if (nec_err_msg != NULL) { return; }

    nec_err_msg = calloc(NEC_NUMBER_OF_ERRORS, sizeof(char*));

    nec_err_msg[NEC_ERR_UNKNOWN_COMMAND ] = 
        "Unknown command";
    nec_err_msg[NEC_ERR_NOT_SUPPORTED] = 
        "The current model doesn't support this function";
    nec_err_msg[NEC_ERR_SWITCHER_COMPAT] = 
        "The model is not compatible with the Switcher";
    nec_err_msg[NEC_ERR_PCVIEWER_COMPAT] = 
        "The model is not compatible with the PC Viewer";
    nec_err_msg[NEC_ERR_INVALID] = 
        "Invalid values specified";
    nec_err_msg[NEC_ERR_TERM_UNAVAIL] = 
        "Specified terminal is not available or cannot be selected";
    nec_err_msg[NEC_ERR_LANG_UNAVAIL] = 
        "Selected language is not available";
    nec_err_msg[NEC_ERR_MEM_RESERVATION] = 
        "Available memory reservation error";
    nec_err_msg[NEC_ERR_EXTERNAL_CTL_BROKE] = 
        "External control not working";
    nec_err_msg[NEC_ERR_OPER_MEM] = 
        "Operating memory";
    nec_err_msg[NEC_ERR_STANDBY] = 
        "Standby";
    nec_err_msg[NEC_ERR_FORCED_MUTE_MODE] = 
        "On forced on-screen mute mode";
    nec_err_msg[NEC_ERR_LINK_MODE] = 
        "Link mode working";
    nec_err_msg[NEC_ERR_OTHER_SIGNAL] = 
        "Displaying a signal other than the PC viewer";
    nec_err_msg[NEC_ERR_NO_SIGNAL] = 
        "No signal";
    nec_err_msg[NEC_ERR_TEST_PATTERN] = 
        "Displaying a test pattern or PC Card files screen";
    nec_err_msg[NEC_ERR_NO_PCCARD] = 
        "No PC card is inserted";
    nec_err_msg[NEC_ERR_MEMOPER_FAILED] = 
        "Memory operation failed";
    nec_err_msg[NEC_ERR_SWITCHER_MODE] = 
        "Switcher mode working";
    nec_err_msg[NEC_ERR_ENTRY_LIST] = 
        "Displaying Entry List";
    nec_err_msg[NEC_ERR_INVAL_GROUPNUM] = 
        "Group number / sub category number is not correct";
    nec_err_msg[NEC_ERR_GAIN_NOTAVAIL] = 
        "Selected gain is not available";
    nec_err_msg[NEC_ERR_ADJ_FAIL] = 
        "Adjustment failed";
    nec_err_msg[NEC_ERR_NO_START_REQ] = 
        "Start is not requested";
    nec_err_msg[NEC_ERR_STORING] = 
        "Cannot process due to storing";
    nec_err_msg[NEC_ERR_EXCEEDS_BLKS_REQ] = 
        "Exceeds the total number of blocks required at time of start";
    nec_err_msg[NEC_ERR_INCONSECUTIVE_BLKS] = 
        "The block number of transferred data is not consecutive";

    nec_err_msg[NEC_ERR_ENOTSUPPORTED + NEC_LAST_ERR] = 
        "Not supported";
    nec_err_msg[NEC_ERR_EPARAM + NEC_LAST_ERR] = 
        "Parameter error";
    nec_err_msg[NEC_ERR_EOPERMODE + NEC_LAST_ERR] = 
        "Operation mode error";
    nec_err_msg[NEC_ERR_EGAIN + NEC_LAST_ERR] = 
        "Gain-related error";
    nec_err_msg[NEC_ERR_ELOGOTRANS + NEC_LAST_ERR] = 
        "Logo transfer error";
}

int nec_errno(struct nec_err* err, struct nec_msg* msg) {
    init_err();

    if (msg->data == NULL) { return 1; }

    err->no = *((short int*)msg->data);
    err->class = err->no & (short int)(0xFF00);
    err->msg = nec_err_str(err->no);

    return 0;
}
