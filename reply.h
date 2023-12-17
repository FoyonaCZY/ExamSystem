//
// Created by CZY on 2023/12/13.
//

#pragma once

#include <stdio.h>
#include "FileOperations.h"
#include "Struct.h"
#include "mongoose.h"

extern const char *ReplyHeaders;
Questions *Q_Header;
Students *S_Header;

void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data);