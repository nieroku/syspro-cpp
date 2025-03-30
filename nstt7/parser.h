#ifndef PARSER_H
#define PARSER_H

#include "lang.h"

std::shared_ptr<Expr> parse(std::string_view);

#endif
