// main.js

import { createApp } from 'vue';
import App from './App.vue';
import router from './router';

import ElementPlus from 'element-plus';
import 'element-plus/dist/index.css'; // 注意这里的引入方式

const app = createApp(App);

app.use(router);
app.use(ElementPlus);

app.mount('#app');
