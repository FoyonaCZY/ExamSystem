<!-- Teacher.vue -->

<template>
  <div>
    <h1>教师管理面板</h1>

    <!-- 添加题目按钮 -->
    <el-button @click="toggleForm">添加题目</el-button>
    <el-button @click="goToStatisticsPage">查看数据</el-button>
    <el-button @click="goToHomePage">返回主页</el-button>
    <!-- 添加题目的表单 -->
    <br>
    <br>
    <el-form v-if="showForm" :model="newQuestion" label-width="80px">
      <el-form-item label="题目内容">
        <el-input v-model="newQuestion.content" :maxlength="1000"></el-input>
      </el-form-item>
      <el-form-item label="答案(N/Y)">
        <el-input v-model="newQuestion.answer"></el-input>
      </el-form-item>
      <el-form-item label="分值">
        <el-input-number v-model="newQuestion.score" :max="500" :min="0"></el-input-number>
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="addQuestion">添加题目</el-button>
      </el-form-item>
    </el-form>
    <!-- 题目列表 -->
    <h2>题目列表</h2>
    <el-table :data="problemList" width="80%">
      <el-table-column prop="Content" label="题目内容"></el-table-column>
      <el-table-column prop="Answer" label="答案"></el-table-column>
      <el-table-column prop="Points" label="分值"></el-table-column>
      <el-table-column label="操作">
        <template v-slot="scope">
          <el-button type="danger" @click="handleDelete(scope.row)">删除</el-button>
        </template>
      </el-table-column>
    </el-table>

  </div>
</template>

<script>
import {onMounted, ref} from 'vue';
import axios from 'axios';
import {ElMessageBox} from 'element-plus';
import {useRouter} from "vue-router";
import router from "@/router";

export default {
  name: 'TeacherDashboard',
  methods: {
    goToStatisticsPage() {
      this.$router.push('/statistics');
    },
    goToHomePage() {
      // 使用Vue Router进行导航到主页
      this.$router.push('/');
    },
    handleSortChange({prop, order}) {
      // 处理排序逻辑
      console.log(`Sorting by ${prop} in ${order} order`);
      // 在这里可以调用获取题目列表的方法，传递排序参数
      // this.getProblemList({ sortBy: prop, sortOrder: order });
    },
    async handleDelete(row) {
      // 处理删除逻辑
      const confirmResult = await ElMessageBox.confirm('确认删除题目？', '警告', {
        type: 'warning'
      });

      if (confirmResult === 'confirm') {
        const deleteResult = await this.deleteQuestion(row.Content);

        if (deleteResult === 'OK\n') {
          // 删除成功，显示成功的消息框
          await new Promise(resolve => {
            ElMessageBox.alert('删除题目成功！', '成功', {
              type: 'success',
              onClose: () => {
                console.log('Alert closed');
              },
            }).finally(() => {
              // 使用 finally 确保 resolve 被执行
              router.go(0);
              resolve();
            });
          });
        } else {
          // 删除失败，显示失败的消息框
          await new Promise(resolve => {
            ElMessageBox.alert('删除题目失败，请检查是否存在相关问题', '错误', {
              type: 'error',
              onClose: () => resolve(),
            });
          });
        }
      }
    },
    async deleteQuestion(content) {
      try {
        const response = await axios.post('http://localhost:8000/api/DelQues', {
          Content: content,
        }, {
          headers: {
            'Content-Type': 'application/json',
          },
        });

        return response.data;
      } catch (error) {
        console.error('Error deleting question:', error);
        return 'NOTOK\n';
      }
    },
    // 其他方法和属性...
  },
  setup() {
    const newQuestion = ref({
      content: '',
      answer: '',
      score: null,
    });
    const showForm = ref(false);
    const problemList = ref([]);
    const toggleForm = () => {
      showForm.value = !showForm.value;
    };
    const router = useRouter();
    const addQuestion = async () => {
      try {
        const response = await axios.post('http://localhost:8000/api/InsQues', {
          Content: newQuestion.value.content,
          Answer: newQuestion.value.answer,
          Points: parseFloat(newQuestion.value.score),
        }, {
          headers: {
            'Content-Type': 'application/json',
          },
        });
        console.log(response);
        // 处理响应
        if (response.data === 'OK\n') {
          // 插入成功，显示成功的消息框
          await new Promise(resolve => {
            ElMessageBox.alert('添加题目成功！', '成功', {
              type: 'success',
              onClose: () => {
                console.log('Alert closed');
              },
            }).finally(() => {
              // 使用 finally 确保 resolve 被执行
              resetForm();
              router.go(0);
              resolve();
            });
          });
        } else {
          // 插入失败，显示失败的消息框
          await new Promise(resolve => {
            ElMessageBox.alert('添加题目失败，请检查题目是否重复，以及信息是否合法', '错误', {
              type: 'error',
              onClose: () => resolve(),
            });
          });
        }
      } catch (error) {
        console.error('Error adding question:', error);
      }
    };

    const resetForm = () => {
      // 检查是否存在表单元素，避免在元素不存在时触发相关逻辑
      if (showForm.value) {
        // 重置表单数据
        newQuestion.value.content = '';
        newQuestion.value.answer = '';
        newQuestion.value.score = null;

        // 隐藏表单
        showForm.value = false;
      }
    };

    const getProblemList = async () => {
      try {
        // 发送获取题目列表的请求
        const response = await axios.get('http://localhost:8000/api/GetQuesList');
        // 更新题目列表
        problemList.value = response.data.problems || [];
      } catch (error) {
        console.error('Error fetching problem list:', error);
      }
    };

    // 组件挂载时获取题目列表
    onMounted(async () => {
      await getProblemList();
    });
    return {newQuestion, showForm, toggleForm, addQuestion, problemList};
  },
};
</script>
