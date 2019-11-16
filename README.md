# AndroidLibnsgif(安卓Gif动画解析播放)
This is a simple android gif decoder and player based on libnsgif.

[![Travis](https://img.shields.io/appveyor/ci/gruntjs/grunt.svg)](https://github.com/huzongyao/AndroidLibnsgif)
[![Travis](https://img.shields.io/badge/libnsgif-v0.2.1-brightgreen.svg)](http://www.netsurf-browser.org/projects/libnsgif/)

#### Screenshot
![screenshot](https://github.com/huzongyao/AndLibNsgif/blob/master/misc/screen.gif?raw=true)

#### Detail
* This project is for demonstrate how to plug in jni with gradle and android studio
* Compiled with ndk and cmake.
* Also you may improve it to use for production
* The main code and logic is from libnsgif: http://www.netsurf-browser.org/projects/libnsgif/
* I have put the global static fields into structures to avoid instances share same resource
* there are more powerful image libraries could support gif animations such as fresco
* Libnsgif is licensed under the [MIT Licence](http://opensource.org/licenses/mit-license.php).

#### Begin To Use
```gradle
dependencies {
    compile 'com.hzy:nsgif:1.2.0'
}
```

### About Me
 * GitHub: [https://huzongyao.github.io/](https://huzongyao.github.io/)
 * ITEye博客：[https://hzy3774.iteye.com/](https://hzy3774.iteye.com/)
 * 新浪微博: [https://weibo.com/hzy3774](https://weibo.com/hzy3774)

### Contact To Me
 * QQ: [377406997](https://wpa.qq.com/msgrd?v=3&uin=377406997&site=qq&menu=yes)
 * Gmail: [hzy3774@gmail.com](mailto:hzy3774@gmail.com)
 * Foxmail: [hzy3774@qq.com](mailto:hzy3774@qq.com)
 * WeChat: hzy3774

 ![image](https://raw.githubusercontent.com/hzy3774/AndroidP7zip/master/misc/wechat.png)

### Others
 * 想捐助我喝杯热水(¥0.01起捐)</br>
 ![donate](https://github.com/huzongyao/JChineseChess/blob/master/misc/donate.png?raw=true)