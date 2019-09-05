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
```
--选择一个本地目录
进入一个目录，空白处右键Git Bush here，然后会自动进入命令行，并将此目录作为命令行当前目录。
或直接打开git-bash.exe，cd进入一个目录下。
--从远程仓库clone一份副本
例如，从GitHub上打开一个respository,找到clone，里面有一个https的链接地址，复制下来。
在命令行下键入git clone https://github.com/Find-A-Job/CPLUSPLUS.git,回车等待，下载结束后会在当前目录下创建一份副本
--操作所在目录，检查状态
cd进入这个仓库目录，键入git status，回车查看当前状态，每执行完一次命令行操作后，都可以检查一下状态
--添加，修改这个目录里的内容
直接修改文件，拷贝文件，删除文件等操作都只影响本地
--提交到'暂存'
键入git add .，将所有改动记录到'暂存'
--更新本地仓库
键入git commit -m "xxxx"，将'暂存'信息更新到本地仓库
--提交到远程仓库
键入git push xxx master， 将本地仓库的改动更新到远程仓库
--从远程仓库拉取新文件到本地
在提交到远程仓库时，可能会失败，其原因是远程仓库最新的文件，与本地存放的上一次clone或fetch到的文件不一致，这时候需要先同步一下。
同步过后可能会存在crush，需要自己解决后，才能重新提交
```
