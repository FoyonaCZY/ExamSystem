// router/index.js

import {createRouter, createWebHashHistory} from 'vue-router';
import Home from '../components/Home.vue';
import TeacherDashboard from '../components/Teacher.vue';
import StudentDashboard from '../components/Student.vue'; // 新添加的学生页面组件
import Statistics from '../components/Statistics.vue';

const routes = [
    {
        path: '/',
        name: 'HomeView',
        component: Home,
    },
    {
        path: '/teacher',
        name: 'TeacherDashboard',
        component: TeacherDashboard,
    },
    {
        path: '/student',
        name: 'StudentDashboard',
        component: StudentDashboard,
    },
    {
        path: '/statistics',
        name: 'StatisticsPage',
        component: Statistics,
    },
];

const router = createRouter({
    history: createWebHashHistory(),
    routes,
});

export default router;
