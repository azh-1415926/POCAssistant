create database POC;

use POC;

CREATE TABLE if not exists users
(
    id CHAR(10) NOT NULL,
    name TEXT NOT NULL,
    password TEXT NOT NULL,
    role INT,
    create_time datetime,
    update_time datetime,
    last_login_time datetime,
    primary key(id)
);

CREATE TABLE if not exists class
(
    id CHAR(8) NOT NULL,
    name TEXT NOT NULL,
    teacher_id CHAR(10) NOT NULL,
    primary key(id),
    foreign key(teacher_id) REFERENCES users(id)
);

CREATE TABLE if not exists student
(
    id CHAR(10) NOT NULL,
    class_id CHAR(8) NOT NULL,
    teacher_id CHAR(10) NOT NULL,
    primary key(id),
    foreign key(id) REFERENCES users(id),
    foreign key(class_id) REFERENCES class(id),
    foreign key(teacher_id) REFERENCES users(id)
);

CREATE TABLE if not exists course_content
(
    id INT AUTO_INCREMENT NOT NULL,
    name TEXT NOT NULL,
    content TEXT NOT NULL,
    create_user CHAR(10),
    primary key(id),
    foreign key(create_user) REFERENCES users(id)
);

CREATE TABLE if not exists course
(
    id INT AUTO_INCREMENT NOT NULL,
    chapter INT NOT NULL,
    section INT NOT NULL,
    course_content_id INT NOT NULL,
    primary key(id),
    foreign key(course_content_id) REFERENCES course_content(id)
);

CREATE TABLE if not exists experiment
(
	id INT AUTO_INCREMENT NOT NULL,
	class_id CHAR(10) NOT NULL,
	name TEXT NOT NULL,
    content TEXT NOT NULL,
    create_time datetime,
    primary key(id),
    foreign key(class_id) REFERENCES class(id)
);

CREATE TABLE if not exists experiment_record
(
	id INT AUTO_INCREMENT NOT NULL,
	student_id CHAR(10) NOT NULL,
	experiment_id INT NOT NULL,
    code TEXT NOT NULL,
    finish_time datetime,
    score INT,
    isCorrect INT NOT NULL,
    primary key(id),
    foreign key(student_id) REFERENCES users(id),
    foreign key(experiment_id) REFERENCES experiment(id)
);

CREATE TABLE if not exists quiz
(
	id INT AUTO_INCREMENT NOT NULL,
	chapter INT NOT NULL,
	type INT NOT NULL,
    content TEXT NOT NULL,
    optionA TEXT NOT NULL,
    optionB TEXT NOT NULL,
    optionC TEXT,
    optionD TEXT,
    answer TEXT NOT NULL,
    primary key(id)
);

CREATE TABLE if not exists collected_quiz
(
	id INT AUTO_INCREMENT NOT NULL,
    student_id CHAR(10) NOT NULL,
    quiz_id INT NOT NULL,
    primary key(id),
    foreign key(student_id) REFERENCES users(id),
    foreign key(quiz_id) REFERENCES quiz(id),
    UNIQUE (student_id, quiz_id)
);

CREATE TABLE if not exists wrong_quiz
(
	id INT AUTO_INCREMENT NOT NULL,
    student_id CHAR(10) NOT NULL,
    quiz_id INT NOT NULL,
    wrong_count INT NOT NULL,
    primary key(id),
    foreign key(student_id) REFERENCES users(id),
    foreign key(quiz_id) REFERENCES quiz(id),
    UNIQUE (student_id, quiz_id)
);

insert into users values("2109104047","azh","1",0,NOW(),NOW(),NOW());
insert into users values("admin","admin","admin",2,NOW(),NOW(),NOW());

insert into course_content values(NULL,"引论","","admin");
insert into course_content values(NULL,"程序设计语言与编译程序的联系","","admin");
insert into course_content values(NULL,"编译程序构造及有关概念","","admin");
insert into course_content values(NULL,"形式语言与编译实现技术","","admin");

insert into course_content values(NULL,"文法与语言","","admin");
insert into course_content values(NULL,"符号串与符号串集合","","admin");
insert into course_content values(NULL,"文法与语言形式的定义","","admin");
insert into course_content values(NULL,"语言的分类","","admin");
insert into course_content values(NULL,"文法等价与等价变换","","admin");

