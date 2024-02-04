# Project Title

This project is a C++ application. It is a key-value store that supports concurrent operations and is designed to handle
triples of data. The project also includes a parser for R2RML files and a query handler for MySQL.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing
purposes.

### Prerequisites

- C++ compiler with C++20 support
- MySQL server

## R2RML

R2RML（RDB to RDF Mapping Language）是一种用于描述关系型数据库（RDB）到 RDF（Resource Description
Framework）的映射规则的语言。RDF是一种用于表示资源的框架，它使用统一的资源标识符（URI）来唯一标识资源，并通过三元组（主语、谓语、宾语）的形式表示资源之间的关系。

R2RML 的主要目标是允许将关系型数据库中的数据映射到 RDF 格式，使得数据库中的结构和内容能够以 RDF 的形式暴露给语义网（Semantic
Web）应用。这种映射对于将现有的关系型数据整合到语义网中，实现跨数据源的语义查询和推理非常有用。

R2RML 规范定义了三个主要的映射元素：

1. **TriplesMap（三元组映射）：** 表示一个数据库表或查询的映射。它定义了如何将数据库中的数据映射到 RDF 三元组。

2. **SubjectMap（主语映射）：** 定义了如何映射关系型数据库中的主语信息到 RDF 中的主语。

3. **PredicateObjectMap（谓语宾语映射）：** 定义了如何映射关系型数据库中的谓语和宾语信息到 RDF 三元组中的谓语和宾语。

R2RML 使用模板（template）、常量（constant）、列名（column）等概念来描述映射规则，使得用户能够根据实际需求定义灵活的映射规则。通过
R2RML，可以将关系型数据库中的结构化数据映射为具有语义信息的 RDF 数据，为语义网应用提供了丰富的数据源。

### Example

考虑一个关系型数据库中的表 "Employees"，包含列 "EmployeeID"、"FirstName"、"LastName" 和 "Department"。我们希望将这些数据映射到
RDF 格式，以便在语义网中使用。

1. **创建 TriplesMap：**

```turtle
@prefix rr: <http://www.w3.org/ns/r2rml#> .

<#EmployeeTriplesMap>
  a rr:TriplesMap ;
  rr:logicalTable [ rr:tableName "Employees" ] ;
  rr:subjectMap [ rr:template "http://example.org/employees/{EmployeeID}" ] ;
  rr:predicateObjectMap [
    rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/worksAt" ] ;
    rr:objectMap [ rr:column "Department" ]
  ] ;
  rr:predicateObjectMap [
    rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/firstName" ] ;
    rr:objectMap [ rr:column "FirstName" ]
  ] ;
  rr:predicateObjectMap [
    rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/lastName" ] ;
    rr:objectMap [ rr:column "LastName" ]
  ] .
```

这个示例中，`EmployeeTriplesMap` 是一个 TriplesMap，定义了与 "Employees" 表的映射。`rr:logicalTable`
定义了数据库表，`rr:subjectMap` 定义了主语的映射规则，使用了模板生成主语的 URI。

2. **主语映射 SubjectMap：**

```turtle
<#SubjectMap>
  a rr:SubjectMap ;
  rr:template "http://example.org/employees/{EmployeeID}" .
```

这个 SubjectMap 定义了主语的映射规则，使用了模板生成主语的 URI。

3. **谓语宾语映射 PredicateObjectMap：**

```turtle
<#PredicateObjectMap1>
  a rr:PredicateObjectMap ;
  rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/worksAt" ] ;
  rr:objectMap [ rr:column "Department" ] .

<#PredicateObjectMap2>
  a rr:PredicateObjectMap ;
  rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/firstName" ] ;
  rr:objectMap [ rr:column "FirstName" ] .

<#PredicateObjectMap3>
  a rr:PredicateObjectMap ;
  rr:predicateMap [ rr:constant "http://xmlns.com/foaf/0.1/lastName" ] ;
  rr:objectMap [ rr:column "LastName" ] .
```

这些 PredicateObjectMap 定义了谓语和宾语的映射规则，使用了常量生成谓语，使用列名生成宾语。

