# kob

&emsp;&emsp;本项目为随书源码《c++微服务实践》
[![Build Status](https://travis-ci.org/gottingen/kob.svg?branch=master)](https://travis-ci.org/gottingen/kob)
# 前言

## 背景
&emsp;&emsp;微服务是一种分布式系统解决方案，推动细粒度的服务开发、测试、以及部署。
这些服务相互协同工作，每个服务有着自己独立的小生态。微服务的建模是以业务为中心，避免
了传统的分层架构的诸多问题。微服务整个了近年来先进的技术和理念，避免了架构设计上诸多
缺陷。

&emsp;&emsp;伴随着微服务理论的成熟，涌现了一大批微服务框架，广泛的应用到僧产环境中，
为企业提供了巨大收益。这写框架工具主要是java，go为主，使用这些框架，屏蔽工具内不复杂
设计、事件等处理。业务开发人员专注与业务的迭代开发，极大的提升了业务交付能力和项目质量。
在高并发、低延迟、高吞吐这个领域，目前还是一片空白。本书旨在接好一种微服务的实现方式，
同时，用c++实现这样的一套框架。

## 谁适合读本书

&emsp;&emsp;

## 内容安排

&emsp;&emsp;本书是以实践为主线进行，因此，本书每章都会从设计、运维、测试、集成、性能
几个方面去阐述实践的过程。

### 第一部分

* 第一章，基础环境
    
    这部分内容将从编译环境、构建工具，以及版本管理这几个方面阐述项目的周边配套生态。

* 第二张 编译环境
    
    这一章节内容，主要阐述c++编译器的特性。同时，介绍项目如何让框架去实现狂平台等特性。
    限于时间关系，重点介绍linux平台以及gcc编译器。
    
* 第三章 迈出第一步
    
    本章通过展示一个迷你stl实现，跨越c++1x，兼容c++11， c++14，c++17。同时，从性能
    和稳定的角度，修改部分标准stl的实现。
    
* 第四章 并发
    
    该部分内通介绍c++并行程序设计。
    
* 第五章 网络
    
    该部分介绍网络编程
    
* 第六章 内存

    linux内存管理以及内存使用优化
    
* 第七章 IO操作
    
    linux 文件系统操作以及优化
    
* 第八章 反应堆

    网络，IO，统一模型处理
    
* 第九章 服务框架

### 第二部分

* 第十章 监控与警报

* 第十一章 指标体系

* 第十二章 服务与自运维

* 第十三章 资源与调度

    