insert into course_content values(NULL,"词法分析","","admin");
insert into course_content values(NULL,"引言","","admin");
insert into course_content values(NULL,"正则表达式与有穷状态自动机","","admin");
insert into course_content values(NULL,"词法分析程序的实现","","admin");
insert into course_content values(NULL,"词法分析程序的自动生成","","admin");

insert into course_content values(NULL,"语法分析---自顶向下分析技术","","admin");
insert into course_content values(NULL,"引言","","admin");
insert into course_content values(NULL,"带回溯的自顶向下的分析技术","","admin");
insert into course_content values(NULL,"无回溯的自顶向下分析技术","","admin");

insert into course_content values(NULL,"语法分析---自底向上分析技术","","admin");
insert into course_content values(NULL,"概述","","admin");
insert into course_content values(NULL,"简单优化分析技术","","admin");
insert into course_content values(NULL,"算符优先分析技术","","admin");
insert into course_content values(NULL,"LR(K) 分析技术","","admin");

insert into course_content values(NULL,"语义分析与目标代码生成","","admin");
insert into course_content values(NULL,"概述","","admin");
insert into course_content values(NULL,"说明部分的分析","","admin");
insert into course_content values(NULL,"目标代码的生成","","admin");
insert into course_content values(NULL,"LR(K) 分析技术","","admin");

insert into course_content values(NULL,"运行环境","","admin");
insert into course_content values(NULL,"引言","","admin");
insert into course_content values(NULL,"存储分配策略","","admin");
insert into course_content values(NULL,"符号表","","admin");

insert into course_content values(NULL,"代码优化","","admin");
insert into course_content values(NULL,"概述","","admin");
insert into course_content values(NULL,"基本块与流图","","admin");
insert into course_content values(NULL,"与循环有关的优化","","admin");
insert into course_content values(NULL,"窥孔优化","","admin");

insert into course values(NULL,0,0,1);
insert into course values(NULL,0,1,2);
insert into course values(NULL,0,2,3);
insert into course values(NULL,0,3,4);

insert into course values(NULL,1,0,5);
insert into course values(NULL,1,1,6);
insert into course values(NULL,1,2,7);
insert into course values(NULL,1,3,8);
insert into course values(NULL,1,4,9);
insert into course values(NULL,2,0,10);
insert into course values(NULL,2,1,11);
insert into course values(NULL,2,2,12);
insert into course values(NULL,2,3,13);
insert into course values(NULL,2,4,14);
insert into course values(NULL,3,0,15);
insert into course values(NULL,3,1,16);
insert into course values(NULL,3,2,17);
insert into course values(NULL,3,3,18);
insert into course values(NULL,4,0,19);
insert into course values(NULL,4,1,20);
insert into course values(NULL,4,2,21);
insert into course values(NULL,4,3,22);
insert into course values(NULL,4,4,23);
insert into course values(NULL,5,0,24);
insert into course values(NULL,5,1,25);
insert into course values(NULL,5,2,26);
insert into course values(NULL,5,3,27);
insert into course values(NULL,5,4,28);
insert into course values(NULL,6,0,29);
insert into course values(NULL,6,1,30);
insert into course values(NULL,6,2,31);
insert into course values(NULL,6,3,32);
insert into course values(NULL,7,0,33);
insert into course values(NULL,7,1,34);
insert into course values(NULL,7,2,35);
insert into course values(NULL,7,3,36);
insert into course values(NULL,7,4,37);

