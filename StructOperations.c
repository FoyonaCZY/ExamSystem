//
// Created by CZY on 2023/12/7.
//

#include <stdio.h>
#include "Struct.h"
#include "mongoose.h"
#include "FileOperations.h"

bool is_ok(char *s)//判断名称是否合法
{
    for (int i = 0; i < strlen(s); i++) {
        if (!((s[i] <= 'z' && s[i] >= 'a') || (s[i] <= 'Z' && s[i] >= 'A') || s[i] == '.' || s[i] == ',' ||
              (s[i] >= '0' && s[i] <= '9') || (s[i] == ' ')))
            return false;
    }
    for (int i = 0; i < strlen(s) - 1; i++) {
        if (s[i] == ' ' && s[i + 1] == ' ')return false;
    }
    return true;
}

//初始化题目链表
Questions *Q_initLinkList() {
    Questions *pHeader = malloc(sizeof(Questions));
    if (pHeader == NULL) {
        return NULL;
    }
    pHeader->next = NULL;
    return pHeader;
}

//初始化学生链表
Students *S_initLinkList() {
    Students *pHeader = malloc(sizeof(Students));
    if (pHeader == NULL) {
        return NULL;
    }
    pHeader->next = NULL;
    return pHeader;
}

//尾插题目
Questions *Q_Insert(Questions *pHeader, char *contents, char answer, double points) {
    Questions *pCurr = pHeader;
    while (pCurr->next != NULL) {
        pCurr = pCurr->next;
    }
    Questions *pIns = malloc(sizeof(Questions));
    if (pIns == NULL || Q_Search(pHeader, contents) != NULL || !is_ok(contents)) {
        return NULL;
    }
    pCurr->next = pIns;
    strcpy(pIns->content, contents);
    pIns->answer = answer;
    pIns->points = points;
    pIns->next = NULL;
    return pIns;
}

//尾插学生
Students *S_Insert(Students *pHeader, char *major, char *class, char *name, char *answer, double points) {
    Students *pCurr = pHeader;
    while (pCurr->next != NULL) {
        pCurr = pCurr->next;
    }
    Students *pIns = malloc(sizeof(Students));
    if (pIns == NULL || S_Search(pHeader, name) != NULL) {
        return NULL;
    }
    pCurr->next = pIns;
    strcpy(pIns->major, major);
    strcpy(pIns->class, class);
    strcpy(pIns->name, name);
    strcpy(pIns->answer, answer);
    pIns->points = points;
    pIns->next = NULL;
    S_Print(pHeader);
    return pIns;
}

//打印题目
char *Q_Print(Questions *pHeader) {
    Questions *pCurr = pHeader->next;
    char *Res = (char *) malloc(100000);
    strcpy(Res, "{\"problems\":[");
    while (pCurr != NULL) {
        char *Temp = (char *) malloc(strlen(pCurr->content) + 100);
        sprintf(Temp, "{\"Content\":\"%s\",\"Answer\":\"%c\",\"Points\":%.2f}", pCurr->content, pCurr->answer,
                pCurr->points);
        strcat(Res, Temp);
        if (pCurr->next != NULL)strcat(Res, ",");
        pCurr = pCurr->next;
        free(Temp);
    }
    strcat(Res, "]}");
    printf("%s\n", Res);
    Q_Write(Res);
    return Res;
}

//查找题目
Questions *Q_Search(Questions *pHeader, char *contents) {
    Questions *pCurr = pHeader;
    if (pCurr == NULL)return NULL;
    while (strcmp(pCurr->content, contents) != 0) {
        pCurr = pCurr->next;
        if (pCurr == NULL)break;
    }
    return pCurr;
}

//查找学生
Students *S_Search(Students *pHeader, char *contents) {
    Students *pCurr = pHeader;
    if (pCurr == NULL)return NULL;
    while (strcmp(pCurr->name, contents) != 0) {
        pCurr = pCurr->next;
        if (pCurr == NULL)break;
    }
    return pCurr;
}

//删除题目
bool Q_Del(Questions *pHeader, char *contents) {
    Questions *pCurr = pHeader;
    Questions *Prev = pHeader;
    if (pCurr == NULL)return false;
    while (strcmp(pCurr->content, contents) != 0) {
        Prev = pCurr;
        pCurr = pCurr->next;
        if (pCurr == NULL)break;
    }
    if (pCurr == NULL)return false;
    Prev->next = pCurr->next;
    free(pCurr);
    return true;
}

//批改答案
double Correct(Questions *pCurr, const char *answer, int step, double score) {
    if (pCurr == NULL)return score;
    else {
        if (answer[step] == pCurr->answer)return Correct(pCurr->next, answer, step + 1, score + pCurr->points);
        else return Correct(pCurr->next, answer, step + 1, score);
    }
}

//打印学生列表
char *S_Print(Students *pHeader) {
    Students *pCurr = pHeader->next;
    char *Res = (char *) malloc(100000);
    strcpy(Res, "{\"students\":[");
    while (pCurr != NULL) {
        char *Temp = (char *) malloc(strlen(pCurr->name) + strlen(pCurr->major) + strlen(pCurr->answer) + 100);
        sprintf(Temp, "{\"Name\":\"%s\",\"Major\":\"%s\",\"Class\":\"%s\",\"Answer\":\"%s\",\"Points\":%.2f}",
                pCurr->name, pCurr->major, pCurr->class, pCurr->answer,
                pCurr->points);
        strcat(Res, Temp);
        if (pCurr->next != NULL)strcat(Res, ",");
        pCurr = pCurr->next;
        free(Temp);
    }
    strcat(Res, "]}");
    printf("%s\n", Res);
    S_Write(Res);
    return Res;
}

//删除学生
bool S_Del(Students *pHeader, char *contents) {
    Students *pCurr = pHeader;
    Students *Prev = pHeader;
    if (pCurr == NULL)return false;
    while (strcmp(pCurr->name, contents) != 0) {
        Prev = pCurr;
        pCurr = pCurr->next;
        if (pCurr == NULL)break;
    }
    if (pCurr == NULL)return false;
    Prev->next = pCurr->next;
    free(pCurr);
    return true;
}

//排序
void S_Sort(Students *pHeader, int method) {
    int tot = 0;
    Students *pCurr = pHeader->next;
    while (pCurr != NULL) {
        tot++;
        pCurr = pCurr->next;
    }
    if (tot <= 1)return;
    for (int i = 1; i <= tot; i++) {
        pCurr = pHeader->next;
        Students *Prev = pHeader;
        Students *Next = pCurr->next;
        while (Next != NULL) {
            if (pCurr->points > Next->points && method == 1) {
                Prev->next = Next;
                pCurr->next = Next->next;
                Next->next = pCurr;
                Prev = Next;
                Next = pCurr->next;
            } else if (pCurr->points < Next->points && method == 2) {
                Prev->next = Next;
                pCurr->next = Next->next;
                Next->next = pCurr;
                Prev = Next;
                Next = pCurr->next;
            } else {
                Prev = Prev->next;
                pCurr = pCurr->next;
                Next = Next->next;
            }
        }
    }
}