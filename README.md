# AndLibNsgif(安卓Gif动画解析播放)
This is a simple android gif decoder and player based on libnsgif.

#### Detail
* This project is for demonstrate how to plug in jni with gradle and android studio
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

#### Screenshot
![screenshot](https://github.com/huzongyao/AndLibNsgif/blob/master/misc/screen.gif?raw=true)

### About Me
 * GitHub: [http://huzongyao.github.io/](http://huzongyao.github.io/)
 * ITEye博客：[http://hzy3774.iteye.com/](http://hzy3774.iteye.com/)
 * 新浪微博: [http://weibo.com/hzy3774](http://weibo.com/hzy3774)

### Contact To Me
 * QQ: [377406997](http://wpa.qq.com/msgrd?v=3&uin=377406997&site=qq&menu=yes)
 * Gmail: [hzy3774@gmail.com](mailto:hzy3774@gmail.com)
 * Foxmail: [hzy3774@qq.com](mailto:hzy3774@qq.com)