insert into quiz values(NULL,1,0,"编译器的主要功能是什么？","程序设计","程序翻译","程序调试","数据处理","B");
insert into quiz values(NULL,3,0,"词法分析器的主要任务是什么？","将源代码分解成多个语句","将源代码分解成多个单词","检查源代码的语法正确性","将单词转换为相应的语法单位","B");
insert into quiz values(NULL,4,0,"下列哪个是自上而下的语法分析方法？","LL(1)分析法","LR(1)分析法","LALR(1)分析法","GLR分析法","A");
insert into quiz values(NULL,6,0,"语义分析的主要任务是什么？","检查程序的语法正确性","检查程序的类型正确性","将源代码转换为目标代码","进行程序的优化","B");
insert into quiz values(NULL,2,0,"编译过程主要分为哪几个阶段？","词法分析、语法分析、代码优化","语法分析、代码生成、程序调试","词法分析、语法分析、语义分析、中间代码生成、代码优化、目标代码生成","语义分析、中间代码生成、目标代码生成","C");
insert into quiz values(NULL,2,0,"在编译过程中，哪个阶段负责将源代码中的标识符和关键字识别出来？","语法分析","词法分析","语义分析","代码生成","B");
insert into quiz values(NULL,1,0,"下列哪项不是编译器的组成部分？","词法分析器","语法分析器","解释器","代码生成器","C（注：解释器是执行程序的另一种方式，不是编译器的组成部分）");
insert into quiz values(NULL,3,0,"在语法分析阶段，编译器使用什么来描述程序的语法结构？","正规文法","上下文无关文法","语法树（或抽象语法树）","有限状态机","C（注：虽然B选项“上下文无关文法”也用于描述语法但其只是文法中的一种；在此问题的上下文中，更准确的答案是C，因为语法树是语法分析阶段的直接产物）");
insert into quiz values(NULL,6,0,"语义分析阶段的主要任务之一是进行类型检查，以下哪项不是类型检查的内容？","变量在使用前是否已被定义","运算符的操作数类型是否匹配","函数的调用顺序是否正确","赋值语句的左右两边类型是否兼容","C（注：这更多属于语法或逻辑错误的范畴，不是类型检查的重点）");
insert into quiz values(NULL,2,0,"在编译器的优化阶段，以下哪种优化技术不是常见的？","常量折叠","死代码消除","代码混淆","循环展开","C（注：代码混淆是为了保护代码不被轻易理解，不是编译器的优化目标）");
insert into quiz values(NULL,2,0,"在编译原理中，符号表的主要作用是？","存储源代码的文本内容","存储标识符的属性和作用域信息","记录编译过程中的错误信息","存储目标代码的二进制表示","B");
insert into quiz values(NULL,5,0,"以下哪种方法不是语法分析中使用的？","递归下降分析","LL(1)分析","动态规划","LR分析","C（注：动态规划是算法设计中的一种技术，不是语法分析方法）");
insert into quiz values(NULL,1,0,"在编译器的上下文中，中间代码是一种什么形式的代码？","机器语言代码","汇编语言代码","介于源代码和目标代码之间的抽象表示","高级语言代码","C");
insert into quiz values(NULL,5,0,"编译器在进行语法分析时，如果遇到语法错误，通常会？","立即停止编译并报告错误","尝试恢复并继续编译","忽略错误并继续编译","无法确定，取决于编译器的实现","B（注：许多现代编译器都具有一定的错误恢复能力，紧急方式的恢复策略）");
insert into quiz values(NULL,1,0,"以下哪个不是代码优化中常见的目标？","减少代码大小","提高运行速度","增加代码可读性","减少内存使用","C（注：优化通常是为了提高性能，而不是可读性）");
insert into quiz values(NULL,5,0,"在语法分析过程中，当遇到一个语法错误时，编译器通常采取的策略是？","总是报告第一个错误并停止编译","可能报告多个错误并尝试继续编译","从不报告错误，只是默默地生成错误的代码","只报告最后一个错误","B（注：一些编译器会尝试恢复并报告后续错误）");
insert into quiz values(NULL,2,0,"以下哪个概念与编译器的代码生成阶段紧密相关？","抽象语法树","符号表","目标机器的指令集","词法单元","C");
insert into quiz values(NULL,2,0,"在编译器的设计中，哪个阶段负责处理标识符的作用域和生命周期？","词法分析","语法分析","语义分析","代码生成","C（注：语义分析处理标识符的作用域、生命周期和类型等信息）");
insert into quiz values(NULL,3,0,"词法分析的主要任务是什么？","检查程序的语法结构","将源代码分解成单词，并识别出各个单词的类别","生成目标代码","分析程序的语义","B");
insert into quiz values(NULL,3,0,"在词法分析中，以下哪个概念用于描述词法单元的组合方式？","语法树","正则表达式","抽象语法树","有限状态机","B（注：正则表达式用于描述词法单元的模式）");
insert into quiz values(NULL,3,0,"词法分析器通常使用哪种数据结构来存储状态信息？","栈","队列","有限自动机","哈希表","C");
insert into quiz values(NULL,3,0,"以下哪个不是词法分析阶段需要处理的任务？","识别标识符和关键字","过滤掉注释和空白字符","检查变量的作用域","将数字字符串转换为数值常量","C（注：这是语义分析的任务）");
insert into quiz values(NULL,3,0,"在编译器的设计中，词法分析器通常位于哪个阶段之后？","语法分析","无，词法分析是编译过程的第一个阶段","语义分析","代码生成","B");
insert into quiz values(NULL,3,0,"以下哪个是词法分析器可能输出的结果？","语法树","词法记号流（或词法记号序列）","目标代码","抽象语法树","B");
insert into quiz values(NULL,3,0,"在词法分析中，处理保留字和关键字时，通常使用哪种方法？","正则表达式匹配","查找保留字表（或关键字表）","语法分析","语义分析","B（注：保留字表或关键字表用于存储所有关键字和它们的类别）");
insert into quiz values(NULL,3,0,"以下哪个不是词法分析器需要识别的单词类型？","标识符","关键字","语句","运算符","C（注：语句是语法分析阶段处理的对象）");
insert into quiz values(NULL,3,0,"词法分析器在识别单词时，通常会忽略哪些字符？","关键字","标识符","空白字符和注释","运算符","C");
insert into quiz values(NULL,2,0,"在编译器的上下文中，“单词”是指什么？","语法树的节点","源代码中的字符序列，它代表一个逻辑单元","源代码中的最小语法单元（如标识符、关键字、运算符等）","目标代码中的指令","C");
insert into quiz values(NULL,4,0,"语法分析的主要任务是什么？","将源代码分解成单词","根据语法规则分析源代码的语法结构","生成目标代码","检查程序的语义","B");
insert into quiz values(NULL,4,0,"在语法分析中，以下哪个概念用于描述源代码的结构？","词法单元流","语法树（或抽象语法树）","有限状态机","正则表达式","B");
insert into quiz values(NULL,5,0,"语法分析器通常使用哪种方法来解析源代码？","逐字符扫描","基于语法规则的分析（如递归下降分析、LL分析、LR分析等）","直接生成目标代码","使用正则表达式匹配","B");
insert into quiz values(NULL,4,0,"以下哪个不是语法分析阶段需要处理的任务？","检查源代码的括号匹配","分析变量的类型（注：这是语义分析的任务）","识别语句和表达式的结构","处理语法错误","B");
insert into quiz values(NULL,5,0,"以下哪个是语法分析器可能输出的结果？","词法单元流","语法树（或抽象语法树）","目标代码","符号表","B");
insert into quiz values(NULL,4,0,"在语法分析中，处理递归结构时，通常使用哪种技术？","正则表达式匹配","递归下降分析","有限自动机","动态规划","B");
insert into quiz values(NULL,4,0,"以下哪个不是语法分析器需要识别的语法结构？","语句","表达式","单词（注：单词是词法分析的对象）","函数定义","C");
insert into quiz values(NULL,2,0,"在编译器的上下文中，“语法错误”是指什么？","源代码中的拼写错误","源代码违反了语法规则","源代码中的逻辑错误","源代码中的性能问题","B");
insert into quiz values(NULL,5,0,"以下哪个是语法分析器在解析源代码时可能会使用的数据结构？","栈","语法栈（用于存储解析过程中的状态信息）","队列","哈希表","B（注：虽然A栈是一般的数据结构，但在这里更具体地指的是语法分析器中用于存储解析状态的栈，因此B选项更为准确）");
insert into quiz values(NULL,1,0,"中间代码的主要目的是什么？","直接执行程序","为编译器后端提供与机器无关的程序表示","替代高级语言","作为源代码的另一种形式","B");
insert into quiz values(NULL,1,0,"以下哪个是中间代码可能采用的表示形式？","汇编语言","机器语言","三地址码（或类似的三元组表示）","高级语言","C");
insert into quiz values(NULL,1,0,"中间代码与目标代码的主要区别是什么？","中间代码是抽象的，目标代码是具体的","中间代码与机器无关，目标代码与机器相关","中间代码用于编译阶段，目标代码用于执行阶段","中间代码是高级语言的表示，目标代码是低级语言的表示","B");


