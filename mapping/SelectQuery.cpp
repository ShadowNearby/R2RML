//
// Created by yanjs on 2022/11/1.
//

#include "SelectQuery.h"

#include <utility>
#include "omp.h"
//const std::string schema_name = "gtfs-sql";

//namespace
inline std::string castQuery(mysqlx::Session& ses, std::string table, std::string db) {
	mysqlx::SqlResult result = ses.sql("select column_name from information_schema.`COLUMNS` where table_name='" + table + "' and table_schema='" + db + "'").execute();
	std::vector<mysqlx::Row>v = result.fetchAll();
	std::string sqlquery = "SELECT ";
	for (mysqlx::Row r : v) {
		sqlquery += std::string("cast(") + std::string(r[0]) + std::string(" as char) as " + std::string(r[0]) + ",");
	}
	sqlquery = sqlquery.substr(0, sqlquery.size() - 1) + " From " + db + "." + table + ";";
	return sqlquery;
}

SelectQuery::SelectQuery(std::string user, std::string password, std::string schema_name, int thread_num, ConKVStore* store, std::chrono::steady_clock::time_point start) : session(
	mysqlx::Session("localhost", 33060, user, std::move(password))), threadnum(thread_num), db(session.getSchema(schema_name)), result(store),start(start)
{
	this->schema_name = schema_name;
	//auto start = std::chrono::steady_clock::now();
	double query_time = getAll();
	//std::cout<<"Query time in all "<<query_time<<std::endl;
//	auto end = std::chrono::steady_clock::now();
	//std::cout<<"All tables read and processed in " << std::chrono::duration < double >(end - start).count()<<" seconds" << std::endl;
}

double SelectQuery::getAll()
{
	//double querytime = 0;
	for (const auto& tableName : db.getTableNames()) {
		std::string table_name = tableName;
		//auto start = std::chrono::steady_clock::now();
		//auto sqlResult = db.getTable(tableName).select("*").execute();
		auto sqlResult = session.sql(castQuery(session, tableName, this->schema_name)).execute();
		auto& columns = sqlResult.getColumns();

		
		std::vector<mysqlx::Row> temp = sqlResult.fetchAll();
		//auto end = std::chrono::steady_clock::now();
		//querytime += std::chrono::duration < double >(end - start).count();
		int len = temp.size(), rowlen = temp[0].colCount();
		auto a = std::chrono::high_resolution_clock::now();
		//if (tableName == "shapes")threadnum = 1;
		int fragmentsize = (len + threadnum - 1) / threadnum;
		auto v1 = new std::vector<std::vector<size_t>>(len, std::vector<size_t>(rowlen));

		//std::cout << "conversion started\n";
		/*if (temp[0][j].getType() == mysqlx::Value::DOUBLE)omp_set_num_threads(1);
		else omp_set_num_threads(threadnum);*/
		omp_set_num_threads(threadnum);
#pragma omp parallel for schedule(static)
		for (int k = 0; k < threadnum; k++) {

			for (int l = fragmentsize * k; l < fragmentsize * (k + 1) && l < len; l++)
			{
				for (int j = 0; j < rowlen; j++)
				{
					if (!temp[l][j].isNull()) {
						
						size_t id = result->get(std::string(temp[l][j]));
						(*v1)[l][j] = id;
					}
					else (*v1)[l][j] = 0;
					//temp[l][j] = "";
					//std::cout << "here\n";
				}
				temp[l].clear();
			}
		}
		std::vector<mysqlx::Row> tempEmpty;
		temp.swap(tempEmpty);
		//auto b = std::chrono::high_resolution_clock::now();
		//std::cout << "conversion time: " << std::chrono::duration_cast<std::chrono::microseconds>(b - a).count() / 1000000.0 << ", size : " << len << std::endl;
		std::unordered_map<std::string, int> labels;
		int i = 0;
		for (const auto& item : columns) {
			labels[item.getColumnLabel()] = i;
			++i;
		}
		tables_index[table_name] = labels;
		tables[table_name] = v1;
		//        size_t column_count = labels.size();
		//        auto table = new std::vector<std::vector<mysqlx::Value>>;
		//        for (const auto &row: rows) {
		//            std::vector<mysqlx::Value> vec_row(column_count);
		//            for (int j = 0; j < column_count; ++j) {
		//                vec_row[j] = row[j];
		//            }
		//            table->emplace_back(vec_row);
		//        }
		//        tables[table_name] = table;
	}
}


