#ifndef __NEC_H
#define __NEC_H

#include <stdint.h>

// First byte of nec_error data
#define NEC_ERR_ENOTSUPPORTED 0x0000
#define NEC_ERR_EPARAM        0x0100
#define NEC_ERR_EOPERMODE     0x0200
#define NEC_ERR_EGAIN         0x0300
#define NEC_ERR_ELOGOTRANS    0x0400

// Second byte of nec_error data
// should not be used by programmer
//
// ENOTSUPPORTED
#define NEC_ERR_UNKNOWN_COMMAND      0x0
#define NEC_ERR_NOT_SUPPORTED        0x1
#define NEC_ERR_SWITCHER_COMPAT      0x2
#define NEC_ERR_PCVIEWER_COMPAT      0x3

// EPARAM
#define NEC_ERR_INVALID              0x100
#define NEC_ERR_TERM_UNAVAIL         0x101
#define NEC_ERR_LANG_UNAVAIL         0x102

// EOPERMODE
#define NEC_ERR_MEM_RESERVATION      0x200
#define NEC_ERR_EXTERNAL_CTL_BROKE   0x201
#define NEC_ERR_OPER_MEM             0x202
#define NEC_ERR_STANDBY              0x203
#define NEC_ERR_FORCED_MUTE_MODE     0x204
#define NEC_ERR_LINK_MODE            0x205
#define NEC_ERR_OTHER_SIGNAL         0x206
#define NEC_ERR_NO_SIGNAL            0x207
#define NEC_ERR_TEST_PATTERN         0x208
#define NEC_ERR_NO_PCCARD            0x209
#define NEC_ERR_MEMOPER_FAILED       0x20A
#define NEC_ERR_SWITCHER_MODE        0x20B
#define NEC_ERR_ENTRY_LIST           0x20C

// EGAIN
#define NEC_ERR_INVAL_GROUPNUM       0x300
#define NEC_ERR_GAIN_NOTAVAIL        0x301
#define NEC_ERR_ADJ_FAIL             0x302

// ELOGOTRANS
#define NEC_ERR_NO_START_REQ         0x400
#define NEC_ERR_STORING              0x401
#define NEC_ERR_EXCEEDS_BLKS_REQ     0x402
#define NEC_ERR_INCONSECUTIVE_BLKS   0x403
#define NEC_LAST_ERR 0x404

#define NEC_NUMBER_OF_ERRORS 32

static char** nec_err_msg;

#define nec_err_str(x) (nec_err_msg[x])
#define nec_errclass_str(x) (nec_err_msg[(x & 0xF00) + NEC_LAST_ERR])

// Near as I can tell, we never have a message over 255 bytes long, so we can
// get away with not conforming exactly to the spec so we can avoid struct
// alignment issues
struct nec_msg_hdr {
    uint16_t command;
    uint8_t projector_id;
    uint8_t model_code;
    uint8_t data_len;
};

struct nec_msg {
    struct nec_msg_hdr* hdr;
    void* data;
    uint8_t chksm;
};

struct nec_err {
    uint8_t class;
    uint8_t no;
    const char* msg;
};

int nec_errno(struct nec_err* err, const struct nec_msg* msg);

#endif