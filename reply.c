//
// Created by CZY on 2023/12/13.
//

#include <stdio.h>
#include "FileOperations.h"
#include "Struct.h"
#include "mongoose.h"
#include "reply.h"

const char *ReplyHeaders = "HTTP/1.1 200 OK\r\n"
                           "Access-Control-Allow-Origin: *\r\n"
                           "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
                           "Access-Control-Allow-Headers: Content-Type\r\n"
                           "Content-Type: application/json\r\n";

bool if_num(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9')return false;
    }
    return true;
}

bool InsQues(struct mg_str json) {//从请求内容中提取题目信息，判定合法性并插入题目

    char *content = mg_json_get_str(json, "$.Content");
    char *ans = mg_json_get_str(json, "$.Answer");
    double point = 0;
    bool result = mg_json_get_num(json, "$.Points", &point);
    if (strlen(content) == 0 || (strcmp(ans, "N") != 0 && strcmp(ans, "Y") != 0) || !result) {
        return false;
    }
    bool ret = Q_Insert(Q_Header, content, ans[0], point);
    free(content);
    free(ans);
    return ret;
}

bool InsStu(struct mg_str json) {//从请求内容中提取学生信息，判定合法性并插入学生
    if (Q_Header->next == NULL)return false;
    char *major = mg_json_get_str(json, "$.Major");
    char *class = mg_json_get_str(json, "$.Class");
    char *name = mg_json_get_str(json, "$.Name");
    char *answer = mg_json_get_str(json, "$.Answer");
    double points = Correct(Q_Header->next, answer, 0, 0);
    printf("%.2f\n", points);
    bool ret = S_Insert(S_Header, major, class, name, answer, points);
    free(major);
    free(class);
    free(name);
    free(answer);
    return ret;
}

void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {                                     //判断HTTP事件
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_vcmp(&hm->method, "OPTIONS") == 0) {         //处理预检OPTIONS请求
            mg_http_reply(c, 200, ReplyHeaders, "");
        } else if (mg_http_match_uri(hm, "/api/InsQues")) {              //插入题目

            bool InsResult = InsQues(hm->body);
            const char *Res = "OK";
            if (!InsResult)Res = "NOTOK";
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
        } else if (mg_http_match_uri(hm, "/api/GetQuesList")) {              //获取题目列表

            char *Res = Q_Print(Q_Header);
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
            free(Res);
        } else if (mg_http_match_uri(hm, "/api/DelQues")) {              //删除题目
            char *content = mg_json_get_str(hm->body, "$.Content");
            bool DelResult = Q_Del(Q_Header, content);
            free(content);
            const char *Res = "OK";
            if (!DelResult)Res = "NOTOK";
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
        } else if (mg_http_match_uri(hm, "/api/InsStu")) {
            printf("%s\n", hm->body.ptr);
            bool InsResult = InsStu(hm->body);
            const char *Res = "OK";
            if (!InsResult)Res = "NOTOK";
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
        } else if (mg_http_match_uri(hm, "/api/GetStuList")) {              //获取学生列表

            char *Res = S_Print(S_Header);
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
            free(Res);
        } else if (mg_http_match_uri(hm, "/api/DelStu")) {              //删除学生
            char *content = mg_json_get_str(hm->body, "$.Name");
            bool DelResult = S_Del(S_Header, content);
            free(content);
            const char *Res = "OK";
            if (!DelResult)Res = "NOTOK";
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", Res);//返回插入结果
        } else if (mg_http_match_uri(hm, "/api/SortUp")) {
            S_Sort(S_Header, 1);
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", "OK");
        } else if (mg_http_match_uri(hm, "/api/SortDown")) {
            S_Sort(S_Header, 2);
            mg_http_reply(c, 200, ReplyHeaders, "%s\n", "OK");
        } else {
            mg_http_reply(c, 200, "", "%s\n", "NOT API");
        }
    }
}


