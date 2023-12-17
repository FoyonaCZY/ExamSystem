<template>
  <div>
    <h1>学生答题面板</h1>
    <el-button @click="goToHomePage">返回主页</el-button>
    <br>
    <br>
    <!-- 学生个人信息表单 -->
    <el-form :model="studentInfo" label-width="80px">
      <el-form-item label="姓名">
        <el-input v-model="studentInfo.name" @input="validateName" :maxlength="50"></el-input>
      </el-form-item>
      <el-form-item label="专业">
        <el-input v-model="studentInfo.major" @input="validateMajor" :maxlength="20"></el-input>
      </el-form-item>
      <el-form-item label="班级">
        <el-input-number v-model="studentInfo.class" :max="50" :min="1" type="number"></el-input-number>
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="startAnswering">开始作答</el-button>
      </el-form-item>
    </el-form>

    <!-- 题目列表 -->
    <div v-if="showProblemList && problemList && problemList.length > 0">
      <h2>题目列表</h2>
      <el-table :data="problemList" width="80%">
        <el-table-column prop="Content" label="判断语法正误"></el-table-column>
        <!-- 其他需要显示的列，不包括答案 -->
        <el-table-column label="您的答案(N/Y)">
          <template v-slot="scope">
            <!-- 输入框用于学生填写答案 -->
            <el-input v-model="scope.row.answer" :maxlength="1" :minlength="1"></el-input>
          </template>
        </el-table-column>
      </el-table>
      <br>
      <el-button type="primary" @click="submitAnswers">提交答案</el-button>
    </div>

    <div v-else>
      <p v-if="!showProblemList">请点击 "开始作答" 按钮</p>
      <p v-else>No problems available.</p>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import {ElMessageBox} from "element-plus";
import router from "@/router";

export default {
  name: 'StudentDashboard',
  data() {
    return {
      studentInfo: {
        name: '',
        major: '',
        class: '',
      },
      problemList: [],
      showProblemList: false, // 控制是否显示题目列表
    };
  },
  methods: {
    goToHomePage() {
      // 使用Vue Router进行导航到主页
      this.$router.push('/');
    },
    async startAnswering() {
      // 在按钮点击事件中调用 getProblemList 方法
      await this.getProblemList();
      this.showProblemList = true; // 显示题目列表
    },
    async getProblemList() {
      try {
        const response = await axios.post('http://localhost:8000/api/GetQuesList', {
          // 发送学生个人信息
          name: this.studentInfo.name,
          major: this.studentInfo.major,
          class: this.studentInfo.class,
        });
        this.problemList = response.data.problems || [];
      } catch (error) {
        console.error('Error fetching problem list:', error);
      }
    },
    submitAnswers() {
      // 获取学生答案并拼接为字符串
      const answerString = this.problemList.map(item => item.answer).join('');
      // 构建请求的 body
      const requestBody = {
        Major: this.studentInfo.major,
        Class: String(parseInt(this.studentInfo.class)),
        Name: this.studentInfo.name,
        Answer: answerString,
      };

      // 发送提交答案的请求
      axios.post('http://localhost:8000/api/InsStu', requestBody, {
        headers: {
          'Content-Type': 'application/json',
        },
      })
          .then(response => {
            if (response.data === 'OK\n') {
              // 提交成功，显示成功的消息框
              this.showSuccessMessage();

            } else {
              // 提交失败，显示失败的消息框
              this.showErrorMessage();
            }
          })
          .catch(error => {
            console.error('Error submitting answers:', error);
            // 提交失败，显示失败的消息框
            this.showErrorMessage();
          });
    },


    showSuccessMessage() {

      ElMessageBox.alert('提交答案成功！', '成功', {
        type: 'success',
        onClose: () => {

        },
      }).finally(() => {
        router.go(0);
      });
    },

    showErrorMessage() {
      ElMessageBox.alert('提交答案失败，请稍后重试', '错误', {
        type: 'error',
      });
    },

    validateName() {
      // 使用正则表达式限制只能包含汉字和英文字母
      this.studentInfo.name = this.studentInfo.name.replace(/[^\u4e00-\u9fa5a-zA-Z]/g, '');
    },
    validateMajor() {
      // 使用正则表达式限制只能包含汉字和英文字母
      this.studentInfo.major = this.studentInfo.major.replace(/[^\u4e00-\u9fa5a-zA-Z]/g, '');
    },
  },
};
</script>
