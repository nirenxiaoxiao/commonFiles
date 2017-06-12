========================================================================
    活动模板库：DocEditHelper 项目概述
========================================================================

************************系统要求************************
项目文件使用VS2017创建，编译器最低要求是VC2013。因为使用了std::thread等标准库。

该ActiveX控件用于帮助使用本地编辑器来打开指定URL对应的文档。

ActiveX控件名是 'DocumentEditHelper Class'。

项目附带了ActiveX Control Test Container程序用来测试ActiveX控件的方法和回调事件，
项目文件已经配置为生成成功后自动打开Test Container测试控件。

控件提供的方法：
    LoadDocument
	使用本地编辑器打开指定URL的文档。
	声明：
		LoadDocument(url);

		url: 要打开文档对应的URL。

	返回值:
		该文档文件对应的GUID。

	使用本地编辑器打开指定URL的文档。该方法完成以下操作：
		1.从该URL下载文档至本地临时位置。
		2.调用关联的本地编辑器，打开该文档，打开成功则将该文档加入到控件监视的列表。
		3.当文件的内容发生变化，或者打开文档的编辑器进程关闭时，自动将更改后的文件内容上传至服务器（未实现）。

	IsDocumentLoaded
	检测指定URL的文档是否已经打开。
	声明：
		IsDocumentLoaded(url);

		url:要检测的URL。

	检查指定URL的文档是否已经被本地编辑器打开。该方法将遍历控件内部的文档监视列表。如果找到对应的URL，则返回true。

控件属性：
	DisableLoadDuplicateURL: 是否禁止多次下载打开同一个URL的文档（默认为true）。

控件发出的事件：
	OnDownloadProgress(documentURL, docIdentifier, downloadBytes, totalBytes);
		指示文档下载的进度。
    OnDownloadBegin(documentURL, docIdentifier);
		在指定URL的文档下载之前，控件发出该通知。
    OnDownloadResult(documentURL, docIdentifier, errorCode, errorMsg);
		通知指定文档下载的结果。errorCode:libcurl的返回值。 errorMsg:libcurl的错误信息。
    OnDocumentModified(documentURL, docIdentifier);
		本地的文档被更改时，发出该通知。
    OnDocumentOpen(documentURL, docIdentifier, errorCode, OpenResult);
		下载好的文档被本地编辑器打开时，发出该通知。
    OnDocumentClosed(documentURL, docIdentifier);
		文档对应的本地编辑器关闭时，发出该通知。

内部实现：
	控件对象启动时，首先会创建一个临时目录。临时目录的位置如下：
		%temp%\DocEditHelperTemp
	    win7以上应该位于以下位置：C:\Users\<当前用户>\AppData\Local\Temp\DocEditHelperTemp

	然后启动后台线程，真正的工作都在后台线程内运行。
	控件对象提供的接口，通过向后台线程发送消息和对应操作的信息结构体，来完成对应的操作。
	后台工作线程对应的函数是DocHelperWorker::RunWork()，主要逻辑如下：

	在该线程内初始化windows消息队列
	while(running)
		检查监视中的文档对应的程序是否仍然在运行
		if(收到其它线程发过来的消息)
			处理该消息
		end
	end
	线程结束时关闭所有正在监视的文档对应的进程

	控件还会另外启动一个线程，用来监视临时文件目录的变化。
	当目录中的文件发生变化时，会向工作线程发消息，通知文件发生变化的相关信息，工作线程据此来运行相关操作，例如上传文件。

目前的缺陷：
	下载文件的操作是在工作线程上串行运行的（通过其它线程发消息来执行），不能并发下载。
	其它线程调用工作线程时，使用PostThreadMessage来向工作线程发消息。
	当工作线程被阻塞时，例如下载文件。PostThreadMessage发送的消息会被丢弃，造成内存泄露等问题。