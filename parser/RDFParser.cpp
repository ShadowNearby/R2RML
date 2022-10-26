//
// Created by yanjs on 2022/10/13.
//

#include "RDFParser.h"

RDFParser::RDFParser() : turtleParser_(sin_)
{}

RDFParser::RDFParser(std::istream &_fin) : turtleParser_(_fin)
{}

size_t RDFParser::parseFile(std::vector<Triple> &_triple_vector,
                            const std::string &_error_log,
                            size_t _startLine)
{
    std::string rawSubject, rawPredicate, rawObject;
    std::string subject, predicate, object;
    std::string objectSubType;
    std::string errorMsg;
    Type::Type_ID objectType;
    std::streambuf *coutBuf = std::cout.rdbuf();
    std::ofstream ofile;
    if (!_error_log.empty()) {
        if (_error_log == "NULL")
            std::cout.setstate(std::ios_base::badbit);  // Silent output
        else {
            ofile.open(_error_log, std::ios::app);
            if (ofile) {
                std::cout << "Error log file: " << _error_log << std::endl;
                std::cout.rdbuf(ofile.rdbuf());
            } else
                std::cout << "Error log file cannot be opened." << std::endl;
        }
    }

    size_t numLines = _startLine;
    int prev_line = -1;  // For recognizing errors thrown by TurtleParser in the
    // same line of the input file
    while (_triple_vector.size() < RDFParser::TRIPLE_NUM_PER_GROUP) {
        numLines++;
        try {
            if (!this->turtleParser_.parse(rawSubject, rawPredicate, rawObject,
                                           objectType, objectSubType))
                break;
        } catch (const TurtleParser::Exception &_e) {
            errorMsg = _e.message;
            // Sync the line number in errorMsg, then get rid of the part before ':'
            // format: "lexer error in line 14276: unexpected character ?"
            size_t num_l = errorMsg.find("line") + 5;
            size_t num_r = errorMsg.find(':');
            int curr_line = stoi(errorMsg.substr(num_l, num_r - num_l));
            if (curr_line == prev_line) {
                numLines--;
                continue;
            }

            errorMsg = errorMsg.substr(0, num_l - 9) + errorMsg.substr(num_r);

            // TODO: get the actual corresponding line
            std::cout << "Line " << numLines << " (<" << rawSubject
                      << "...> ...): " << errorMsg << std::endl;
            this->turtleParser_.discardLine();
            continue;
        }

        subject = "<" + rawSubject + ">";
        predicate = "<" + rawPredicate + ">";

        Triple::ObjectType tripleObjectType = Triple::None;
        if (objectType == Type::Type_URI) {
            object = "<" + rawObject + ">";
            tripleObjectType = Triple::Entity;
        } else {
            if (objectType == Type::Type_Literal)
                object = "\"" + rawObject + "\"";
            else if (objectType == Type::Type_CustomLanguage) {
                object = "\"" + rawObject;
                object += "\"@" + objectSubType;
            } else if (objectType == Type::Type_String)
                object =
                        "\"" + rawObject + "\"^^<http://www.w3.org/2001/XMLSchema#string>";
            else if (objectType == Type::Type_Integer) {
                try {
                    stoll(rawObject);
                } catch (std::invalid_argument &e) {
                    errorMsg = "Object integer value invalid";
                    std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                              << rawPredicate << "> <" << rawObject << ">): " << errorMsg
                              << std::endl;
                    continue;
                } catch (std::out_of_range &e) {
                    errorMsg = "Object integer out of range";
                    std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                              << rawPredicate << "> <" << rawObject << ">): " << errorMsg
                              << std::endl;
                    continue;
                }
                object =
                        "\"" + rawObject + "\"^^<http://www.w3.org/2001/XMLSchema#integer>";
            } else if (objectType == Type::Type_Decimal)
                object =
                        "\"" + rawObject + "\"^^<http://www.w3.org/2001/XMLSchema#decimal>";
            else if (objectType == Type::Type_Double) {
                try {
                    stod(rawObject);
                } catch (std::invalid_argument &e) {
                    errorMsg = "Object double value invalid";
                    std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                              << rawPredicate << "> <" << rawObject << ">): " << errorMsg
                              << std::endl;
                    continue;
                }
                if (rawObject.length() == 3 && toupper(rawObject[0]) == 'N' &&
                    toupper(rawObject[1]) == 'A' && toupper(rawObject[2]) == 'N') {
                    errorMsg = "Object double value is NaN";
                    std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                              << rawPredicate << "> <" << rawObject << ">): " << errorMsg
                              << std::endl;
                    continue;
                }
                object =
                        "\"" + rawObject + "\"^^<http://www.w3.org/2001/XMLSchema#double>";
            } else if (objectType == Type::Type_Boolean)
                object =
                        "\"" + rawObject + "\"^^<http://www.w3.org/2001/XMLSchema#boolean>";
            else if (objectType == Type::Type_CustomType) {
                if (objectSubType == "http://www.w3.org/2001/XMLSchema#long") {
                    try {
                        stoll(rawObject);
                    } catch (std::invalid_argument &e) {
                        errorMsg = "Object long value invalid";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    } catch (std::out_of_range &e) {
                        errorMsg = "Object long value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                } else if (objectSubType == "http://www.w3.org/2001/XMLSchema#int") {
                    long long ll;
                    try {
                        ll = stoll(rawObject);
                    } catch (std::invalid_argument &e) {
                        errorMsg = "Object int value invalid";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    } catch (std::out_of_range &e) {
                        errorMsg = "Object int value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                    if (ll < (long long) INT_MIN || ll > (long long) INT_MAX) {
                        errorMsg = "Object int value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                } else if (objectSubType == "http://www.w3.org/2001/XMLSchema#short") {
                    long long ll;
                    try {
                        ll = stoll(rawObject);
                    } catch (std::invalid_argument &e) {
                        errorMsg = "Object short value invalid";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    } catch (std::out_of_range &e) {
                        errorMsg = "Object short value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                    if (ll < (long long) SHRT_MIN || ll > (long long) SHRT_MAX) {
                        errorMsg = "Object short value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                } else if (objectSubType == "http://www.w3.org/2001/XMLSchema#byte") {
                    long long ll;
                    try {
                        ll = stoll(rawObject);
                    } catch (std::invalid_argument &e) {
                        errorMsg = "Object byte value invalid";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    } catch (std::out_of_range &e) {
                        errorMsg = "Object byte value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                    if (ll < (long long) SCHAR_MIN || ll > (long long) SCHAR_MAX) {
                        errorMsg = "Object byte value out of range";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                } else if (objectSubType == "http://www.w3.org/2001/XMLSchema#float") {
                    try {
                        stof(rawObject);
                    } catch (std::invalid_argument &e) {
                        errorMsg = "Object float value invalid";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                    if (rawObject.length() == 3 && toupper(rawObject[0]) == 'N' &&
                        toupper(rawObject[1]) == 'A' && toupper(rawObject[2]) == 'N') {
                        errorMsg = "Object float value is NaN";
                        std::cout << "Line " << numLines << " (<" << rawSubject << "> <"
                                  << rawPredicate << "> <" << rawObject
                                  << ">): " << errorMsg << std::endl;
                        continue;
                    }
                }
                object = "\"" + rawObject + "\"^^<";
                object += objectSubType + ">";
            }
            tripleObjectType = Triple::Literal;
        }

        _triple_vector.emplace_back(Triple(subject, predicate, object, tripleObjectType));
    }
    if (!_error_log.empty()) {
        if (_error_log == "NULL")
            std::cout.clear();
        else {
            ofile.close();
            std::cout.rdbuf(coutBuf);
        }
    }
    std::cout << "RDFParser parseFile done!" << std::endl;

    return numLines - 1;
}

size_t RDFParser::parseLines(std::vector<Triple> &_triple_vector,
                             const std::string &_lines)
{
    this->sin_.clear();
    this->sin_ << _lines;
    return parseFile(_triple_vector);
}


