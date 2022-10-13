//
// Created by yanjs on 2022/10/7.
//
#include "Triple.h"

Triple::Triple()
{
    subject_ = "";
    predicate_ = "";
    object_ = "";
    objectType_ = None;
}

Triple::Triple(const std::string &_subject, const std::string &_predicate, const std::string &_object,
               const ObjectType _objectType)
{
    this->subject_ = _subject;
    this->predicate_ = _predicate;
    this->object_ = _object;
    this->objectType_ = _objectType;
}

Triple::Triple(const Triple &_triple)
{
    this->object_ = _triple.object_;
    this->predicate_ = _triple.predicate_;
    this->subject_ = _triple.subject_;
    this->objectType_ = _triple.objectType_;
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

ObjectType Triple::getObjectType() const
{
    return objectType_;
}

void Triple::setSubject(const std::string &_subject)
{
    this->subject_ = _subject;
}

void Triple::setPredicate(const std::string &_predicate)
{
    this->predicate_ = _predicate;
}

void Triple::setObject(const std::string &_object)
{
    this->object_ = _object;
}


void Triple::setObjectType(ObjectType objectType)
{
    this->objectType_ = objectType;
}