-- insert into quiz values(NULL,1,1,"这是一道判断题","选项A","选项B","","","正确");

insert into quiz values(NULL,1,1,"编译器的任务是将高级语言程序翻译成机器语言程序。","","","","","对");
insert into quiz values(NULL,3,1,"词法分析是编译过程中的第一个阶段，它的主要任务是将源代码分解成单词。","","","","","对");
insert into quiz values(NULL,4,1,"语法分析器在解析源代码时，会直接生成目标代码。","","","","","错（语法分析器生成的是语法树或抽象语法树，而不是目标代码）");
insert into quiz values(NULL,1,1,"中间代码是一种与机器无关的程序表示，它方便编译器的后端进行优化和转换。","","","","","对");
insert into quiz values(NULL,6,1,"语义分析阶段的主要任务是检查源代码的语法结构是否正确。","","","","","错（语义分析阶段的主要任务是检查源代码的语义是否正确，如类型检查、作用域检查等）");
insert into quiz values(NULL,1,1,"编译器的优化阶段主要是为了提高生成的目标代码的执行效率。","","","","","对");
insert into quiz values(NULL,1,1,"目标代码生成器将中间代码直接翻译成机器语言指令。","","","","","对（但通常还会考虑目标机器的特性和优化）");
insert into quiz values(NULL,3,1,"词法分析器在识别单词时，会考虑源代码的上下文信息。","","","","","错（词法分析器主要基于正则表达式等模式匹配技术识别单词，通常不考虑上下文信息，上下文敏感的分析通常在语法或语义分析阶段进行）");
insert into quiz values(NULL,1,1,"编译器的前端主要负责源代码的分析和转换，而后端主要负责目标代码的生成和优化。","","","","","对");
insert into quiz values(NULL,1,1,"在编译过程中，如果源代码存在语法错误，编译器一定会报错并停止编译。","","","","","对（但某些编译器可能会提供继续编译的选项，以尽可能多地报告错误或警告）");