## Details

### `R2RMLParser`

主要功能是解析R2RML映射。R2RML是一种用于将关系数据库数据映射到RDF数据模型的语言。

在这个函数中，首先通过查询存储中的三元组来获取所有的`TriplesMap`。然后，对每一个`TriplesMap`
，解析其逻辑表（LogicalTable）、主题映射（SubjectMap）和谓词对象映射（PredicateObjectMap）。在解析过程中，会根据R2RML的规则，获取并设置相关的属性，如模板（template）、常量（constant）、列（column）等。

此外，这个函数还处理了对象映射（ObjectMap）中的引用对象映射（RefObjectMap）。如果存在引用对象映射，会找到对应的父`TriplesMap`
，并将其主题映射设置为当前对象映射。

总的来说，`R2RMLParser::parse`函数的主要功能是解析R2RML映射，将关系数据库的表结构和数据映射到RDF的三元组结构中。

### `SelectQuery`

`getJoinRows`函数的主要功能是执行SQL联接查询，并将查询结果转换为内部数据结构以供后续处理。

具体来说，这个函数首先根据传入的表名、引用对象映射（RefObjectMap）、主题列（subject_columns）和对象列（object_columns）构建SQL联接查询语句。然后，执行这个查询语句并获取查询结果。

接下来，函数将查询结果转换为内部数据结构。这个过程中，它会创建一个新的向量`v1`
来存储查询结果，并使用OpenMP并行处理来加速转换过程。转换过程中，每一行的每一列都会被转换为一个内部ID，并存储在`v1`中。

最后，函数会构建一个标签（labels）映射，用于记录每一列的名称和对应的索引位置。这个映射会被存储在`join_index`中，以供后续处理使用。

总的来说，`getJoinRows`函数的主要功能是执行SQL联接查询，并将查询结果转换为内部数据结构以供后续处理。

`getAll`函数的主要功能是从数据库中获取所有表的数据，并将这些数据存储在内部数据结构中以供后续处理。

具体来说，函数首先遍历数据库中的所有表名。对于每一个表名，它会构建一个SQL查询语句，用于获取该表的所有数据。然后，执行这个查询语句并获取查询结果。

接下来，函数将查询结果转换为内部数据结构。这个过程中，它会创建一个新的向量`v1`
来存储查询结果，并使用OpenMP并行处理来加速转换过程。转换过程中，每一行的每一列都会被转换为一个内部ID，并存储在`v1`中。

最后，函数会构建一个标签（labels）映射，用于记录每一列的名称和对应的索引位置。这个映射会被存储在`tables_index`中，以供后续处理使用。

总的来说，`getAll`函数的主要功能是从数据库中获取所有表的数据，并将这些数据存储在内部数据结构中以供后续处理。

### `Handle`

是`Handle`类的构造函数，它的主要作用是初始化`Handle`对象并处理R2RML映射。

在这个函数中，首先创建了一个`ConKVStore`对象`result`和一个`SelectQuery`对象`selectQuery`。`result`
用于存储查询结果，`selectQuery`用于执行SQL查询。

然后，函数调用`parser.parse(store)`来解析R2RML映射。解析结果会存储在`R2RMLParser::triplesMaps`中。

接下来，函数遍历`R2RMLParser::triplesMaps`中的每一个`TriplesMap`。对于每一个`TriplesMap`
，函数会处理其主题映射（SubjectMap）和谓词对象映射（PredicateObjectMap）。

在处理主题映射时，如果主题映射有主题类（subjectClass），函数会调用`replaceTemplate`来替换模板中的占位符。

在处理谓词对象映射时，函数会处理每一个对象映射（ObjectMap）。如果对象映射是非父映射（non-parentTriplesMap），函数会调用`replaceTemplate`
来替换模板中的占位符。如果对象映射是父映射（parentTriplesMap），函数会调用`selectQuery.getJoinRows`
来执行SQL联接查询，并调用`replaceTemplate`来替换模板中的占位符。

总的来说，`Handle::Handle`的主要作用是初始化`Handle`对象并处理R2RML映射。