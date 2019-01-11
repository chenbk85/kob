# IO and buffer

&emsp;&emsp; io_buffer 对象主要应用在网络编程和系统IO编程中。主要参考了nginx的中
ngx_buf数据结构，以及linux 内核中的sk_buf;

## 需求分析
&emsp;&emsp; 在动手写代码之前，回想一下之前做过的业务，冥想一下我们的愿景，有这么几个想法浮现出来。

* zero copy, 我们希望数据的持续接收和发送，在用户态，至少在append，read过程中，尽量做到zero copy
* sharing, 在某些业务场景中，比如LRU缓存系统中，我们希望能通过sharing方式将数据装载到缓冲区，
不进行数据的拷贝。而且是在多线程环境中。
* memory control, 在某些场景中，我们需要对齐的内存，在某些场景中，我们需要mmap的内存。在某些场景中，我们需要的
线程内部buffer，不进行sharing，是否可以做一些优化。
* life control, 生命周期管理，内存的生命周期控制，在c++编程中是非常重要，稍有不慎，你将获得很赞的名字（core某）
* cpp like, 希望能像stl一样，有通用的接口可供使用，减少使用者的学习成本。
* Synchronization 在多线程环境下使用io_buffer，同一时间，只有一个线程去写数据，可以多线程去读数据。多线程写数据
将不被允许
* file or memory, 在网络编程中，某些时候我们并不需要将数据拷贝到用户态内存中，可以通过sendfile直接从内核态拷贝到网络缓冲区。
  这是nginx的用法。在多个用户请求相同的文件时，这部分缓存是linux帮我们做了优化，通过page cache来完成。

* 实现中，尽量避免使用反射机制
* 尽量做到泛型化

&emsp;&emsp;