/*
 * at_cvoice.c
 *
 *  Created on: Apr 20, 2012
 *      Author: carlos
 */
#include <string.h>
#include <regex.h>
#include "at_cvoice.h"

static regex_t _cvoice_regex;
static int _matchgrp_no;

void at_cvoice_query_all(at_command_t *cmd)
{
	STR_ALLOC(cmd->str_cmd, sizeof("AT^CVOICE?") + 3 /* \r\n\0 */);
	strcpy(cmd->str_cmd.s, "AT^CVOICE?\r\n");

	cmd->id = AT_CVOICE;
	cmd->type = AT_TYPE_QUERY_ALL;
}

_bool at_cvoice_parse_response(at_io_t *io, at_cvoice_resp_t *cvoice)
{
	int ret_code, i;
	char *error_message = NULL;
	regmatch_t matches[_matchgrp_no];

	if (io->at_cmd.id != AT_CVOICE)
	{
		LOG(L_ERROR, "%s: invalid command type %d\n", __FUNCTION__, io->at_cmd.id);
		return FALSE;
	}

	switch (io->at_cmd.type)
	{
	case AT_TYPE_QUERY_ALL:
		sscanf(io->response.s, "\r\n^CVOICE: 0, %d, %d, %d\r\nOK\r\n", &cvoice->sampling_rate, &cvoice->data_bit, &cvoice->ptime);

		LOG(L_DEBUG, "%s: CVOICE -> sampling rate=%d data_bit=%d ptime=%d\n", __FUNCTION__,
			cvoice->sampling_rate,
			cvoice->data_bit,
			cvoice->ptime);

		return TRUE;
		break;
	case AT_TYPE_ASSIGN:
	case AT_TYPE_QUERY_VALUE:
		LOG(L_WARNING, "%s: unused type %d\n", __FUNCTION__, io->at_cmd.type);
		break;
	default:
		LOG(L_ERROR, "%s: unknown type %d\n", __FUNCTION__, io->at_cmd.type);
		break;
	}

	return FALSE;
}
