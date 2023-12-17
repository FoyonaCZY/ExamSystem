<!-- Home.vue -->

<template>
  <div>
    <h1>欢迎使用英语测试系统</h1>
    <el-button @click="goToStudentPage">进入学生端</el-button>
    <el-button @click="goToTeacherPage">进入教师端</el-button>
  </div>
</template>

<script>
import { useRouter } from 'vue-router';

export default {
  name: 'HomeView', // 将组件名改为 "HomeView"
  setup() {
    const router = useRouter();

    const goToStudentPage = () => {
      router.push('/student');
    };

    const goToTeacherPage = () => {
      router.push('/teacher');
    };

    return { goToStudentPage, goToTeacherPage };
  },
};
</script>
