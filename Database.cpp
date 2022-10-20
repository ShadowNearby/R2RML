//
// Created by 刘洋 on 2022/10/14.
//

#include "Database.h"


Database::Database() {
    all_subs.clear();
}

#if 1

bool Database::handleTriple(Triple *triple) {
    std::string sub = triple->getSubject(),
            pre = triple->getPredicate(),
            obj = triple->getObject();
    auto iter_sub = sub_map.find(sub);
    auto iter_pre = pre_map.find(pre);
    auto iter_obj = obj_map.find(obj);

    SUB_TYPE *SUB = nullptr;
    PRE_TYPE *PRE = nullptr;
    OBJ_TYPE *OBJ = nullptr;
    if (iter_sub == sub_map.end()) {
        SUB_TYPE *sub_node = new SUB_TYPE();
        sub_map.insert(STR_SUB_PAIR(sub, sub_node));
        SUB = sub_node;
    }
        ///------------subject----------------
    else {
        SUB = iter_sub->second;
    }
    SUB->single_triple.push_back(triple);
    ///------------predicate--------------
    if (iter_pre == pre_map.end()) {//predicate doesn't exist
        auto pre_node = new PRE_TYPE();
        pre_map.insert(std::pair<std::string, PRE_TYPE *>(pre, pre_node));
        PRE = pre_node;
    } else {
        PRE = iter_pre->second;
    }
    PRE->single_triple.push_back(triple);
    auto iter_sub_pre = PRE->sub_pre_triple.find(sub);
    if (iter_sub_pre != PRE->sub_pre_triple.end()) {//<s,p,?>exist
        iter_sub_pre->second.push_back(triple);
    } else {//<s,p,?>doesn't exist
        std::vector<Triple *> vec;
        vec.push_back(triple);
        PRE->sub_pre_triple.insert(STR_TRI_PAIR(sub, vec));
    }
    ///--------------object---------------
    if (iter_obj == obj_map.end()) {
        auto obj_node = new OBJ_TYPE();
        obj_map.insert(std::pair<std::string, OBJ_TYPE *>(obj, obj_node));
        OBJ = obj_node;
    } else {
        OBJ = iter_obj->second;
    }
    OBJ->single_triple.push_back(triple);
    auto iter_pre_obj = OBJ->pre_obj_triple.find(pre);
    if (iter_pre_obj != OBJ->pre_obj_triple.end()) {//<?,p,o>exists
        iter_pre_obj->second.push_back(triple);
    } else {//<?,p,o>doesn't exist
        std::vector<Triple *> vec;
        vec.push_back(triple);
        OBJ->pre_obj_triple.insert(STR_TRI_PAIR(pre, vec));
    }
    auto iter_spo = OBJ->spo_triple.find(STR_STR_PAIR(sub, pre));
    if (iter_spo != OBJ->spo_triple.end()) {
        ///TODO: 抛出错误信息，triple重复出现
        return false;
    } else {//<s,p,o> doesn't exist
        OBJ->spo_triple.insert(std::pair<STR_STR_PAIR, Triple *>(std::make_pair(sub, pre), triple));
    }
    /// -----------s->p->o push-------------
#if 0
    auto tmp = std::find(SUB->sub_to_pre.begin(), SUB->sub_to_pre.end(), PRE);
    auto tmp2 = std::find(PRE->pre_to_obj.begin(), PRE->pre_to_obj.end(), OBJ);
    auto tmp3 = std::find(OBJ->obj_to_sub.begin(), OBJ->obj_to_sub.end(), SUB);

    if (tmp == SUB->sub_to_pre.end())
        SUB->sub_to_pre.push_back(PRE);
    if (tmp2 == PRE->pre_to_obj.end())
        PRE->pre_to_obj.push_back(OBJ);
    if (tmp3 == OBJ->obj_to_sub.end())
        OBJ->obj_to_sub.push_back(SUB);
#endif
    /// -----------all subjects-------------
    auto iter_all_subs = std::find(all_subs.begin(), all_subs.end(), sub);
    if (iter_all_subs == all_subs.end())
        all_subs.push_back(sub);
    return true;
}

