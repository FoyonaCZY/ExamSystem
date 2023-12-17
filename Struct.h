//
// Created by CZY on 2023/12/7.
//

#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

//储存题目信息的结构体链表
typedef struct Questions {
    char content[2000];//题目内容
    char answer;//正确答案
    double points;//分值
    struct Questions *next;//指向下一道题的指针
} Questions;

//储存学生信息的结构体链表
typedef struct Students {
    char major[100];//学生专业
    char class[100];//学生班级
    char name[100];//学生姓名
    char answer[100];//答案
    double points;//得分
    struct Students *next;//指向下一位学生的指针
} Students;

Questions *Q_initLinkList();//初始化题目链表
Questions *Q_Insert(Questions *pHeader, char *contents, char answer, double points);//尾插题目
char *Q_Print(Questions *pHeader);//获取题目列表
Questions *Q_Search(Questions *pHeader, char *contents);//查找题目
bool Q_Del(Questions *pHeader,char *contents);
Students *S_initLinkList();//初始化学生链表
Students *S_Insert(Students *pHeader, char *major, char *class, char *name, char *answer, double points);//尾插学生
Students *S_Search(Students *pHeader, char *contents);//查找学生
double Correct(Questions *pCurr,const char *answer,int step,double score);//批改答案
char *S_Print(Students *pHeader);//获取学生数据列表
bool S_Del(Students *pHeader,char *contents);
void S_Sort(Students *pHeader,int method);//排序