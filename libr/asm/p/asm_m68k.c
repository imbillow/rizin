/* radare - GPL3 - Copyright 2009 nibble<.ds@gmail.com> */

#include <stdio.h>
#include <string.h>

#include <r_types.h>
#include <r_lib.h>
#include <r_util.h>
#include <r_asm.h>

#include "m68k/m68k_disasm/m68k_disasm.h"


static int disassemble(struct r_asm_t *a, struct r_asm_aop_t *aop, u8 *buf, u64 len)
{
	m68k_word bof[4];
	m68k_word iaddr = (m68k_word)a->pc;
	char opcode[256];
	char operands[256];

	struct DisasmPara_68k dp;
	/* initialize DisasmPara */
	memcpy(bof, buf, 4);
	dp.opcode = opcode;
	dp.operands = operands;
	dp.iaddr = &iaddr;
	dp.instr = bof;
	M68k_Disassemble(&dp);
	sprintf(aop->buf_asm, "%s %s", opcode, operands);
	r_hex_bin2str((u8*)bof, 4, aop->buf_hex);
	memcpy(aop->buf, bof, 4);
	aop->inst_len = 4;

	return aop->inst_len;
}

static struct r_asm_handle_t r_asm_plugin_m68k = {
	.name = "asm_m68k",
	.desc = "M68K disassembly plugin",
	.init = NULL,
	.fini = NULL,
	.disassemble = &disassemble,
	.assemble = NULL
};

struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_ASM,
	.data = &r_asm_plugin_m68k
};
