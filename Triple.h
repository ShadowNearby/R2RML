//
// Created by yanjs on 2022/10/7.
//

#pragma once

#include "Utility.h"

enum ObjectType
{
    None, Entity, Literal
};

class Triple
{
private:
    std::string subject_;
    std::string predicate_;
    std::string object_;
    ObjectType objectType_;

public:
    Triple();

    explicit Triple(const std::string &subject, const std::string &predicate, const std::string &object,
                    ObjectType objectType = None);

    Triple(const Triple &triple);

    [[nodiscard]] const std::string &getSubject() const;

    [[nodiscard]] const std::string &getPredicate() const;

    [[nodiscard]] const std::string &getObject() const;

    [[nodiscard]] ObjectType getObjectType() const;


    void setSubject(const std::string &subject);

    void setPredicate(const std::string &predicate);

    void setObject(const std::string &object);

    void setObjectType(ObjectType objectType);

};
