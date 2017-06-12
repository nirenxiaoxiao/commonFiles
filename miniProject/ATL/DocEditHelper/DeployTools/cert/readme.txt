------------------------------------------------------------
数字签名使用方法：

方法一：
1、直接使用signcode对exe/dll/ocx文件进行签名
     signcode -spc thunisoft.spc -v thunisoft.pvk -t http://timestamp.wosign.com/timestamp [filename]

方法二：
1、使用pvkimprt.exe导入数字签名证书，按照提示默认加入即可，此过程执行一次即可
     pvkimprt  thunisoft.spc  thunisoft.pvk

2、使用signcode.exe对exe/dll/ocx文件进行签名
     signcode -cn 北京华宇信息技术有限公司 -t http://timestamp.wosign.com/timestamp [filename]

两种方法的区别：
方法一每次签名时都需要输入密码，适用于少量文件签名
方法二在导入时需要输入密码，之后签名时不需要，适用于大量文件签名和自动签名

推荐使用方法二来做自动构建

------------------------------------------------------------
公司数字签名证书，签名失效的解决办法
问题一：
问题描述：
    最近发现文书校对程序，签名失效了。结果是，原来签名的程序，数字签名全部失效。
    会报如下错误：“一个反签名无效，文件可能已被改动”
问题原因：
    经过和我们颁发证书的公司（WoSign）沟通，是原来时间戳认证（http://timestamp.wosign.com/timestamp）到期了。

解决办法：
    签名时不增加时间戳：
    原来签名方法：signcode -cn 北京华宇信息技术有限公司 -t http://timestamp.wosign.com/timestamp [filename]

   现在应该改为：signcode -cn 北京华宇信息技术有限公司 [filename]
  
   原来加过时间戳签名的程序，都需要重新进行签名。

问题二：
问题描述：
       如果取消时间戳，签名正常。但是，在某些机器上显示“无法验证签名中的证书”
问题原因：
       机器中没有安装根证书。

解决办法：
       运行附件更新证书中的《WoSignRootUpdate.exe》程序，安装根证书，就能验证通过了

问题三：
问题描述：
   签名时报如下错误：
   Error: There is more than one valid certificate in the my store
   Error: Signing Failed.  Result = 80092008, (-2146885624)

问题原因：
  公司存在多份数字签名证书，如果，你机器上装了多份证书，那么在签名时会报，有多个证书

解决办法：
  单击“开始→运行”，输入“certmgr.msc”，在打开的窗口中，删除多导入的证书即可


