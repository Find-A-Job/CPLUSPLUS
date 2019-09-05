# git使用手册

* 常用命令<br>
```
$ git clone xxx           //从远程仓库克隆一份副本，xxx表示远程地址，示例：https://github.com/Find-A-Job/CPLUSPLUS.git
$ git status              //查看当前状态
$ git add .               //点表示添加'所有'到'暂存'
$ git commit -m "xxxx"    //提交到本地仓库,xxx代表'记号'
$ git push xxx master     //提交到远程仓库的主分支，xxx代表远程仓库名，如果没绑定，到时候会弹出账号密码输入框
$ git remote -v           //列出所有远程仓库
$ git rm --cached \*.log  //从'暂存'中删除记录，但不删除本地文件
$ git fetch xxx           //从远程仓库拉取'本地没有的·不存在的'文件，xxx代表远程仓库名
```

* 工作流程<br>
先从远程仓库克隆<br>
