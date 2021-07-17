# Docsify部署

[Docsify参考文档](https://docsify.js.org/#/zh-cn/)

## 部署安装
+ 安装脚手架工具 docsify-cli
```
$ npm i docsify-cli -g
```

+ 初始化文档
> 注意这里的文件名约定为 docs 也是官方推荐，请按照规则设置，否则发到 Github 可能会出现一些问题

```
$ docsify init docs

Initialization succeeded! Please run docsify serve docs
```

执行完以上命令 docs 文件目录下会生成以下 3 个文件：
+ index.html：入口文件
+ README.md：会做为主页内容渲染
+ .nojekyll：用于阻止 GitHub Pages 会忽略掉下划线开头的文件

+ 启动本地预览

```
$ docsify serve docs

Serving /Users/may/Nodejs-Roadmap/docs now.
Listening at http://localhost:3000
```

## 配置
+ 定制导航页

官方支持两种方式，可以在 HTML 里设置，但是链接要以 #/ 开头，另外一种通过 Markdown 配置导航，我们这里用的也是后者
首先配置 `loadNavbar: true`，之后创建 `docs/_navbar.md`文件.

`docs/index.html`
```
<script>
  window.$docsify = {
    loadNavbar: true
  }
</script>
<script src="//unpkg.com/docsify"></script>
```
这里配置并不是很复杂，根据缩进生成对应的目录结构，注意目录的跳转链接是当前 (docs) 目录下的文件

`docs/_navbar.md`

```
* Introduction
    * [简介](README.md)

* JavaScript
    * [基础](/javascript/base.md)
    * [This](/javascript/this.md)
...
```
## 发布
+ gitee上创建一个工程

+ 提交本地文件到工程

+ 发布pages