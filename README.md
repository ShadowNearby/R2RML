# Project Title

This project is a C++ application. It is a key-value store that supports concurrent operations and is designed to handle triples of data. The project also includes a parser for R2RML files and a query handler for MySQL.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- C++ compiler with C++20 support
- MySQL server

## Details
### `R2RMLParser`
主要功能是解析R2RML映射。R2RML是一种用于将关系数据库数据映射到RDF数据模型的语言。

在这个函数中，首先通过查询存储中的三元组来获取所有的`TriplesMap`。然后，对每一个`TriplesMap`，解析其逻辑表（LogicalTable）、主题映射（SubjectMap）和谓词对象映射（PredicateObjectMap）。在解析过程中，会根据R2RML的规则，获取并设置相关的属性，如模板（template）、常量（constant）、列（column）等。

此外，这个函数还处理了对象映射（ObjectMap）中的引用对象映射（RefObjectMap）。如果存在引用对象映射，会找到对应的父`TriplesMap`，并将其主题映射设置为当前对象映射。

总的来说，`R2RMLParser::parse`函数的主要功能是解析R2RML映射，将关系数据库的表结构和数据映射到RDF的三元组结构中。
### `SelectQuery`
`getJoinRows`函数的主要功能是执行SQL联接查询，并将查询结果转换为内部数据结构以供后续处理。

具体来说，这个函数首先根据传入的表名、引用对象映射（RefObjectMap）、主题列（subject_columns）和对象列（object_columns）构建SQL联接查询语句。然后，执行这个查询语句并获取查询结果。

接下来，函数将查询结果转换为内部数据结构。这个过程中，它会创建一个新的向量`v1`来存储查询结果，并使用OpenMP并行处理来加速转换过程。转换过程中，每一行的每一列都会被转换为一个内部ID，并存储在`v1`中。

最后，函数会构建一个标签（labels）映射，用于记录每一列的名称和对应的索引位置。这个映射会被存储在`join_index`中，以供后续处理使用。

总的来说，`getJoinRows`函数的主要功能是执行SQL联接查询，并将查询结果转换为内部数据结构以供后续处理。

`getAll`函数的主要功能是从数据库中获取所有表的数据，并将这些数据存储在内部数据结构中以供后续处理。

具体来说，函数首先遍历数据库中的所有表名。对于每一个表名，它会构建一个SQL查询语句，用于获取该表的所有数据。然后，执行这个查询语句并获取查询结果。

接下来，函数将查询结果转换为内部数据结构。这个过程中，它会创建一个新的向量`v1`来存储查询结果，并使用OpenMP并行处理来加速转换过程。转换过程中，每一行的每一列都会被转换为一个内部ID，并存储在`v1`中。

最后，函数会构建一个标签（labels）映射，用于记录每一列的名称和对应的索引位置。这个映射会被存储在`tables_index`中，以供后续处理使用。

总的来说，`getAll`函数的主要功能是从数据库中获取所有表的数据，并将这些数据存储在内部数据结构中以供后续处理。
### `Handle`
是`Handle`类的构造函数，它的主要作用是初始化`Handle`对象并处理R2RML映射。

在这个函数中，首先创建了一个`ConKVStore`对象`result`和一个`SelectQuery`对象`selectQuery`。`result`用于存储查询结果，`selectQuery`用于执行SQL查询。

然后，函数调用`parser.parse(store)`来解析R2RML映射。解析结果会存储在`R2RMLParser::triplesMaps`中。

接下来，函数遍历`R2RMLParser::triplesMaps`中的每一个`TriplesMap`。对于每一个`TriplesMap`，函数会处理其主题映射（SubjectMap）和谓词对象映射（PredicateObjectMap）。

在处理主题映射时，如果主题映射有主题类（subjectClass），函数会调用`replaceTemplate`来替换模板中的占位符。

在处理谓词对象映射时，函数会处理每一个对象映射（ObjectMap）。如果对象映射是非父映射（non-parentTriplesMap），函数会调用`replaceTemplate`来替换模板中的占位符。如果对象映射是父映射（parentTriplesMap），函数会调用`selectQuery.getJoinRows`来执行SQL联接查询，并调用`replaceTemplate`来替换模板中的占位符。

总的来说，`Handle::Handle`的主要作用是初始化`Handle`对象并处理R2RML映射。