#ifndef LANG_H
#define LANG_H

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "string_map.h"

using integer = int64_t;

class Expr;

class Env {
  std::optional<std::reference_wrapper<const Env>> inner = std::nullopt;
  StringMap<std::shared_ptr<Expr>> map;

 public:
  Env() {};
  Env(const Env &inner) : inner(inner) {};
  std::shared_ptr<Expr> get(std::string_view) const;
  void set(std::string_view, std::shared_ptr<Expr>);
};

class Expr {
 public:
  std::shared_ptr<Expr> eval() const {
    Env env;
    return eval(env);
  };
  virtual std::shared_ptr<Expr> eval(const Env &) const {
    throw std::logic_error("TODO");
  };
  virtual operator integer() const { throw "not a value"; }
  virtual std::shared_ptr<Expr> operator()(const Env &, std::shared_ptr<Expr>)
      const {
    throw "not a function";
  }
  virtual operator std::string() const = 0;

  virtual ~Expr() = default;
};

class ValExpr final : public Expr {
  integer val;

 public:
  ValExpr(integer val) : val(val) {};

  virtual std::shared_ptr<Expr> eval(const Env &) const;
  virtual operator integer() const;

  virtual operator std::string() const;
};

class VarExpr final : public Expr {
  std::string id;

 public:
  VarExpr(std::string id) : id(id) {};

  virtual std::shared_ptr<Expr> eval(const Env &) const;

  virtual operator std::string() const;
};

class AddExpr final : public Expr {
  std::shared_ptr<Expr> e1;
  std::shared_ptr<Expr> e2;

 public:
  AddExpr(std::shared_ptr<Expr> e1, std::shared_ptr<Expr> e2)
      : e1(e1), e2(e2) {}

  virtual std::shared_ptr<Expr> eval(const Env &) const;

  virtual operator std::string() const;
};

class IfExpr final : public Expr {
  std::shared_ptr<Expr> e1;
  std::shared_ptr<Expr> e2;
  std::shared_ptr<Expr> e_then;
  std::shared_ptr<Expr> e_else;

 public:
  IfExpr(
      std::shared_ptr<Expr> e1,
      std::shared_ptr<Expr> e2,
      std::shared_ptr<Expr> e_then,
      std::shared_ptr<Expr> e_else
  )
      : e1(e1), e2(e2), e_then(e_then), e_else(e_else) {}

  virtual std::shared_ptr<Expr> eval(const Env &) const;

  virtual operator std::string() const;
};

class LetExpr final : public Expr {
  std::string id;
  std::shared_ptr<Expr> e_value;
  std::shared_ptr<Expr> e_body;

 public:
  LetExpr(
      std::string id,
      std::shared_ptr<Expr> e_value,
      std::shared_ptr<Expr> e_body
  )
      : id(id), e_value(e_value), e_body(e_body) {}

  virtual std::shared_ptr<Expr> eval(const Env &) const;

  virtual operator std::string() const;
};

class FuncExpr final : public Expr {
  std::string arg_id;
  std::shared_ptr<Expr> e_body;

 public:
  FuncExpr(std::string arg_id, std::shared_ptr<Expr> e_body)
      : arg_id(arg_id), e_body(e_body) {}

  virtual std::shared_ptr<Expr> eval(const Env &) const;
  virtual std::shared_ptr<Expr> operator()(const Env &, std::shared_ptr<Expr>)
      const;

  virtual operator std::string() const;
};

class CallExpr final : public Expr {
  std::shared_ptr<Expr> e_body;
  std::shared_ptr<Expr> e_arg;

 public:
  CallExpr(std::shared_ptr<Expr> e_body, std::shared_ptr<Expr> e_arg)
      : e_body(e_body), e_arg(e_arg) {}

  virtual std::shared_ptr<Expr> eval(const Env &) const;

  virtual operator std::string() const;
};

#endif
