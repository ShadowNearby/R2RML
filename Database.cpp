//
// Created by 刘洋 on 2022/10/14.
//

#include "Database.h"

Database::Database(){
    all_subs.clear();
}

bool Database::handleTriple(const Triple *triple)
{
    std::string sub = triple->getSubject(),
                pre = triple->getPredicate(),
                obj = triple->getObject();
    auto iter_sub = sub_map.find(sub);
    auto iter_pre = pre_map.find(pre);
    auto iter_obj = obj_map.find(obj);
    if(iter_sub != sub_map.end()){
        if(iter_pre != pre_map.end()) {
            iter_sub->second->sub_to_pre.push_back(iter_pre->second);
            std::pair p = std::make_pair(sub,triple);
            ///??
            //iter_pre->second->sub_pre_triple.insert(p);
        }
        else{
            PRE_TYPE* pre = new PRE_TYPE();
        }
    }
    return false;
}