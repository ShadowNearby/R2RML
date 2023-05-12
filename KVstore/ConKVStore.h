//
// Created by yanjs on 2023/3/8.
//

#pragma once

#include "../util/Utility.h"
#include "../util/Triple.h"

class ConKVStore
{
public:
	ConKVStore();

	bool insert(const std::string& subject, const std::string& predicate, const std::string& object);

	bool insert(const Triple& triple);

	size_t insert(const std::vector<Triple>& triples);

	void insert_dict(const std::string& subject, const std::string& predicate, const std::string& object);

	size_t getTriplesBySubPreObj(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& subject,
		const std::string& predicate,
		const std::string& object);

	size_t getTriplesBySubPre(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& subject,
		const std::string& predicate);

	size_t getTriplesBySubObj(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& subject,
		const std::string& object);

	size_t getTriplesByPreObj(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& predicate,
		const std::string& object);

	size_t getTriplesBySub(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& subject);

	size_t getTriplesByPre(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& predicate);

	size_t getTriplesByObj(folly::ConcurrentHashMap<size_t, Triple>& result, const std::string& object);

	size_t getAllTriples(folly::ConcurrentHashMap<size_t, Triple>& result);

	size_t lock(const std::string& str)
	{
		auto hash = std::hash<std::string>{}(str) % 128;
		id_locks[hash]->lock();
		return hash;
	}

	void unlock(const std::string& str)
	{
		auto hash = std::hash<std::string>{}(str) % 128;
		id_locks[hash]->unlock();
	}
	size_t get(const std::string& s) {
		// std::cout << "get\n";
		auto ptr = string2id.find(s);
		if (ptr != string2id.end())return (*ptr).second;
		else {
			size_t hash = lock(s);
			ptr = string2id.find(s);
			if (ptr != string2id.end()) {
				unlock(s);
				return (*ptr).second;
			}
			else {
				size_t sID = ids.at(hash);
				ids.at(hash)++;
				string2id.insert_or_assign(s, sID);
				id2string.insert_or_assign(sID, s);
				unlock(s);
				return sID;
			}
		}

	}
	std::string get(const size_t id) {
		return id2string[id];
	}
	std::vector<std::mutex*> id_locks;
	//    size_t id;
	std::vector<size_t> ids;
	size_t count;
	folly::ConcurrentHashMap<size_t, std::string> id2string;
	folly::ConcurrentHashMap<std::string, size_t> string2id;
	folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char>*, pair_hash> subidpreid2objidList;
	folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char>*, pair_hash> subidobjid2preidList;
	folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char>*, pair_hash> preidobjid2subidList;
	folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>*> subid2preidobjidList;
	folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>*> preid2subidobjidList;
	folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>*> objid2subidpreidList;
	folly::ConcurrentHashMap<std::tuple<size_t, size_t, size_t>, char, tuple_hash> triple2id;
};