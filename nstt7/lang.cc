#include "lang.h"

#include <format>

Expr Env::get(std::string_view id) const {
  if (auto it = map.find(id); it != map.end())
    return it->second;
  if (base)
    return base->get().get(id);
  throw "unbound id";
}

Env::Builder &Env::Builder::set(std::string id, Expr expr) {
  env.map[id] = expr;
  return *this;
}

Expr ValExpr::eval(const Env &) const {
  const auto weak = weak_from_this();
  return weak.expired() ? std::make_shared<ValExpr>(*this) : weak.lock();
}
Expr VarExpr::eval(const Env &env) const { return env.get(id); }
Expr AddExpr::eval(const Env &env) const {
  return std::make_shared<ValExpr>((integer)*e1->eval(env) + *e2->eval(env));
}
Expr IfExpr::eval(const Env &env) const {
  if ((integer)*e1->eval(env) > *e2->eval(env))
    return e_then->eval(env);
  else
    return e_else->eval(env);
}
Expr LetExpr::eval(const Env &env) const {
  return e_body->eval(Env::build(&env).set(id, e_value));
}
Expr FuncExpr::eval(const Env &) const {
  const auto weak = weak_from_this();
  return weak.expired() ? std::make_shared<FuncExpr>(*this) : weak.lock();
}
Expr CallExpr::eval(const Env &env) const {
  return (*e_body->eval(env))(env, e_arg->eval(env));
}

ValExpr::operator integer() const { return val; }

Expr FuncExpr::operator()(const Env &env, Expr arg) const {
  return e_body->eval(Env::build(&env).set(arg_id, arg));
}

ValExpr::operator std::string() const { return std::format("(val {})", val); }
VarExpr::operator std::string() const { return std::format("(var {})", id); }
AddExpr::operator std::string() const {
  return std::format("(add {} {})", (std::string)*e1, (std::string)*e2);
}
IfExpr::operator std::string() const {
  return std::format(
      "(if {} {} then {} else {})",
      (std::string)*e1,
      (std::string)*e2,
      (std::string)*e_then,
      (std::string)*e_else
  );
}
LetExpr::operator std::string() const {
  return std::format(
      "(let {} = {} in {})", id, (std::string)*e_value, (std::string)*e_body
  );
}
FuncExpr::operator std::string() const {
  return std::format("(function {} {})", arg_id, (std::string)*e_body);
}
CallExpr::operator std::string() const {
  return std::format("(call {} {})", (std::string)*e_arg, (std::string)*e_body);
}