double SelectQuery::getJoinRows(std::string tableName, const RefObjectMap& refObjectMap,
	const std::vector<std::string>& subject_columns,
	const std::vector<std::string>& object_columns)
{
	std::cout << "JoinRows Begin: " << std::chrono::duration<double>(std::chrono::steady_clock::now() - start)<<std::endl;
	static double cost = 0;//, clear_cost = 0;
	//auto start = std::chrono::steady_clock::now();
	//    printf("start join clear\n");
	delete join_table;
	join_table = nullptr;
	join_index.clear();
	//    printf("end join clear\n");
	auto end = std::chrono::steady_clock::now();
	//clear_cost += std::chrono::duration<double>(end - start).count();
	//printf("clear cost %f\n", clear_cost);
	std::string child_table = std::move(tableName);
	std::string parent_table = refObjectMap.parentTableName.substr(1, refObjectMap.parentTableName.size() - 2);
	std::vector<std::string> child_columns;
	std::vector<std::string> parent_columns;
	for (const auto& item : refObjectMap.join) {
		child_columns.emplace_back(item.child.substr(1, item.child.size() - 2));
		parent_columns.emplace_back(item.parent.substr(1, item.parent.size() - 2));
	}
	std::string child_get_columns, parent_get_columns;
	for (const auto& item : subject_columns) {
		child_get_columns += "cast(child." + item + " as char) as " + item + ",";
		//child_get_columns += "child." + item + ",";
	}
	for (const auto& item : object_columns) {
		parent_get_columns += "cast(parent." + item + " as char) as " + item + ",";
		//parent_get_columns += "parent." + item + ",";
	}
	parent_get_columns.pop_back();
	parent_get_columns.push_back(' ');
	std::string sql =
		"select distinct " + child_get_columns + parent_get_columns + "from (select * from `" + schema_name + "`." +
		child_table + ") as child,\n" +
		"  (select * from `" + schema_name + "`." + parent_table +
		") as parent\n" +
		"  where child." + child_columns.front() + "=parent." + parent_columns.front() + "\n";
	for (int i = 1; i < child_columns.size(); ++i) {
		std::string and_sql = "and child." + child_columns[i] + "=parent." + parent_columns[i] + "\n";
		sql += and_sql;
	}
	sql.pop_back();
	sql += ';';
	std::cout << sql << std::endl;
	//start = std::chrono::steady_clock::now();
	auto sqlResult = session.sql(sql).execute();
	//    printf("%s\n", sql.c_str());
	auto& columns = sqlResult.getColumns();
	auto temp = std::make_shared<std::vector<mysqlx::Row>>();
	*temp = sqlResult.fetchAll();
	//auto mid2 = std::chrono::steady_clock::now();
	int len = temp->size(), rowlen = (*temp)[0].colCount();
	int fragmentsize = (len + threadnum - 1) / threadnum;
	auto v1 = new std::vector<std::vector<size_t>>(len, std::vector<size_t>(rowlen));
	std::cout << "Join conversion begins "<<" at " << std::chrono::duration<double>(std::chrono::steady_clock::now() - start) << std::endl;
	omp_set_num_threads(threadnum);
	//auto mid1 = std::chrono::steady_clock::now();
//#pragma omp parallel for schedule(static)
//	for (int k = 0; k < threadnum; k++) {
//		for (int l = fragmentsize * k; l < fragmentsize * (k + 1) && l < len; l++)
//		{
//			for (int j = 0; j < rowlen; j++)
//			{
//				if (!temp[l][j].isNull())(*v1)[l][j] = result->get(std::string(temp[l][j]));
//				else (*v1)[l][j] = 0;
//				//temp[l][j] = "";
//			}
//			temp[l].clear();
//			//temp[l] = mysqlx::Row();
//			//std::memset(&temp[l], sizeof(mysqlx::Row), 0);
//		}
//	}
#pragma omp parallel for schedule(static)
	for (int l = 0; l < len; l++) {
		
			for (int j = 0; j < rowlen; j++)
			{
				if (!(*temp)[l][j].isNull())(*v1)[l][j] = result->get(std::string((*temp)[l][j]));
				else (*v1)[l][j] = 0;
				//temp[l][j] = "";
			}
			(*temp)[l].clear();
			//temp[l] = mysqlx::Row();
			//std::memset(&temp[l], sizeof(mysqlx::Row), 0);
		
	}
	std::cout << "Join conversion ends " << " at " << std::chrono::duration<double>(std::chrono::steady_clock::now() - start) << std::endl;
	//auto mid3 = std::chrono::steady_clock::now();
	//std::memset(&temp[0], sizeof(mysqlx::Row) * len, 0);
	
	//end = std::chrono::steady_clock::now();
    //printf("%s\n%f\n", sql.c_str(), std::chrono::duration<double>(end - start).count());
	//std::cout << "time spent on query execution:" << std::chrono::duration<double>(mid1 - start).count()<<std::endl;
	//std::cout << "time spent on clear:" << std::chrono::duration<double>(end - mid3).count() << std::endl;
	//std::cout << "time spent on conversion and clear:" << std::chrono::duration<double>(end - mid1).count() << std::endl;
	//std::cout << "time spent on query as a whole:" << std::chrono::duration<double>(end - start).count() << std::endl;
	std::unordered_map<std::string, int> labels;
	int index = 0;
	size_t child_column_size = subject_columns.size();
	for (const auto& item : columns) {
		std::string column_name = index < child_column_size ? "$" : "#";
		column_name += item.getColumnLabel();
		//std::cout << column_name << std::endl;
		labels[column_name] = index;
		++index;
	}
	join_index = labels;
	join_table = v1;
	std::cout << "hereJoinRow "<< std::chrono::duration<double>(std::chrono::steady_clock::now() - start)<<std::endl;
	//end = std::chrono::steady_clock::now();
	//cost += std::chrono::duration<double>(end - start).count();
	return 0;//(double)std::chrono::duration<double>(mid2 - start).count();
}

SelectQuery::~SelectQuery()
{
	session.close();
}

//void SelectQuery::getRows(std::string tableName)
//{
//    static double cost = 0;
//    for (auto &item: result) {
//        delete item.second;
//    }
//    result.clear();
//    auto start = std::chrono::steady_clock::now();
//    auto table = db.getTable(tableName);
//    auto sqlResult = table.select("*").execute();
//    auto &columns = sqlResult.getColumns();
//    std::vector<std::string> labels;
//    for (const auto &item: columns) {
//        labels.emplace_back(item.getColumnLabel());
//    }
//    auto it = sqlResult.begin();
//    std::vector<decltype(*it)> itVec;
//    for (; it != sqlResult.end(); ++it) {
//        itVec.push_back(*it);
//    }
//    omp_set_num_threads(3);
//#pragma omp parallel for
//    for (int j = 0; j < itVec.size(); ++j) {
//        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
//        for (int i = 0; i < labels.size(); ++i) {
//            row->insert_or_assign(labels[i], itVec[j][i]);
//        }
//        result.insert_or_assign(j, row);
//    }
//    auto end = std::chrono::steady_clock::now();
//    cost += std::chrono::duration<double>(end - start).count();
//    printf("non-join sql runTime: %f\n", cost);
//}