insert into quiz values(NULL,1,2,"编译器的主要任务是将（     ）翻译成（     ）。","","","","","高级语言程序；机器语言程序。");
insert into quiz values(NULL,2,2,"在编译过程中，（     ）阶段负责将源代码中的字符序列转换为记号（token）序列。","","","","","词法分析。");
insert into quiz values(NULL,3,2,"（     ）阶段根据语言的语法规则，将词法分析产生的记号序列转换成一个语法树。","","","","","语法分析。");
insert into quiz values(NULL,2,2,"（     ）阶段检查源程序的语义一致性，并进行一些中间表示的转换。","","","","","语义分析。");
insert into quiz values(NULL,6,2,"（     ）阶段将经过语义分析后的中间表示转换为更接近目标机器的中间代码。","","","","","中间代码生成。");
insert into quiz values(NULL,1,2,"（     ）阶段对中间代码进行变换，以提高目标代码的运行效率。","","","","","代码优化。");
insert into quiz values(NULL,1,2,"（     ）阶段将中间代码转换为具体的机器指令。","","","","","目标代码生成。");
insert into quiz values(NULL,3,2,"在编译器的设计中，（     ）是一种常用的描述词法分析器行为的工具。","","","","","状态转换图或有限自动机。");
insert into quiz values(NULL,1,2,"（     ）是一种描述程序设计语言语法的有效工具，它不考虑上下文信息。","","","","","上下文无关文法或2型文法。");
insert into quiz values(NULL,1,2,"编译器中的（     ）负责在编译过程中检测并报告源程序中的错误。","","","","","错误处理机制或错误处理模块。");


-- insert into quiz values(NULL,1,3,"这是一道编程题","","","","","答案");

-- select * from quiz where chapter=1;

-- 查询指定学生、指定章的题目，附带上收藏信息
-- SELECT 
--     q.*,
--     EXISTS (
--         SELECT 1 
--         FROM collected_quiz cq 
--         WHERE cq.quiz_id = q.id 
--           AND cq.student_id = '2109104047'
--     ) AS collected
-- FROM 
--     quiz q
-- WHERE 
--     q.chapter = 1;
   
-- 查询指定学生、指定章收藏的题目
-- SELECT q.*
-- FROM quiz q
-- WHERE q.chapter = 1
--   AND EXISTS (
--     SELECT 1 
--     FROM collected_quiz cq 
--     WHERE cq.quiz_id = q.id 
--       AND cq.student_id = '2109104047'
--   );
 
--  -- 批量插入指定学生收藏的题目id到 collected_quiz
--  INSERT IGNORE INTO collected_quiz (student_id, quiz_id)
-- SELECT 
--   '2109104047',  -- 替换为学生ID，如 'S1001'
--   id 
-- FROM quiz 
-- WHERE id IN (1, 2);  -- 指定需要收藏的题目ID列表

