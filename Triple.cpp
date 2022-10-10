//
// Created by yanjs on 2022/10/7.
//
#include "Triple.h"

Triple::Triple()
{
    subject_ = "";
    predicate_ = "";
    object_ = "";
}

Triple::Triple(const std::string &line)
{

}

Triple::Triple(const std::string &subject, const std::string &predicate, const std::string &object)
{
    this->subject_ = subject;
    this->predicate_ = predicate;
    this->object_ = object;
}

Triple::Triple(const Triple &triple)
{
    this->object_ = triple.object_;
    this->predicate_ = triple.predicate_;
    this->subject_ = triple.subject_;
}

const std::string &Triple::getSubject() const
{
    return subject_;
}

const std::string &Triple::getPredicate() const
{
    return predicate_;
}

const std::string &Triple::getObject() const
{
    return object_;
}

void Triple::setSubject(const std::string &subject)
{
    this->subject_ = subject;
}

void Triple::setPredicate(const std::string &predicate)
{
    this->predicate_ = predicate;
}

void Triple::setObject(const std::string &object)
{
    this->object_ = object;
}