#endif

std::vector<Triple *> &Database::findTriple(std::string sub, std::string pre, std::string obj) {
    std::vector<Triple *> ans;
    if (sub == "?" && pre == "?" && obj == "?") {
        for (int i = 0; i < all_subs.size(); ++i) {
            auto tmp = sub_map.find(all_subs[i]);
            for (int j = 0; j < tmp->second->single_triple.size(); ++j) {
                ans.push_back(tmp->second->single_triple[j]);
            }
        }
    } else if (sub == "?" && pre == "?") {
        auto tmp = obj_map.find(obj);
        if (tmp == obj_map.end()) {
            /// TODO:CANNOT FIND
        } else {
            ans = tmp->second->single_triple;
        }
    } else if (sub == "?" && obj == "?") {
        auto tmp = pre_map.find(pre);
        if (tmp == pre_map.end()) {
            /// TODO:CANNOT FIND
        } else {
            ans = tmp->second->single_triple;
        }
    } else if (pre == "?" && obj == "?") {
        auto tmp = sub_map.find(sub);
        if (tmp == sub_map.end()) {
            /// TODO:CANNOT FIND
        } else {
            ans = tmp->second->single_triple;
        }
    } else if (sub == "?") {
        auto tmp = obj_map.find(obj);
        if (tmp == obj_map.end()) {
            /// TODO:CANNOT FIND
        } else {
            auto obj_tmp = tmp->second->pre_obj_triple.find(pre);
            if (obj_tmp == tmp->second->pre_obj_triple.end()) {
                /// TODO:CANNOT FIND
            } else {
                ans = obj_tmp->second;
            }
        }
    } else if (pre == "?") {
        auto tmp = sub_map.find(sub);
        if (tmp == sub_map.end()) {
            /// TODO：没有找到
        } else {
            auto sub_tmp = tmp->second->obj_sub_triple.find(obj);
            if (sub_tmp == tmp->second->obj_sub_triple.end()) {
                ///TODO: CANNOT FIND
            } else {
                ans = sub_tmp->second;
            }
        }
    } else if (obj == "?") {
        auto tmp = pre_map.find(pre);
        if (tmp == pre_map.end()) {
            ///TODO:CANNOT FIND
        } else {
            auto pre_tmp = tmp->second->sub_pre_triple.find(sub);
            if (pre_tmp == tmp->second->sub_pre_triple.end()) {
                ///TODO: CANNOT FIND
            } else {
                ans = pre_tmp->second;
            }
        }
    } else {
        auto tmp = obj_map.find(obj);
        if (tmp == obj_map.end()) {
            ///TODO: CANNOT FIND
        } else {
            auto spo_tmp = tmp->second->spo_triple.find(std::pair<std::string, std::string>(sub, pre));
            if (spo_tmp == tmp->second->spo_triple.end()) {
                ///TODO: CANNOTFIND
            } else {
                ans.push_back(spo_tmp->second);
            }
        }
    }
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i]->getSubject() << ' ' << ans[i]->getPredicate()
                  << ' ' << ans[i]->getObject() << std::endl;
    }
    return ans;
}

#if 0

bool Database::handleTriple(Triple *triple) {
    std::string sub = triple->getSubject(),
            pre = triple->getPredicate(),
            obj = triple->getObject();
    auto SUB = sub_map.find(sub);
    auto PRE = pre_map.find(pre);
    auto OBJ = obj_map.find(obj);
    size_t sub_id, pre_id, obj_id;
    // single-map
    if (SUB == sub_map.end()) {
        sub_id = sub_len;
        sub_map.insert(STR_ID_PAIR(sub, sub_len++));
    } else {
        sub_id = SUB->second;
    }
    if (PRE == pre_map.end()) {
        pre_id = pre_len;
        pre_map.insert(STR_ID_PAIR(pre, pre_len++));
    } else {
        pre_id = PRE->second;
    }
    if (OBJ == obj_map.end()) {
        obj_id = obj_len;
        obj_map.insert(STR_ID_PAIR(obj, obj_len++));
    } else {
        obj_id = OBJ->second;
    }

}

#endif