-- -- 批量删除指定学生收藏的题目id到 collected_quiz
-- DELETE FROM collected_quiz 
-- WHERE student_id = '2109104047' 
-- AND quiz_id IN (1,3);

-- -- 批量插入指定学生错误的题目id到 wrong_quiz
-- INSERT INTO wrong_quiz (student_id, quiz_id, wrong_count)
-- VALUES 
--   ('2109104047', 1, 1),
--   ('2109104047', 2, 1),  -- 假设题目ID=2已存在
--   ('2109104047', 3, 1)   -- 按需扩展更多记录
-- ON DUPLICATE KEY UPDATE 
--   wrong_count = wrong_count + 1;
 
--  -- 批量删除指定学生错误的题目id到 wrong_quiz
-- DELETE FROM wrong_quiz 
-- WHERE student_id = '2109104047' 
-- AND quiz_id IN (1,2,3) AND wrong_count=1;

-- UPDATE IGNORE wrong_quiz
-- SET wrong_count=wrong_count-1
-- WHERE student_id = '2109104047' 
-- AND quiz_id IN (1,2,3)

-- SELECT COALESCE(MAX(chapter), 0) AS max_chapter FROM quiz;

-- SELECT s.id,u.name 
-- FROM student s
-- LEFT JOIN users u ON s.id = u.id 
-- WHERE s.class_id='21091040'

-- SELECT e.id,e.name,e.content,er.code 
-- FROM experiment_record er 
-- LEFT JOIN experiment e ON er.experiment_id = e.id 
-- LEFT JOIN student s ON s.id = er.student_id 
-- WHERE s.id='2109104047'

-- -- 用户总数
-- SELECT COUNT(*) AS total_users FROM users;

-- -- 学生总数
-- SELECT COUNT(*) AS total_students FROM users WHERE role = 1;

-- -- 教师总数
-- SELECT COUNT(*) AS total_teachers FROM users WHERE role = 2;

-- -- 最近登录的用户
-- SELECT name FROM users ORDER BY last_login_time DESC LIMIT 1;

-- -- 最久未登录的用户
-- SELECT name FROM users ORDER BY last_login_time ASC LIMIT 1;

-- -- 收藏题目最多的学生
-- SELECT 
--     u.name AS student_name, 
--     COUNT(cq.quiz_id) AS total_collected
-- FROM collected_quiz cq
-- JOIN users u ON cq.student_id = u.id
-- GROUP BY u.id, u.name
-- ORDER BY total_collected DESC
-- LIMIT 1;

-- -- 答错题目最多的学生
-- SELECT 
--     u.id AS student_id, 
--     u.name AS student_name, 
--     SUM(wq.wrong_count) AS total_wrong
-- FROM wrong_quiz wq
-- JOIN users u ON wq.student_id = u.id
-- GROUP BY u.id, u.name
-- ORDER BY total_wrong DESC
-- LIMIT 1;

-- -- 管理班级数最多的老师
-- SELECT 
--     u.name AS teacher_name
-- FROM class c
-- JOIN users u ON c.teacher_id = u.id
-- GROUP BY u.id, u.name
-- ORDER BY total_classes DESC
-- LIMIT 1;

-- SELECT
--     -- 用户总数
--     (SELECT COUNT(*) FROM users) AS total_users,
--     -- 学生总数
--     (SELECT COUNT(*) FROM users WHERE role = 1) AS total_students,
--     -- 教师总数
--     (SELECT COUNT(*) FROM users WHERE role = 2) AS total_teachers,
--     -- 最近登录的用户
--     (SELECT name FROM users ORDER BY last_login_time DESC LIMIT 1) AS recent_login_user,
--     -- 最久未登录的用户
--     (SELECT name FROM users ORDER BY last_login_time ASC LIMIT 1) AS inactive_user;
    
-- SELECT e.* 
--         FROM experiment e 
--         LEFT JOIN experiment_record er ON e.id = er.experiment_id AND er.student_id = '2109104039'
--         LEFT JOIN student s ON e.class_id = s.class_id AND er.student_id = s.id
--         WHERE er.id IS NULL;
        
-- SELECT e.id,e.name,e.content,er.code 
-- FROM experiment_record er 
-- LEFT JOIN experiment e ON er.experiment_id = e.id and er.student_id='2109104039'
-- LEFT JOIN student s ON s.id = er.student_id 
-- WHERE er.isCorrect=0 and e.id IS NOT NULL ;