# 开题报告

**课题研究（项目设计）目的和意义（含国内外研究现状综述**

传统的编译原理课程教学往往侧重于理论讲解，缺乏实践操作的即时反馈。本项目旨在通过开发小助手，为学生提供一个实践平台，使他们能够在课程学习中即时验证编译原理中的概念和算法，从而加深理解。编译原理课程内容复杂、抽象，对初学者而言难度较大。本项目通过设计友好的用户界面和直观的操作流程，帮助学生逐步理解编译过程的各个环节，降低学习门槛。通过小助手中的示例代码、实验指导和在线帮助文档，学生可以在课外时间自主进行编译原理的学习和实践，培养他们的自主学习能力和解决问题的能力。教师可以利用小助手进行课堂演示，展示编译过程的内部机制，同时也可以作为学生作业的辅助批改工具，提高批改效率。

通过引入现代化的教学手段和工具，推动编译原理课程的教学改革，使教学更加贴近实际应用，提高学生的实践能力和创新能力。小助手提供了丰富的实践资源和即时反馈机制，有助于加深学生对编译原理的理解和掌握，从而提高教学质量。本项目的研究成果可以作为编译原理课程的教学资源，供其他教师和学生使用，促进教学资源的共享和交流。

在国内，随着计算机教育的不断发展，越来越多的高校开始重视编译原理课程的教学。近年来，一些高校和科研机构开始尝试将编译原理与实际应用相结合，开发出一系列的教学辅助工具。这些工具大多采用Java、C#等编程语言实现，而在C++领域的研究相对较少。尽管如此，仍有一些学者和教师在探索基于C++的编译原理教学工具的开发，以期提高教学效果和学习体验。

在国外，编译原理课程的教学和研究起步较早，教学辅助工具的开发也相对成熟。许多知名的大学和研究机构都拥有自己的编译原理教学平台和工具。这些工具不仅功能丰富、操作简便，而且注重与课程内容的紧密结合。此外，国外学者还积极探索将编译原理与人工智能、大数据等新兴技术相结合，开发出更加智能化的教学工具。然而，在C++领域的教学辅助工具方面，虽然也有相关研究，但相对于其他编程语言而言仍显不足。

**课题研究（项目设计）内容（提纲）**

本系统基于C++，面向学生、教师群体打造一个系统学习编译原理课程的平台，有助于帮助学生深入理解其中原理，主要功能体现在三个模块，分为在线学习、在线实验、在线测试模块。

1.在线学习模块

设计课程目录结构，组织学习资料。

对于编译原理课程的知识点，教师可上传到在线学习模块，帮助学生梳理知识点。

学生可在此浏览课程的知识点。

可增加额外功能，例如提供学习进度跟踪、笔记记录等功能

2.在线实验模块

设计实验环境，提供编码环境，学生可编写代码用于完成实验（词法分析、语法分析、语义分析、代码生成等）。

实现实验代码编辑器，支持C++代码编写和编译。

提供实验指导、代码示例和错误提示，帮助学生完成实验。

3.在线测试模块

设计题库系统，包含选择题、填空题、编程题等多种题型。

实现自动批改功能，比如选择题。

提供成绩统计、错题集、学习报告等反馈机制。

**研究（设计）方法和思路（技术路线）**

本项目是编译原理小助手，开发平台为Windows平台，使用Visual Studio以及Visual Studio Code作为开发工具，基于B/S架构开发，将采用开源免费的MySQL作为数据库，使用Git工具对项目版本进行管理。前端将使用WebAssembly技术+Qt图形框架进行开发，将C/C++源代码编译为web汇编代码，然后在浏览器上运行。后端同样使用C++进行开发，处理业务逻辑和数据库操作，将使用一些高性能web库，比如drogon。通过分离生产代码与开发代码来保持程序的稳定性。系统采用MVC模式开发，将把本项目分为三大层，分别为模型（Model）、视图（View）、控制器（Controller），这样分离成独立的组件，每个组件都负责其特定的任务，这种分离有助于减少组件之间的依赖性和耦合度，从而提高了代码的可维护性和可扩展性。