//
// Created by CZY on 2023/12/7.
//

#include <stdio.h>
#include "FileOperations.h"
#include "Struct.h"
#include "mongoose.h"
#include "reply.h"

int main() {
    Q_Header = Q_initLinkList();
    Q_Read(Q_Header);
    S_Header = S_initLinkList();
    S_Read(S_Header);
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                                      // 初始化连接
    mg_log_set(MG_LL_DEBUG);                                // 设置日志输出
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, &mgr);  // 设置监听端口
    for (;;) {
        mg_mgr_poll(&mgr, 1000);                       // 循环监听
    }
    mg_mgr_free(&mgr);                                      // 释放连接
    return 0;
}