#include "FileOperations.h"
#include "Struct.h"
#include "mongoose.h"
#include "cJSON.h"

void Q_Read(Questions *pHeader) {
    FILE *file = fopen("questions.txt", "r");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 读取文件内容
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0';  // 确保字符串结束符

    // 使用 cJSON 解析 JSON 文本
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        fclose(file);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // 解析 JSON 中的题目信息
    cJSON *problemsArray = cJSON_GetObjectItem(json, "problems");
    if (problemsArray != NULL && cJSON_IsArray(problemsArray)) {
        int arraySize = cJSON_GetArraySize(problemsArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON *problem = cJSON_GetArrayItem(problemsArray, i);

            char content[256];
            char answer = 0;
            double points = 0;

            cJSON *contentItem = cJSON_GetObjectItem(problem, "Content");
            cJSON *answerItem = cJSON_GetObjectItem(problem, "Answer");
            cJSON *pointsItem = cJSON_GetObjectItem(problem, "Points");

            if (contentItem != NULL && cJSON_IsString(contentItem) &&
                answerItem != NULL && cJSON_IsString(answerItem) &&
                pointsItem != NULL && cJSON_IsNumber(pointsItem)) {

                strcpy(content, contentItem->valuestring);
                sscanf(answerItem->valuestring, "%c", &answer);
                points = pointsItem->valuedouble;

                // 插入问题到链表中
                Q_Insert(pHeader, content, answer, points);
            }
        }
    }

    // 释放 cJSON 对象和缓冲区
    cJSON_Delete(json);
    free(buffer);

    fclose(file);
}

void Q_Write(char *content) {
    // 以写入模式打开文件

    FILE *file = fopen("questions.txt", "w");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // 将content写入文件
    fprintf(file, "%s", content);
    fflush(file);

    // 关闭文件
    fclose(file);

    printf("File Ques Success\n");
}

void S_Write(char *content) {
    // 以写入模式打开文件
    FILE *file = fopen("students.txt", "w");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // 将content写入文件
    fprintf(file, "%s", content);
    fflush(file);
    // 关闭文件
    fclose(file);

    printf("File Stu Success\n");
}

void S_Read(Students *pHeader) {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 读取文件内容
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0';  // 确保字符串结束符

    // 使用 cJSON 解析 JSON 文本
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        fclose(file);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // 解析 JSON 中的学生信息
    cJSON *studentsArray = cJSON_GetObjectItem(json, "students");
    if (studentsArray != NULL && cJSON_IsArray(studentsArray)) {
        int arraySize = cJSON_GetArraySize(studentsArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON *student = cJSON_GetArrayItem(studentsArray, i);

            char name[256];
            char major[256];
            char class[256];
            char answer[256];  // 改成字符串类型
            double points = 0;

            cJSON *nameItem = cJSON_GetObjectItem(student, "Name");
            cJSON *majorItem = cJSON_GetObjectItem(student, "Major");
            cJSON *classItem = cJSON_GetObjectItem(student, "Class");
            cJSON *answerItem = cJSON_GetObjectItem(student, "Answer");
            cJSON *pointsItem = cJSON_GetObjectItem(student, "Points");

            if (nameItem != NULL && cJSON_IsString(nameItem) &&
                majorItem != NULL && cJSON_IsString(majorItem) &&
                classItem != NULL && cJSON_IsString(classItem) &&
                answerItem != NULL && cJSON_IsString(answerItem) &&
                pointsItem != NULL && cJSON_IsNumber(pointsItem)) {

                strcpy(name, nameItem->valuestring);
                strcpy(major, majorItem->valuestring);
                strcpy(class, classItem->valuestring);
                strcpy(answer, answerItem->valuestring);  // 将字符串复制到answer中
                points = pointsItem->valuedouble;

                // 插入学生信息到链表中
                S_Insert(pHeader, major, class, name, answer, points);
            }
        }
    }

    // 释放 cJSON 对象和缓冲区
    cJSON_Delete(json);
    free(buffer);

    fclose(file);
}