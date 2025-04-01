#ifndef LANG_H
#define LANG_H

#include <memory>
#include <optional>
#include <string>

#include "string_map.h"

using integer = int64_t;

class BaseExpr;
using Expr = std::shared_ptr<const BaseExpr>;

class Env {
  std::optional<std::reference_wrapper<const Env>> base = std::nullopt;
  StringMap<Expr> map;

 public:
  class Builder;
  static Builder build();
  static Builder build(const Env *);
  Env(Builder);

  Expr get(std::string_view) const;

 private:
  Env() {};
  Env(const Env *base) : base(*base) {};
};

class Env::Builder {
  friend Env;
  Env env;

 public:
  Builder &set(std::string, Expr);

 private:
  Builder(Env env) : env(env) {}
};

inline Env::Env(Builder builder) { std::swap(*this, builder.env); }

inline Env::Builder Env::build() { return Builder(Env()); }
inline Env::Builder Env::build(const Env *env) { return Builder(Env(env)); }

class BaseExpr {
 public:
  virtual Expr eval() const final { return eval(Env::build()); };
  virtual Expr eval(const Env &) const = 0;
  virtual operator integer() const { throw "not a value"; }
  virtual Expr operator()(const Env &, Expr) const { throw "not a function"; }
  virtual operator std::string() const = 0;

  virtual ~BaseExpr() = default;
};

class ValExpr final : public BaseExpr,
                      public std::enable_shared_from_this<ValExpr> {
  integer val;

 public:
  ValExpr(integer val) : val(val) {};

  virtual Expr eval(const Env &) const;
  virtual operator integer() const;

  virtual operator std::string() const;
};

class VarExpr final : public BaseExpr {
  std::string id;

 public:
  VarExpr(std::string id) : id(id) {};

  virtual Expr eval(const Env &) const;

  virtual operator std::string() const;
};

class AddExpr final : public BaseExpr {
  Expr e1;
  Expr e2;

 public:
  AddExpr(Expr e1, Expr e2) : e1(e1), e2(e2) {}

  virtual Expr eval(const Env &) const;

  virtual operator std::string() const;
};

class IfExpr final : public BaseExpr {
  Expr e1;
  Expr e2;
  Expr e_then;
  Expr e_else;

 public:
  IfExpr(Expr e1, Expr e2, Expr e_then, Expr e_else)
      : e1(e1), e2(e2), e_then(e_then), e_else(e_else) {}

  virtual Expr eval(const Env &) const;

  virtual operator std::string() const;
};

class LetExpr final : public BaseExpr {
  std::string id;
  Expr e_value;
  Expr e_body;

 public:
  LetExpr(std::string id, Expr e_value, Expr e_body)
      : id(id), e_value(e_value), e_body(e_body) {}

  virtual Expr eval(const Env &) const;

  virtual operator std::string() const;
};

class FuncExpr final : public BaseExpr,
                       public std::enable_shared_from_this<FuncExpr> {
  std::string arg_id;
  Expr e_body;

 public:
  FuncExpr(std::string arg_id, Expr e_body) : arg_id(arg_id), e_body(e_body) {}

  virtual Expr eval(const Env &) const;
  virtual Expr operator()(const Env &, Expr) const;

  virtual operator std::string() const;
};

class CallExpr final : public BaseExpr {
  Expr e_body;
  Expr e_arg;

 public:
  CallExpr(Expr e_body, Expr e_arg) : e_body(e_body), e_arg(e_arg) {}

  virtual Expr eval(const Env &) const;

  virtual operator std::string() const;
};

#endif
