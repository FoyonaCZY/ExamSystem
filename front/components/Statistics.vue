<!-- components/Statistics.vue -->

<template>
  <div>
    <h1>数据统计</h1>
    <el-button @click="goToTeacherPage">返回教师端</el-button>
    <el-button @click="goToHomePage">返回主页</el-button>
    <br>
    <br>
    <el-button type="primary" @click="sortByPoints(true)">按分数升序</el-button>
    <el-button type="primary" @click="sortByPoints(false)">按分数降序</el-button>
    <el-button type="primary" @click="showAverageScores">成绩统计</el-button>
    <br>
    <br>
    <el-input v-model="searchName" placeholder="输入学生姓名"></el-input>
    <el-input v-model="searchMajor" placeholder="输入专业"></el-input>
    <el-input v-model="searchClass" placeholder="输入班级"></el-input>
    <br>
    <br>
    <el-button type="primary" @click="searchStudents">搜索</el-button>
    <el-button type="danger" @click="resetSearch">重置</el-button>
    <br>
    <br>
    <el-table :data="problemList" width="80%">
      <el-table-column prop="Name" label="学生姓名"></el-table-column>
      <el-table-column prop="Major" label="专业"></el-table-column>
      <el-table-column prop="Class" label="班级"></el-table-column>
      <el-table-column prop="Answer" label="提交答案"></el-table-column>
      <el-table-column prop="Points" label="得分"></el-table-column>
      <el-table-column label="操作">
        <template v-slot="scope">
          <el-button type="danger" @click="handleDelete(scope.row)">删除</el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
import {onMounted, ref} from "vue";
import axios from "axios";
import {ElMessageBox} from "element-plus";
import router from "@/router";

