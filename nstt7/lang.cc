#include "lang.h"

#include <format>

std::shared_ptr<Expr> Env::get(std::string_view id) const {
  if (auto it = map.find(id); it != map.end()) return it->second;
  if (inner) return inner->get().get(id);
  throw "unbound id";
}

void Env::set(std::string_view id, std::shared_ptr<Expr> value) {
  map[std::string(id)] = value;
}

std::shared_ptr<Expr> ValExpr::eval(Env const&) const {
  return std::make_shared<ValExpr>(*this);
}
std::shared_ptr<Expr> VarExpr::eval(Env const& env) const {
  return env.get(id);
}
std::shared_ptr<Expr> AddExpr::eval(Env const& env) const {
  return std::make_shared<ValExpr>((integer)*e1->eval(env) + *e2->eval(env));
}
std::shared_ptr<Expr> LetExpr::eval(Env const& env) const {
  Env updated_env(env);
  updated_env.set(id, e_value);
  return e_body->eval(updated_env);
}

ValExpr::operator integer() const { return val; }

ValExpr::operator std::string() const { return std::format("(val {})", val); }
VarExpr::operator std::string() const { return std::format("(var {})", id); }
AddExpr::operator std::string() const {
  return std::format("(add {} {})", (std::string)*e1, (std::string)*e2);
}
IfExpr::operator std::string() const {
  return std::format("(if {} {} then {} else {})", (std::string)*e1,
                     (std::string)*e2, (std::string)*e_then,
                     (std::string)*e_else);
}
LetExpr::operator std::string() const {
  return std::format("(let {} = {} in {})", id, (std::string)*e_value,
                     (std::string)*e_body);
}
FuncExpr::operator std::string() const {
  return std::format("(function {} {})", arg_id, (std::string)*e_body);
}
CallExpr::operator std::string() const {
  return std::format("(call {} {})", (std::string)*e_arg, (std::string)*e_body);
}
