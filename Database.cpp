//
// Created by 刘洋 on 2022/10/14.
//

#include "Database.h"


Database::Database(){
    all_subs.clear();
}

bool Database::handleTriple(Triple *triple)
{
    std::string sub = triple->getSubject(),
                pre = triple->getPredicate(),
                obj = triple->getObject();
    auto iter_sub = sub_map.find(sub);
    auto iter_pre = pre_map.find(pre);
    auto iter_obj = obj_map.find(obj);
    if(iter_sub != sub_map.end()){//subject exist
        ///------------subject----------------
        auto SUB = iter_sub->second;
        SUB->single_triple.push_back(triple);
        ///------------predicate--------------
        if(iter_pre == pre_map.end()){//predicate doesn't exist
            PRE_TYPE *pre_node = new PRE_TYPE();
            pre_map.insert(std::pair<std::string,PRE_TYPE*>(pre,pre_node));
        }
        //predicate exists
        auto PRE = iter_pre->second;
        PRE->single_triple.push_back(triple);
        auto iter_sub_pre = PRE->sub_pre_triple.find(sub);
        if(iter_sub_pre != PRE->sub_pre_triple.end()){//<s,p,?>exist
            iter_sub_pre->second.push_back(triple);
        }
        else{//<s,p,?>doesn't exist
           std::vector<Triple*> vec;
           vec.push_back(triple);
           PRE->sub_pre_triple.insert(STR_TRI_PAIR (sub,vec));
        }
        ///--------------object---------------
        if(iter_obj == obj_map.end()){
            OBJ_TYPE *obj_node = new OBJ_TYPE();
            obj_map.insert(std::pair<std::string,OBJ_TYPE*>(obj,obj_node));
        }
        // object exists
        auto OBJ = iter_obj->second;
        OBJ->single_triple.push_back(triple);
        auto iter_pre_obj = OBJ->pre_obj_triple.find(pre);
        if(iter_pre_obj != OBJ->pre_obj_triple.end()){//<?,p,o>exists
            iter_pre_obj->second.push_back(triple);
        }
        else{//<?,p,o>doesn't exist
            std::vector<Triple*> vec;
            vec.push_back(triple);
            OBJ->pre_obj_triple.insert(STR_TRI_PAIR(pre,vec));
        }
        auto iter_spo = OBJ->spo_triple.find(STR_STR_PAIR(sub,pre));
        if(iter_spo != OBJ->spo_triple.end()){
            ///TODO: 抛出错误信息，triple重复出现
        }
        else{//<s,p,o> doesn't exist
            OBJ->spo_triple.insert(std::pair<STR_STR_PAIR,Triple*>(std::make_pair(sub,pre),triple));
        }
        /// -----------s->p->o push-------------
        auto tmp = std::find(SUB->sub_to_pre.begin(), SUB->sub_to_pre.end(), PRE);
        auto tmp2 = std::find(PRE->pre_to_obj.begin(),PRE->pre_to_obj.end(),OBJ);
        auto tmp3 = std::find(OBJ->obj_to_sub.begin(), OBJ->obj_to_sub.end(), SUB);
        if(tmp == SUB->sub_to_pre.end())SUB->sub_to_pre.push_back(PRE);
        if(tmp2 == PRE->pre_to_obj.end())PRE->pre_to_obj.push_back(OBJ);
        if(tmp3 == OBJ->obj_to_sub.end())OBJ->obj_to_sub.push_back(SUB);
        /// -----------all subjects-------------
        auto iter_all_subs = std::find(all_subs.begin(), all_subs.end(), sub);
        if(iter_all_subs == all_subs.end()) all_subs.push_back(sub);
    }
    return false;
}