export default {
  name: 'StatisticsPage',
  methods: {
    async sortByPoints(ascending) {
      const apiUrl = ascending ? 'http://localhost:8000/api/SortUp' : 'http://localhost:8000/api/SortDown';

      try {
        // 发送排序请求
        await axios.get(apiUrl);
        // 刷新页面
        this.$router.go(0);
      } catch (error) {
        console.error('Error sorting by points:', error);
      }
    },
    goToHomePage() {
      // 使用Vue Router进行导航到主页
      this.$router.push('/');
    },
    goToTeacherPage() {
      // 使用Vue Router进行导航到主页
      this.$router.push('/teacher');
    },
    async handleDelete(row) {
      // 处理删除逻辑
      const confirmResult = await ElMessageBox.confirm('确认删除学生成绩？', '警告', {
        type: 'warning'
      });

      if (confirmResult === 'confirm') {
        const deleteResult = await this.deleteQuestion(row.Name);

        if (deleteResult === 'OK\n') {
          // 删除成功，显示成功的消息框
          await new Promise(resolve => {
            ElMessageBox.alert('删除学生成绩成功！', '成功', {
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
            ElMessageBox.alert('删除学生成绩失败，请检查是否存在相关问题', '错误', {
              type: 'error',
              onClose: () => resolve(),
            });
          });
        }
      }
    },
    async deleteQuestion(content) {
      try {
        const response = await axios.post('http://localhost:8000/api/DelStu', {
          Name: content,
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
  },
  setup() {
    const originalProblemList = ref([]); // 保存原始的题目列表
    const problemList = ref([]);
    const searchName = ref('');
    const searchMajor = ref('');
    const searchClass = ref('');

    const getProblemList = async () => {
      try {
        // 发送获取题目列表的请求
        const response = await axios.get('http://localhost:8000/api/GetStuList');
        // 更新题目列表
        originalProblemList.value = response.data.students || [];
        problemList.value = [...originalProblemList.value]; // 创建一个副本用于显示
      } catch (error) {
        console.error('Error fetching problem list:', error);
      }
    };

    const searchStudents = () => {
      // 使用输入的文本进行本地过滤
      const filteredStudents = originalProblemList.value.filter(student => {
        const lowerCaseSearchName = searchName.value.toLowerCase();
        const lowerCaseSearchMajor = searchMajor.value.toLowerCase();
        const lowerCaseSearchClass = searchClass.value.toLowerCase();

        // 检查学生姓名、专业和班级是否包含输入的文本
        const nameMatch = lowerCaseSearchName === '' || student.Name.toLowerCase().includes(lowerCaseSearchName);
        const majorMatch = lowerCaseSearchMajor === '' || student.Major.toLowerCase().includes(lowerCaseSearchMajor);
        const classMatch = lowerCaseSearchClass === '' || student.Class.toLowerCase().includes(lowerCaseSearchClass);

        // 如果有任一条件匹配，返回 true
        return nameMatch && majorMatch && classMatch;
      });

      // 更新题目列表
      problemList.value = filteredStudents;
    };

    const resetSearch = () => {
      // 重置搜索，恢复原始的题目列表
      problemList.value = [...originalProblemList.value];
    };

    const showAverageScores = () => {
      // 计算全部学生平均分、最高分、最低分、各专业平均分和班级总分、平均分
      const allStudents = originalProblemList.value;
      const allPoints = allStudents.map(student => parseFloat(student.Points) || 0);
      const totalAverage = allPoints.length > 0 ? (allPoints.reduce((acc, point) => acc + point, 0) / allPoints.length).toFixed(2) : 0;
      const highestScoreStudent = allStudents.length > 0 ? allStudents.reduce((prev, curr) => (parseFloat(curr.Points) || 0) > (parseFloat(prev.Points) || 0) ? curr : prev, {}) : {};
      const lowestScoreStudent = allStudents.length > 0 ? allStudents.reduce((prev, curr) => (parseFloat(curr.Points) || 0) < (parseFloat(prev.Points) || Infinity) ? curr : prev, {}) : {};


      const majors = Array.from(new Set(allStudents.map(student => student.Major)));

      const majorClassStats = majors.map(major => {
        const studentsInMajor = allStudents.filter(student => student.Major === major);
        const classesInMajor = Array.from(new Set(studentsInMajor.map(student => student.Class)));

        const classStats = classesInMajor.map(className => {
          const studentsInClass = studentsInMajor.filter(student => student.Class === className);
          const pointsInClass = studentsInClass.map(student => parseFloat(student.Points) || 0);

          const classTotal = pointsInClass.reduce((acc, point) => acc + point, 0).toFixed(2);
          const classAverage = pointsInClass.length > 0 ? (classTotal / pointsInClass.length).toFixed(2) : 0;

          return {className, classTotal, classAverage};
        });

        return {major, classStats};
      });

      // 使用信息框显示信息
      const message = `
        <p>全部学生平均分: ${totalAverage}</p>
        <p>最高分学生: ${highestScoreStudent.Name}, 分数: ${highestScoreStudent.Points}</p>
        <p>最低分学生: ${lowestScoreStudent.Name}, 分数: ${lowestScoreStudent.Points}</p>
        <p>各专业平均分和班级统计:</p>
        <ul>
          ${majorClassStats.map(item => `
            <li>${item.major}:
              <ul>
                ${item.classStats.map(classStat => `
                  <li>${classStat.className} 班 - 总分: ${classStat.classTotal}, 平均分: ${classStat.classAverage}</li>
                `).join('')}
              </ul>
            </li>
          `).join('')}
        </ul>
      `;

      ElMessageBox.alert(message, '学生成绩统计', {
        dangerouslyUseHTMLString: true,
      });
    };

    // 组件挂载时获取题目列表
    onMounted(async () => {
      await getProblemList();
    });

    // 将 originalProblemList 添加到返回的对象中
    return {
      problemList,
      searchName,
      searchMajor,
      searchClass,
      searchStudents,
      resetSearch,
      showAverageScores,
      originalProblemList
    };
  },

};
</script>

<style scoped>
/* 添加样式 */
</style>
